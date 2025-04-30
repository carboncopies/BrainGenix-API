  //==============================================================//
  // This file is part of the BrainGenix-API Unified API Endpoint //
  //==============================================================//

  /*
      Description: This file is where all routes are linked into the system
      Additional Notes: None
      Date Created: 2023-06-23
  */

  #pragma once

  // Standard Libraries (BG convention: use <> instead of "")
  #include <memory>
  #include <string>

  // Third-Party Libraries (BG convention: use <> instead of "")
  #include <restbed>

  // Internal Libraries (BG convention: use <> instead of "")
  #include <Server/Server.h>

  #include <Resource/Forbidden.h>
  #include <Resource/NotFound.h>

  #include <Resource/Hello.h>
  #include <Resource/Root.h>
  #include <Resource/Status.h>
  #include <Resource/Version.h>

  #include <Resource/Auth/GetToken.h>

  #include <Resource/NES/NES.h>
  #include <Resource/EVM/EVM.h>


  #include <Resource/NES/Visualizer/RenderImage.h>


  #include <BG/Common/Logger/Logger.h>


  namespace BG {
  namespace API {
  namespace Server {

  class EndpointManager {

    private:
      // Keep a list of resources below, so they stay in scope
      std::shared_ptr<Resource::NotFound::Route> NotFound_;
      std::shared_ptr<Resource::Forbidden::Route> Forbidden_;

      std::shared_ptr<Resource::Root::Route> Root_;
      std::shared_ptr<Resource::Hello::Route> Hello_;
      std::shared_ptr<Resource::Diagnostic::Version::Route> Diagnostic_Version_;
      std::shared_ptr<Resource::Diagnostic::Status::Route> Diagnostic_Status_;

      std::shared_ptr<Resource::Auth::GetToken::Route> Auth_GetToken_;


      std::shared_ptr<Resource::NES::Visualizer::RenderImage::Route> NES_Visualizer_RenderImage_;

      std::shared_ptr<Resource::NES::Route> NES_;
      std::shared_ptr<Resource::EVM::Route> EVM_;



      BG::Common::Logger::LoggingSystem* Logger_;

    public:
      /**
       * @brief Construct a new Endpoint Manager object
       *
       */
      EndpointManager(BG::Common::Logger::LoggingSystem* _Logger);

      /**
       * @brief Destroy the Endpoint Manager object
       *
       */
      ~EndpointManager();

      /**
       * @brief This function is where all routes are added to the service.
       * They're included in this file from the Route directory and then added in the AddRoutes function body.
       *
       * @param _Server
       */
      void AddRoutes(restbed::Service &_Service, Server &_Server);
  };

  }; // namespace Server
  }; // namespace API
  }; // namespace BG
