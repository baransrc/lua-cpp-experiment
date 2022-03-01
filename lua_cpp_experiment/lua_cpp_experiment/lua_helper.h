#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

inline void print_panic(sol::optional<std::string> maybe_msg) 
{
	std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
	
	if (maybe_msg)
	{
		const std::string& msg = maybe_msg.value();
		std::cerr << "\terror message: " << msg << std::endl;
	}
	// When this function exits, Lua will exhibit default behavior and abort()
}