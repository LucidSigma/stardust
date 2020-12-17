#include "stardust/debug/message_box/MessageBox.h"

#include "stardust/data/Types.h"

namespace stardust
{
	namespace message_box
	{
		void Show(const String& title, const String& message, const Type type)
		{
			SDL_ShowSimpleMessageBox(static_cast<u32>(type), title.c_str(), message.c_str(), nullptr);
		}
	}
}