#include "engine/luamanager.h"

#include <cassert>

using namespace luabind;
using namespace std;

namespace Typhon
{
	LuaManager::LuaManager()
	{
		luaState = luaL_newstate();
		luaL_openlibs(luaState);
		open(luaState);
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
			cout << "Error loading Lua script.  Error code: " << result << "\n"; 
			cout << "\n" << lua_tostring(luaState, -1) << "\n";
			return false;
		}

		return true;
	}
}