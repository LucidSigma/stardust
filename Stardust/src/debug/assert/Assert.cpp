#include "stardust/debug/assert/Assert.h"

namespace stardust
{
    namespace debug
    {
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
    }
}