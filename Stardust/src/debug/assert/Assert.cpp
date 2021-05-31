#include "stardust/debug/assert/Assert.h"

#include "stardust/debug/logging/Log.h"

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
                Log::Error("Assertion failed. Information:");

                return SDL_GetDefaultAssertionHandler()(assertionData, userData);
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