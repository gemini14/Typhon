#ifndef USERNAME_H
#define USERNAME_H

#include <string>

namespace Typhon
{
	std::string ConvertWideToCharString(const std::wstring &str);
	std::wstring GetUsername();
}

#endif