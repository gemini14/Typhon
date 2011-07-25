#include "utility/username.h"

#include <cstdlib>
#include <string>

#include <Windows.h>
#include <LMCons.h>

using namespace std;

namespace Typhon
{
	static std::wstring GetUsernameLinux();
	static std::wstring GetUsernameWin();	


	std::wstring GetUsername()
	{
		std::wstring name;
#ifdef WIN32
		name = GetUsernameWin();
#else
		name = GetUsernameLinux();
#endif
		return name;
	}
	
	std::string ConvertWideToCharString(const std::wstring &str)
	{
		// int length = wcstombs(nullptr, str.c_str(), 
		std::string mbName;
		return mbName;
	}

	std::wstring GetUsernameLinux()
	{
		return L"Player 1";
	}

	std::wstring GetUsernameWin()
	{
		char user[UNLEN + 1];
		DWORD nameSize = sizeof user;
		if(GetUserName(user, &nameSize))
		{
			int length = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, user, -1, nullptr, 0);
			wchar_t *convertedName = new wchar_t[length];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, user, -1, convertedName, length);
			std::wstring name(convertedName);
			delete[] convertedName;
			return name;
		}
		else
		{
			return L"Player 1";
		}
	}
}