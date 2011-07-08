#ifndef WIN32
#include <cstdio>
#endif

#include <unistd.h>

#include <metrics/metrics.h>

namespace Typhon
{

int Metrics::GetPerfScore()
{
	int score = 0;
	int numCPU = 0, speedCPU = 0, memory = 0;

#ifdef WIN32
#else
	// get number of cores
	numCPU = sysconf(_SC_NPROCESSORS_ONLN);
	printf("%d\n",numCPU);
	// get installed physical memory
	FILE *resultStream;

	// run grep on /proc/meminfo
	resultStream = popen("grep \"MemTotal*\" /proc/meminfo", "r");

	if (!resultStream)
	{
		return -1;
	}

	fscanf(resultStream, "*%d", &memory);
	printf("%d\n",memory);

	if (pclose(resultStream) != 0)
	{
		fprintf(stderr, " Error: Failed to close command stream \n");
	}
#endif

	score = (numCPU * speedCPU) * 0.5 + memory * 0.5;
	return score;
}

}
