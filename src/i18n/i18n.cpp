#include "i18n/i18n.h"

#include <clocale>
#include <cstdlib>
#include <iostream>
#include <utility>

#include "engine/luamanager.h"
#include "utility/utility.h"

using namespace luabind;
using namespace std;

namespace Typhon
{
	

	I18N::I18N(LuaManager *lua)
		: lua(lua), language(EN)
	{
		langSelector = nullptr;
		setlocale(LC_ALL, "");
		
		// add new language entries here after adding them to the enum declaration
		langMap[EN] = "en";
		langMap[DE] = "de";
	}

	I18N::~I18N()
	{
	}

	void I18N::AddElementWithText(irr::gui::IGUIElement *element, const std::string &retrievalKey)
	{
		auto elementTuple = make_tuple(element, retrievalKey);
		i18nElements.insert(make_pair(element->getID(), elementTuple));
		element->setText(GetText(language, retrievalKey).c_str());
	}

	void I18N::ChangeLanguage(const LANG lang)
	{
		language = lang;

		for(auto iter = i18nElements.begin(); iter != i18nElements.end(); ++iter)
		{
			std::get<0>(iter->second)->setText(GetText(language, std::get<1>(iter->second)).c_str());
		}
	}

	void I18N::ClearAllElements()
	{
		i18nElements.erase(i18nElements.begin(), i18nElements.end());
	}

	void I18N::ClearElement(const int elementID)
	{
		i18nElements.erase(elementID);
	}

	std::string I18N::ConvertLangToString(LANG lang)
	{
		if(langMap.find(lang) != langMap.end())
		{
			return langMap[lang];
		}

		return "";
	}

	LANG I18N::ConvertStringToLang(const std::string &langStr)
	{
		for(auto iter = langMap.begin(); iter != langMap.end(); ++iter)
		{
			if(iter->second == langStr)
			{
				return iter->first;
			}
		}

		return INVALID;
	}

	int I18N::GetNumberOfLanguages()
	{
		return static_cast<int>(langMap.size());
	}

	std::wstring I18N::GetText(const LANG lang, const std::string &entry)
	{
		// Depending on text retrieval frequency, may want to load a whole language's text at once,
		// store (and convert) it, and return text as needed.  If only GUI/HUD uses text, this should
		// be sufficient.
		string text;
		try
		{
			text = call_function<std::string>(lua->luaState, "GetText", ConvertLangToString(lang), entry); 
		}
		catch (luabind::error& e)
		{
			string error = lua_tostring(lua->luaState, -1 );
			cout << "\n" << e.what() << "\n" << error << "\n";
			return L"";
		}
		
		return ConvertStrToWide(text);
	}
}
