#include "stardust/debug/message_box/MessageBox.h"

#include "stardust/data/Types.h"

namespace stardust
{
	namespace message_box
	{
		void Show(const StringView& title, const StringView& message, const Type type)
		{
			SDL_ShowSimpleMessageBox(static_cast<u32>(type), title.data(), message.data(), nullptr);
		}
	}
}