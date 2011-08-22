#ifndef I18N_H
#define I18N_H

#include <map>
#include <string>
#include <tuple>

#include <boost/noncopyable.hpp>

#include "irrlicht/irrlicht.h"

namespace Typhon
{
	class LuaManager;

	enum LANG { EN, DE };
	
	class I18N : boost::noncopyable
	{
	private:

		typedef std::tuple<irr::gui::IGUIElement*, std::string> GUIItem;
		typedef std::map<int, GUIItem> I18NElementsMap;
		
		I18NElementsMap i18nElements;

		LuaManager *lua;

	public:

		irr::gui::IGUIComboBox *langSelector;		
		LANG language;

		I18N(LuaManager *lua);
		~I18N();

		void AddElementWithText(irr::gui::IGUIElement *element, const std::string &retrievalKey);
		void ChangeLanguage(const LANG lang);
		void ClearAllElements();
		void ClearElement(const int elementID);
		std::string ConvertLangToString(LANG lang);
		int GetNumberOfLanguages();
		std::wstring GetText(const LANG lang, const std::string &entry);
	};
}
#endif