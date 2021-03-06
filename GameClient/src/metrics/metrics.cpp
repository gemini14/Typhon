#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else // Linux
#include <unistd.h>
#endif

#include <string>

#include <boost/lexical_cast.hpp>

#include "logger/logger.h"
#include "metrics/metrics.h"

using namespace std;

namespace Typhon
{

int Metrics::GetPerfScore()
{
	long numCores = 0, speedCPU = 0;
	int score = 0, memory = 0;

#ifdef WIN32
	// get number of cores
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	numCores = sysinfo.dwNumberOfProcessors;

	// get CPU speed (max)
	HKEY key;
	long error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0, KEY_QUERY_VALUE, &key);
	
	if(error != ERROR_SUCCESS)
	{
		char buffer[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, buffer, 256, 0);
		Log("Error: Problem opening registry key to find CPU speed.");
	}
	else
	{
		// query registry key
		DWORD bufferSize = 128;
		long data;
		if(RegQueryValueEx(key, "~MHz", nullptr, nullptr, (LPBYTE)&data, &bufferSize) == ERROR_SUCCESS)
		{
			speedCPU = data;
		}
	}

	// get installed physical memory
	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(memoryStatus);
	GlobalMemoryStatusEx(&memoryStatus);

	// divide by 1048576 (1024^2) to get memory in MB
	memory = static_cast<int>(memoryStatus.ullTotalPhys / 1048576);
#else
	// get number of cores
	numCores = sysconf(_SC_NPROCESSORS_ONLN);
	Log("Number of cores: " + boost::lexical_cast<string>(numCores));

	// get CPU speed (max)
	FILE *resultCPU = popen("cat /proc/cpuinfo | grep \"cpu MHz\"", "r");

	if (resultCPU)
	{
		float temp;
		while (fscanf(resultCPU, "%*s %*s %*c %f", &temp) != EOF)
		{
			speedCPU += temp;
		}

		speedCPU /= numCores;

		if (pclose(resultCPU) == -1)
		{
			Log("Error: Failed to close command stream after querying system for CPU speed.");
		}
	}

	// get installed physical memory
	FILE *resultMem = popen("grep \"MemTotal*\" /proc/meminfo", "r");

	if (resultMem)
	{

		fscanf(resultMem, "%*s %d", &memory);
		Log("Memory: " +  boost::lexical_cast<string>(memory));

		if (pclose(resultMem) == -1)
		{
			Log("Error: Failed to close command stream after querying system for memory.");
		}
	}
#endif

	score = static_cast<int>((speedCPU * .4) + (memory * 0.4) + (numCores * .2));
	return score;
}

}
