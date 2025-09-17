#include "VSDAConnectionManager.h"
#include <stdexcept>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

VSDAConnectionManager::VSDAConnectionManager(BG::Common::Logger::LoggingSystem *logger,
                                             RPCManager *rpcManager,
                                             ConfigParser *config)
    : logger_(logger), rpcManager_(rpcManager), config_(config) {}

VSDAConnectionManager::~VSDAConnectionManager()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (leaderRpc_)
    {
        leaderRpc_->Stop();
    }
}

void VSDAConnectionManager::Initialize()
{
    // Register the endpoints that VSDA nodes will call to register themselves
    rpcManager_->AddRoute("RegisterVSDANode", logger_,
                          [this](const std::string &jsonRequest)
                          {
                              return this->RegisterVSDANode(jsonRequest);
                          });

    rpcManager_->AddRoute("SetVSDALeader", logger_,
                          [this](const std::string &jsonRequest)
                          {
                              return this->SetVSDALeader(jsonRequest);
                          });

    logger_->Log("[VSDAConnectionManager] Initialized VSDA RPC endpoints", 4);
}

std::string VSDAConnectionManager::RegisterVSDANode(const std::string &jsonRequest)
{
    std::lock_guard<std::mutex> lock(mutex_);

    try
    {
        json request = json::parse(jsonRequest);
        std::string nodeId = request["nodeId"];
        std::string host = request["host"];
        int port = request["port"];

        logger_->Log("[VSDAConnectionManager] Received registration from VSDA node: " +
                         nodeId + " at " + host + ":" + std::to_string(port),
                     4);

        // Create a bidirectional RPC connection to the VSDA node
        if (!leaderRpc_)
        {
            int rpcPort = config_->GetInt("VSDA.RPCPort", 8003);
            int timeout = config_->GetInt("VSDA.Timeout_ms", 5000);
            std::string advertisedHost = config_->GetString("VSDA.AdvertisedHost", "localhost");

            leaderRpc_ = std::make_unique<BidirectionalRpc>(rpcPort, true, logger_, timeout, "VSDALeader");
            leaderRpc_->SetAdvertisedHost(advertisedHost);
            leaderRpc_->Start();
            logger_->Log("[VSDAConnectionManager] Created bidirectional RPC client on port " + std::to_string(rpcPort), 4);
        }

        // Update the connection to point to the VSDA node
        leaderRpc_->UpdatePeer(host, port);
        logger_->Log("[VSDAConnectionManager] Updated peer to: " + host + ":" + std::to_string(port), 4);

        // Test the connection if health check is enabled
        bool enableHealthCheck = config_->GetBool("VSDA.EnableHealthCheck", true);
        if (enableHealthCheck)
        {
            bool healthCheck = leaderRpc_->Call<bool>("HealthCheck");
            if (!healthCheck)
            {
                throw std::runtime_error("Health check failed");
            }
            logger_->Log("[VSDAConnectionManager] Health check passed", 4);
        }

        // Bind any pending callbacks now that we have an active connection
        for (const auto &callback : pendingCallbacks_)
        {
            try
            {
                leaderRpc_->Bind(callback.first, callback.second);
                logger_->Log("[VSDAConnectionManager] Bound pending callback: " + callback.first, 4);
            }
            catch (const std::exception &e)
            {
                logger_->Log("[VSDAConnectionManager] Failed to bind pending callback " + callback.first + ": " + std::string(e.what()), 8);
            }
        }

        logger_->Log("[VSDAConnectionManager] Successfully connected to VSDA node: " + nodeId, 4);

        // Return success response
        json response;
        response["success"] = true;
        response["message"] = "Node registered successfully";
        return response.dump();
    }
    catch (const std::exception &e)
    {
        logger_->Log("[VSDAConnectionManager] Failed to register VSDA node: " + std::string(e.what()), 8);
        if (leaderRpc_)
        {
            leaderRpc_->Stop();
            leaderRpc_.reset();
        }

        // Return error response
        json response;
        response["success"] = false;
        response["error"] = e.what();
        return response.dump();
    }
}

std::string VSDAConnectionManager::SetVSDALeader(const std::string &jsonRequest)
{
    std::lock_guard<std::mutex> lock(mutex_);

    try
    {
        json request = json::parse(jsonRequest);
        std::string nodeId = request["nodeId"];

        logger_->Log("[VSDAConnectionManager] Set VSDA leader: " + nodeId, 4);
        currentLeaderNodeId_ = nodeId;

        // Return success response
        json response;
        response["success"] = true;
        response["message"] = "Leader set successfully";
        return response.dump();
    }
    catch (const std::exception &e)
    {
        logger_->Log("[VSDAConnectionManager] Failed to set VSDA leader: " + std::string(e.what()), 8);

        // Return error response
        json response;
        response["success"] = false;
        response["error"] = e.what();
        return response.dump();
    }
}

std::string VSDAConnectionManager::CallVSDALeader(const std::string &method,
                                                  const std::string &params)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (!leaderRpc_)
    {
        throw std::runtime_error("No VSDA leader configured");
    }

    try
    {
        if (params.empty())
        {
            return leaderRpc_->Call<std::string>(method);
        }
        else
        {
            return leaderRpc_->Call<std::string>(method, params);
        }
    }
    catch (const std::exception &e)
    {
        logger_->Log("[VSDAConnectionManager] RPC call to VSDA leader failed: " +
                         std::string(e.what()),
                     8);
        throw;
    }
}

bool VSDAConnectionManager::HasVSDALeader() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return leaderRpc_ && leaderRpc_->IsConnected();
}

// BidirectionalRpc* VSDAConnectionManager::GetBidirectionalRPC() {
//     return leaderRpc_.get();
// }

bool VSDAConnectionManager::RegisterBidirectionalCallback(std::string _CallbackName, std::function<std::string(const std::string &)> _callback)
{
    std::lock_guard<std::mutex> lock(mutex_);

    logger_->Log("[VSDAConnectionManager] Registering bidirectional callback: " + _CallbackName, 4);

    // Store the callback for later binding
    pendingCallbacks_.push_back(std::make_pair(_CallbackName, _callback));

    // If we already have a connected RPC, bind it immediately
    if (leaderRpc_ && leaderRpc_->IsConnected())
    {
        try
        {
            leaderRpc_->Bind(_CallbackName, _callback);
            logger_->Log("[VSDAConnectionManager] Callback " + _CallbackName + " bound immediately to existing RPC connection", 4);
            return true;
        }
        catch (const std::exception &e)
        {
            logger_->Log("[VSDAConnectionManager] Failed to bind callback " + _CallbackName + " immediately: " + std::string(e.what()), 8);
            return false;
        }
    }

    logger_->Log("[VSDAConnectionManager] Callback " + _CallbackName + " stored for later binding when RPC connection is available", 4);
    return true;
}