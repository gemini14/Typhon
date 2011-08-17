#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <string>

#include <boost/noncopyable.hpp>

extern "C"
{
#include "luabind/lua.h"
#include "luabind/lualib.h"
#include "luabind/lauxlib.h"
}

#include "luabind/luabind.hpp"
#include "luabind/object.hpp"

namespace Typhon
{
	class LuaManager : boost::noncopyable
	{
	private:

		lua_State *luaState;

	public:

		LuaManager();
		~LuaManager();

		luabind::object GetLuaObject(const std::string &objName);
		bool LoadScript(const std::string &filename);
	};
}

#endif