#ifndef I18N_H
#define I18N_H

#include <string>

#include <boost/noncopyable.hpp>

namespace Typhon
{
	class LuaManager;

	enum LANG { EN, DE };
	
	class I18N : boost::noncopyable
	{
	private:

		LuaManager *lua;

		std::string ConvertLangToString(LANG lang);

	public:

		I18N(LuaManager *lua);
		~I18N();

		std::wstring GetText(LANG lang, const std::string &entry);
	};
}
#endif