#include "Input.h"

/*! \brief Brief description.
*  Input class captures and processs keyboard and mouse events
*
*/
Input::Input()
{
	Quit = cmd_s = cmd_a = cmd_w = cmd_d = keyDown = cmd_mouseleft = cmd_mouseleftUp = cmd_mouseright = cmd_mouserightUp = once = cmd_z = cmd_x = cmd_c = false;
}

std::vector<int> Input::keys;
std::vector<int> Input::downKeys;
std::vector<int> Input::upKeys;

Input::~Input()
{
}

void Input::update()
{
	while (SDL_PollEvent(&eventQueue))
	{
		downKeys.clear();
		upKeys.clear();

		//Allows the user to close the program using the "x" button
		if (eventQueue.type == SDL_QUIT)
		{
			Quit = true;
		}

		if (eventQueue.type == SDL_KEYDOWN)
		{
			keyDown = true;
			keyboard(eventQueue.key.keysym.sym);
		}
		if (eventQueue.type == SDL_KEYUP)
		{
			keyDown = false;
			std::cout << "Key up" << std::endl;
			keyboardUp(eventQueue.key.keysym.sym);
		}

		if (eventQueue.type == SDL_MOUSEBUTTONDOWN)
		{
			if (eventQueue.button.button == SDL_BUTTON_LEFT)
			{
				std::cout << "Left Mouse Button has been pressed! \n";
				cmd_mouseleft = true;
				cmd_mouseleftUp = false;
			}
		}

		if (eventQueue.type == SDL_MOUSEBUTTONUP)
		{
			if (eventQueue.button.button == SDL_BUTTON_LEFT)
			{
				std::cout << "Left Mouse Button was lifted! \n";
				cmd_mouseleft = false;
				cmd_mouseleftUp = true;
			}
		}

		if (eventQueue.type == SDL_MOUSEBUTTONDOWN)
		{
			if (eventQueue.button.button == SDL_BUTTON_RIGHT)
			{
				std::cout << "Right Mouse Button has been pressed! \n";
				cmd_mouseleft = true;
				cmd_mouseleftUp = false;
			}
		}

		if (eventQueue.type == SDL_MOUSEBUTTONUP)
		{
			if (eventQueue.button.button == SDL_BUTTON_RIGHT)
			{
				std::cout << "Right Mouse Button was lifted! \n";
				cmd_mouseleft = false;
				cmd_mouseleftUp = true;
			}
		}


	}

	if (SDL_MOUSEMOTION)
	{
		SDL_GetMouseState(&mouse_x, &mouse_y);
		
		/// move the mouse back to the middle of the screen each frame
		SDL_WarpMouseInWindow(NULL, 400, 300);
	}
}

bool Input::getKey(int keyCode)
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (keys.at(i) == keyCode)
		{
			return true;
		}
	}
	return false;
}

bool Input::getKeyDown(int keyCode)
{
	for (int i = 0; i < downKeys.size(); i++)
	{
		if (keys.at(i) == keyCode)
		{
			return true;
		}
	}
	return false;
}

bool Input::getKeyUp(int keyCode)
{
	for (int i = 0; i < upKeys.size(); i++)
	{
		if (upKeys.at(i) == keyCode)
		{
			return true;
		}
	}
	return false;
}

void Input::keyboard(unsigned char key)
{
	_keyboard(key);
}

void Input::_keyboard(int key)
{
	// Display key value to show response to keypress
	// Notice when a key is held there is a delay before it repeats
	// And how many times the up key can be polled before another key repeat is called

	keys.push_back(key); // Add key to currently pressed keys
	downKeys.push_back(key); // Register that this key was pressed this frame (will be cleared when display refreshes next frame)
							 // 122 120 99
	for (size_t i = 0; i < keys.size(); i++)
	{
		if (keys.at(i) == 27)
		{
			Quit = true;
		}
		else if (keys.at(i) == 115)
		{
			cmd_s = true;
		}
		else if (keys.at(i) == 97)
		{
			cmd_a = true;
		}
		else if (keys.at(i) == 119)
		{
			cmd_w = true;
		}
		else if (keys.at(i) == 100)
		{
			cmd_d = true;
		}
		else if (keys.at(i) == 122)
		{
			cmd_z = true;
		}
		else if (keys.at(i) == 120)
		{
			cmd_x = true;
		}
		else if (keys.at(i) == 99)
		{
			cmd_c = true;
		}
		else
		{
			std::cout << key << std::endl;
		}
	}

}

void Input::keyboardUp(unsigned char key)
{
	_keyboardUp(key);
}

void Input::_keyboardUp(int key)
{
	upKeys.push_back(key); // Register that this key was released this frame

	for (size_t i = 0; i < keys.size(); i++)
	{
		if (keys.at(i) == key)
		{
			if (key == 115)
			{
				cmd_s = false;

				once = false;
			}
			else if (keys.at(i) == 97)
			{
				cmd_a = false;
			}
			else if (keys.at(i) == 119)
			{
				cmd_w = false;
			}
			else if (keys.at(i) == 100)
			{
				cmd_d = false;
			}
			else if (keys.at(i) == 122)
			{
				cmd_z = false;
			}
			else if (keys.at(i) == 120)
			{
				cmd_x = false;
			}
			else if (keys.at(i) == 99)
			{
				cmd_c = false;
			}
			keys.erase(keys.begin() + i); // Remove key from currently pressed keys
			i--;
		}
	}
}
