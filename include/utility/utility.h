#ifndef UTILITY_H
#define UTILITY_H

#include <string>

namespace Typhon
{
	std::wstring ConvertStrToWide(const std::string &str);
	std::string ConvertWideToStr(const std::wstring &str);
}

#endif