#include "utility/utility.h"

#ifdef WIN32
#include <Windows.h>
#else
#endif

using namespace std;

namespace Typhon
{
	std::wstring ConvertStrToWide(const std::string &str)
	{
		if(!str.length())
		{
			return L"";
		}

#ifdef WIN32
		auto size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		if(!size)
		{
			return L"";
		}
		wchar_t *buffer = new wchar_t[size];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, size);
		wstring convertedText(buffer);
		delete []buffer;
		return convertedText;
#else
		// TODO: Add Linux equivalent
#endif
	}

	std::string ConvertWideToStr(const std::wstring &str)
	{
		if(!str.length())
		{
			return "";
		}

#ifdef WIN32
		auto size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if(!size)
		{
			return "";
		}
		char *buffer = new char[size];
		WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, buffer, size, nullptr, nullptr);
		string convertedText(buffer);
		delete []buffer;
		return convertedText;
#else
		// TODO: Add Linux equivalent
#endif
	}
}
