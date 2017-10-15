#ifndef _INPUT_H
#define _INPUT_H

#include "CPU.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_ARGS 99

void getCPUInput(char* keystate)
{


}

void getWindowInput()
{

}

void getFile(const char* fileDirectory)
{
	FILE *fp = fopen(fileDirectory, "rb");
	fclose(fp);
}

void getArgs(char* args[])
{
	char* newArgs[MAX_ARGS];
	strcpy(newArgs, args);
	//parse arguments

	for(int i=0;i<sizeof(newArgs)/sizeof(char*);++i)
	{
		if(strcmp(newArgs[i], "-f") == 0)
		{
			getFile(newArgs[i+1]);
			i += 1;
		}
	}
}

#endif
