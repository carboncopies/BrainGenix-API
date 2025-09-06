#include "BidirectionalRPC.h"
#include <stdexcept>
#include <chrono>
#include <future>

// -----------------------------------------------------------------------------
// Construction / Destruction
// -----------------------------------------------------------------------------
BidirectionalRpc::BidirectionalRpc(int  serverPort,
                                   bool callBackConnect,
                                   BG::Common::Logger::LoggingSystem* logger,
                                   int timeoutMs,
                                   std::string Label)
    : m_serverPort(serverPort),
      m_callBackConnect(callBackConnect),
      m_peerHost(""),
      m_peerPort(0),
      m_server(serverPort),
      m_reconnectAttempts(0),
      m_timeoutMs(timeoutMs),
      Logger(logger),
      Label(Label) {

    m_server.bind("HealthCheck", []() { return true; });

    if (m_callBackConnect) {
        m_server.bind("__hookbackConnect",
                      [this](const std::string& host, int port) {
                          this->HookbackConnect(host, port);
                      });
    }

    if (Logger) {
        Logger->Log("[BiDirRpc] [" + Label + "] Listening on port " +
                    std::to_string(serverPort) + ", callback-mode = " +
                    (m_callBackConnect ? "true" : "false"), 0);
    }
}

BidirectionalRpc::~BidirectionalRpc() {
    Stop();
}

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------
void BidirectionalRpc::Start() {
    if (m_running) return;

    m_running = true;
    m_serverThread     = std::thread(&BidirectionalRpc::RunServer, this);
    m_healthCheckThread = std::thread(&BidirectionalRpc::HealthCheckLoop, this);

    if (Logger) Logger->Log("[BiDirRpc] [" + Label + "] Started", 0);
}

void BidirectionalRpc::Stop() {
    if (!m_running) return;

    m_running = false;

    {
        std::lock_guard<std::mutex> lock(m_clientMutex);
        m_client.reset();
    }
    m_server.stop();

    if (m_serverThread.joinable())     m_serverThread.join();
    if (m_healthCheckThread.joinable()) m_healthCheckThread.join();

    if (Logger) Logger->Log("[BiDirRpc] [" + Label + "] Stopped", 0);
}

void BidirectionalRpc::UpdatePeer(const std::string& host, int port) {
    std::lock_guard<std::mutex> lock(m_clientMutex);
    m_peerHost = host;
    m_peerPort = port;
    m_reconnectAttempts = 0;
    m_client.reset();

    if (Logger) {
        Logger->Log("[BiDirRpc] [" + Label + "] Peer updated to " + host + ":" +
                    std::to_string(port), 0);
    }
}

bool BidirectionalRpc::IsConnected() {
    std::lock_guard<std::mutex> lock(m_clientMutex);
    return m_client && TestConnection();
}

// -----------------------------------------------------------------------------
// Private helpers
// -----------------------------------------------------------------------------
void BidirectionalRpc::RunServer() {
    if (Logger) Logger->Log("[BiDirRpc] [" + Label + "] Server thread running", 0);
    m_server.run();
}

void BidirectionalRpc::HealthCheckLoop() {
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(HEALTH_CHECK_INTERVAL_MS));

        if (m_peerHost.empty() || m_peerPort == 0) continue;

        std::lock_guard<std::mutex> lock(m_clientMutex);

        if (!m_client) {
            if (Logger) Logger->Log("[BiDirRpc] [Health] [" + Label + "] No client, attempting reconnect", 0);
            Reconnect();
        } else {
            try {
                // Use timeout for health check
                auto future = std::async(std::launch::async, [this]() {
                    return m_client->call("HealthCheck");
                });
                
                if (future.wait_for(std::chrono::milliseconds(m_timeoutMs)) != std::future_status::ready) {
                    throw std::runtime_error("Health check timeout");
                }
                
                future.get(); // Check for exceptions
                m_reconnectAttempts = 0;
                if (Logger) Logger->Log("[BiDirRpc] [Health] [" + Label + "] OK For Host '" + m_peerHost + ":" + std::to_string(m_peerPort) + "'", 0);
            } catch (const std::exception& e) {
                if (Logger) Logger->Log("[BiDirRpc] [Health] [" + Label + "] Failed: " + std::string(e.what()), 1);
                m_client.reset();
                Reconnect();
            }
        }
    }
}

bool BidirectionalRpc::TestConnection() {
    try {
        // Use timeout for connection test
        auto future = std::async(std::launch::async, [this]() {
            m_client->call("HealthCheck");
        });
        
        return future.wait_for(std::chrono::milliseconds(m_timeoutMs)) == std::future_status::ready;
    } catch (const std::exception&) {
        return false;
    }
}

bool BidirectionalRpc::Reconnect() {
    if (m_reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
        if (Logger) {
            Logger->Log("[BiDirRpc] [Reconnect] [" + Label + "] Max attempts reached, giving up", 1);
        }
        return false;
    }

    try {
        m_client = std::make_unique<rpc::client>(m_peerHost, m_peerPort);
        m_client->set_timeout(m_timeoutMs);

        // Test connection with timeout
        auto future = std::async(std::launch::async, [this]() {
            m_client->call("HealthCheck");
        });
        
        if (future.wait_for(std::chrono::milliseconds(m_timeoutMs)) != std::future_status::ready) {
            throw std::runtime_error("Connection timeout");
        }
        
        future.get(); // Check for exceptions

        if (m_callBackConnect) {
            // Ask the peer to connect back to us with timeout
            auto hookbackFuture = std::async(std::launch::async, [this]() {
                m_client->call("__hookbackConnect", m_MyHost, m_serverPort);
            });
            
            if (hookbackFuture.wait_for(std::chrono::milliseconds(m_timeoutMs)) != std::future_status::ready) {
                if (Logger) Logger->Log("[BiDirRpc] [Reconnect] [" + Label + "] Hookback call timeout", 1);
            } else {
                hookbackFuture.get();
                if (Logger) Logger->Log("[BiDirRpc] [Reconnect] [" + Label + "] Sent __hookbackConnect to peer", 0);
            }
        }

        m_reconnectAttempts = 0;
        if (Logger) Logger->Log("[BiDirRpc] [Reconnect] [" + Label + "] Success", 0);
        return true;
    } catch (const std::exception& e) {
        m_client.reset();
        ++m_reconnectAttempts;
        if (Logger) {
            Logger->Log("[BiDirRpc] [Reconnect] [" + Label + "] Failed (" + std::string(e.what()) + ")", 1);
        }
        return false;
    }
}

void BidirectionalRpc::SetAdvertisedHost(std::string _Host) {
    m_MyHost = _Host;
}

// -----------------------------------------------------------------------------
// Hook executed when the remote asks *us* to connect back
// -----------------------------------------------------------------------------
void BidirectionalRpc::HookbackConnect(const std::string& host, int port) {
    std::lock_guard<std::mutex> lock(m_clientMutex);

    if (m_peerHost == host && m_peerPort == port && m_client && TestConnection()) {
        if (Logger) {
            Logger->Log("[BiDirRpc] [__hookbackConnect] [" + Label + "] Already connected to " +
                        host + ":" + std::to_string(port) + ", ignoring.", 0);
        }
        return;
    }

    if (Logger) {
        Logger->Log("[BiDirRpc] [__hookbackConnect] [" + Label + "] Peer asked us to connect to " +
                    host + ":" + std::to_string(port), 0);
    }
    UpdatePeer(host, port);
}