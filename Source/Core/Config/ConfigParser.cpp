#include <Config/ConfigParser.h>

#include <BG/Common/Logger/Logger.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

// Log levels
static constexpr int LOG_DEBUG = 0;
static constexpr int LOG_INFO = 5;
static constexpr int LOG_WARN = 7;
static constexpr int LOG_FATAL = 10;

// Helper to safely log (in case Logger_ is null)
#define LOG(Level, Msg)                   \
    do                                    \
    {                                     \
        if (Logger_)                      \
            Logger_->Log((Msg), (Level)); \
    } while (0)
ConfigParser::ConfigParser(
    const std::string &_YamlFilePath,
    int _ArgC,
    char **_ArgV,
    BG::Common::Logger::LoggingSystem *_Logger)
    : YamlFilePath_(_YamlFilePath), ArgC_(_ArgC), ArgV_(_ArgV), Logger_(_Logger)
{
    LOG(LOG_INFO, "[ConfigParser] Initializing with config file '" + YamlFilePath_ + "'");

    // Load from file if specified
    if (!YamlFilePath_.empty())
    {
        if (LoadConfigFromFile())
        {
            LOG(LOG_INFO, "[ConfigParser] Successfully loaded config from '" + YamlFilePath_ + "'");
        }
        else
        {
            LOG(LOG_WARN, "[ConfigParser] Failed to load config from '" + YamlFilePath_ + "'");
        }
    }

    // Parse command line
    ParseCommandLineArguments(ArgC_, ArgV_);
}

ConfigParser::~ConfigParser()
{
    LOG(LOG_DEBUG, "[ConfigParser] Destroyed");
}

int ConfigParser::GetInt(const std::string &_Path, int _DefaultValue)
{
    auto It = Store_.find(_Path);
    if (It != Store_.end())
    {
        if (auto Value = std::get_if<long long>(&It->second))
        {
            LOG(LOG_DEBUG, "[ConfigParser] GetInt('" + _Path + "') = " + std::to_string(*Value));
            return static_cast<int>(*Value);
        }
    }
    LOG(LOG_DEBUG, "[ConfigParser] GetInt('" + _Path + "') using default = " + std::to_string(_DefaultValue));
    return _DefaultValue;
}

float ConfigParser::GetFloat(const std::string &_Path, float _DefaultValue)
{
    auto It = Store_.find(_Path);
    if (It != Store_.end())
    {
        if (auto Value = std::get_if<double>(&It->second))
        {
            LOG(LOG_DEBUG, "[ConfigParser] GetFloat('" + _Path + "') = " + std::to_string(*Value));
            return static_cast<float>(*Value);
        }
    }
    LOG(LOG_DEBUG, "[ConfigParser] GetFloat('" + _Path + "') using default = " + std::to_string(_DefaultValue));
    return _DefaultValue;
}

bool ConfigParser::GetBool(const std::string &_Path, bool _DefaultValue)
{
    auto It = Store_.find(_Path);
    if (It != Store_.end())
    {
        if (auto Value = std::get_if<bool>(&It->second))
        {
            LOG(LOG_DEBUG, "[ConfigParser] GetBool('" + _Path + "') = " + std::string(*Value ? "true" : "false"));
            return *Value;
        }
    }
    LOG(LOG_DEBUG, "[ConfigParser] GetBool('" + _Path + "') using default = " + std::string(_DefaultValue ? "true" : "false"));
    return _DefaultValue;
}

std::string ConfigParser::GetString(const std::string &_Path, const std::string &_DefaultValue)
{
    auto It = Store_.find(_Path);
    if (It != Store_.end())
    {
        if (auto Value = std::get_if<std::string>(&It->second))
        {
            LOG(LOG_DEBUG, "[ConfigParser] GetString('" + _Path + "') = \"" + *Value + "\"");
            return *Value;
        }
    }
    LOG(LOG_DEBUG, "[ConfigParser] GetString('" + _Path + "') using default = \"" + _DefaultValue + "\"");
    return _DefaultValue;
}

