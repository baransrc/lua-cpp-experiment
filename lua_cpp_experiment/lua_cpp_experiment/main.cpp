#include <iostream>

#include "entity_component.h"
#include "lua_helper.h"
#include "script.h"

//bool check_lua(lua_State* L, int result)
//{
//	if (result != 0)
//	{
//		std::string error_message = lua_tostring(L, -1);
//		std::cout << error_message << std::endl;
//
//		return false;
//	}
//
//	return true;
//}
//
//void vanilla_lua_test()
//{
//	// Create lua state:
//	lua_State* L = luaL_newstate();
//
//	// Load all lua libraries:
//	luaL_openlibs(L);
//
//	if (!check_lua(L, luaL_dofile(L, "experiment.lua")))
//	{
//		// Close and free lua_State L:
//		lua_close(L);
//
//		return;
//	}
//
//	std::cout << "Hello World from CPP" << std::endl;
//
//	// Try getting the "box" called print_test on the
//	// top of the stack:
//	lua_getglobal(L, "print_test");
//
//	// Check if top of the stack is a function:
//	// NOTE: In lua we have indexed stack, and the top
//	// of the stack is -1.
//	if (lua_isfunction(L, -1))
//	{
//		// Run the function and if there is a problem
//		// log the error message given by lua:
//		if (check_lua(L, lua_pcall(L, 0, 0, 0)))
//		{
//			std::cout << "This works my dude" << std::endl;
//		}
//	}
//
//	// Close and free lua_State L:
//	lua_close(L);
//}

