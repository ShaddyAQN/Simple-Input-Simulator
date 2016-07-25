#pragma once
#include <string>
#include <Windows.h>

namespace input_simulator
{
	void key_press(int key_code, int duration);
	void key_down(int key_code);
	void key_up(int key_code);

	void hardware_key_press(int key_code, int duration);
	void hardware_key_down(int key_code);
	void hardware_key_up(int key_code);

	void type_string(const std::string& str);			//Test only, not reliable
	void hardware_type_string(const std::string& str);	//Test only, not reliable

	void mouse_move(const POINT& coordinates);
	void mouse_move(HWND window, POINT coordinates);

	void mouse_left_press(int duration);
	void mouse_left_down();
	void mouse_left_up();

	void mouse_right_press(int duration);
	void mouse_right_down();
	void mouse_right_up();
}

