#include "stardust/scripting/ScriptEngine.h"

#include "stardust/ui/UI.h"

namespace stardust
{
    auto stardust::ScriptEngine::LoadUIFunctions() -> void
    {
        m_luaState.new_enum<ui::Anchor>("anchor", {
            { "TOP_LEFT", ui::Anchor::TopLeft },
            { "TOP_CENTRE", ui::Anchor::TopCentre },
            { "TOP_RIGHT", ui::Anchor::TopRight },
            { "CENTRE_LEFT", ui::Anchor::CentreLeft },
            { "CENTRE", ui::Anchor::Centre },
            { "CENTRE_RIGHT", ui::Anchor::CentreRight },
            { "BOTTOM_LEFT", ui::Anchor::BottomLeft },
            { "BOTTOM_CENTRE", ui::Anchor::BottomCentre },
            { "BOTTOM_RIGHT", ui::Anchor::BottomRight },
        });
    }
}
