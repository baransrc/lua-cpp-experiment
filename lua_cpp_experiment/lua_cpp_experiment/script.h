#pragma once

#include "lua_helper.h"

class Entity;

class LuaScript
{
public:
	LuaScript(std::string script_file_name, Entity* entity);
	~LuaScript() = default;

	void LoadDependency(sol::table dependency_table);
	void Execute(std::string method_name);

	const sol::state& GetState() const;
	const sol::table& GetAsTable() const;
	const std::string& GetName() const;

	static bool CheckIfValid(const sol::load_result& load_result);

private:
	void LoadScriptSources();
	bool LoadPath();

private:
	Entity* attached_entity;
	sol::state lua_state;
	sol::table script_table;
	std::string name;
	std::string path;
};