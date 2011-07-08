#ifndef WIN32
#include <cstdio>
#endif

#include <unistd.h>

#include <metrics/metrics.h>

namespace Typhon
{

int Metrics::GetPerfScore()
{
	long numCores = 0;
	int score = 0, memory = 0;
	float speedCPU = 0;

#ifdef WIN32
#else
	// get number of cores
	numCores = sysconf(_SC_NPROCESSORS_ONLN);
	printf("Number of cores: %ld\n", numCores);

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
			printf(
					"Error: Failed to close command stream after querying CPU speed.\n");
		}
	}

	// get installed physical memory
	FILE *resultMem = popen("grep \"MemTotal*\" /proc/meminfo", "r");

	if (resultMem)
	{

		fscanf(resultMem, "%*s %d", &memory);
		printf("Memory: %d\n", memory);

		if (pclose(resultMem) == -1)
		{
			printf(
					"Error: Failed to close command stream after querying memory.\n");
		}
	}
#endif

	score = (numCores * speedCPU) * 0.5 + memory * 0.5;
	return score;
}

}
