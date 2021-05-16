#include "stardust/input/mouse/MouseButtonCodes.h"

namespace stardust
{
    [[nodiscard]] String MouseButtonToString(const MouseButton mouseButton)
    {
        switch (mouseButton)
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

        default:
            return "";
        }
    }
}