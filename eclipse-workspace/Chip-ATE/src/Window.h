#ifndef _WINDOW_H
#define _WINDOW_H

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

#define W 64
#define H 32

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

uint32_t color = 0xFF00CC00; //green

char isRunning = 1;

int initSDL()
{
	SDL_Init(SDL_INIT_VIDEO);
	return 1;
}


SDL_Window* createWindow()
{
	initSDL();
	SDL_Window* window;
	window = SDL_CreateWindow("Chip-ATE",
							SDL_WINDOWPOS_CENTERED,
							SDL_WINDOWPOS_CENTERED,
							SCREEN_WIDTH,
							SCREEN_HEIGHT,
							SDL_WINDOW_RESIZABLE);

	if(window == NULL)
	{
		return NULL;
	}

	else
	{
		return window;
	}
}

SDL_Renderer* createRenderer(SDL_Window* arg_window)
{
	SDL_Renderer* renderer;
	renderer = SDL_CreateRenderer(arg_window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL)
	{
		return NULL;
	}
	else
	{
		return renderer;
	}
}

SDL_Texture* createScreen(SDL_Renderer* arg_renderer)
{
	return SDL_CreateTexture(arg_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, W, H);
}

void copyScreenBuffer(uint32_t* pixels, uint8_t* cpuScreenBuffer)
{
	for (int i=0;i<W*H;++i)
	{
		pixels[i] = 0x00000000;
		uint32_t currentPixel = cpuScreenBuffer[i] * color;
		pixels[i] = currentPixel;
	}
}

//DEBUG: Remove this function when possible
void fillScreen(uint8_t* cpuScreenBuffer)
{
	memset(cpuScreenBuffer, 1, W*H);
}

void destroyWindow(SDL_Window* arg_window, SDL_Renderer* arg_renderer, SDL_Texture* arg_screen)
{
	SDL_DestroyTexture(arg_screen);
	SDL_DestroyWindow(arg_window);
	SDL_DestroyRenderer(arg_renderer);
	SDL_Quit();
}

void drawWindow(SDL_Window* arg_window, SDL_Renderer* arg_renderer, SDL_Texture* arg_screen, uint8_t* screenBuffer)
{
	uint32_t* pixels = (uint32_t*)malloc(sizeof(uint32_t)*W*H);
	copyScreenBuffer(pixels, screenBuffer);
	SDL_UpdateTexture(arg_screen, NULL, pixels, 4*W);
	SDL_RenderCopy(arg_renderer, arg_screen, NULL, NULL);
	SDL_RenderPresent(arg_renderer);
	SDL_RenderClear(arg_renderer);
}

#endif