bool ConfigParser::LoadConfigFromFile(const std::string &_Path)
{
    std::string ActualPath = _Path.empty() ? YamlFilePath_ : _Path;
    try
    {
        YAML::Node Root = YAML::LoadFile(ActualPath);
        ParseYamlNode(Root);
        LOG(LOG_INFO, "[ConfigParser] Loaded config from file '" + ActualPath + "'");
        return true;
    }
    catch (const YAML::Exception &E)
    {
        LOG(LOG_WARN, "[ConfigParser] YAML parsing error in '" + ActualPath + "': " + std::string(E.what()));
        return false;
    }
}

bool ConfigParser::LoadConfigFromString(const std::string &_YamlContent)
{
    try
    {
        YAML::Node Root = YAML::Load(_YamlContent);
        ParseYamlNode(Root);
        LOG(LOG_INFO, "[ConfigParser] Loaded config from string (" + std::to_string(_YamlContent.size()) + " chars)");
        return true;
    }
    catch (const YAML::Exception &E)
    {
        LOG(LOG_WARN, "[ConfigParser] YAML parsing error in string: " + std::string(E.what()));
        return false;
    }
}

int ConfigParser::ParseCommandLineArguments(int _ArgC, char **_ArgV)
{
    Positionals_.clear();
    bool AfterDoubleDash = false;

    for (int I = 1; I < _ArgC; ++I)
    {
        std::string Arg = _ArgV[I];

        if (AfterDoubleDash)
        {
            Positionals_.push_back(Arg);
            LOG(LOG_INFO, "[ConfigParser] Positional argument = '" + Arg + "'");
            continue;
        }

        if (Arg == "--")
        {
            AfterDoubleDash = true;
            LOG(LOG_INFO, "[ConfigParser] Found '--', remaining args are positional");
            continue;
        }

        // Handle flags starting with '-'
        if (Arg[0] == '-')
        {
            size_t DashCount = 0;
            while (DashCount < Arg.size() && Arg[DashCount] == '-')
            {
                DashCount++;
            }
            std::string Key = Arg.substr(DashCount);

            if (Key.empty())
            {
                LOG(LOG_WARN, "[ConfigParser] Ignoring empty option '" + Arg + "'");
                continue;
            }

            // Case 1: --key=value
            if (auto EqPos = Key.find('='); EqPos != std::string::npos)
            {
                std::string Val = Key.substr(EqPos + 1);
                std::string K = Key.substr(0, EqPos);
                Value Value = ParseScalar(Val);
                SetValue(K, Value);
                LOG(LOG_INFO, "[ConfigParser] CLI override " + K + " = " + ValueToString(Value));
            }
            // Case 2: --key nextArg (if next arg is not a flag)
            else if (I + 1 < _ArgC && _ArgV[I + 1][0] != '-')
            {
                std::string Val = _ArgV[++I];
                Value Value = ParseScalar(Val);
                SetValue(Key, Value);
                LOG(LOG_INFO, "[ConfigParser] CLI override " + Key + " = " + ValueToString(Value));
            }
            // Case 3: --key (flag → true)
            else
            {
                SetValue(Key, true);
                LOG(LOG_INFO, "[ConfigParser] CLI flag set " + Key + " = true");
            }
        }
        else
        {
            Positionals_.push_back(Arg);
            LOG(LOG_INFO, "[ConfigParser] Positional argument = '" + Arg + "'");
        }
    }

    return 0;
}

void ConfigParser::SetValue(const std::string &_Key, const Value &_Value)
{
    Store_[_Key] = _Value;
    Defaults_.insert(_Key);
    LOG(LOG_DEBUG, "[ConfigParser] SetValue('" + _Key + "') = " + ValueToString(_Value));
}

ConfigParser::Value ConfigParser::GetValue(const std::string &_Key) const
{
    auto It = Store_.find(_Key);
    if (It != Store_.end())
    {
        LOG(LOG_DEBUG, "[ConfigParser] GetValue('" + _Key + "') found");
        return It->second;
    }
    LOG(LOG_DEBUG, "[ConfigParser] GetValue('" + _Key + "') not found");
    return {};
}

