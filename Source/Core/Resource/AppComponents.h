#pragma once

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/network/monitor/ConnectionMonitor.hpp>
#include <oatpp/network/monitor/ConnectionMaxAgeChecker.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/interceptor/RequestInterceptor.hpp>
#include <oatpp-openssl/server/ConnectionProvider.hpp>
#include <oatpp-openssl/Config.hpp>
#include <Config/Config.h>
#include <chrono>

/**
 * @class AppComponent
 * @brief Manages the creation and wiring of application components using oatpp's dependency injection.
 *
 * This class is responsible for setting up the server's network configuration,
 * routing, and core functionalities like JSON mapping and error handling.
 */
class AppComponent {
public:
  AppComponent(BG::API::Config::Config* _Config) : Config_(_Config) {}
  
  BG::API::Config::Config* Config_;

  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
      auto tcpProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({ Config_->Host, static_cast<unsigned short>(Config_->PortNumber), oatpp::network::Address::IP_4 });

      auto monitor = std::make_shared<oatpp::network::monitor::ConnectionMonitor>(tcpProvider);
      monitor->addMetricsChecker(std::make_shared<oatpp::network::monitor::ConnectionMaxAgeChecker>(
        std::chrono::hours(1)
      ));

      if (Config_->UseHTTPS) {
        std::string keyURI = "file://" + Config_->KeyFilePath,
          certURI = "file://" + Config_->CrtFilePath;

        std::cout << "Loading Private Key From: " << keyURI << std::endl << "Loading Certificate From: " << certURI << std::endl;
      
        auto config = oatpp::openssl::Config::createDefaultServerConfigShared(certURI, keyURI);

        // have to cast because even though both monitor and this ssl connection provider are 
        // ServerConnectionProvider types, we can't return 2 explicitly different shared_ptrs
        return std::static_pointer_cast<oatpp::network::ServerConnectionProvider>(
          oatpp::openssl::server::ConnectionProvider::createShared(config, monitor));
      }

      return std::static_pointer_cast<oatpp::network::ServerConnectionProvider>(monitor);
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    //connectionHandler->addRequestInterceptor(std::make_shared<MyRequestInterceptor>());
    return connectionHandler;
  }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

};