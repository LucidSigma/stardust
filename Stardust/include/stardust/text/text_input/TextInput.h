#pragma once
#ifndef STARDUST_TEXT_INPUT_H
#define STARDUST_TEXT_INPUT_H

namespace stardust
{
    namespace text_input
    {
        extern auto Start() -> void;
        extern auto Stop() -> void;

        [[nodiscard]] extern auto IsActive() -> bool;
    }
}

#endif
