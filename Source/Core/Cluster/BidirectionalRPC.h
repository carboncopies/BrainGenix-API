#pragma once

#include <BG/Common/Logger/Logger.h>
#include <rpc/client.h>
#include <rpc/server.h>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <future>

/**
 * @class BidirectionalRpc
 * @brief Robust bidirectional RPC channel with health monitoring & reconnect.
 *
 * Each node runs both an RPC server (listening on `serverPort`) **and** keeps
 * an RPC client that points to the remote peer.  
 *
 * When `callBackConnect == true` the remote peer is asked to connect *back*
 * by invoking `__hookbackConnect(host, port)`.
 *
 * Logging (via BG::Common::Logger::LoggingSystem) is performed only when
 * Logger != nullptr.  Level 0 = verbose debug, 1+ = warnings / errors.
 */
class BidirectionalRpc {
public:
    /**
     * @param serverPort      TCP port on which *this* node listens.
     * @param callBackConnect If true, instruct the peer to connect back to us.
     * @param logger          Optional external logger (may be nullptr).
     * @param timeoutMs       Timeout for RPC calls in milliseconds.
     */
    explicit BidirectionalRpc(int serverPort,
                              bool callBackConnect = true,
                              BG::Common::Logger::LoggingSystem* logger = nullptr,
                              int timeoutMs = 5000);

    ~BidirectionalRpc();

    /** Start the internal server & health-check threads. */
    void Start();

    /** Stop all threads and close sockets. */
    void Stop();

    /**
     * Bind a callable to an RPC name (thin wrapper around rpc::server::bind).
     * The callback becomes reachable by the peer.
     */
    template <typename Func>
    void Bind(const std::string& methodName, Func func) {
        m_server.bind(methodName, func);
    }

    /**
     * Call a remote method on the peer.
     * Automatically reconnects if necessary (respecting MAX_RECONNECT_ATTEMPTS).
     *
     * @throws std::runtime_error when the peer is unreachable or the call fails.
     */
    template <typename ReturnType, typename... Args>
    ReturnType Call(const std::string& methodName, Args... args) {
        std::lock_guard<std::mutex> lock(m_clientMutex);

        if (m_peerHost.empty() || m_peerPort == 0) {
            throw std::runtime_error("No peer configured");
        }

        if (!m_client && !Reconnect()) {
            throw std::runtime_error("Failed to connect to peer after maximum attempts");
        }

        try {
            // Use async with timeout for the actual call
            auto future = std::async(std::launch::async, [this, &methodName, args...]() {
                auto result = m_client->call(methodName, args...);
                if constexpr (!std::is_void_v<ReturnType>) {
                    return result.template as<ReturnType>();
                }
            });
            
            if (future.wait_for(std::chrono::milliseconds(m_timeoutMs)) != std::future_status::ready) {
                m_client.reset();
                throw std::runtime_error("RPC call timed out: " + methodName);
            }
            
            if constexpr (!std::is_void_v<ReturnType>) {
                return future.get();
            } else {
                future.get(); // Just wait for completion
            }
        } catch (const std::exception& e) {
            m_client.reset();
            throw std::runtime_error(std::string("RPC call failed: ") + e.what());
        }

        if constexpr (std::is_void_v<ReturnType>) {
            return;
        } else {
            throw std::runtime_error("Unexpected return path in RPC call");
        }
    }

    /** Update peer address/port.  Resets reconnect counter. */
    void UpdatePeer(const std::string& host, int port);

    /** @return true if we believe the connection is alive. */
    bool IsConnected();

    /** Set the host that we advertise when initiating a loopback call */
    void SetAdvertisedHost(std::string _Host);

private:
    void RunServer();
    void HealthCheckLoop();
    bool Reconnect();
    bool TestConnection();

    // ------------------------------------------------------------------
    // Internal hook used when callBackConnect == true
    // ------------------------------------------------------------------
    void HookbackConnect(const std::string& host, int port);

    // ------------------------------------------------------------------
    // Data members
    // ------------------------------------------------------------------
    const int  m_serverPort;
    bool       m_callBackConnect;

    std::string m_peerHost;
    int         m_peerPort;

    rpc::server                    m_server;
    std::unique_ptr<rpc::client>   m_client;
    std::mutex                     m_clientMutex;
    std::atomic<bool>              m_running{false};
    std::thread                    m_serverThread;
    std::thread                    m_healthCheckThread;
    int                            m_reconnectAttempts{0};
    std::string                    m_MyHost;
    int                            m_timeoutMs;

    static constexpr int MAX_RECONNECT_ATTEMPTS   = 5;
    static constexpr int HEALTH_CHECK_INTERVAL_MS = 5000;

    BG::Common::Logger::LoggingSystem* Logger;   // may be nullptr
};