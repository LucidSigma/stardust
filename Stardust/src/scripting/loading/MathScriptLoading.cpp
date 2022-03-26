#include "stardust/scripting/ScriptEngine.h"

#include <algorithm>

#include "stardust/application/Application.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace
    {
        template <typename V>
        concept FloatingPointVectorType = std::is_same_v<V, Vector2> ||
            std::is_same_v<V, Vector3> ||
            std::is_same_v<V, Vector4>;

        template <typename V>
        concept IntegerVectorType = std::is_same_v<V, IVector2> ||
            std::is_same_v<V, IVector3> ||
            std::is_same_v<V, IVector4> ||
            std::is_same_v<V, UVector2> ||
            std::is_same_v<V, UVector3> ||
            std::is_same_v<V, UVector4>;

        template <typename U>
        concept IntegerVectorUnderlyingType = std::is_same_v<U, i32> || std::is_same_v<U, u32>;

        template <FloatingPointVectorType V, typename Constructors>
        auto LoadVectorType(sol::state& luaState) -> void
        {
            String vectorTypeName = "vector";

            if constexpr (std::is_same_v<V, Vector2>)
            {
                vectorTypeName += "2";
            }
            else if constexpr (std::is_same_v<V, Vector3>)
            {
                vectorTypeName += "3";
            }
            else if constexpr (std::is_same_v<V, Vector4>)
            {
                vectorTypeName += "4";
            }

            sol::usertype<V> vectorType = luaState.new_usertype<V>(
                vectorTypeName,
                Constructors{ },
                "x", &V::x,
                "y", &V::y,
                "normalise", [](const V& self) -> V { return glm::normalize(self); },
                "length", [](const V& self) -> f32 { return glm::length(self); },
                "distance", [](const V& self, const V& other) -> f32 { return glm::distance(self, other); },
                "dot", [](const V& self, const V& other) -> f32 { return glm::dot(self, other); }
            );

            if constexpr (!std::is_same_v<V, Vector4>)
            {
                vectorType.set_function(sol::meta_function::addition, sol::overload(
                    sol::resolve<auto(const V&, const V&) -> V>(glm::operator +),
                    sol::resolve<auto(const V&, f32) -> V>(glm::operator +),
                    sol::resolve<auto(f32, const V&) -> V>(glm::operator +)
                ));
                vectorType.set_function(sol::meta_function::subtraction, sol::overload(
                    sol::resolve<auto(const V&, const V&) -> V>(glm::operator -),
                    sol::resolve<auto(const V&, f32) -> V>(glm::operator -),
                    sol::resolve<auto(f32, const V&) -> V>(glm::operator -)
                ));
                vectorType.set_function(sol::meta_function::multiplication, sol::overload(
                    sol::resolve<auto(const V&, const V&) -> V>(glm::operator *),
                    sol::resolve<auto(const V&, f32) -> V>(glm::operator *),
                    sol::resolve<auto(f32, const V&) -> V>(glm::operator *)
                ));
                vectorType.set_function(sol::meta_function::division, sol::overload(
                    sol::resolve<auto(const V&, const V&) -> V>(glm::operator /),
                    sol::resolve<auto(const V&, f32) -> V>(glm::operator /),
                    sol::resolve<auto(f32, const V&) -> V>(glm::operator /)
                ));
            }
            else
            {
                vectorType.set_function(sol::meta_function::addition, sol::resolve<auto(const V&, const V&) -> V>(glm::operator +));
                vectorType.set_function(sol::meta_function::subtraction, sol::resolve<auto(const V&, const V&) -> V>(glm::operator -));
                vectorType.set_function(sol::meta_function::multiplication, sol::resolve<auto(const V&, const V&) -> V>(glm::operator *));
                vectorType.set_function(sol::meta_function::division, sol::resolve<auto(const V&, const V&) -> V>(glm::operator /));
            }

            if constexpr (std::is_same_v<V, Vector3> || std::is_same_v<V, Vector4>)
            {
                vectorType.set_function("z", &V::z);
            }

            if constexpr (std::is_same_v<V, Vector4>)
            {
                vectorType.set_function("w", &V::w);
            }

            if constexpr (std::is_same_v<V, Vector2>)
            {
                vectorType.set_function("to_int", [](const V& self) -> IVector2 { return IVector2{ self.x, self.y }; });
                vectorType.set_function("to_unsigned_int", [](const V& self) -> UVector2 { return UVector2{ self.x, self.y }; });

                luaState[vectorTypeName].get<Table>().set(
                    "ZERO", Vector2Zero,
                    "ONE", Vector2One,
                    "UP", Vector2Up,
                    "DOWN", Vector2Down,
                    "LEFT", Vector2Left,
                    "RIGHT", Vector2Right
                );
            }
            else if constexpr (std::is_same_v<V, Vector3>)
            {
                vectorType.set_function("cross", [](const V& self, const V& other) -> V { return glm::cross(self, other); });
                vectorType.set_function("to_int", [](const V& self) -> IVector3 { return IVector3{ self.x, self.y, self.z }; });
                vectorType.set_function("to_unsigned_int", [](const V& self) -> UVector3 { return UVector3{ self.x, self.y, self.z }; });

                luaState[vectorTypeName].get<Table>().set(
                    "ZERO", Vector3Zero,
                    "ONE", Vector3One,
                    "UP", Vector3Up,
                    "DOWN", Vector3Down,
                    "LEFT", Vector3Left,
                    "RIGHT", Vector3Right,
                    "FORWARD", Vector3Forward,
                    "BACK", Vector3Back
                );
            }
            else if constexpr (std::is_same_v<V, Vector4>)
            {
                vectorType.set_function("to_int", [](const V& self) -> IVector4 { return IVector4{ self.x, self.y, self.z, self.w }; });
                vectorType.set_function("to_unsigned_int", [](const V& self) -> UVector4 { return UVector4{ self.x, self.y, self.z, self.w }; });

                luaState[vectorTypeName].get<Table>().set(
                    "ZERO", Vector4Zero,
                    "ONE", Vector4One
                );
            }
        }

        template <IntegerVectorType V, IntegerVectorUnderlyingType Underlying, typename Constructors>
        auto LoadIntegerVectorType(sol::state& luaState) -> void
        {
            String vectorTypeName;

            if constexpr (std::is_same_v<Underlying, i32>)
            {
                vectorTypeName = "ivector";
            }
            else if constexpr (std::is_same_v<Underlying, u32>)
            {
                vectorTypeName = "uvector";
            }

            if constexpr (std::is_same_v<V, IVector2> || std::is_same_v<V, UVector2>)
            {
                vectorTypeName += "2";
            }
            else if constexpr (std::is_same_v<V, IVector3> || std::is_same_v<V, UVector3>)
            {
                vectorTypeName += "3";
            }
            else if constexpr (std::is_same_v<V, IVector4> || std::is_same_v<V, UVector4>)
            {
                vectorTypeName += "4";
            }

            sol::usertype<V> vectorType = luaState.new_usertype<V>(
                vectorTypeName,
                Constructors{ },
                "x", &V::x,
                "y", &V::y
            );

            if constexpr (!std::is_same_v<V, IVector4> && !std::is_same_v<V, UVector4>)
            {
                vectorType.set_function(sol::meta_function::addition, sol::overload(
                    sol::resolve<auto(const V&, const V&) -> V>(glm::operator +),
                    sol::resolve<auto(const V&, Underlying) -> V>(glm::operator +),
                    sol::resolve<auto(Underlying, const V&) -> V>(glm::operator +)
                ));
                vectorType.set_function(sol::meta_function::subtraction, sol::overload(
                    sol::resolve<auto(const V&, const V&) -> V>(glm::operator -),
                    sol::resolve<auto(const V&, Underlying) -> V>(glm::operator -),
                    sol::resolve<auto(Underlying, const V&) -> V>(glm::operator -)
                ));
                vectorType.set_function(sol::meta_function::multiplication, sol::overload(
                    sol::resolve<auto(const V&, const V&) -> V>(glm::operator *),
                    sol::resolve<auto(const V&, Underlying) -> V>(glm::operator *),
                    sol::resolve<auto(Underlying, const V&) -> V>(glm::operator *)
                ));
                vectorType.set_function(sol::meta_function::division, sol::overload(
                    sol::resolve<auto(const V&, const V&) -> V>(glm::operator /),
                    sol::resolve<auto(const V&, Underlying) -> V>(glm::operator /),
                    sol::resolve<auto(Underlying, const V&) -> V>(glm::operator /)
                ));
            }
            else
            {
                vectorType.set_function(sol::meta_function::addition, sol::resolve<auto(const V&, const V&) -> V>(glm::operator +));
                vectorType.set_function(sol::meta_function::subtraction, sol::resolve<auto(const V&, const V&) -> V>(glm::operator -));
                vectorType.set_function(sol::meta_function::multiplication, sol::resolve<auto(const V&, const V&) -> V>(glm::operator *));
                vectorType.set_function(sol::meta_function::division, sol::resolve<auto(const V&, const V&) -> V>(glm::operator /));
            }

            if constexpr (std::is_same_v<V, IVector3> || std::is_same_v<V, IVector4> || std::is_same_v<V, UVector3> || std::is_same_v<V, UVector4>)
            {
                vectorType.set_function("z", &V::z);
            }

            if constexpr (std::is_same_v<V, IVector4> || std::is_same_v<V, UVector4>)
            {
                vectorType.set_function("w", &V::w);
            }

            if constexpr (std::is_same_v<V, IVector2> || std::is_same_v<V, UVector2>)
            {
                vectorType.set_function("to_float", [](const V& self) -> Vector2 { return Vector2{ self.x, self.y }; });

                if constexpr (std::is_same_v<V, IVector2>)
                {
                    luaState[vectorTypeName].get<Table>().set(
                        "ZERO", IVector2Zero,
                        "ONE", IVector2One,
                        "UP", IVector2Up,
                        "DOWN", IVector2Down,
                        "LEFT", IVector2Left,
                        "RIGHT", IVector2Right
                    );
                }
                else if constexpr (std::is_same_v<V, UVector2>)
                {
                    luaState[vectorTypeName].get<Table>().set(
                        "ZERO", UVector2Zero,
                        "ONE", UVector2One
                    );
                }
            }
            else if constexpr (std::is_same_v<V, IVector3> || std::is_same_v<V, UVector3>)
            {
                vectorType.set_function("to_float", [](const V& self) -> Vector3 { return Vector3{ self.x, self.y, self.z }; });

                if constexpr (std::is_same_v<V, IVector3>)
                {
                    luaState[vectorTypeName].get<Table>().set(
                        "ZERO", IVector3Zero,
                        "ONE", IVector3One,
                        "UP", IVector3Up,
                        "DOWN", IVector3Down,
                        "LEFT", IVector3Left,
                        "RIGHT", IVector3Right,
                        "FORWARD", IVector3Forward,
                        "BACK", IVector3Back
                    );
                }
                else if constexpr (std::is_same_v<V, UVector3>)
                {
                    luaState[vectorTypeName].get<Table>().set(
                        "ZERO", UVector3Zero,
                        "ONE", UVector3One
                    );
                }
            }
            else if constexpr (std::is_same_v<V, IVector4> || std::is_same_v<V, UVector4>)
            {
                vectorType.set_function("to_float", [](const V& self) -> Vector4 { return Vector4{ self.x, self.y, self.z, self.w }; });

                if constexpr (std::is_same_v<V, IVector4>)
                {
                    luaState[vectorTypeName].get<Table>().set(
                        "ZERO", IVector4Zero,
                        "ONE", IVector4One
                    );
                }
                else if constexpr (std::is_same_v<V, UVector4>)
                {
                    luaState[vectorTypeName].get<Table>().set(
                        "ZERO", UVector4Zero,
                        "ONE", UVector4One
                    );
                }
            }
        }
    }

    auto ScriptEngine::LoadMathFunctions() -> void
    {
        LoadVectorType<Vector2, sol::constructors<Vector2(), Vector2(f32), Vector2(f32, f32), Vector2(const Vector2)>>(m_luaState);
        LoadVectorType<Vector3, sol::constructors<Vector3(), Vector3(f32), Vector3(f32, f32, f32), Vector3(const Vector3)>>(m_luaState);
        LoadVectorType<Vector4, sol::constructors<Vector4(), Vector4(f32), Vector4(f32, f32, f32, f32), Vector4(const Vector4)>>(m_luaState);

        LoadIntegerVectorType<IVector2, i32, sol::constructors<IVector2(), IVector2(i32), IVector2(i32, i32), IVector2(const IVector2)>>(m_luaState);
        LoadIntegerVectorType<IVector3, i32, sol::constructors<IVector3(), IVector3(i32), IVector3(i32, i32, i32), IVector3(const IVector3)>>(m_luaState);
        LoadIntegerVectorType<IVector4, i32, sol::constructors<IVector4(), IVector4(i32), IVector4(i32, i32, i32, i32), IVector4(const IVector4)>>(m_luaState);

        LoadIntegerVectorType<UVector2, u32, sol::constructors<UVector2(), UVector2(u32), UVector2(u32, u32), UVector2(const UVector2)>>(m_luaState);
        LoadIntegerVectorType<UVector3, u32, sol::constructors<IVector3(), UVector3(u32), UVector3(u32, u32, u32), UVector3(const UVector3)>>(m_luaState);
        LoadIntegerVectorType<UVector4, u32, sol::constructors<IVector4(), UVector4(u32), UVector4(u32, u32, u32, u32), UVector4(const UVector4)>>(m_luaState);

        m_luaState.new_usertype<Quaternion>(
            "quaternion",
            sol::constructors<Quaternion(), Quaternion(f32, Vector3), Quaternion(f32, f32, f32, f32), Quaternion(const Quaternion&)>(),
            "x", &Quaternion::x,
            "y", &Quaternion::y,
            "z", &Quaternion::z,
            "w", &Quaternion::w,
            "from_angle_axis", [](const f32 angle, const Vector3 axis) -> Quaternion { return glm::angleAxis(angle, axis); }
        );

        m_luaState["quaternion"].get<Table>().set("IDENTITY", QuaterionIdentity);

        auto mathNamespace = m_luaState["math"].get_or_create<Table>();

        mathNamespace.set_function("clamp", sol::overload(
            [](const f32 value, const f32 min, const f32 max) -> f32 { return std::clamp(value, min, max); },
            [](const i32 value, const i32 min, const i32 max) -> i32 { return std::clamp(value, min, max); }
        ));
    }
}
