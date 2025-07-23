#include <Util/RPCHelpers.h>
#include <RPC/ClientManager.h>

namespace BG {
namespace API {
namespace Util {


// Common RPC interface - used both for internal requests between services (NES<->EVM), and external requests to the HTTP POST API    
bool NESQueryJSON(std::shared_ptr<::rpc::client> _Client, std::atomic_bool* _IsNESClientHealthy, std::string _Route, std::string _Query, std::string* _Result) {
    if (!(*_IsNESClientHealthy)) {
        return false;
    }
    if (!_Client ||_Client->get_connection_state() != rpc::client::connection_state::connected) {
        return false;
    }
    try {
        (*_Result) = _Client->call(_Route.c_str(), _Query).as<std::string>();
    } catch (::rpc::timeout& e) {
        std::cout<<"ERR: NES Connection timed out!\n";
        return false;
    } catch (::rpc::rpc_error& e) {
        std::cout<<"ERR: NES remote returned RPC error\n";
        return false;
    } catch (std::system_error& e) {
        std::cout<<"ERR: Cannot talk to NES host\n";
        return false;
    }
    return true;
}

// Common RPC interface - used both for internal requests between services (NES<->EVM), and external requests to the HTTP POST API    
bool EVMQueryJSON(std::shared_ptr<::rpc::client> _Client, std::atomic_bool* _IsEVMClientHealthy, std::string _Route, std::string _Query, std::string* _Result) {
    if (!(*_IsEVMClientHealthy)) {
        return false;
    }
    if (!_Client || _Client->get_connection_state() != rpc::client::connection_state::connected) {
        return false;
    }
    try {
        (*_Result) = _Client->call(_Route.c_str(), _Query).as<std::string>();
    } catch (::rpc::timeout& e) {
        std::cout<<"ERR: EVM Connection timed out!\n";
        return false;
    } catch (::rpc::rpc_error& e) {
        std::cout<<"ERR: EVM remote returned RPC error\n";
        return false;
    } catch (std::system_error& e) {
        std::cout<<"ERR: Cannot talk to EVM host\n";
        return false;
    }
    return true;
}

std::string GetFile(const std::string& _Handle) {
  nlohmann::json GetImageQuery;
  GetImageQuery["ImageHandle"] = _Handle;
 
  nlohmann::json QueryItem;
  QueryItem["ReqID"] = 0;
  QueryItem["VSDA/GetImage"] = GetImageQuery;

  std::vector<nlohmann::json> Queries{QueryItem};
  nlohmann::json FinalQuery = Queries;

  std::string Result;
  bool Status = g_Manager->NESQueryJSON("NES", FinalQuery.dump(), &Result);
  if (!Status) {
      return "";
  }

  nlohmann::json ResultJSON = nlohmann::json::parse(Result)[0];

  return ResultJSON["ImageData"];
}


}; // Close Namespace Util
}; // Close Namespace API
}; // Close Namespace BG