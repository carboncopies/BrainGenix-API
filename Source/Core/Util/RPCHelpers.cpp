#include <Util/RPCHelpers.h>
#include <nlohmann/json.hpp>


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
        std::cout<<"ERR: NES Connection timed out!\n  Route: "+_Route+'\n';
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

bool VSDAQueryJSON(VSDAConnectionManager* _VSDAManager, std::string _Method, std::string _Params, std::string* _Result) {

    std::cout<<"Making query: "<<_Method<<_Params<<std::endl;
    if (!_VSDAManager || !_VSDAManager->HasVSDALeader()) {
        return false;
    }
    
    try {
        if (_Params.empty()) {
            (*_Result) = _VSDAManager->CallVSDALeader(_Method);
        } else {
            (*_Result) = _VSDAManager->CallVSDALeader(_Method, _Params);
        }
        return true;
    } catch (const std::runtime_error& e) {
        std::cout << "ERR: VSDA Connection failed: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cout << "ERR: VSDA request failed: " << e.what() << std::endl;
        return false;
    }
}

std::string GetFile(RPCClientManager* _Manager, const std::string& _Handle) {
  nlohmann::json GetImageQuery;
  GetImageQuery["ImageHandle"] = _Handle;
 
  nlohmann::json QueryItem;
  QueryItem["ReqID"] = 0;
  QueryItem["VSDA/GetImage"] = GetImageQuery;

  std::vector<nlohmann::json> Queries{QueryItem};
  nlohmann::json FinalQuery = Queries;

  std::string Result;
  bool Status = _Manager->NESQueryJSON("NES", FinalQuery.dump(), &Result);
  if (!Status) {
    return "";
  }

  nlohmann::json ResultJSON = nlohmann::json::parse(Result)[0];

  return ResultJSON["ImageData"];
}

