#include "stardust/input/mouse/MouseCodes.h"

namespace stardust
{
    [[nodiscard]] auto GetInternalMouseButtonName(const MouseButton button) -> String
    {
        switch (button)
        {
        case MouseButton::Left:
            return "MouseLeft";

        case MouseButton::Middle:
            return "MouseMiddle";

        case MouseButton::Right:
            return "MouseRight";

        case MouseButton::Thumb1:
            return "MouseThumb1";

        case MouseButton::Thumb2:
            return "MouseThumb2";

        case MouseButton::Unknown:
        [[unlikely]] default:
            return "Unknown";
        }
    }

    [[nodiscard]] auto GetInternalMouseAxisName(const MouseAxis axis) -> String
    {
        switch (axis)
        {
        case MouseAxis::X:
            return "MouseX";

        case MouseAxis::Y:
            return "MouseY";

        case MouseAxis::Scroll:
            return "MouseScroll";

        case MouseAxis::Unknown:
        [[unlikely]] default:
            return "Unknown";
        }
    }
}
