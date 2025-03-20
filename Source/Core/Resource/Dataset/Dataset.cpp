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

// Add hex conversion helper function inside Route class
std::string to_hex(size_t number) {
    std::stringstream ss;
    ss << std::hex << number;
    return ss.str();
}

void Route::RouteCallback(const std::shared_ptr<restbed::Session> _Session) {
    Server_->TotalQueries++;

    const auto request = _Session->get_request();
    const std::string Path = request->get_path_parameter("Path");
    const std::string File = request->get_path_parameter("File");
    const std::string Level = request->get_path_parameter("Level");
    const std::string Level2 = request->get_path_parameter("Level2");

    std::string FullPath = "NeuroglancerDatasets/" + Path;
    if (Level == "info") {
        FullPath += "/" + Level;
    } else if (Level == "provenance") {
        FullPath += "/" + Level;
    } else {
        FullPath += "/" + Level + "/" + Level2 + "/" + File;
    }

    std::string Result = GetFile(FullPath);
    if (Result.empty()) {
        _Session->close(restbed::NOT_FOUND);
        return;
    }
    
    auto DecodedString = base64_decode(Result, false);
    const size_t MAX_CHUNK_SIZE = 4096; // Adjust this value as needed

    // Set chunked transfer encoding headers
    const std::multimap<std::string, std::string> headers {
        { "Content-Type", "application/octet-stream" },
        { "Transfer-Encoding", "chunked" }
    };

    // Send response headers
    _Session->yield(restbed::OK, headers, 
        [DecodedString, MAX_CHUNK_SIZE](const std::shared_ptr<restbed::Session> session) {
            size_t offset = 0;
            const auto& data = DecodedString;
            
            // // Lambda for recursive chunk sending
            // std::function<void(size_t)> send_chunk = [&, session](size_t offset) {
            //     if (offset >= data.size()) {
            //         // Send final chunk and close
            //         const std::string final_chunk = "0\r\n\r\n";
            //         session->yield(restbed::Bytes(final_chunk.begin(), final_chunk.end()), 
            //             [session](const std::shared_ptr<restbed::Session>) {
            //                 session->close();
            //             });
            //         return;
            //     }

            //     // Calculate chunk size
            //     const size_t chunk_size = std::min(MAX_CHUNK_SIZE, data.size() - offset);
                
            //     // Format chunk header
            //     const std::string chunk_header = to_hex(chunk_size) + "\r\n";
                
            //     // Create full chunk with header and data
            //     std::string full_chunk = chunk_header + 
            //                            data.substr(offset, chunk_size) + 
            //                            "\r\n";
                                       
            //     // Convert to Bytes
            //     restbed::Bytes chunk_bytes(full_chunk.begin(), full_chunk.end());

            //     // Send current chunk and schedule next
            //     session->yield(chunk_bytes, 
            //         [offset, chunk_size, &send_chunk](const std::shared_ptr<restbed::Session> session) {
            //             send_chunk(offset + chunk_size);
            //         });
            // };

            std::function<void(size_t)> send_chunk = [&, session](size_t offset) {
                if (offset >= data.size()) {
                    const std::string final_chunk = "0\r\n\r\n";
                    auto final_ptr = std::make_shared<std::string>(final_chunk);
                    session->yield(restbed::Bytes(final_ptr->begin(), final_ptr->end()), 
                        [final_ptr, session](...) { session->close(); });
                    return;
                }
            
                const size_t chunk_size = std::min(MAX_CHUNK_SIZE, data.size() - offset);
                const std::string chunk_header = to_hex(chunk_size) + "\r\n";
                auto chunk_ptr = std::make_shared<std::string>(
                    chunk_header + data.substr(offset, chunk_size) + "\r\n"
                );
            
                session->yield(restbed::Bytes(chunk_ptr->begin(), chunk_ptr->end()), 
                    [chunk_ptr, offset, chunk_size, &send_chunk](...) {
                        send_chunk(offset + chunk_size);
                    });
            };

            // Start sending chunks
            send_chunk(0);
        });
}

}; // Close Namespace

}; // Close Namespace Resource
}; // Close Namespace API
}; // Close Namespace BG