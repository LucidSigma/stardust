#include "stardust/text/Markup.h"

namespace std
{
    [[nodiscard]] auto hash<stardust::TextOutline>::operator ()(const stardust::TextOutline& outline) const noexcept -> stardust::usize
    {
        stardust::usize seed = 0u;

        glm::detail::hash_combine(seed, std::hash<stardust::f32>()(outline.thickness));
        glm::detail::hash_combine(seed, std::hash<stardust::Colour>()(outline.colour));
        glm::detail::hash_combine(seed, std::hash<stardust::IVector2>()(outline.offset));

        return seed;
    }

    [[nodiscard]] auto hash<stardust::TextDropShadow>::operator ()(const stardust::TextDropShadow& dropShadow) const noexcept -> stardust::usize
    {
        stardust::usize seed = 0u;

        glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::f32>>()(dropShadow.thickness));
        glm::detail::hash_combine(seed, std::hash<stardust::Colour>()(dropShadow.colour));
        glm::detail::hash_combine(seed, std::hash<stardust::IVector2>()(dropShadow.offset));

        return seed;
    }

    [[nodiscard]] auto hash<stardust::TextDecorativeLine>::operator ()(const stardust::TextDecorativeLine& decorativeLine) const noexcept -> stardust::usize
    {
        stardust::usize seed = 0u;

        glm::detail::hash_combine(seed, std::hash<stardust::f32>()(decorativeLine.thickness));
        glm::detail::hash_combine(seed, std::hash<stardust::Colour>()(decorativeLine.colour));
        glm::detail::hash_combine(seed, std::hash<stardust::IVector2>()(decorativeLine.offset));

        return seed;
    }

    [[nodiscard]] auto hash<stardust::Markup>::operator ()(const stardust::Markup& markup) const noexcept -> stardust::usize
    {
        stardust::usize seed = 0u;

        glm::detail::hash_combine(seed, std::hash<stardust::Colour>()(markup.colour));
        glm::detail::hash_combine(seed, std::hash<stardust::TextAlignment>()(markup.textAlignment));
        glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::TextOutline>>()(markup.outline));
        glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::TextDropShadow>>()(markup.dropShadow));
        glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::TextDecorativeLine>>()(markup.underline));
        glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::TextDecorativeLine>>()(markup.strikethrough));
        glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::TextDecorativeLine>>()(markup.overline));
        glm::detail::hash_combine(seed, std::hash<stardust::Optional<stardust::u32>>()(markup.wrapLength));
        glm::detail::hash_combine(seed, std::hash<stardust::localisation::TextLocalisationInfo>()(markup.localisation));
        glm::detail::hash_combine(seed, std::hash<stardust::Font::RenderMode>()(markup.defaultRenderMode));

        return seed;
    }
}
