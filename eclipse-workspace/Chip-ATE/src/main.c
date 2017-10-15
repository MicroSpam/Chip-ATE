/*
 ============================================================================
 Name        : Chip8.c
 Author      : MicroSpam
 Version     :
 Copyright   : i don't give a shit about this, so do whatever you want.
 Description : Chip-8 Interpreter
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "CPU.h"
#include "Window.h"
#include "Input.h"

int main(int argc, char* argv[])
{
	//parse args
	getArgs(argv);

	struct CPU* i_CPU;
	i_CPU = initCPU();

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* screen;

	window = createWindow();
	renderer = createRenderer(window);
	screen = createScreen(renderer);

	while(isRunning == 1)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				isRunning = 0;
			}

			runCPU(i_CPU);
			drawWindow(window, renderer, screen, i_CPU->screenBuffer);
		}

	}

	destroyWindow(window, renderer, screen);
	destroyCPU(i_CPU);

	return 0;
}
