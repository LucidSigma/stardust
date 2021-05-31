#include "stardust/debug/assert/Assert.h"

#include <format>

#include "stardust/debug/logging/Log.h"
#include "stardust/debug/message_box/MessageBox.h"

#ifndef WIN32
    #define __cdecl
#endif

namespace stardust
{
    namespace debug
    {
        namespace
        {
            [[nodiscard]] SDL_AssertState __cdecl AssertionHandler(const SDL_AssertData* const assertionData, void* const userData)
            {
                const String errorMessage = std::format(
                    "Assertion failure at {} ({}:{}); triggered {} {}.\n\tFailing condition: \"{}\".",
                    assertionData->function, assertionData->filename, assertionData->linenum,
                    assertionData->trigger_count, assertionData->trigger_count == 1 ? "time" : "times",
                    assertionData->condition
                );

                Log::Error("{}", errorMessage);

                const message_box::ButtonID selectedButton = message_box::ShowComplex("Assertion Failed", errorMessage, message_box::Type::Warning, Vector<message_box::ButtonData>{
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
                });

                return static_cast<SDL_AssertState>(selectedButton);
            }
        }

        [[nodiscard]] Vector<AssertionData> GetAssertionReport()
        {
            Vector<AssertionData> assertionReport{ };
            const SDL_AssertData* currentAssertion = SDL_GetAssertionReport();

            while (currentAssertion != nullptr)
            {
                assertionReport.emplace_back();

                assertionReport.back().isAlwaysIgnored = static_cast<bool>(currentAssertion->always_ignore);

                assertionReport.back().triggerCount = currentAssertion->trigger_count;
                assertionReport.back().condition = currentAssertion->condition;
                
                assertionReport.back().filename = currentAssertion->filename;
                assertionReport.back().lineNumber = static_cast<u32>(currentAssertion->linenum);
                assertionReport.back().functionName = currentAssertion->function;

                currentAssertion = currentAssertion->next;
            }

            return assertionReport;
        }

        void ResetAssertionReport()
        {
            SDL_ResetAssertionReport();
        }

        void InitialiseAssertionCallback()
        {
            SDL_SetAssertionHandler(AssertionHandler, nullptr);
        }

        void ResetAssertionCallback()
        {
            SDL_SetAssertionHandler(SDL_GetDefaultAssertionHandler(), nullptr);
        }
    }
}

#ifndef WIN32
    #undef __cdecl
#endif