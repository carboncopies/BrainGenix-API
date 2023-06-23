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
class bgReponse_Shape_ALL_Create : public oatpp::DTO {
  DTO_INIT(bgReponse_Shape_ALL_Create, DTO)
  DTO_FIELD(Int32, StatusCode);        // Enum indicating the status of this API call.
  DTO_FIELD(Int64, ShapeID);           // ID of the resulting shape created here (if status indicates success, otherwise this is -1).
}

/**
 * Reponse Format For /NES/Compartment/_ALL_/Create
*/
class bgReponse_Compartment_ALL_Create : public oatpp::DTO {
  DTO_INIT(bgReponse_Compartment_ALL_Create, DTO)
  DTO_FIELD(Int32, StatusCode);        // Enum indicating the status of this API call.
  DTO_FIELD(Int64, CompartmentID);     // ID of the resulting compartment created here (if status indicates success, otherwise this is -1).
}

/**
 * Reponse Format For /NES/Connection/_ALL_/Create
*/
class bgReponse_Connection_ALL_Create : public oatpp::DTO {
  DTO_INIT(bgReponse_Connection_ALL_Create, DTO)
  DTO_FIELD(Int32, StatusCode);        // Enum indicating the status of this API call.
  DTO_FIELD(Int64, ConnectionID);      // ID of the resulting connection created here (if status indicates success, otherwise this is -1).
}

/**
 * Reponse Format For /NES/Tool/PatchClampDAC/Create
*/
class bgReponse_Tool_PatchClampDAC_Create : public oatpp::DTO {
  DTO_INIT(bgReponse_Tool_PatchClampDAC_Create, DTO)
  DTO_FIELD(Int32, StatusCode);        // Enum indicating the status of this API call.
  DTO_FIELD(Int64, PatchClampDACID);   // ID of the resulting PatchClampDAC created here (if status indicates success, otherwise this is -1).
}

/**
 * Reponse Format For /NES/Tool/PatchClampDAC/SetOutputList
*/
class bgReponse_Tool_PatchClampDAC_SetOutputList : public oatpp::DTO {
  DTO_INIT(bgReponse_Tool_PatchClampDAC_SetOutputList, DTO)
  DTO_FIELD(Int32, StatusCode);        // Enum indicating the status of this API call.
}

/**
 * Reponse Format For /NES/Tool/PatchClampADC/Create
*/
class bgReponse_Tool_PatchClampADC_Create : public oatpp::DTO {
  DTO_INIT(bgReponse_Tool_PatchClampADC_Create, DTO)
  DTO_FIELD(Int32, StatusCode);        // Enum indicating the status of this API call.
  DTO_FIELD(Int64, PatchClampADCID);   // ID of the resulting PatchClampADC created here (if status indicates success, otherwise this is -1).
}

/**
 * Reponse Format For /NES/Tool/PatchClampADC/SetSampleRate
*/
class bgReponse_Tool_PatchClampADC_SetSampleRate : public oatpp::DTO {
  DTO_INIT(bgReponse_Tool_PatchClampADC_SetSampleRate, DTO)
  DTO_FIELD(Int32, StatusCode);        // Enum indicating the status of this API call.
}

/**
 * Reponse Format For /NES/Tool/PatchClampADC/GetRecordedData
*/
class bgReponse_Tool_PatchClampADC_Create : public oatpp::DTO {
  DTO_INIT(bgReponse_Tool_PatchClampADC_Create, DTO)
  DTO_FIELD(Int32, StatusCode);              // Enum indicating the status of this API call.
  DTO_FIELD(Vector<float>, RecordedData_mV); // List of voltages recorded by the ADC.
  DTO_FIELD(float32, Timestep_ms);           // Gets the sample timestep rate for the ADC in milliseconds. 
}


/**
 * Reponse Format For /NES/Simulation/Create
*/
class bgReponse_Simulation_Create : public oatpp::DTO {
  DTO_INIT(bgReponse_Simulation_Create, DTO)
  DTO_FIELD(Int32, StatusCode);              // Enum indicating the status of this API call.
  DTO_FIELD(Int64, SimulationID);           // Gets the sample timestep rate for the ADC in milliseconds. 
}


/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)