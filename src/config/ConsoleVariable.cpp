#include "ConsoleVariable.h"

#include <functional>
#include "utils/filesystemtools/DiskFile.h"
#include <utils/Utils.h>
#include "config/Config.h"
#include "Context.h"

namespace Ship {

ConsoleVariable::ConsoleVariable() {
    Load();
}

ConsoleVariable::~ConsoleVariable() {
    SPDLOG_TRACE("destruct console variables");
}

std::shared_ptr<CVar> ConsoleVariable::Get(const char* name) {
    auto it = mVariables.find(name);
    return it != mVariables.end() ? it->second : nullptr;
}

int32_t ConsoleVariable::GetInteger(const char* name, int32_t defaultValue) {
    auto variable = Get(name);

    if (variable != nullptr) {
        assert(variable->Type == ConsoleVariableType::Integer);
        return variable->GetInteger();
    }

    return defaultValue;
}

float ConsoleVariable::GetFloat(const char* name, float defaultValue) {
    auto variable = Get(name);

    if (variable != nullptr) {
        assert(variable->Type == ConsoleVariableType::Float);
        return variable->GetFloat();
    }

    return defaultValue;
}

const char* ConsoleVariable::GetString(const char* name, const char* defaultValue) {
    auto variable = Get(name);

    if (variable != nullptr) {
        assert(variable->Type == ConsoleVariableType::String);
        return variable->GetString();
    }

    return defaultValue;
}

Color_RGBA8 ConsoleVariable::GetColor(const char* name, Color_RGBA8 defaultValue) {
    auto variable = Get(name);

    if (variable != nullptr) {
        assert(variable->Type == ConsoleVariableType::Color);
        return variable->GetColor();
    }

    return defaultValue;
}

Color_RGB8 ConsoleVariable::GetColor24(const char* name, Color_RGB8 defaultValue) {
    auto variable = Get(name);

    if (variable != nullptr) {
        assert(variable->Type == ConsoleVariableType::Color24);
        return variable->GetColor24();
    }

    return defaultValue;
}

void ConsoleVariable::SetInteger(const char* name, int32_t value) {
    auto& variable = mVariables[name];
    if (variable == nullptr) {
        variable = std::make_shared<CVar>();
        variable->Type = ConsoleVariableType::Integer;
        variable->Default = value;
    }

    variable->Value = value;
}

void ConsoleVariable::SetFloat(const char* name, float value) {
    auto& variable = mVariables[name];
    if (variable == nullptr) {
        variable = std::make_shared<CVar>();
        variable->Type = ConsoleVariableType::Float;
        variable->Default = value;
    }

    variable->Value = value;
}

void ConsoleVariable::SetString(const char* name, const char* value) {
    auto& variable = mVariables[name];
    size_t length = strlen(value);
    if (variable == nullptr) {
        variable = std::make_shared<CVar>();
        variable->Type = ConsoleVariableType::String;
        variable->Default = std::string(value);
    }

    variable->Value = std::string(value);
}

void ConsoleVariable::SetColor(const char* name, Color_RGBA8 value) {
    auto& variable = mVariables[name];
    if (!variable) {
        variable = std::make_shared<CVar>();
        variable->Type = ConsoleVariableType::Color;
        variable->Default = value;
    }

    variable->Value = value;
}

void ConsoleVariable::SetColor24(const char* name, Color_RGB8 value) {
    auto& variable = mVariables[name];
    if (!variable) {
        variable = std::make_shared<CVar>();
        variable->Type = ConsoleVariableType::Color24;
        variable->Default = value;
    }

    variable->Value = value;
}

void ConsoleVariable::RegisterInteger(const char* name, int32_t defaultValue) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();

        variable->Type = ConsoleVariableType::Integer;
        variable->Value = variable->Default = defaultValue;

        mVariables.insert({ name, variable });
    }
}

void ConsoleVariable::RegisterFloat(const char* name, float defaultValue) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();

        variable->Type = ConsoleVariableType::Float;
        variable->Value = variable->Default = defaultValue;

        mVariables.insert({ name, variable });
    }
}

void ConsoleVariable::RegisterString(const char* name, const char* defaultValue) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();
        size_t length = strlen(defaultValue);

        variable->Type = ConsoleVariableType::String;

        variable->Value = variable->Default = std::string(defaultValue);

        mVariables.insert({ name, variable });
    }
}

void ConsoleVariable::RegisterColor(const char* name, Color_RGBA8 defaultValue) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();

        variable->Type = ConsoleVariableType::Color;
        variable->Value = variable->Default = defaultValue;

        mVariables.insert({ name, variable });
    }
}

void ConsoleVariable::RegisterColor24(const char* name, Color_RGB8 defaultValue) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();

        variable->Type = ConsoleVariableType::Color24;
        variable->Value = variable->Default = defaultValue;

        mVariables.insert({ name, variable });
    }
}

