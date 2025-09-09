#pragma once

#include <string>
#include <map>
#include <vector>
#include <variant>
#include <yaml-cpp/yaml.h>

#include <BG/Common/Logger/Logger.h>


class ConfigParser {
public:
    // Supported value types
    using Value = std::variant<long long, double, bool, std::string>;

    // Constructor: PascalCase, pointer to logger
    ConfigParser(const std::string& _YamlFilePath, int _ArgC, char** _ArgV, BG::Common::Logger::LoggingSystem* _Logger);

    // Destructor
    ~ConfigParser();

    // Getters - PascalCase
    int GetInt(const std::string& _Path, int _DefaultValue = 0);
    float GetFloat(const std::string& _Path, float _DefaultValue = 0.0f);
    bool GetBool(const std::string& _Path, bool _DefaultValue = false);
    std::string GetString(const std::string& _Path, const std::string& _DefaultValue = "");

    // Load config
    bool LoadConfigFromFile(const std::string& _Path = "");
    bool LoadConfigFromString(const std::string& _YamlContent);

    // Parse command line
    int ParseCommandLineArguments(int _ArgC, char** _ArgV);

    // Set value manually
    void SetValue(const std::string& _Key, const Value& _Value);

    // Get raw value
    Value GetValue(const std::string& _Key) const;

    // Debug dump
    void Dump(std::ostream& _Os) const;

    // Access positionals
    const std::vector<std::string>& GetPositionals() const;

    // Parse scalar string to typed value
    Value ParseScalar(const std::string& _Value);

private:
    // Recursive YAML parser
    void ParseYamlNode(const YAML::Node& _Node, const std::string& _CurrentPath = "");

    // Convert value to string for logging/dumping
    std::string ValueToString(const Value& _Value) const;

    // Member variables
    std::string YamlFilePath_;
    int ArgC_;
    char** ArgV_;
    BG::Common::Logger::LoggingSystem* Logger_;

    std::map<std::string, Value> Store_;
    std::set<std::string> Defaults_;
    std::vector<std::string> Positionals_;
};

// Standalone helper
YAML::Node LoadYamlFromFile(const std::string& _FilePath);