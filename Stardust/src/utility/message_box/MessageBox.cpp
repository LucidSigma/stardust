#include "stardust/utility/message_box/MessageBox.h"

#include <ranges>

namespace stardust
{
    namespace message_box
    {
        auto Show(const StringView title, const StringView message, const Type type) -> void
        {
            SDL_ShowSimpleMessageBox(static_cast<u32>(type), title.data(), message.data(), nullptr);
        }

        auto ShowComplex(const StringView title, const StringView message, const Type type, const List<ButtonData>& buttons) -> ButtonID
        {
            ButtonID selectedButton = 0;

            List<SDL_MessageBoxButtonData> buttonData{ };
            buttonData.reserve(buttons.size());

            for (const auto& button : std::ranges::reverse_view(buttons))
            {
                buttonData.emplace_back();
                buttonData.back().flags = 0u;
                buttonData.back().buttonid = button.id;
                buttonData.back().text = button.text.c_str();

                for (const auto flag : button.flags)
                {
                    buttonData.back().flags |= static_cast<u32>(flag);
                }
            }

            const SDL_MessageBoxData messageBoxData{
                .flags = static_cast<u32>(type),
                .window = nullptr,
                .title = title.data(),
                .message = message.data(),
                .numbuttons = static_cast<i32>(buttons.size()),
                .buttons = buttonData.data(),
                .colorScheme = nullptr,
            };

            SDL_ShowMessageBox(&messageBoxData, &selectedButton);

            return selectedButton;
        }
    }
}