std::map<std::string, ConfigParser::Value> ConfigParser::GetAllValuesUnderPath(const std::string &_PathPrefix) const
{
    std::map<std::string, Value> Result;
    std::string Prefix = _PathPrefix;

    // Ensure prefix ends with '.' for proper path matching, unless it's empty
    if (!Prefix.empty() && Prefix.back() != '.')
    {
        Prefix += '.';
    }

    for (const auto &Kv : Store_)
    {
        const std::string &Key = Kv.first;

        // If prefix is empty, include all keys
        // Otherwise, include keys that start with the prefix
        if (Prefix.empty() || Key.substr(0, Prefix.length()) == Prefix)
        {
            Result[Key] = Kv.second;
        }
    }

    LOG(LOG_DEBUG, "[ConfigParser] GetAllValuesUnderPath('" + _PathPrefix + "') found " + std::to_string(Result.size()) + " values");
    return Result;
}

nlohmann::json ConfigParser::ConvertPathsToJson(const std::map<std::string, Value> &_Values) const
{
    nlohmann::json Result = nlohmann::json::object();

    for (const auto &Kv : _Values)
    {
        const std::string &Path = Kv.first;
        const Value &Val = Kv.second;

        // Split path by dots
        std::vector<std::string> PathParts;
        std::string Current = Path;
        size_t Pos = 0;

        while ((Pos = Current.find('.')) != std::string::npos)
        {
            PathParts.push_back(Current.substr(0, Pos));
            Current = Current.substr(Pos + 1);
        }
        PathParts.push_back(Current);

        // Navigate/create nested structure
        nlohmann::json *CurrentNode = &Result;
        for (size_t I = 0; I < PathParts.size() - 1; ++I)
        {
            const std::string &Part = PathParts[I];

            // Handle array indices like "key[0]"
            size_t BracketPos = Part.find('[');
            if (BracketPos != std::string::npos)
            {
                std::string ArrayKey = Part.substr(0, BracketPos);
                std::string IndexStr = Part.substr(BracketPos + 1);
                IndexStr.pop_back(); // Remove ']'
                int Index = std::stoi(IndexStr);

                if (!(*CurrentNode).contains(ArrayKey))
                {
                    (*CurrentNode)[ArrayKey] = nlohmann::json::array();
                }

                // Ensure array is large enough
                while ((*CurrentNode)[ArrayKey].size() <= Index)
                {
                    (*CurrentNode)[ArrayKey].push_back(nlohmann::json::object());
                }

                CurrentNode = &((*CurrentNode)[ArrayKey][Index]);
            }
            else
            {
                if (!(*CurrentNode).contains(Part))
                {
                    (*CurrentNode)[Part] = nlohmann::json::object();
                }
                CurrentNode = &((*CurrentNode)[Part]);
            }
        }

        // Set the final value
        const std::string &FinalKey = PathParts.back();

        // Handle array indices in final key
        size_t BracketPos = FinalKey.find('[');
        if (BracketPos != std::string::npos)
        {
            std::string ArrayKey = FinalKey.substr(0, BracketPos);
            std::string IndexStr = FinalKey.substr(BracketPos + 1);
            IndexStr.pop_back(); // Remove ']'
            int Index = std::stoi(IndexStr);

            if (!(*CurrentNode).contains(ArrayKey))
            {
                (*CurrentNode)[ArrayKey] = nlohmann::json::array();
            }

            // Ensure array is large enough
            while ((*CurrentNode)[ArrayKey].size() <= Index)
            {
                (*CurrentNode)[ArrayKey].push_back(nullptr);
            }

            // Convert Value to JSON value
            if (auto P = std::get_if<bool>(&Val))
            {
                (*CurrentNode)[ArrayKey][Index] = *P;
            }
            else if (auto P = std::get_if<long long>(&Val))
            {
                (*CurrentNode)[ArrayKey][Index] = *P;
            }
            else if (auto P = std::get_if<double>(&Val))
            {
                (*CurrentNode)[ArrayKey][Index] = *P;
            }
            else if (auto P = std::get_if<std::string>(&Val))
            {
                (*CurrentNode)[ArrayKey][Index] = *P;
            }
        }
        else
        {
            // Convert Value to JSON value
            if (auto P = std::get_if<bool>(&Val))
            {
                (*CurrentNode)[FinalKey] = *P;
            }
            else if (auto P = std::get_if<long long>(&Val))
            {
                (*CurrentNode)[FinalKey] = *P;
            }
            else if (auto P = std::get_if<double>(&Val))
            {
                (*CurrentNode)[FinalKey] = *P;
            }
            else if (auto P = std::get_if<std::string>(&Val))
            {
                (*CurrentNode)[FinalKey] = *P;
            }
        }
    }

    LOG(LOG_DEBUG, "[ConfigParser] ConvertPathsToJson() converted " + std::to_string(_Values.size()) + " paths to JSON");
    return Result;
}

