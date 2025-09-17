#include <Config/ConfigManager.h>
#include <Config/ConfigParser.h>
#include <RPC/RPCManager.h>
#include <stdexcept>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ConfigManager::ConfigManager(BG::Common::Logger::LoggingSystem *_Logger,
                             ConfigParser *_Config,
                             VSDAConnectionManager *_VSDAConnectionManager)
    : Logger_(_Logger), ConfigParser_(_Config), VSDAConnectionManager_(_VSDAConnectionManager), Initialized_(false)
{

    if (!Logger_)
    {
        throw std::invalid_argument("Logger cannot be null");
    }
    if (!ConfigParser_)
    {
        throw std::invalid_argument("ConfigParser cannot be null");
    }
    if (!VSDAConnectionManager_)
    {
        throw std::invalid_argument("VSDAConnectionManager cannot be null");
    }

    Logger_->Log("[ConfigManager] ConfigManager constructed successfully", 5);
}

ConfigManager::~ConfigManager()
{
    if (Initialized_)
    {
        Logger_->Log("[ConfigManager] ConfigManager destroyed", 4);
    }
}

void ConfigManager::Initialize()
{
    if (Initialized_)
    {
        Logger_->Log("[ConfigManager] Already initialized, skipping", 7);
        return;
    }

    Logger_->Log("[ConfigManager] Initializing ConfigManager", 5);

    // Register the configuration callback with the VSDA connection manager
    // This safely stores the callback and binds it when the RPC connection becomes available
    VSDAConnectionManager_->RegisterBidirectionalCallback("GetConfiguration",
                                                          [this](const std::string &request) -> std::string
                                                          {
                                                              return this->GetConfigurationCallback(request);
                                                          });

    Initialized_ = true;
    Logger_->Log("[ConfigManager] ConfigManager initialized successfully with VSDA callback registered", 5);
}

std::string ConfigManager::GetConfigurationCallback(const std::string &_Request)
{
    if (!Initialized_)
    {
        Logger_->Log("[ConfigManager] GetConfigurationCallback called but not initialized", 8);
        return json{{"error", "ConfigManager not initialized"}}.dump();
    }

    try
    {
        Logger_->Log("[ConfigManager] VSDA leader requested configuration for path: " + _Request, 4);

        // Parse the request to get the config path
        json requestData;
        std::string configPath;

        if (!_Request.empty())
        {
            try
            {
                requestData = json::parse(_Request);
                if (requestData.contains("path") && requestData["path"].is_string())
                {
                    configPath = requestData["path"].get<std::string>();
                }
                else
                {
                    configPath = _Request; // If not JSON, treat the whole request as the path
                }
            }
            catch (const json::parse_error &)
            {
                configPath = _Request; // If parsing fails, treat the whole request as the path
            }
        }

        if (configPath.empty())
        {
            return json{{"error", "No configuration path specified"}}.dump();
        }

        // Get all values under the specified path
        std::map<std::string, ConfigParser::Value> pathValues = ConfigParser_->GetAllValuesUnderPath(configPath);

        if (pathValues.empty())
        {
            Logger_->Log("[ConfigManager] No configuration values found for path: " + configPath, 7);
            return json{{"error", "No configuration values found for path: " + configPath}}.dump();
        }

        // Convert to nested JSON structure
        json configJson = ConfigParser_->ConvertPathsToJson(pathValues);

        // Create response with the path and the complete configuration subtree
        json response;
        response["path"] = configPath;
        response["config"] = configJson;

        Logger_->Log("[ConfigManager] Configuration provided for path: " + configPath + " (" + std::to_string(pathValues.size()) + " values)", 4);
        return response.dump();
    }
    catch (const std::exception &e)
    {
        Logger_->Log("[ConfigManager] Error getting configuration: " + std::string(e.what()), 8);
        return json{{"error", e.what()}}.dump();
    }
}