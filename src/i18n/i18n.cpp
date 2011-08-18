#include "i18n/i18n.h"

#include <clocale>
#include <cstdlib>
#include <iostream>

#include "engine/luamanager.h"

using namespace luabind;
using namespace std;

namespace Typhon
{
	std::string I18N::ConvertLangToString(LANG lang)
	{
		switch(lang)
		{
		case EN:
			return "en";
		case DE:
			return "de";
		default:
			std::cout << "Bad language code passed in.\n";
			return "";
		}

		return "";
	}

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

		setlocale(LC_ALL, "");
	}

	I18N::~I18N()
	{
	}

	std::wstring I18N::GetText(LANG lang, const std::string &entry)
	{
		string text;
		try
		{
			text = call_function<std::string>(lua->luaState, "GetText", ConvertLangToString(lang) , entry); 
		}
		catch (luabind::error& e)
		{
			string error = lua_tostring(lua->luaState, -1 );
			cout << "\n" << e.what() << "\n" << error << "\n";
			return L"";
		}

		
		int size = mbstowcs(nullptr, text.c_str(), 0);
		if(size == -1)
		{
			return L"";
		}
		wchar_t *buffer = new wchar_t[size + 1];
		mbstowcs(buffer, text.c_str(), size + 1);
		wstring convertedText(buffer);
		delete []buffer;
		return convertedText;
	}
}