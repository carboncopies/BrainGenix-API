#include <string>
#include <memory>
#include <cstdlib>
#include <fstream>
#include <restbed>
#include <streambuf>
#include <sstream> // Add this include for hex conversion


#include <cpp-base64/base64.cpp>

#include <nlohmann/json.hpp>


#include <Resource/Dataset/Dataset.h>


namespace BG {
namespace API {
namespace Resource {

namespace Dataset {

Route::Route(Server::Server *_Server, BG::API::RPC::Manager* _Manager, restbed::Service &_Service) {
  Server_ = _Server;
  Manager_ = _Manager;

  // Setup Callback
  auto Callback(std::bind(&Route::RouteCallback, this, std::placeholders::_1));

  // Register This Route With Server
  std::shared_ptr<restbed::Resource> RouteResource = std::make_shared<restbed::Resource>();

  std::set<std::string> Values = {
    "/Dataset/{Path: .*}/{Level: .*}/{File: .*}",
    "/Dataset/{Path: .*}/{Level: .*}/{Level2: .*}/{File: .*}",
  };

  RouteResource->set_paths(Values);
  RouteResource->set_method_handler("GET", Callback);
  _Service.publish(RouteResource);

}

Route::~Route() {

}





std::string Route::GetFile(std::string _Handle) {

    nlohmann::json GetImageQuery;
    GetImageQuery["ImageHandle"] = _Handle;

   
    nlohmann::json QueryItem;
    QueryItem["ReqID"] = 0;
    QueryItem["VSDA/GetImage"] = GetImageQuery;


    std::vector<nlohmann::json> Queries{QueryItem};
    nlohmann::json FinalQuery = Queries;


    std::string Result;
    bool Status = Manager_->NESQueryJSON("NES", FinalQuery.dump(), &Result);
    if (!Status) {
        return "";
    }
    nlohmann::json ResultJSON = nlohmann::json::parse(Result)[0];

    return ResultJSON["ImageData"];

}

/* TEMPORARILY DEACTIVATED THIS CODE AS WE TRY PERPLEXITY'S SUGGESTION
   FOR Transfer-Encoding: chunked
void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    Server_->TotalQueries++;

    const auto request = _Session->get_request();
    const std::string Path = request->get_path_parameter("Path");
    const std::string File = request->get_path_parameter("File");
    const std::string Level = request->get_path_parameter("Level");
    const std::string Level2 = request->get_path_parameter("Level2");

    
    // Get File
    std::string FullPath = "NeuroglancerDatasets/" + Path;
    // std::cout<<FullPath<<"|"<<Level<<"|"<<Level2<<"|"<<File<<std::endl;
    if (Level == "info") {
        FullPath += "/" + Level;
    } else if (Level == "provenance") {
        FullPath += "/" + Level;
    } else {
        FullPath += "/" + Level + "/" + Level2 + "/" + File;
    }


    std::string Result = GetFile(FullPath);
    if (Result == "") {
        _Session->close( restbed::NOT_FOUND );
        return;
    }
    auto DecodedString = base64_decode(Result, false);

    
    const std::multimap< std::string, std::string > headers {
        { "Content-Type", "application/octet-stream" },
        { "Content-Length", std::to_string( DecodedString.length( ) ) }
    };
    
    _Session->close( restbed::OK, DecodedString, headers );
  
}
*/

void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    Server_->TotalQueries++;

    const auto request = _Session->get_request();
    const std::string Path = request->get_path_parameter("Path");
    const std::string File = request->get_path_parameter("File");
    const std::string Level = request->get_path_parameter("Level");
    const std::string Level2 = request->get_path_parameter("Level2");

    // Get File
    std::string FullPath = "NeuroglancerDatasets/" + Path;
    if (Level == "info") {
        FullPath += "/" + Level;
    } else if (Level == "provenance") {
        FullPath += "/" + Level;
    } else {
        FullPath += "/" + Level + "/" + Level2 + "/" + File;
    }

    std::string Result = GetFile(FullPath);
    if (Result == "") {
        _Session->close(restbed::NOT_FOUND);
        return;
    }
    auto DecodedString = base64_decode(Result, false);

    // Set headers for chunked transfer encoding
    const std::multimap<std::string, std::string> headers {
        { "Content-Type", "application/octet-stream" },
        { "Transfer-Encoding", "chunked" }
    };

    // Send the response in chunks
    _Session->yield(restbed::OK, headers, [DecodedString](const std::shared_ptr<restbed::Session> session) {
        // Send the data in chunks
        const size_t chunk_size = 1024; // Define your chunk size
        size_t offset = 0;

        while (offset < DecodedString.length()) {
            size_t length = std::min(chunk_size, DecodedString.length() - offset);
            std::string chunk = DecodedString.substr(offset, length);
            session->yield(chunk);
            offset += length;
        }

        // Close the session after sending all chunks
        session->close();
    });
}

}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG