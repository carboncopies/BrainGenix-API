//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This header file includes the header files of all other data transfer objects used by oat++. 
    Additional Notes: None
    Date Created: 2023-06-22
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")

// Third-Party Libraries (BG convention: use <> instead of "")
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>


// Internal Libraries (BG convention: use <> instead of "")



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

/* TODO - Add more DTOs here */

/**
 * Reponse Format For /NES/Geometry/Shape/_ALL_/Create
*/
class bgReponse_ShapeCreate : public oatpp::DTO {
  DTO_INIT(bgReponse_ShapeCreate, DTO)
  DTO_FIELD(Int32, bgStatus);   // Enum indicating the status of this API call.
  DTO_FIELD(Int64, bgShapeID);  // ID of the resulting shape created here (if status indicates success, otherwise this is -1).
}

/**
 * Reponse Format For /NES/Compartments/BS/Create
*/
class bgReponse_CompartmentsBSCreate : public oatpp::DTO {
  DTO_INIT(bgReponse_CompartmentsBSCreate, DTO)
  DTO_FIELD(Int32, bgStatus);         // Enum indicating the status of this API call.
  DTO_FIELD(Int64, bgCompartmentID);  // ID of the resulting compartment created here (if status indicates success, otherwise this is -1).
}


/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)