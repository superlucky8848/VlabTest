#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vad.h"

int main(int argc, char* argv[])
{
	clock_t start=clock(), diff;
	fprintf(stderr, "Test Begin\n");
	
	char* pcm8KName = argv[1];
	char* textGridName = NULL;
	float * sted = NULL;
	int n_max_interval = 5000;

	int wavNameLen = strlen(pcm8KName);
	textGridName = (char *) malloc(sizeof(char) * wavNameLen + 9);
	if(!textGridName)
	{
		fprintf(stderr, "Failed to alloc buffer for textGridName!\n\n");
		return -1;
	}
	strcpy(textGridName, pcm8KName);
	textGridName[wavNameLen-3]=0;
	strcat(textGridName, "TextGrid");
	
	fprintf(stderr, "textGridName: %s\n", textGridName);

	sted = (float*)malloc(sizeof(float) * n_max_interval * 2);
	if(!sted)
	{
		fprintf(stderr, "Failed to alloc buffer for vad!\n\n");
		free(textGridName);
		return -1;
	}

	FILE * fptr = fopen(textGridName, "w");
	if(!fptr)
	{
		fprintf(stderr, "Failed to open output file!\n\n");
		free(textGridName);
		free(sted);
		return -1;
	}

	int n_speech_interval = vad(pcm8KName, sted, n_max_interval, 16, 0.4F);
	
	fprintf(fptr, "File type = \"ooTextFile\"\n");
	fprintf(fptr, "Object class = \"TextGrid\"\n\n");
	fprintf(fptr, "xmin = 0 \n");
	fprintf(fptr, "xmax = %f \n", sted[2*n_speech_interval-1]);
	fprintf(fptr, "tiers? <exists> \n");
	fprintf(fptr, "size = 1 \n");
	fprintf(fptr, "item []: \n");
	fprintf(fptr, "    item [1]:\n");
	fprintf(fptr, "        class = \"IntervalTier\" \n");
	fprintf(fptr, "        name = \"VAD\" \n");
	fprintf(fptr, "        xmin = %f \n");
	fprintf(fptr, "        xmax = %f \n", sted[2*n_speech_interval-1]);
	fprintf(fptr, "        intervals: size = %d \n", n_speech_interval);
	fflush(fptr);
	
	for(int i = 0; i < n_speech_interval; i ++)
	{
		fprintf(fptr, "        intervals [%d]:\n", i+1);
		fprintf(fptr, "            xmin = %f \n", sted[i*2]);
		fprintf(fptr, "            xmax = %f \n", sted[i*2+1]);
		fprintf(fptr, "            text = \"\" \n");
		fflush(fptr);
	}

	fclose(fptr);

	free(sted);
	free(textGridName);
		
	diff = clock()-start;
	int msec = diff *1000/CLOCKS_PER_SEC;
	fprintf(stderr, "Test End.[%d.%03d sec]\n", msec/1000, msec%1000);

	_CrtDumpMemoryLeaks();
	system("PAUSE");
	return 0;
}

