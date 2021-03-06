#include "utility/utility.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <arpa/inet.h>
#include <cstdlib>
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
		size_t len = mbstowcs(nullptr, str.c_str(), -1);
		if(len == static_cast<size_t>(-1))
		{
			return L"";
		}
		wchar_t *buffer = new wchar_t[len + 1];
		mbstowcs(buffer, str.c_str(), len + 1);
		wstring convertedText(buffer);
		delete []buffer;
		return convertedText;
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
		size_t len = wcstombs(nullptr, str.c_str(), -1);
		if(len == static_cast<size_t>(-1))
		{
			return "";
		}
		char *buffer = new char[len + 1];
		std::wcstombs(buffer, str.c_str(), len + 1);
		string convertedText(buffer);
		delete []buffer;
		return convertedText;
#endif
	}

	std::string GetIPStringForm(sockaddr_in *address)
	{
		char IP_str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &address->sin_addr, IP_str, INET_ADDRSTRLEN);
		return string(IP_str);
	}

	unsigned long GetNetworkIP(const sockaddr_in &address)
	{
#ifdef WIN32
		return address.sin_addr.S_un.S_addr;
#else
		return address.sin_addr.s_addr;
#endif
	}

	unsigned long GetNetworkIP(const in_addr &address)
	{
#ifdef WIN32
		return address.S_un.S_addr;
#else
		return address.s_addr;
#endif
	}

	const in_addr StoreIPNumber(const unsigned long ip)
	{
		in_addr newAddr;
#ifdef WIN32
		newAddr.S_un.S_addr = ip;
#else
		newAddr.s_addr = ip;
#endif
		return newAddr;
	}
}
