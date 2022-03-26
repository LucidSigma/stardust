#include "stardust/scripting/ScriptEngine.h"

#include "stardust/application/Application.h"
#include "stardust/ecs/components/ScreenTransformComponent.h"
#include "stardust/ecs/components/TransformComponent.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/geometry/Shapes.h"
#include "stardust/graphics/Graphics.h"
#include "stardust/graphics/sorting_layer/SortingLayer.h"
#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace
    {
        auto LoadCameraFunctions(sol::state& luaState, Application& application) -> void
        {
            auto cameraNamespace = luaState["camera"].get_or_create<Table>();

            cameraNamespace.set_function("position", [application = &application]() -> Vector2 { return application->GetCamera().GetPosition(); });
            cameraNamespace.set_function("position_3d", [application = &application]() -> Vector3 { return application->GetCamera().GetPosition3D(); });

            cameraNamespace.set_function("set_position", sol::overload(
                [application = &application](const Vector2 position) { return application->GetCamera().SetPosition(position); },
                [application = &application](const Vector3 position) { return application->GetCamera().SetPosition(position); },
                [application = &application](const f32 x, const f32 y) { return application->GetCamera().SetPosition(x, y); },
                [application = &application](const f32 x, const f32 y, const f32 z) { return application->GetCamera().SetPosition(x, y, z); }
            ));

            cameraNamespace.set_function("adjust_position", sol::overload(
                [application = &application](const Vector2 position) { return application->GetCamera().AdjustPosition(position); },
                [application = &application](const Vector3 position) { return application->GetCamera().AdjustPosition(position); },
                [application = &application](const f32 x, const f32 y) { return application->GetCamera().AdjustPosition(x, y); },
                [application = &application](const f32 x, const f32 y, const f32 z) { return application->GetCamera().AdjustPosition(x, y, z); }
            ));

            cameraNamespace.set_function("rotation", [application = &application]() -> f32 { return application->GetCamera().GetRotation(); });
            cameraNamespace.set_function("set_rotation", [application = &application](const f32 rotation) { return application->GetCamera().SetRotation(rotation); });
            cameraNamespace.set_function("adjust_rotation", [application = &application](const f32 rotation) { return application->GetCamera().AdjustRotation(rotation); });
            
            cameraNamespace.set_function("zoom", [application = &application]() -> f32 { return application->GetCamera().GetZoom(); });
            cameraNamespace.set_function("set_zoom", [application = &application](const f32 zoom) { return application->GetCamera().SetZoom(zoom); });
            cameraNamespace.set_function("adjust_zoom", [application = &application](const f32 zoom) { return application->GetCamera().AdjustZoom(zoom); });

            cameraNamespace.set_function("half_size", [application = &application]() -> f32 { return application->GetCamera().GetHalfSize(); });
            cameraNamespace.set_function("set_half_size", [application = &application](const f32 halfSize) { return application->GetCamera().SetHalfSize(halfSize); });
            cameraNamespace.set_function("aspect_ratio", [application = &application]() -> f32 { return application->GetCamera().GetAspectRatio(); });
            cameraNamespace.set_function("pixels_per_unit", [application = &application]() -> f32 { return application->GetCamera().GetPixelsPerUnit(); });
        }

        auto LoadWindowFunctions(sol::state& luaState, Application& application) -> void
        {
            auto windowNamespace = luaState["window"].get_or_create<Table>();

            windowNamespace.set_function("size", [application = &application]() -> UVector2 { return application->GetWindow().GetSize(); });
            windowNamespace.set_function("drawable_size", [application = &application]() -> UVector2 { return application->GetWindow().GetDrawableSize(); });
            windowNamespace.set_function("change_size", [application = &application](const UVector2 newSize) { application->GetWindow().ChangeSize(newSize); });
            windowNamespace.set_function("aspect_ratio", [application = &application]() -> f32 { return application->GetWindow().GetAspectRatio(); });
            windowNamespace.set_function("is_fullscreen", [application = &application]() -> bool { return application->GetWindow().IsFullscreen(); });
            windowNamespace.set_function("toggle_fullscreen", [application = &application]() { application->GetWindow().ToggleFullscreen(); });
            windowNamespace.set_function("is_borderless", [application = &application]() -> bool { return application->GetWindow().IsBorderless(); });
            windowNamespace.set_function("set_borderless", [application = &application](const bool isBorderless) { application->GetWindow().SetBorderless(isBorderless); });
            windowNamespace.set_function("warp_cursor", [application = &application](const IVector2 coordinates) { application->GetWindow().WarpCursor(coordinates); });
        }

        auto LoadGeometryTypes(sol::state& luaState, Table& graphicsNamespace) -> void
        {
            graphicsNamespace.new_usertype<geometry::Line>(
                "line",
                sol::constructors<
                    geometry::Line(),
                    geometry::Line(Vector2),
                    geometry::Line(Vector2, Vector2),
                    geometry::Line(const geometry::Line&)
                >(),
                "point_a", &geometry::Line::pointA,
                "point_b", &geometry::Line::pointB
            );

            graphicsNamespace.new_usertype<geometry::ScreenLine>(
                "screen_line",
                sol::constructors<
                    geometry::ScreenLine(),
                    geometry::ScreenLine(IVector2),
                    geometry::ScreenLine(IVector2, IVector2),
                    geometry::ScreenLine(const geometry::ScreenLine&)
                >(),
                "point_a", &geometry::ScreenLine::pointA,
                "point_b", &geometry::ScreenLine::pointB
            );
        }

        auto LoadTransformTypes(sol::state& luaState) -> void
        {
            luaState.new_usertype<components::Transform>(
                "transform",
                sol::constructors<
                    components::Transform(),
                    components::Transform(Vector2),
                    components::Transform(Vector2, Vector2),
                    components::Transform(Vector2, Vector2, graphics::Reflection),
                    components::Transform(Vector2, Vector2, graphics::Reflection, f32),
                    components::Transform(Vector2, Vector2, graphics::Reflection, f32, const Optional<Vector2>&),
                    components::Transform(Vector2, Vector2, graphics::Reflection, f32, const Optional<Vector2>&, const Optional<Vector2>&),
                    components::Transform(const components::Transform&)
                >(),
                "translation", &components::Transform::translation,
                "scale", &components::Transform::scale,
                "reflection", &components::Transform::reflection,
                "rotation", &components::Transform::rotation,
                "pivot", &components::Transform::pivot,
                "shear", &components::Transform::shear,
                "from_table", [](const Table& table) -> components::Transform
                {
                    components::Transform transform{ };

                    transform.translation = table.get_or<Vector2>("translation", Vector2Zero);
                    transform.scale = table.get_or<Vector2>("scale", Vector2One);
                    transform.reflection = table.get_or("reflection", graphics::Reflection::None);
                    transform.rotation = table.get_or("rotation", 0.0f);
                    transform.pivot = table.get_or<Optional<Vector2>>("pivot", None);
                    transform.shear = table.get_or<Optional<Vector2>>("shear", None);

                    return transform;
                }
            );

            luaState.new_usertype<components::ScreenTransform>(
                "screen_transform",
                sol::constructors<
                    components::ScreenTransform(),
                    components::ScreenTransform(IVector2),
                    components::ScreenTransform(IVector2, graphics::Reflection),
                    components::ScreenTransform(IVector2, graphics::Reflection, f32),
                    components::ScreenTransform(IVector2, graphics::Reflection, f32, const Optional<IVector2>&),
                    components::ScreenTransform(IVector2, graphics::Reflection, f32, const Optional<IVector2>&, const Optional<Vector2>&),
                    components::ScreenTransform(const components::ScreenTransform&)
                >(),
                "translation", &components::ScreenTransform::translation,
                "reflection", &components::ScreenTransform::reflection,
                "rotation", &components::ScreenTransform::rotation,
                "pivot", &components::ScreenTransform::pivot,
                "shear", &components::ScreenTransform::shear,
                "from_table", [](const Table& table) -> components::ScreenTransform
                {
                    components::ScreenTransform screenTransform{ };

                    screenTransform.translation = table.get_or<IVector2>("translation", IVector2Zero);
                    screenTransform.reflection = table.get_or("reflection", graphics::Reflection::None);
                    screenTransform.rotation = table.get_or("rotation", 0.0f);
                    screenTransform.pivot = table.get_or<Optional<IVector2>>("pivot", None);
                    screenTransform.shear = table.get_or<Optional<Vector2>>("shear", None);

                    return screenTransform;
                }
            );
    }
    }

    auto ScriptEngine::LoadGraphicsFunctions(Application& application) -> void
    {
        LoadCameraFunctions(m_luaState, application);
        LoadWindowFunctions(m_luaState, application);

        auto graphicsNamespace = m_luaState["graphics"].get_or_create<Table>();
        graphicsNamespace.new_enum<graphics::Reflection>("reflection", {
            { "NONE", graphics::Reflection::None },
            { "HORIZONTAL", graphics::Reflection::Horizontal },
            { "VERTICAL", graphics::Reflection::Vertical },
            { "BOTH", graphics::Reflection::Both },
        });

        graphicsNamespace.new_usertype<Colour>(
            "colour",
            sol::constructors<
                Colour(),
                Colour(u8, u8, u8),
                Colour(u8, u8, u8, u8),
                Colour(f32, f32, f32),
                Colour(f32, f32, f32, f32),
                Colour(Vector3),
                Colour(Vector4),
                Colour(const Colour&)
            >(),
            "r", &Colour::red,
            "g", &Colour::green,
            "b", &Colour::blue,
            "a", &Colour::alpha
        );

        graphicsNamespace["colour"].get<Table>().set(
            "BLACK", colours::Black,
            "WHITE", colours::White,
            "RED", colours::Red,
            "GREEN", colours::Green,
            "BLUE", colours::Blue,
            "YELLOW", colours::Yellow,
            "MAGENTA", colours::Magenta,
            "CYAN", colours::Cyan
        );

        LoadGeometryTypes(m_luaState, graphicsNamespace);
        LoadTransformTypes(m_luaState);

        graphicsNamespace.new_usertype<graphics::SortingLayer>(
            "sorting_layer",
            sol::constructors<
                graphics::SortingLayer(),
                graphics::SortingLayer(f32),
                graphics::SortingLayer(const graphics::SortingLayer&)
            >(),
            "z", [](const graphics::SortingLayer& self) -> f32 { return self.GetZ(); },
            "set_z", [](graphics::SortingLayer& self, const f32 z) { self.SetZ(z); }
        );
    }
}