void ConfigParser::ParseYamlNode(const YAML::Node &_Node, const std::string &_CurrentPath)
{
    if (_Node.IsMap())
    {
        for (auto It = _Node.begin(); It != _Node.end(); ++It)
        {
            std::string Key = It->first.as<std::string>();
            std::string NewPath = _CurrentPath.empty() ? Key : (_CurrentPath + "." + Key);
            ParseYamlNode(It->second, NewPath);
        }
    }
    else if (_Node.IsSequence())
    {
        for (size_t I = 0; I < _Node.size(); ++I)
        {
            std::string NewPath = _CurrentPath + "[" + std::to_string(I) + "]";
            ParseYamlNode(_Node[I], NewPath);
        }
    }
    else if (_Node.IsScalar())
    {
        std::string ValueStr = _Node.as<std::string>();
        Store_[_CurrentPath] = ParseScalar(ValueStr);
        LOG(LOG_DEBUG, "[ConfigParser] Loaded YAML '" + _CurrentPath + "' = \"" + ValueStr + "\"");
    }
}

ConfigParser::Value ConfigParser::ParseScalar(const std::string &_Value)
{
    if (_Value == "true")
        return true;
    if (_Value == "false")
        return false;

    try
    {
        size_t Pos = 0;
        long long IntVal = std::stoll(_Value, &Pos);
        if (Pos == _Value.length())
        {
            return IntVal;
        }
    }
    catch (...)
    {
    }

    try
    {
        size_t Pos = 0;
        double DoubleVal = std::stod(_Value, &Pos);
        if (Pos == _Value.length())
        {
            return DoubleVal;
        }
    }
    catch (...)
    {
    }

    return _Value;
}

void ConfigParser::Dump(std::ostream &_Os) const
{
    std::vector<std::string> Keys;
    for (const auto &Kv : Store_)
    {
        Keys.push_back(Kv.first);
    }
    std::sort(Keys.begin(), Keys.end());

    for (const auto &Key : Keys)
    {
        _Os << Key << ": " << ValueToString(Store_.at(Key));
        if (Defaults_.count(Key))
        {
            _Os << "  (default)";
        }
        _Os << '\n';
    }

    if (!Positionals_.empty())
    {
        _Os << "positional: [";
        for (size_t I = 0; I < Positionals_.size(); ++I)
        {
            if (I > 0)
                _Os << ", ";
            _Os << Positionals_[I];
        }
        _Os << "]\n";
    }
}

std::string ConfigParser::ValueToString(const Value &_Value) const
{
    if (auto P = std::get_if<bool>(&_Value))
    {
        return *P ? "true" : "false";
    }
    if (auto P = std::get_if<long long>(&_Value))
    {
        return std::to_string(*P);
    }
    if (auto P = std::get_if<double>(&_Value))
    {
        std::ostringstream Oss;
        Oss << *P;
        return Oss.str();
    }
    if (auto P = std::get_if<std::string>(&_Value))
    {
        return '"' + *P + '"';
    }
    return "<unknown>";
}

const std::vector<std::string> &ConfigParser::GetPositionals() const
{
    return Positionals_;
}

YAML::Node LoadYamlFromFile(const std::string &_FilePath)
{
    return YAML::LoadFile(_FilePath);
}
