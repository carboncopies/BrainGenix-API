//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: This file manages the database connection and other associated stuff
    Additional Notes: None
    Date Created: 2023-06-24
*/

#pragma once

// Standard Libraries (BG convention: use <> instead of "")
#include <string>
#include <vector>

// Third-Party Libraries (BG convention: use <> instead of "")
#include <mysql.h>

// Internal Libraries (BG convention: use <> instead of "")





/**
 * @brief DBManager class for the database - will take a config struct (need to make this), and then connect to the db from that struct.
 * Then, methods here can be added to call sql commands as needed.
 * 
 */
class DBManager {

public:

    /**
     * @brief Construct a new DBManager object
     * 
     */
    DBManager();

    /**
     * @brief Destroy the DBManager object
     * 
     */
    ~DBManager();

};


