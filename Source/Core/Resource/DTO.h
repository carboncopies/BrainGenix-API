#pragma once

#include <oatpp/core/data/mapping/type/Object.hpp>
#include <oatpp/core/macro/codegen.hpp>

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Message Data-Transfer-Object
 */
class MessageDto : public oatpp::DTO {

  DTO_INIT(MessageDto, DTO /* Extends */)

  DTO_FIELD(Int32, statusCode);   // Status code field
  DTO_FIELD(String, message);     // Message field

};

class TokenDTO : public oatpp::DTO {

  DTO_INIT(TokenDTO, DTO /* Extends */)

  DTO_FIELD(Int32, StatusCode);     // Message field
  DTO_FIELD(String, AuthKey);     // Message field
};

/* TODO - Add more DTOs here */

class LoginDTO : public oatpp::DTO{
  DTO_INIT(LoginDTO, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, password);
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)