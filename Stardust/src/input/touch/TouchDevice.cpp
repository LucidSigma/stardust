#include "stardust/input/touch/TouchDevice.h"

#include "stardust/types/Pointers.h"

namespace stardust
{
    auto TouchDevice::RecordDollarGestureOnAllDevices() -> void
    {
        SDL_RecordGesture(-1);
    }

    TouchDevice::TouchDevice(const ID id)
        : m_id(id)
    {
        m_type = static_cast<Type>(SDL_GetTouchDeviceType(m_id));
    }

    [[nodiscard]] auto TouchDevice::GetActiveFingers() const -> List<FingerInfo>
    {
        const usize activeFingerCount = static_cast<usize>(SDL_GetNumTouchFingers(m_id));

        List<FingerInfo> activeFingers{ };
        activeFingers.reserve(activeFingerCount);

        for (usize i = 0u; i < activeFingerCount; ++i)
        {
            const ObserverPointer<const SDL_Finger> fingerInfo = SDL_GetTouchFinger(m_id, static_cast<i32>(i));

            if (fingerInfo != nullptr)
            {
                activeFingers.push_back(
                    FingerInfo{
                        .id = fingerInfo->id,
                        .position = Vector2{ fingerInfo->x, fingerInfo->y },
                        .pressure = fingerInfo->pressure,
                    }
                );
            }
        }

        return activeFingers;
    }

    auto TouchDevice::RecordDollarGesture() const -> void
    {
        SDL_RecordGesture(m_id);
    }

    [[nodiscard]] auto TouchDevice::SaveDollarGestureTemplate(const GestureID gestureID, const StringView filepath) const -> Status
    {
        SDL_RWops* dollarTemplatesFile = SDL_RWFromFile(filepath.data(), "wb+");

        if (dollarTemplatesFile == nullptr)
        {
            return Status::Fail;
        }

        const i32 saveResult = SDL_SaveDollarTemplate(gestureID, dollarTemplatesFile);

        SDL_RWclose(dollarTemplatesFile);
        dollarTemplatesFile = nullptr;

        return saveResult == 0 ? Status::Fail : Status::Success;
    }

    [[nodiscard]] auto TouchDevice::SaveAllDollarGestureTemplates(const StringView filepath) const -> Status
    {
        SDL_RWops* dollarTemplatesFile = SDL_RWFromFile(filepath.data(), "wb+");

        if (dollarTemplatesFile == nullptr)
        {
            return Status::Fail;
        }

        const i32 saveResult = SDL_SaveAllDollarTemplates(dollarTemplatesFile);

        SDL_RWclose(dollarTemplatesFile);
        dollarTemplatesFile = nullptr;

        return saveResult == 0 ? Status::Fail : Status::Success;
    }

    [[nodiscard]] auto TouchDevice::LoadDollarGestureTemplates(const List<ubyte>& filepath) const -> Status
    {
        SDL_RWops* dollarTemplatesRWops = SDL_RWFromConstMem(filepath.data(), static_cast<i32>(filepath.size()));

        if (dollarTemplatesRWops == nullptr)
        {
            return Status::Fail;
        }

        const i32 loadResult = SDL_LoadDollarTemplates(m_id, dollarTemplatesRWops);

        SDL_RWclose(dollarTemplatesRWops);
        dollarTemplatesRWops = nullptr;

        return loadResult <= 0 ? Status::Fail : Status::Success;
    }

    [[nodiscard]] auto TouchDevice::LoadDollarGestureTemplates(const StringView filepath) const -> Status
    {
        SDL_RWops* dollarTemplatesRWops = SDL_RWFromFile(filepath.data(), "rb");

        if (dollarTemplatesRWops == nullptr)
        {
            return Status::Fail;
        }

        const i32 loadResult = SDL_LoadDollarTemplates(m_id, dollarTemplatesRWops);

        SDL_RWclose(dollarTemplatesRWops);
        dollarTemplatesRWops = nullptr;

        return loadResult <= 0 ? Status::Fail : Status::Success;
    }

    [[nodiscard]] auto EnumerateTouchDevices() -> List<TouchDevice>
    {
        const usize touchDeviceCount = static_cast<usize>(SDL_GetNumTouchDevices());
        
        List<TouchDevice> touchDevices;
        touchDevices.reserve(touchDeviceCount);

        for (usize i = 0u; i < touchDeviceCount; ++i)
        {
            touchDevices.emplace_back(static_cast<TouchDevice::ID>(SDL_GetTouchDevice(static_cast<i32>(i))));
        }

        return touchDevices;
    }
}
