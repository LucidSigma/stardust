#pragma once
#ifndef STARDUST_TOUCH_DEVICE_H
#define STARDUST_TOUCH_DEVICE_H

#include <SDL2/SDL.h>

#include <type_traits>

#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class TouchDevice final
    {
    public:
        using ID = SDL_TouchID;
        using FingerID = SDL_FingerID;
        using GestureID = SDL_GestureID;

        static constexpr ID VirtualMouseTouchID = SDL_MOUSE_TOUCHID;

        enum class Type
            : std::underlying_type_t<SDL_TouchDeviceType>
        {
            Invalid = SDL_TOUCH_DEVICE_INVALID,
            Direct = SDL_TOUCH_DEVICE_DIRECT,
            IndirectAbsolute = SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE,
            IndirectRelative = SDL_TOUCH_DEVICE_INDIRECT_RELATIVE,
        };

        struct FingerInfo final
        {
            FingerID id;

            Vector2 position;
            f32 pressure;
        };

    private:
        ID m_id = 0;
        Type m_type = Type::Invalid;

    public:
        static auto RecordDollarGestureOnAllDevices() -> void;

        explicit TouchDevice(const ID id);

        [[nodiscard]] auto GetActiveFingers() const -> List<FingerInfo>;

        auto RecordDollarGesture() const -> void;
        [[nodiscard]] auto SaveDollarGestureTemplate(const GestureID gestureID, const StringView filepath) const -> Status;
        [[nodiscard]] auto SaveAllDollarGestureTemplates(const StringView filepath) const -> Status;
        [[nodiscard]] auto LoadDollarGestureTemplates(const List<ubyte>& filepath) const -> Status;
        [[nodiscard]] auto LoadDollarGestureTemplates(const StringView filepath) const -> Status;

        [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }
        [[nodiscard]] inline auto GetType() const noexcept -> Type { return m_type; }
    };

    [[nodiscard]] extern auto EnumerateTouchDevices() -> List<TouchDevice>;
}

#endif
