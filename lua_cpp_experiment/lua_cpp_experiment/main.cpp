#include <iostream>

#include "lua_helper.h"

// Little error checking utility function
bool check_lua(lua_State* L, int result)
{
	if (result != 0)
	{
		std::string error_message = lua_tostring(L, -1);
		std::cout << error_message << std::endl;

		return false;
	}

	return true;
}

int main()
{
	// Create lua state:
	lua_State* L = luaL_newstate();

	// Load all lua libraries:
	luaL_openlibs(L);

	if (!check_lua(L, luaL_dofile(L, "experiment.lua")))
	{
		// Close and free lua_State L:
		lua_close(L);

		system("pause");

		return 0;
	}

	std::cout << "Hello World from CPP" << std::endl;

	// Try getting the "box" called print_test on the
	// top of the stack:
	lua_getglobal(L, "print_test");

	// Check if top of the stack is a function:
	// NOTE: In lua we have indexed stack, and the top
	// of the stack is -1.
	if (lua_isfunction(L, -1))
	{
		// Run the function and if there is a problem
		// log the error message given by lua:
		if (check_lua(L, lua_pcall(L, 0, 0, 0)))
		{
			std::cout << "This works my dude" << std::endl;
		}
	}

	// Close and free lua_State L:
	lua_close(L);

	system("pause");

	return 0;
}