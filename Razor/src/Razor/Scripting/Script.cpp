#include "rzpch.h"
#include "Script.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Razor
{

	Script::Script()
	{
		lua_State* lua = luaL_newstate();

		std::string cmd = "a = 7 + 11";

		int status = luaL_dostring(lua, cmd.c_str());

		if (status == LUA_OK)
		{
			lua_getglobal(lua, "a");

			if (lua_isnumber(lua, -1))
			{
				float n = (float)lua_tonumber(lua, -1);
				Log::trace("Lua return: %f", n);
			}
		}
		else
		{
			std::string error = lua_tostring(lua, -1);
			Log::error("Lua scripting error: %s", error);
		}

		lua_close(lua);
	}

	Script::~Script()
	{
	}

}

