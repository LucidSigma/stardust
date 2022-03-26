#include "stardust/debug/assert/Assert.h"

#include <format>

#include "stardust/debug/logging/Logging.h"
#include "stardust/utility/message_box/MessageBox.h"

#ifndef STARDUST_PLATFORM_WINDOWS
    #define __cdecl
#endif

namespace stardust
{
    namespace debug
    {
        namespace
        {
            [[nodiscard]] auto __cdecl AssertionHandler(const SDL_AssertData* const assertionData, void* const userData) -> SDL_AssertState
            {
                const String errorMessage = std::format(
                    "Assertion failure at {} ({}:{}); triggered {} {}.\n\tFailing condition: \"{}\".",
                    assertionData->function,
                    assertionData->filename,
                    assertionData->linenum,
                    assertionData->trigger_count,
                    assertionData->trigger_count == 1u ? "time" : "times",
                    assertionData->condition
                );

                Log::Error("{}", errorMessage);

                const message_box::ButtonID selectedButton = message_box::ShowComplex(
                    "Assertion Failed",
                    errorMessage,
                    message_box::Type::Warning,
                    List<message_box::ButtonData>{
                        message_box::ButtonData{
                            .id = static_cast<message_box::ButtonID>(SDL_ASSERTION_IGNORE),
                            .text = "Ignore",
                            .flags = { message_box::ButtonFlag::ReturnKeyDefault, message_box::ButtonFlag::EscapeKeyDefault },
                        },
                        message_box::ButtonData{
                            .id = static_cast<message_box::ButtonID>(SDL_ASSERTION_ALWAYS_IGNORE),
                            .text = "Always Ignore",
                            .flags = { },
                        },
                        message_box::ButtonData{
                            .id = static_cast<message_box::ButtonID>(SDL_ASSERTION_BREAK),
                            .text = "Break",
                            .flags = { },
                        },
                        message_box::ButtonData{
                            .id = static_cast<message_box::ButtonID>(SDL_ASSERTION_RETRY),
                            .text = "Retry",
                            .flags = { },
                        },
                        message_box::ButtonData{
                            .id = static_cast<message_box::ButtonID>(SDL_ASSERTION_ABORT),
                            .text = "Abort",
                            .flags = { },
                        },
                    }
                );

                return static_cast<SDL_AssertState>(selectedButton);
            }
        }

        [[nodiscard]] auto GetAssertionReport() -> List<AssertionData>
        {
            List<AssertionData> assertionReport{ };
            const SDL_AssertData* currentAssertion = SDL_GetAssertionReport();

            while (currentAssertion != nullptr)
            {
                assertionReport.emplace_back(
                    AssertionData{
                        .isAlwaysIgnored = static_cast<bool>(currentAssertion->always_ignore),

                        .triggerCount = currentAssertion->trigger_count,
                        .condition = currentAssertion->condition,

                        .filename = currentAssertion->filename,
                        .lineNumber = static_cast<u32>(currentAssertion->linenum),
                        .functionName = currentAssertion->function,
                    }
                );

                currentAssertion = currentAssertion->next;
            }

            return assertionReport;
        }

        auto ClearAssertions() -> void
        {
            SDL_ResetAssertionReport();
        }

        auto InitialiseAssertionCallback() -> void
        {
            SDL_SetAssertionHandler(AssertionHandler, nullptr);
        }

        auto ResetAssertionCallback() -> void
        {
            SDL_SetAssertionHandler(SDL_GetDefaultAssertionHandler(), nullptr);
        }
    }
}

#ifndef STARDUST_PLATFORM_WINDOWS
    #undef __cdecl
#endif
