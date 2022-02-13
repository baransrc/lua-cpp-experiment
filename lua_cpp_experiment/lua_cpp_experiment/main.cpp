#include <iostream>

#include "lua_helper.h"
//#include "assert.hpp"

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

void vanilla_lua_test()
{
	// Create lua state:
	lua_State* L = luaL_newstate();

	// Load all lua libraries:
	luaL_openlibs(L);

	if (!check_lua(L, luaL_dofile(L, "experiment.lua")))
	{
		// Close and free lua_State L:
		lua_close(L);

		return;
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
}

int main()
{
	sol::state lua;

	// Enable base libraries:
	lua.open_libraries(sol::lib::base);

	std::string script_file_name = "experiment.lua";

	// Load script file:
	sol::load_result load_result = lua.load_file(script_file_name);

	// If the loaded script has errors, display it:
	if (!load_result.valid())
	{
		sol::error error = load_result;
		std::cerr << "Failed to load " << script_file_name << " " << error.what() << std::endl;

		system("pause");
		return 0;
	}
	// Run the script:
	load_result();

	// Run the print_test function of the script:
	lua["print_test"]();

	//std::cout << "Lua gives: " << lua_return << " to CPP." << std::endl;

	system("pause");
	return 0;
}