void ConsoleVariable::RegisterInteger(const char* name, int32_t* valuePtr) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();

        variable->Type = ConsoleVariableType::Integer;
        variable->Default = *valuePtr;
        variable->Ptr = static_cast<void*>(valuePtr);

        mVariables.insert({ name, variable });
    }
}
void ConsoleVariable::RegisterFloat(const char* name, float* valuePtr) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();

        variable->Type = ConsoleVariableType::Float;
        variable->Default = *valuePtr;
        variable->Ptr = static_cast<void*>(valuePtr);

        mVariables.insert({ name, variable });
    }
}
void ConsoleVariable::RegisterString(const char* name, const char** valuePtr) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();
        size_t length = strlen(*valuePtr);

        variable->Type = ConsoleVariableType::String;
        variable->Ptr = static_cast<void*>(valuePtr);

        variable->Default = std::string(*valuePtr);

        mVariables.insert({ name, variable });
    }
}
void ConsoleVariable::RegisterColor(const char* name, Color_RGBA8* valuePtr) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();

        variable->Type = ConsoleVariableType::Color;
        variable->Default = *valuePtr;
        variable->Ptr = static_cast<void*>(valuePtr);

        mVariables.insert({ name, variable });
    }
}
void ConsoleVariable::RegisterColor24(const char* name, Color_RGB8* valuePtr) {
    if (!mVariables.contains(name)) {
        std::shared_ptr<CVar> variable = std::make_shared<CVar>();

        variable->Type = ConsoleVariableType::Color24;
        variable->Default = *valuePtr;
        variable->Ptr = static_cast<void*>(valuePtr);

        mVariables.insert({ name, variable });
    }
}

void ConsoleVariable::ClearVariable(const char* name) {
    std::shared_ptr<Config> conf = Context::GetInstance()->GetConfig();
    auto var = Get(name);
    if (var != nullptr) {
        bool color = var->Type == ConsoleVariableType::Color || var->Type == ConsoleVariableType::Color24;
        if (color) {
            std::string a = StringHelper::Sprintf("%s.%s", name, "A");
            std::string b = StringHelper::Sprintf("%s.%s", name, "B");
            std::string g = StringHelper::Sprintf("%s.%s", name, "G");
            std::string r = StringHelper::Sprintf("%s.%s", name, "R");
            std::string t = StringHelper::Sprintf("%s.%s", name, "Type");
            mVariables.erase(a);
            mVariables.erase(b);
            mVariables.erase(g);
            mVariables.erase(r);
            mVariables.erase(t);
            conf->Erase(std::string("CVars.") + a);
            conf->Erase(std::string("CVars.") + b);
            conf->Erase(std::string("CVars.") + g);
            conf->Erase(std::string("CVars.") + r);
            conf->Erase(std::string("CVars.") + t);
        }
    }
    mVariables.erase(name);
    conf->Erase(StringHelper::Sprintf("CVars.%s", name));
}

void ConsoleVariable::ClearBlock(const char* name) {
    std::shared_ptr<Config> conf = Context::GetInstance()->GetConfig();
    conf->EraseBlock(StringHelper::Sprintf("CVars.%s", name));
    Load();
}

void ConsoleVariable::CopyVariable(const char* from, const char* to) {
    auto& variableFrom = mVariables[from];
    if (!variableFrom) {
        return;
    }
    auto& variableTo = mVariables[to];
    if (!variableTo) {
        variableTo = std::make_shared<CVar>();
    }

    variableTo->Type = variableFrom->Type;
    variableTo->Ptr = variableTo->Ptr;
    variableTo->Value = variableFrom->Value;
    variableTo->Default = variableFrom->Default;
}

void ConsoleVariable::Save() {
    std::shared_ptr<Config> conf = Context::GetInstance()->GetConfig();

    for (const auto& variable : mVariables) {
        const std::string key = StringHelper::Sprintf("CVars.%s", variable.first.c_str());

        bool usePtr = variable.second->Ptr != nullptr;

        switch (variable.second->Type) {
            case ConsoleVariableType::String:
            {
                if (variable.second->GetString() != *std::get_if<std::string>(&variable.second->Default)) {
                    conf->SetString(key, std::string(variable.second->GetString()));
                }
            }
            case ConsoleVariableType::Integer:
            {
                if (variable.second->GetInteger() != *std::get_if<int32_t>(&variable.second->Default)) {
                    conf->SetInt(key, variable.second->GetInteger());
                }
            }
            case ConsoleVariableType::Float:
            {
                if (variable.second->GetFloat() != *std::get_if<float>(&variable.second->Default)) {
                    conf->SetFloat(key, variable.second->GetFloat());
                }
            }
            case ConsoleVariableType::Color:
            {
                if (variable.second->GetColor() != *std::get_if<Color_RGBA8>(&variable.second->Default)) {
                    const char* keyStr = key.c_str();
                    Color_RGBA8 color = variable.second->GetColor();
                    conf->SetUInt(StringHelper::Sprintf("%s.R", keyStr), color.r);
                    conf->SetUInt(StringHelper::Sprintf("%s.G", keyStr), color.g);
                    conf->SetUInt(StringHelper::Sprintf("%s.B", keyStr), color.b);
                    conf->SetUInt(StringHelper::Sprintf("%s.A", keyStr), color.a);
                    conf->SetString(StringHelper::Sprintf("%s.Type", keyStr), "RGBA");
                }
            }
            case ConsoleVariableType::Color24:
            {
                if (variable.second->GetColor24() != *std::get_if<Color_RGB8>(&variable.second->Default)) {
                    const char* keyStr = key.c_str();
                    Color_RGB8 color = variable.second->GetColor24();
                    conf->SetUInt(StringHelper::Sprintf("%s.R", keyStr), color.r);
                    conf->SetUInt(StringHelper::Sprintf("%s.G", keyStr), color.g);
                    conf->SetUInt(StringHelper::Sprintf("%s.B", keyStr), color.b);
                    conf->SetString(StringHelper::Sprintf("%s.Type", keyStr), "RGB");
                }
            }
        }
    }

    conf->Save();
}