int main()
{
	sol::state lua(sol::c_call<decltype(&print_panic), &print_panic>);

	// Enable base libraries:
	lua.open_libraries
	(
		sol::lib::base,
		sol::lib::package,
		sol::lib::debug,
		sol::lib::string,
		sol::lib::io,
		sol::lib::coroutine,
		sol::lib::os,
		sol::lib::table,
		sol::lib::math
	);

	sol::load_result settings_load_result = lua.load_file("settings.lua");

	// If the loaded script has errors, display it:
	if (!settings_load_result.valid())
	{
		sol::error error = settings_load_result;
		std::cerr << "Failed to load settings: " << error.what() << std::endl;

		system("pause");
		return 0;
	}
	// Run the settings script:
	settings_load_result();

	// TODO: Get these runtime, and relative to working directory.
	std::string project_path = lua["project_path"];
	std::string script_file_name = lua["script_file_name"];
	std::string lua_path = lua["lua_path"];
	std::string zbs_lib_path = lua["zbs_lib_path"];

	std::string path = "";
	path += "package.path = ";
	path += "\"";
	path += "./?.lua;";
	path += zbs_lib_path + "/lualibs/?/?.lua;";
	path += zbs_lib_path + "/lualibs/?.lua;";
	path += project_path + "/lua-cpp-experiment/scripts/?.lua;";
	path += "\"";
	path += "..package.path";

	std::string c_path = "";
	c_path += "package.cpath = ";
	c_path += "\"";
	// TODO: Get these runtime, and relative to working directory.
	c_path += lua_path + "/luaJIT/lib/?.dll;";
	c_path += lua_path + "/lua_socket/bin/?.dll;";
	c_path += lua_path + "/lua_socket/bin/clibs/?.dll;";
	c_path += lua_path + "/lua_socket/bin/clibs/mime/?.dll;";
	c_path += lua_path + "/lua_socket/bin/clibs/socket/?.dll;";
	c_path += zbs_lib_path + "/bin/?.dll;";
	c_path += zbs_lib_path + "/bin/clibs/?.dll";
	c_path += "\"";
	c_path += "..package.cpath";

	// Add lua file and dll file checkup paths to the script:
	// We don't need to check if these files are valid since 
	// we statically run these from code instead of loading it
	// from file.
	lua.do_string(c_path);
	lua.do_string(path);

	Entity* entity = new Entity();

	LuaScript script_1(script_file_name, entity);
	LuaScript script_2("experiment2.lua", entity);

	sol::safe_function execute_function = script_1.GetAsTable()["execute"];



	//sol::load_result load_result = lua.load_file(script_file_name);

	//// If the loaded script has errors, display it:
	//if (!load_result.valid())
	//{
	//	sol::error error = load_result;
	//	std::cerr << "Failed to load " << script_file_name << " " << error.what() << std::endl;

	//	system("pause");
	//	return 0;
	//}
	//// Run the script:
	//load_result();

	//// Create Entity namespace:
	//sol::table this_namespace = lua["hachiko"].get_or_create<sol::table>();

	//// Add entity and components to lua:
	//component_type_to_lua(this_namespace);
	//component_to_lua(this_namespace);
	//component_x_to_lua(this_namespace);
	//component_y_to_lua(this_namespace);
	//entity_to_lua(this_namespace);

	//// Create an entity:
	//Entity* entity = new Entity();

	//// Add the api to interact with entity to namespace:
	//this_namespace.set("entity", entity);

	//// Run the execute method of the script:
	//sol::table experiment_table = lua["experiment"];
	//sol::protected_function_result script_result = experiment_table["execute"]();

	//// If an error comes up, print to the screen and halt the
	//// execution:
	//if (!script_result.valid()) 
	//{
	//	sol::error error = script_result;
	//	std::cout << "[ERROR-LUA]: " << error.what() << std::endl;
	//	std::cin.ignore();
	//	return 0;
	//}

	//// Test if script really changed entity:
	//std::cout << std::endl << "------------" << std::endl;
	//std::cout << "Result from CPP: " << entity->GetComponent<ComponentY>()->GetYValue() << std::endl;

	//// Create another lua state:
	//sol::state another_lua;
	//
	//// Enable libraries for that lua state:
	//another_lua.open_libraries
	//(
	//	sol::lib::base,
	//	sol::lib::package,
	//	sol::lib::debug,
	//	sol::lib::string,
	//	sol::lib::io,
	//	sol::lib::coroutine,
	//	sol::lib::os,
	//	sol::lib::table,
	//	sol::lib::math
	//);

	//another_lua.do_string(c_path);
	//another_lua.do_string(path);

	//sol::table another_namespace = another_lua["hachiko"].get_or_create<sol::table>();

	//// Add entity and components to lua:
	//component_type_to_lua(another_namespace);
	//component_to_lua(another_namespace);
	//component_x_to_lua(another_namespace);
	//component_y_to_lua(another_namespace);
	//entity_to_lua(another_namespace);

	//std::string table_name = "experiment";
	//sol::table experiment_script_table = lua[table_name];
	//sol::table other_script_table = another_lua["ExperimentScript"].get_or_create<sol::table>();

	//std::cout << "Adding the experiment script table member functions to another script which runs the functions from experiment.lua" << std::endl;

	//for (const auto& entry : experiment_script_table)
	//{
	//	const sol::object& current_object = entry.second;

	//	if (!current_object.is<sol::function>())
	//	{
	//		continue;
	//	}

	//	const std::string& function_name = entry.first.as<std::string>();

	//	std::cout << "Adding: " << function_name << std::endl;

	//	sol::function function = experiment_script_table[function_name];

	//	other_script_table.set_function(function_name, function);
	//}

	//// another_lua["ExperimentScript"].set(other_script_table);

	//sol::protected_function_result result = another_lua.do_string("ExperimentScript:call_from_another_script(888, \"please work\")");

	//if (!result.valid())
	//{
	//	sol::error error = result;
	//	std::cout << "[ERROR-LUA]: " << error.what() << std::endl;
	//	std::cin.ignore();
	//	return 0;
	//}

	//std::string name = lua["experiment"]["name"];

	//std::cout << name << std::endl;


	std::cin.ignore();
	return 0;
}