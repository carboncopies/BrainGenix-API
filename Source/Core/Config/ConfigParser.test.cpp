#include <Config/ConfigParser.h>
#include <gtest/gtest.h>

// Mock out the logger type (we don't need real logging in tests)
// Just pass nullptr unless you have a test logger
using LoggingSystem = BG::Common::Logger::LoggingSystem;

TEST(ConfigParserTest, LoadFromString) {
    // Pass nullptr for logger (not used in this test)
    ConfigParser parser("", 0, nullptr, static_cast<LoggingSystem*>(nullptr));
    std::string yamlContent = R"(
        server:
            port: 8080
            host: localhost
        features:
            enabled: true
            ratio: 3.14
    )";

    ASSERT_TRUE(parser.LoadConfigFromString(yamlContent));

    EXPECT_EQ(parser.GetInt("server.port"), 8080);
    EXPECT_EQ(parser.GetString("server.host"), "localhost");
    EXPECT_TRUE(parser.GetBool("features.enabled"));
    EXPECT_FLOAT_EQ(parser.GetFloat("features.ratio"), 3.14f);
}

TEST(ConfigParserTest, DefaultValues) {
    ConfigParser parser("", 0, nullptr, static_cast<LoggingSystem*>(nullptr));

    EXPECT_EQ(parser.GetInt("nonexistent.int", 42), 42);
    EXPECT_FLOAT_EQ(parser.GetFloat("nonexistent.float", 1.23f), 1.23f);
    EXPECT_TRUE(parser.GetBool("nonexistent.bool", true));
    EXPECT_EQ(parser.GetString("nonexistent.string", "default"), "default");
}

TEST(ConfigParserTest, ParseCommandLineArguments) {
    char* argv[] = {
        (char*)"program",
        (char*)"--port=8080",
        (char*)"--verbose",
        (char*)"--host",
        (char*)"localhost",
        (char*)"positional1",
        (char*)"positional2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Logger is passed as pointer (use nullptr for test)
    ConfigParser parser("", argc, argv, nullptr);
    // Note: parser now parses args in constructor, but we can re-parse safely
    // Or just rely on constructor behavior
    // Optionally re-parse (harmless)
    parser.ParseCommandLineArguments(argc, argv);

    EXPECT_EQ(parser.GetInt("port"), 8080);
    EXPECT_TRUE(parser.GetBool("verbose"));
    EXPECT_EQ(parser.GetString("host"), "localhost");

    const auto& positionals = parser.GetPositionals();
    ASSERT_EQ(positionals.size(), 2);
    EXPECT_EQ(positionals[0], "positional1");
    EXPECT_EQ(positionals[1], "positional2");
}

TEST(ConfigParserTest, ParseScalarValues) {
    ConfigParser parser("", 0, nullptr, static_cast<LoggingSystem*>(nullptr));

    auto testBool = parser.ParseScalar("true");
    ASSERT_TRUE(std::holds_alternative<bool>(testBool));
    EXPECT_TRUE(std::get<bool>(testBool));

    auto testInt = parser.ParseScalar("42");
    ASSERT_TRUE(std::holds_alternative<long long>(testInt));
    EXPECT_EQ(std::get<long long>(testInt), 42);

    auto testDouble = parser.ParseScalar("3.14");
    ASSERT_TRUE(std::holds_alternative<double>(testDouble));
    EXPECT_DOUBLE_EQ(std::get<double>(testDouble), 3.14);

    auto testString = parser.ParseScalar("hello");
    ASSERT_TRUE(std::holds_alternative<std::string>(testString));
    EXPECT_EQ(std::get<std::string>(testString), "hello");

    // Test false
    auto testFalse = parser.ParseScalar("false");
    ASSERT_TRUE(std::holds_alternative<bool>(testFalse));
    EXPECT_FALSE(std::get<bool>(testFalse));

    // Test negative int
    auto testNeg = parser.ParseScalar("-123");
    ASSERT_TRUE(std::holds_alternative<long long>(testNeg));
    EXPECT_EQ(std::get<long long>(testNeg), -123);

    // Test float with exponent
    auto testExp = parser.ParseScalar("1.5e2"); // 150
    ASSERT_TRUE(std::holds_alternative<double>(testExp));
    EXPECT_DOUBLE_EQ(std::get<double>(testExp), 150.0);
}

TEST(ConfigParserTest, CommandLineFlagSyntax) {
    char* argv[] = {
        (char*)"program",
        (char*)"-EnableFeature",
        (char*)"--UseTLS=true",
        (char*)"--Timeout",
        (char*)"30",
        (char*)"--Debug"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ConfigParser parser("", argc, argv, nullptr);

    EXPECT_TRUE(parser.GetBool("EnableFeature"));  // -EnableFeature → true
    EXPECT_TRUE(parser.GetBool("UseTLS"));        // --UseTLS=true → true
    EXPECT_EQ(parser.GetInt("Timeout"), 30);      // --Timeout 30 → 30
    EXPECT_TRUE(parser.GetBool("Debug"));         // --Debug → true
}

TEST(ConfigParserTest, MixedDashAndPositionals) {
    char* argv[] = {
        (char*)"program",
        (char*)"--Input=file.txt",
        (char*)"--Verbose",
        (char*)"--",
        (char*)"pos1",
        (char*)"pos2"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    ConfigParser parser("", argc, argv, nullptr);

    EXPECT_EQ(parser.GetString("Input"), "file.txt");
    EXPECT_TRUE(parser.GetBool("Verbose"));

    const auto& pos = parser.GetPositionals();
    ASSERT_EQ(pos.size(), 2);
    EXPECT_EQ(pos[0], "pos1");
    EXPECT_EQ(pos[1], "pos2");
}

TEST(ConfigParserTest, OverrideYamlWithCommandLine) {
    std::string yamlContent = R"(
        server:
            port: 80
            host: "default.local"
        debug: false
    )";

    char* argv[] = {
        (char*)"program",
        (char*)"--server.port=8080",
        (char*)"--debug"
    };
    int argc = 3;

    ConfigParser parser("", argc, argv, nullptr);
    ASSERT_TRUE(parser.LoadConfigFromString(yamlContent));

    // Command line should override
    EXPECT_EQ(parser.GetInt("server.port"), 8080);
    EXPECT_EQ(parser.GetString("server.host"), "default.local"); // unchanged
    EXPECT_TRUE(parser.GetBool("debug")); // overridden to true
}
