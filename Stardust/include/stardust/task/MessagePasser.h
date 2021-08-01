#pragma once
#ifndef STARDUST_MESSAGE_PASSER_H
#define STARDUST_MESSAGE_PASSER_H

#include <atomic>
#include <concepts>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    template <typename T>
        requires std::default_initializable<T>
    class MessagePasser
    {
    private:
        ConcurrentQueue<T> m_messageQueue{ };
        std::atomic<u32> m_messageCount = 0u;

    public:
        MessagePasser() = default;
        ~MessagePasser() noexcept = default;

        void Send(const T message)
        {
            if (m_messageQueue.enqueue(message))
            {
                ++m_messageCount;
            }
        }

        [[nodiscard]] Optional<T> Receive()
        {
            if (m_messageCount == 0u)
            {
                return;
            }

            if (T message{ };
                m_messageQueue.try_dequeue(message))
            {
                --m_messageCount;

                return message;
            }
            else
            {
                return NullOpt;
            }
        }

        [[nodiscard]] inline bool HasMessage() const noexcept { return m_messageCount != 0u; }
        [[nodiscard]] inline u32 GetMessageCount() const noexcept { return m_messageCount; }
    };
}

#endif