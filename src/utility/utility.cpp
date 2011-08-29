#include "utility/utility.h"

#include <cstdlib>

using namespace std;

namespace Typhon
{
	std::wstring ConvertStrToWide(const std::string &str)
	{
		auto size = mbstowcs(nullptr, str.c_str(), 0);
		if(size == static_cast<size_t>(-1))
		{
			return L"";
		}
		wchar_t *buffer = new wchar_t[size + 1];
		mbstowcs(buffer, str.c_str(), size + 1);
		wstring convertedText(buffer);
		delete []buffer;
		return convertedText;
	}

	std::string ConvertWideToStr(const std::wstring &str)
	{
		auto size = wcstombs(nullptr, str.c_str(), 0);
		if(size == static_cast<size_t>(-1))
		{
			return "";
		}
		char *buffer = new char[size + 1];
		wcstombs(buffer, str.c_str(), size + 1);
		string convertedText(buffer);
		delete []buffer;
		return convertedText;
	}
}
