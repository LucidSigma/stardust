#include <cstdlib>
#include <format>

#include <stardust/Stardust.h>

auto main(const sd::i32 argc, char** const argv) -> sd::i32
{
    sd::message_box::Show("Information", "This is an information modal.", sd::message_box::Type::Information);
    sd::message_box::Show("Warning", "This is a warning modal.", sd::message_box::Type::Warning);
    sd::message_box::Show("Error", "This is an error modal.", sd::message_box::Type::Error);

    const sd::message_box::ButtonID selectedButton = sd::message_box::ShowComplex(
        "Buttons",
        "This message box has four buttons.",
        sd::message_box::Type::Information,
        sd::List<sd::message_box::ButtonData>{
            sd::message_box::ButtonData{
                .id = 1,
                .text = "1",
                .flags = { sd::message_box::ButtonFlag::ReturnKeyDefault, sd::message_box::ButtonFlag::EscapeKeyDefault },
            },
            sd::message_box::ButtonData{
                .id = 2,
                .text = "2",
                .flags = { },
            },
            sd::message_box::ButtonData{
                .id = 3,
                .text = "3",
                .flags = { },
            },
            sd::message_box::ButtonData{
                .id = 4,
                .text = "4",
                .flags = { },
            },
        }
    );

    sd::message_box::Show("Buttons", std::format("You selected button {}.", selectedButton), sd::message_box::Type::Information);

    return EXIT_SUCCESS;
}
