#include "engine/luamanager.h"

#include <cassert>

#include <boost/lexical_cast.hpp>

#include "logger/logger.h"

using namespace luabind;
using namespace std;

namespace Typhon
{
	LuaManager::LuaManager()
	{
		luaState = luaL_newstate();
		luaL_openlibs(luaState);
		open(luaState);

		LoadScript("scripts/script_list.lua");
		auto scripts = GetLuaObject("scriptList");
		assert(type(scripts) == LUA_TTABLE);
		for(luabind::iterator i(scripts), end; i != end; ++i)
		{
			LoadScript(object_cast<std::string>(*i));
		}
	}

	LuaManager::~LuaManager()
	{
		lua_close(luaState);
	}

	luabind::object LuaManager::GetLuaObject(const std::string &objName)
	{
		luabind::object object = globals(luaState)[objName.c_str()];
		assert(object.is_valid());
		// must check if type is valid in caller, only object validity (nil-ness) is checked here
		return object; 
	}

	bool LuaManager::LoadScript(const std::string &filename)
	{
		int result = luaL_dofile(luaState, filename.c_str());
		if(result)
		{
			Log("Error loading Lua script.  Error code: " + boost::lexical_cast<string>(result)); 
			Log("\n" + string(lua_tostring(luaState, -1)));
			return false;
		}

		return true;
	}
}