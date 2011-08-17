#include "i18n/i18n.h"

#include "engine/luamanager.h"

using namespace luabind;

namespace Typhon
{
	I18N::I18N(LuaManager *lua)
		: lua(lua)
	{
		lua->LoadScript("scripts/scriptList.lua");
		auto scripts = lua->GetLuaObject("scriptList");
		assert(type(scripts) == LUA_TTABLE);
		for(luabind::iterator i(scripts), end; i != end; ++i)
		{
			lua->LoadScript(object_cast<std::string>(*i));
		}
	}

	I18N::~I18N()
	{
	}
	
	std::wstring I18N::GetText(LANG lang, const std::string &entry)
	{
		return std::wstring();
	}
}