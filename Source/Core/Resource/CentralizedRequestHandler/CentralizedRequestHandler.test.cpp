//==============================================================//
// This file is part of the BrainGenix-API Unified API Endpoint //
//==============================================================//

/*
    Description: Test cases for CentralizedRequestHandler, specifically testing the ToLower function.
    Additional Notes: None
    Date Created: 2025-12-16
*/

#include <Resource/CentralizedRequestHandler/CentralizedRequestHandler.h>
#include <gtest/gtest.h>

TEST(CentralizedRequestHandlerTest, ToLower_BasicUppercase) {
    std::string input = "HELLO";
    std::string expected = "hello";
    std::string result = CentralizedRequestHandler::ToLower(input);
    EXPECT_EQ(result, expected);
    return;
}