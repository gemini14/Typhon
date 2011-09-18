#ifndef UTILITY_H
#define UTILITY_H

#include <string>

#ifdef WIN32
#include <WS2tcpip.h>
#else
#include <netinet/in.h>
#endif

namespace Typhon
{
	std::wstring ConvertStrToWide(const std::string &str);
	std::string ConvertWideToStr(const std::wstring &str);
	std::string GetIPStringForm(sockaddr_in *address);
	unsigned long GetNetworkIP(const sockaddr_in &address);
	unsigned long GetNetworkIP(const in_addr &address);
	const in_addr StoreIPNumber(const unsigned long ip);
}

#endif
