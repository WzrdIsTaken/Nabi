#pragma once
#include "EngineCore.h" 

// Contains keyboard, mouse and controller key/button codes

// Keyboard
namespace nabi::Input::Keys
{
	// I got all the keys from here https://wiki.nexusmods.com/index.php/DirectX_Scancodes_And_How_To_Use_Them

	int constexpr Space = 32;
    int constexpr Apostrophe = 39;
	int constexpr Comma = 44;
	int constexpr Minus = 45;
	int constexpr Period = 46;
	int constexpr Slash = 47;
	int constexpr Alpha0 = 48;
	int constexpr Alpha1 = 49;
	int constexpr Alpha2 = 50;
	int constexpr Alpha3 = 51;
	int constexpr Alpha4 = 52;
	int constexpr Alpha5 = 53;
	int constexpr Alpha6 = 54;
	int constexpr Alpha7 = 55;
	int constexpr Alpha8 = 56;
	int constexpr Alpha9 = 57;
	int constexpr Semicolon = 59;
	int constexpr Equal = 61;

	int constexpr A = 65;
	int constexpr B = 66;
	int constexpr C = 67;
	int constexpr D = 68;
	int constexpr E = 69;
	int constexpr F = 70;
	int constexpr G = 71;
	int constexpr H = 72;
	int constexpr I = 73;
	int constexpr J = 74;
	int constexpr K = 75;
	int constexpr L = 76;
	int constexpr M = 77;
	int constexpr N = 78;
	int constexpr O = 79;
	int constexpr P = 80;
	int constexpr Q = 81;
	int constexpr R = 82;
	int constexpr S = 83;
	int constexpr T = 84;
	int constexpr U = 85;
	int constexpr V = 86;
	int constexpr W = 87;
	int constexpr X = 88;
	int constexpr Y = 89;
	int constexpr Z = 90;

	int constexpr LeftBracket = 91;
	int constexpr Backslash = 92;
	int constexpr RightBracket = 93;
	int constexpr Grave = 96;
	int constexpr Escape = 256;
	int constexpr Enter = 257;
	int constexpr Tab = 258;
	int constexpr Backspace = 259;
	int constexpr Delete = 261;
	int constexpr RightArrow = 0x27;
	int constexpr LeftArrow = 0x25;
	int constexpr DownArrow = 0x28;
	int constexpr UpArrow = 0x26;
} // namespace nabi::Input::Keys

// Mouse
namespace nabi::Input::Mouse
{
	/*
	int constexpr LeftMouseButton  = 256;
	int constexpr RightMouseButton  = 257;
	int constexpr MiddleMouseButton = 258;
	int constexpr MouseButtonThree  = 259;
	int constexpr MouseButtonFour   = 260;
	int constexpr MouseButtonFive   = 261;
	int constexpr MouseButtonSix    = 262;
	int constexpr MouseButtonSeven  = 263;
	int constexpr MouseWheelUp      = 264;
	int constexpr MouseWheelDown    = 265;
	*/

	int constexpr LeftButton  = 1;
	int constexpr RightButton = 2;
} // namespace nabi::Input::Mouse

// Controller
namespace nabi::Input::Controller
{
	unsigned int constexpr ControllerOne = 0u;
	unsigned int constexpr ControllerTwo = 1u;
	unsigned int constexpr ControllerThree = 2u;
	unsigned int constexpr ControllerFour = 3u;
	unsigned int constexpr MasterController = ControllerOne;

	namespace Buttons
	{
		int constexpr DPadUp = 0x0001;
		int constexpr DPadDown = 0x0002;
		int constexpr DPadLeft = 0x0004;
		int constexpr DPadRight = 0x0008;
		int constexpr Start = 0x0010;
		int constexpr Back = 0x0020;
		int constexpr LeftStick = 0x0040;
		int constexpr RightStick = 0x0080;
		int constexpr LeftShoulder = 0x0100;
		int constexpr RightShoulder = 0x0200;
		int constexpr A = 0x1000;
		int constexpr B = 0x2000;
		int constexpr X = 0x4000;
		int constexpr Y = 0x8000;
	} // namespace Buttons

	namespace Axes
	{
		unsigned int constexpr LeftStickX = 0u;
		unsigned int constexpr LeftStickY = 1u;
		unsigned int constexpr RightStickX = 2u;
		unsigned int constexpr RightStickY = 3u;
		unsigned int constexpr LeftTrigger = 4u;
		unsigned int constexpr RightTrigger = 5u;
	} // namespace Axes

	namespace Sticks
	{
		unsigned int constexpr Left = 0u;
		unsigned int constexpr Right = 2u;
	} // namespace Sticks
}
