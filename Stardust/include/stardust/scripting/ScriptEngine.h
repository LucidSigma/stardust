#pragma once
#ifndef STARDUST_SCRIPT_ENGINE_H
#define STARDUST_SCRIPT_ENGINE_H

#include <functional>
#include <type_traits>
#include <utility>

#include <sol/sol.hpp>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    constexpr sol::nil_t Nil = sol::nil;

    class ScriptEngine
    {
    private:
        sol::state m_luaState{ };

    public:
        ScriptEngine() = default;
        ~ScriptEngine() noexcept = default;

        void Initialise(class Application& application);

        [[nodiscard]] Status LoadScript(const StringView& filename);

        template <typename T>
        [[nodiscard]] inline T Get(const StringView& variableName)
        {
            return m_luaState.get<T>(variableName);
        }

        template <typename T>
        inline void Set(const StringView& variableName, const T& value)
        {
            m_luaState[variableName] = value;
        }

        template <typename... Args>
        inline void CreateTable(const StringView& name, Args&&... values)
        {
            m_luaState.create_named_table(name, std::forward<Args>(values)...);
        }

        template <typename T>
        [[nodiscard]] inline std::function<T> GetFunction(const StringView& functionName) const
        {
            return static_cast<std::function<T>>(m_luaState[functionName].get<sol::function>());
        }

        template <typename... Args>
        void SetFunction(const StringView& functionName, Args&&... values)
        {
            m_luaState.set_function(functionName, values...);
        }

        template <typename ReturnType, typename... Args>
        [[nodiscard]] ReturnType CallFunction(const StringView& name, Args&&... args) const
        {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                GetFunction<ReturnType(Args...)>(name)(std::forward<Args>(args)...);
            }
            else
            {
                return GetFunction<ReturnType(Args...)>(name)(std::forward<Args>(args)...);
            }
        }

        [[nodiscard]] inline decltype(auto) operator [](const StringView& variableName) { return m_luaState[variableName]; }

        [[nodiscard]] inline sol::state& GetState() noexcept { return m_luaState; }
        [[nodiscard]] inline const sol::state& GetState() const noexcept { return m_luaState; }

    private:
        template <typename T, typename Constructors>
        void LoadVectorType()
        {
            String vectorTypeName = "vec";

            if constexpr (std::is_same_v<T, Vec2>)
            {
                vectorTypeName += "2";
            }
            else if constexpr (std::is_same_v<T, Vec3>)
            {
                vectorTypeName += "3";
            }
            else if constexpr (std::is_same_v<T, Vec4>)
            {
                vectorTypeName += "4";
            }
            else
            {
                static_assert(false, "Invalid vector type.");
            }

            sol::usertype<T> vectorType = m_luaState.new_usertype<T>(
                vectorTypeName, Constructors{ },
                "x", &T::x,
                "y", &T::y,
                "normalise", [](const T& self) -> T { return glm::normalize(self); },
                "length", [](const T& self) -> f32 { return glm::length(self); },
                "distance", [](const T& self, const T& other) -> f32 { return glm::distance(self, other); },
                "dot", [](const T& self, const T& other) -> f32 { return glm::dot(self, other); },
                sol::meta_function::addition, sol::resolve<T(const T&, const T&)>(glm::operator +),
                sol::meta_function::subtraction, sol::resolve<T(const T&, const T&)>(glm::operator -),
                sol::meta_function::multiplication, sol::resolve<T(const T&, const T&)>(glm::operator *),
                sol::meta_function::division, sol::resolve<T(const T&, const T&)>(glm::operator /)
            );

            if constexpr (!std::is_same_v<T, Vec4>)
            {
                vectorType.set_function(sol::meta_function::addition, sol::resolve<T(const T&, f32)>(glm::operator +));
                vectorType.set_function(sol::meta_function::subtraction, sol::resolve<T(const T&, f32)>(glm::operator -));
                vectorType.set_function(sol::meta_function::multiplication, sol::resolve<T(const T&, const f32)>(glm::operator *));
                vectorType.set_function(sol::meta_function::division, sol::resolve<T(const T&, const f32)>(glm::operator /));
            }

            if constexpr (std::is_same_v<T, Vec3> || std::is_same_v<T, Vec4>)
            {
                vectorType.set_function("z", &T::z);
            }

            if constexpr (std::is_same_v<T, Vec4>)
            {
                vectorType.set_function("w", &T::w);
            }

            if constexpr (std::is_same_v<T, Vec3>)
            {
                vectorType.set_function("cross", [](const T& self, const T& other) -> T { return glm::cross(self, other); });
            }
        }
    };
}

#endif