void ConsoleVariable::Load() {
    std::shared_ptr<Config> conf = Context::GetInstance()->GetConfig();
    conf->Reload();
    if (!mVariables.empty()) {
        mVariables.clear();
    }

    LoadFromPath("", conf->GetNestedJson()["CVars"].items());

    LoadLegacy();
}

void ConsoleVariable::LoadFromPath(
    std::string path, nlohmann::detail::iteration_proxy<nlohmann::detail::iter_impl<nlohmann::json>> items) {
    if (!path.empty()) {
        path += ".";
    }

    for (const auto& item : items) {
        std::string itemPath = path + item.key();
        auto value = item.value();
        switch (value.type()) {
            case nlohmann::detail::value_t::array:
                break;
            case nlohmann::detail::value_t::object:
                if (value.contains("Type") && value["Type"].get<std::string>() == "RGBA") {
                    Color_RGBA8 clr;
                    clr.r = value["R"].get<uint8_t>();
                    clr.g = value["G"].get<uint8_t>();
                    clr.b = value["B"].get<uint8_t>();
                    clr.a = value["A"].get<uint8_t>();
                    SetColor(itemPath.c_str(), clr);
                } else if (value.contains("Type") && value["Type"].get<std::string>() == "RGB") {
                    Color_RGB8 clr;
                    clr.r = value["R"].get<uint8_t>();
                    clr.g = value["G"].get<uint8_t>();
                    clr.b = value["B"].get<uint8_t>();
                    SetColor24(itemPath.c_str(), clr);
                } else {
                    LoadFromPath(itemPath, value.items());
                }

                break;
            case nlohmann::detail::value_t::string:
                SetString(itemPath.c_str(), value.get<std::string>().c_str());
                break;
            case nlohmann::detail::value_t::boolean:
                SetInteger(itemPath.c_str(), value.get<bool>());
                break;
            case nlohmann::detail::value_t::number_unsigned:
            case nlohmann::detail::value_t::number_integer:
                SetInteger(itemPath.c_str(), value.get<int>());
                break;
            case nlohmann::detail::value_t::number_float:
                SetFloat(itemPath.c_str(), value.get<float>());
                break;
            default:;
        }
    }
}
void ConsoleVariable::LoadLegacy() {
    auto conf = Context::GetPathRelativeToAppDirectory("cvars.cfg");
    if (DiskFile::Exists(conf)) {
        const auto lines = DiskFile::ReadAllLines(conf);

        for (const std::string& line : lines) {
            std::vector<std::string> cfg = StringHelper::Split(line, " = ");
            if (line.empty()) {
                continue;
            }
            if (cfg.size() < 2) {
                continue;
            }

            if (cfg[1].find("\"") == std::string::npos && (cfg[1].find("#") != std::string::npos)) {
                std::string value(cfg[1]);
                value.erase(std::remove_if(value.begin(), value.end(), [](char c) { return c == '#'; }), value.end());
                auto splitTest = StringHelper::Split(value, "\r")[0];

                uint32_t val = std::stoul(splitTest, nullptr, 16);
                Color_RGBA8 clr;
                clr.r = val >> 24;
                clr.g = val >> 16;
                clr.b = val >> 8;
                clr.a = val & 0xFF;
                SetColor(cfg[0].c_str(), clr);
            }

            if (cfg[1].find("\"") != std::string::npos) {
                std::string value(cfg[1]);
                value.erase(std::remove(value.begin(), value.end(), '\"'), value.end());
#ifdef _MSC_VER
                SetString(cfg[0].c_str(), _strdup(value.c_str()));
#else
                SetString(cfg[0].c_str(), strdup(value.c_str()));
#endif
            }
            if (Math::IsNumber<float>(cfg[1])) {
                SetFloat(cfg[0].c_str(), std::stof(cfg[1]));
            }
            if (Math::IsNumber<int>(cfg[1])) {
                SetInteger(cfg[0].c_str(), std::stoi(cfg[1]));
            }
        }

        fs::remove(conf);
    }
}
} // namespace Ship
