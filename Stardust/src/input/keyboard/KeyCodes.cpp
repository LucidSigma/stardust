#include "stardust/input/keyboard/KeyCodes.h"

namespace stardust
{
	[[nodiscard]] String KeyCodeToString(const KeyCode key)
	{
		switch (key)
		{
		case KeyCode::A:
			return "A";

		case KeyCode::B:
			return "B";

		case KeyCode::C:
			return "C";

		case KeyCode::D:
			return "D";

		case KeyCode::E:
			return "E";

		case KeyCode::F:
			return "F";

		case KeyCode::G:
			return "G";

		case KeyCode::H:
			return "H";

		case KeyCode::I:
			return "I";

		case KeyCode::J:
			return "J";

		case KeyCode::K:
			return "K";

		case KeyCode::L:
			return "L";

		case KeyCode::M:
			return "M";

		case KeyCode::N:
			return "N";

		case KeyCode::O:
			return "O";

		case KeyCode::P:
			return "P";

		case KeyCode::Q:
			return "Q";

		case KeyCode::R:
			return "R";

		case KeyCode::S:
			return "S";

		case KeyCode::T:
			return "T";

		case KeyCode::U:
			return "U";

		case KeyCode::V:
			return "V";

		case KeyCode::W:
			return "W";

		case KeyCode::X:
			return "X";

		case KeyCode::Y:
			return "Y";

		case KeyCode::Z:
			return "Z";

		case KeyCode::One:
			return "One";

		case KeyCode::Two:
			return "Two";

		case KeyCode::Three:
			return "Three";

		case KeyCode::Four:
			return "Four";

		case KeyCode::Five:
			return "Five";

		case KeyCode::Six:
			return "Six";

		case KeyCode::Seven:
			return "Seven";

		case KeyCode::Eight:
			return "Eight";

		case KeyCode::Nine:
			return "Nine";

		case KeyCode::Zero:
			return "Zero";

		case KeyCode::Return:
			return "Enter";

		case KeyCode::Escape:
			return "Escape";

		case KeyCode::Backspace:
			return "Backspace";

		case KeyCode::Tab:
			return "Tab";

		case KeyCode::Space:
			return "Space";

		case KeyCode::Minus:
			return "Minus";

		case KeyCode::Equals:
			return "Equals";

		case KeyCode::LeftBracket:
			return "LeftBracket";

		case KeyCode::RightBracket:
			return "RightBracket";

		case KeyCode::Backslash:
			return "Backslash";

		case KeyCode::NonUSHash:
			return "NonUSHash";

		case KeyCode::Semicolon:
			return "Semicolon";

		case KeyCode::Apostrophe:
			return "Apostrophe";

		case KeyCode::Grave:
			return "Grave";

		case KeyCode::Comma:
			return "Comma";

		case KeyCode::Period:
			return "Period";

		case KeyCode::ForwardSlash:
			return "ForwardSlash";

		case KeyCode::CapsLock:
			return "CapsLock";

		case KeyCode::F1:
			return "F1";

		case KeyCode::F2:
			return "F2";

		case KeyCode::F3:
			return "F3";

		case KeyCode::F4:
			return "F4";

		case KeyCode::F5:
			return "F5";

		case KeyCode::F6:
			return "F6";

		case KeyCode::F7:
			return "F7";

		case KeyCode::F8:
			return "F8";

		case KeyCode::F9:
			return "F9";

		case KeyCode::F10:
			return "F10";

		case KeyCode::F11:
			return "F11";

		case KeyCode::F12:
			return "F12";

		case KeyCode::PrintScreen:
			return "PrintScreen";

		case KeyCode::ScrollLock:
			return "ScrollLock";

		case KeyCode::Pause:
			return "Pause";

		case KeyCode::Insert:
			return "Insert";

		case KeyCode::Home:
			return "Home";

		case KeyCode::PageUp:
			return "PageUp";

		case KeyCode::Delete:
			return "Delete";

		case KeyCode::End:
			return "End";

		case KeyCode::PageDown:
			return "PageDown";

		case KeyCode::Right:
			return "Right";

		case KeyCode::Left:
			return "Left";

		case KeyCode::Down:
			return "Down";

		case KeyCode::Up:
			return "Up";

		case KeyCode::NumLock:
			return "NumLock";

		case KeyCode::KeypadDivide:
			return "KeypadDivide";

		case KeyCode::KeypadMultiply:
			return "KeypadMultiply";

		case KeyCode::KeypadMinus:
			return "KeypadMinus";

		case KeyCode::KeypadPlus:
			return "KeypadPlus";

		case KeyCode::KeypadEnter:
			return "KeypadEnter";

		case KeyCode::Keypad1:
			return "Keypad1";

		case KeyCode::Keypad2:
			return "Keypad2";

		case KeyCode::Keypad3:
			return "Keypad3";

		case KeyCode::Keypad4:
			return "Keypad4";

		case KeyCode::Keypad5:
			return "Keypad5";

		case KeyCode::Keypad6:
			return "Keypad6";

		case KeyCode::Keypad7:
			return "Keypad7";

		case KeyCode::Keypad8:
			return "Keypad8";

		case KeyCode::Keypad9:
			return "Keypad9";

		case KeyCode::Keypad0:
			return "Keypad0";

		case KeyCode::KeypadPeriod:
			return "KeypadPeriod";

		case KeyCode::NonUSBackslash:
			return "NonUSBackslash";

		case KeyCode::KeypadEquals:
			return "KeypadEquals";

		case KeyCode::KeypadComma:
			return "KeypadComma";

		case KeyCode::International1:
			return "International1";

		case KeyCode::International2:
			return "International2";

		case KeyCode::International3:
			return "International3";

		case KeyCode::International4:
			return "International4";

		case KeyCode::International5:
			return "International5";

		case KeyCode::International6:
			return "International6";

		case KeyCode::International7:
			return "International7";

		case KeyCode::International8:
			return "International8";

		case KeyCode::International9:
			return "International9";

		case KeyCode::Lang1:
			return "Lang1";

		case KeyCode::Lang2:
			return "Lang2";

		case KeyCode::Lang3:
			return "Lang3";

		case KeyCode::Lang4:
			return "Lang4";

		case KeyCode::Lang5:
			return "Lang5";

		case KeyCode::Lang6:
			return "Lang6";

		case KeyCode::Lang7:
			return "Lang7";

		case KeyCode::Lang8:
			return "Lang8";

		case KeyCode::Lang9:
			return "Lang9";

		case KeyCode::LeftCtrl:
			return "LeftCtrl";

		case KeyCode::LeftShift:
			return "LeftShift";

		case KeyCode::LeftAlt:
			return "LeftAlt";

		case KeyCode::LeftGUI:
			return "LeftGUI";

		case KeyCode::RightCtrl:
			return "RightCtrl";

		case KeyCode::RightShift:
			return "RightShift";

		case KeyCode::RightAlt:
			return "RightAlt";

		case KeyCode::RightGUI:
			return "RightGUI";

		case KeyCode::AltGr:
			return "AltGr";

		default:
			return "";
		}
	}
}