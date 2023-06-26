//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file provides typedefs used by the rest of the system.
    Additional Notes: None
    Date Created: 2023-06-24
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <cstdint>

// Third-Party Libraries (BG convention: use <> instead of "")


// Internal Libraries (BG convention: use <> instead of "")



namespace BG {

    //- General BG Types -//
    /**
     * @brief Enum showing system state codes.
     * 
     */
    enum bgServiceStatus {
        SERVICE_HEALTHY,
        SERVICE_DEGRADED,
        SERVICE_FAILED,
        SERVICE_CONFIG_ERR,
        SERVICE_VERSION_MISMATCH
    };


    //- NES Specific Types -//
    namespace NES {
        typedef int64_t bgSimulationID;
    };


};