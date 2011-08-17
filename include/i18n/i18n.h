#ifndef I18N_H
#define I18N_H

#include <string>

#include <boost/noncopyable.hpp>

namespace Typhon
{
	class LuaManager;

	class I18N : boost::noncopyable
	{
	private:

		LuaManager *lua;

	public:
		
		enum LANG { EN, DE };

		I18N(LuaManager *lua);
		~I18N();

		std::wstring GetText(LANG lang, const std::string &entry);
	};
}
#endif