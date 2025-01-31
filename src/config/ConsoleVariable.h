#pragma once

#include "libultraship/color.h"
#include <nlohmann/json.hpp>
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <variant>

namespace Ship {
typedef enum class ConsoleVariableType { Integer, Float, String, Color, Color24 } ConsoleVariableType;

class CVar {
public:
    using ValueType = std::variant<int32_t, float, std::string, Color_RGBA8, Color_RGB8>;

    int32_t GetInteger() const { return Ptr != nullptr ? *static_cast<int32_t*>(Ptr) : *std::get_if<int32_t>(&Value); }
    float GetFloat() const { return Ptr != nullptr ? *static_cast<float*>(Ptr) : *std::get_if<float>(&Value); }
    const char* GetString() const { return Ptr != nullptr ? *static_cast<char**>(Ptr) : std::get_if<std::string>(&Value)->c_str(); }
    Color_RGBA8 GetColor() const { return Ptr != nullptr ? *static_cast<Color_RGBA8*>(Ptr) : *std::get_if<Color_RGBA8>(&Value); }
    Color_RGB8 GetColor24() const { return Ptr != nullptr ? *static_cast<Color_RGB8*>(Ptr) : *std::get_if<Color_RGB8>(&Value); }

    std::string Name;
    ConsoleVariableType Type;

    void* Ptr = nullptr;
    ValueType Value;
    ValueType Default;
};

class ConsoleVariable {
  public:
    ConsoleVariable();
    ~ConsoleVariable();

    std::shared_ptr<CVar> Get(const char* name);

    int32_t GetInteger(const char* name, int32_t defaultValue);
    float GetFloat(const char* name, float defaultValue);
    const char* GetString(const char* name, const char* defaultValue);
    Color_RGBA8 GetColor(const char* name, Color_RGBA8 defaultValue);
    Color_RGB8 GetColor24(const char* name, Color_RGB8 defaultValue);

    void SetInteger(const char* name, int32_t value);
    void SetFloat(const char* name, float value);
    void SetString(const char* name, const char* value);
    void SetColor(const char* name, Color_RGBA8 value);
    void SetColor24(const char* name, Color_RGB8 value);

    void RegisterInteger(const char* name, int32_t defaultValue);
    void RegisterFloat(const char* name, float defaultValue);
    void RegisterString(const char* name, const char* defaultValue);
    void RegisterColor(const char* name, Color_RGBA8 defaultValue);
    void RegisterColor24(const char* name, Color_RGB8 defaultValue);

    void RegisterInteger(const char* name, int32_t* valuePtr);
    void RegisterFloat(const char* name, float* valuePtr);
    void RegisterString(const char* name, const char** valuePtr);
    void RegisterColor(const char* name, Color_RGBA8* valuePtr);
    void RegisterColor24(const char* name, Color_RGB8* valuePtr);

    void ClearVariable(const char* name);
    void ClearBlock(const char* name);
    void CopyVariable(const char* from, const char* to);

    void Save();
    void Load();

  protected:
    void LoadFromPath(std::string path,
                      nlohmann::detail::iteration_proxy<nlohmann::detail::iter_impl<nlohmann::json>> items);
    void LoadLegacy();

  private:
    std::unordered_map<std::string, std::shared_ptr<CVar>> mVariables;
};
} // namespace Ship
