#include "input_simulator.hpp"
#include <Windows.h>
#include <thread>

void input_simulator::key_press(int key_code, int duration)
{
	key_down(key_code);
	std::this_thread::sleep_for(std::chrono::milliseconds(duration));
	key_up(key_code);
}

void input_simulator::key_down(int key_code)
{
	INPUT input{0};
	
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = static_cast<WORD>(key_code);
	input.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &input, sizeof input);
}

void input_simulator::key_up(int key_code)
{
	INPUT input{ 0 };

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = static_cast<WORD>(key_code); 
	input.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &input, sizeof input);
}

void input_simulator::hardware_key_press(int key_code, int duration)
{
	hardware_key_down(key_code);
	std::this_thread::sleep_for(std::chrono::milliseconds(duration));
	hardware_key_up(key_code);
}

void input_simulator::hardware_key_down(int key_code)
{
	INPUT input{ 0 };

	//Set up the INPUT structure
	input.type = INPUT_KEYBOARD;

	//This let's you do a hardware scan instead of a virtual keypress
	input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_SCANCODE;
	input.ki.wScan = 0xFF & MapVirtualKey(key_code, MAPVK_VK_TO_VSC);

	SendInput(1, &input, sizeof input);
}

void input_simulator::hardware_key_up(int key_code)
{
	INPUT input{ 0 };

	//Set up the INPUT structure
	input.type = INPUT_KEYBOARD;

	//This let's you do a hardware scan instead of a virtual keypress
	input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	input.ki.wScan = 0xFF & MapVirtualKey(key_code, MAPVK_VK_TO_VSC);

	SendInput(1, &input, sizeof input);
}

void input_simulator::type_string(const std::string& str)
{
	const auto locale = GetKeyboardLayout(0);
	for(const char& c : str) {
		const int key_code = 0xFF & VkKeyScanEx(c, locale);

		key_press(key_code, 50);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}

void input_simulator::hardware_type_string(const std::string& str)
{
	const auto locale = GetKeyboardLayout(0);
	for (const char& c : str) {
		const int key_code = VkKeyScanEx(c, locale);

		if (key_code & 0x100)	// check upper byte for shift flag
			hardware_key_down(VK_SHIFT);

		if (key_code & 0x200)	// check upper byte for ctrl flag
			hardware_key_down(VK_CONTROL);

		if(key_code & 0x400)	// check upper byte for alt flag
			hardware_key_down(VK_MENU);

		hardware_key_press(0xFF & key_code, 50);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));

		if (key_code & 0x100)	// check upper byte for shift flag
			hardware_key_up(VK_SHIFT);

		if (key_code & 0x200)	// check upper byte for ctrl flag
			hardware_key_up(VK_CONTROL);

		if (key_code & 0x400)	// check upper byte for alt flag
			hardware_key_up(VK_MENU);
	}
}

void input_simulator::mouse_move(const POINT& coordinates)
{
	INPUT input {0};
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.dx = coordinates.x * 65536 / GetSystemMetrics(SM_CXSCREEN);//x being coord in pixels
	input.mi.dy = coordinates.y * 65536 / GetSystemMetrics(SM_CYSCREEN);//y being coord in pixels
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof input);
}

void input_simulator::mouse_move(HWND window, POINT coordinates)
{
	RECT rect;
	GetWindowRect(window, &rect);

	coordinates.x += rect.left;
	coordinates.y += rect.top;

	GetClientRect(window, &rect);

	coordinates.x += rect.left;
	coordinates.y += rect.top;

	mouse_move(coordinates);
}

void input_simulator::mouse_left_press(int duration)
{
	mouse_left_down();
	std::this_thread::sleep_for(std::chrono::milliseconds(duration));
	mouse_left_up();
}

void input_simulator::mouse_left_down()
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof input);
}

void input_simulator::mouse_left_up()
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof input);
}

void input_simulator::mouse_right_press(int duration)
{
	mouse_right_down();
	std::this_thread::sleep_for(std::chrono::milliseconds(duration));
	mouse_right_up();
}

void input_simulator::mouse_right_down()
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof input);
}

void input_simulator::mouse_right_up()
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof input);
}
