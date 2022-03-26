#include "stardust/scripting/ScriptEngine.h"

#include "stardust/animation/easings/Easings.h"

namespace stardust
{
    auto ScriptEngine::LoadAnimationFunctions() -> void
    {
        auto easingsNamespace = m_luaState["easings"].get_or_create<Table>();

        easingsNamespace.set_function("linear", easings::EaseLinear);

        easingsNamespace.set_function("quad_in", easings::EaseInQuad);
        easingsNamespace.set_function("quad_out", easings::EaseOutQuad);
        easingsNamespace.set_function("quad_in_out", easings::EaseInOutQuad);

        easingsNamespace.set_function("cubic_in", easings::EaseInCubic);
        easingsNamespace.set_function("cubic_out", easings::EaseOutCubic);
        easingsNamespace.set_function("cubic_in_out", easings::EaseInOutCubic);

        easingsNamespace.set_function("quart_in", easings::EaseInQuart);
        easingsNamespace.set_function("quart_out", easings::EaseOutQuart);
        easingsNamespace.set_function("quart_in_out", easings::EaseInOutQuart);

        easingsNamespace.set_function("quint_in", easings::EaseInQuint);
        easingsNamespace.set_function("quint_out", easings::EaseOutQuint);
        easingsNamespace.set_function("quint_in_out", easings::EaseInOutQuint);

        easingsNamespace.set_function("sine_in", easings::EaseInSine);
        easingsNamespace.set_function("sine_out", easings::EaseOutSine);
        easingsNamespace.set_function("sine_in_out", easings::EaseInOutSine);

        easingsNamespace.set_function("exponential_in", easings::EaseInExponential);
        easingsNamespace.set_function("exponential_out", easings::EaseOutExponential);
        easingsNamespace.set_function("exponential_in_out", easings::EaseInOutExponential);

        easingsNamespace.set_function("circle_in", easings::EaseInCircle);
        easingsNamespace.set_function("circle_out", easings::EaseOutCircle);
        easingsNamespace.set_function("circle_in_out", easings::EaseInOutCircle);

        easingsNamespace.set_function("back_in", easings::EaseInBack);
        easingsNamespace.set_function("back_out", easings::EaseOutBack);
        easingsNamespace.set_function("back_in_out", easings::EaseInOutBack);

        easingsNamespace.set_function("elastic_in", easings::EaseInElastic);
        easingsNamespace.set_function("elastic_out", easings::EaseOutElastic);
        easingsNamespace.set_function("elastic_in_out", easings::EaseInOutElastic);

        easingsNamespace.set_function("bounce_in", easings::EaseInBounce);
        easingsNamespace.set_function("bounce_out", easings::EaseOutBounce);
        easingsNamespace.set_function("bounce_in_out", easings::EaseInOutBounce);

        easingsNamespace.set_function("heaviside_step", easings::EaseHeavisideStep);
    }
}
