#include "script.h"
#include "entity_component.h"

LuaScript::LuaScript(std::string script_file_name, Entity* entity)
{	
	attached_entity = entity;

    // TODO: This is so error prone, implement a better way of this in
    // the actual engine.
	path = script_file_name;
    name = path.substr(script_file_name.find_last_of('/'), script_file_name.find_first_of('.'));

	LoadScriptSources();
}

void LuaScript::LoadDependency(sol::table dependency_table)
{

}

void LuaScript::Execute(std::string method_name)
{

}

const sol::state& LuaScript::GetState() const
{
    return lua_state;
}

const sol::table& LuaScript::GetAsTable() const
{
    return script_table;
}

const std::string& LuaScript::GetName() const
{
    return name;
}

bool LuaScript::CheckIfValid(const sol::load_result& load_result)
{
	if (!load_result.valid())
	{
		sol::error error = load_result;
		std::cerr << "FAILED: " << error.what() << std::endl;

		return false;
	}

	return true;
}

void LuaScript::LoadScriptSources()
{
	lua_state = sol::state(sol::c_call<decltype(&print_panic), &print_panic>);

	// Enable base libraries:
	lua_state.open_libraries
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

	// Load lookup paths for dlls and script files:
	bool loaded_path = LoadPath();

	if (loaded_path)
	{
		sol::load_result script_load_result = lua_state.load_file(path);
		bool loaded_script = CheckIfValid(script_load_result);

		if (loaded_script)
		{
			// Create Entity namespace:
			sol::table this_namespace = lua_state["hachiko"].get_or_create<sol::table>();

			// Add entity and components to lua:
			component_type_to_lua(this_namespace);
			component_to_lua(this_namespace);
			component_x_to_lua(this_namespace);
			component_y_to_lua(this_namespace);
			entity_to_lua(this_namespace);

			// Add the api to interact with entity to namespace:
			this_namespace.set("entity", attached_entity);

			// Run the resulting script (Sort of compile it):
			script_load_result();
		}
	}

	script_table = lua_state["ExperimentScript"].get_or_create<sol::table>();
}

bool LuaScript::LoadPath()
{
	sol::load_result settings_load_result = lua_state.load_file("settings.lua");

	if (!CheckIfValid(settings_load_result))
	{
		return false;
	}

	// Run the settings script:
	settings_load_result();

	// TODO: Get these runtime, and relative to working directory.
	std::string project_path = lua_state["project_path"];
	std::string script_file_name = lua_state["script_file_name"];
	std::string lua_path = lua_state["lua_path"];
	std::string zbs_lib_path = lua_state["zbs_lib_path"];

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
	lua_state.do_string(c_path);
	lua_state.do_string(path);

	return true;
}