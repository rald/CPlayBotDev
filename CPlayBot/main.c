#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "common.h"

#define ROBOT_IMPLEMENTATION
#include "robot.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;
SDL_Event event;

bool quit = false;

int main(void)
{
	int nrobots = 1;
	Robot *robots[nrobots];

	srand(time(NULL));

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_SetRenderTarget(renderer, display);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	for (int i = 0; i < nrobots; i++)
	{
		robots[i] = Robot_Create(i, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, rand() % 360);
	}

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		SDL_SetRenderTarget(renderer, display);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 32);

		SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_RenderFillRect(renderer, &rect);

		for (int i = 0; i < nrobots; i++)
		{
			Robot_Draw(renderer, robots[i]);
		}

		/*			
		for (int i = 0; i < nrobots; i++)
		{
			Robot_Move(robots[i], 16);
			Robot_Turn(robots[i], (rand() % 3 - 1) * (rand() % 16));
		}
*/

		for (int i = 0; i < nrobots; i++)
		{
			Robot_Scan(renderer, robots[i], robots, nrobots, 300, 90);
		}

		int numTouchDevices = SDL_GetNumTouchDevices();
		for (int i = 0; i < numTouchDevices; i++)
		{
			SDL_TouchID touchId = SDL_GetTouchDevice(i);
			if (touchId)
			{
				int numTouchFingers = SDL_GetNumTouchFingers(touchId);
				for (int j = 0; j < numTouchFingers; j++)
				{
					SDL_Finger *finger = SDL_GetTouchFinger(touchId, j);
					if (finger)
					{
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
						DrawCircle(renderer, (int)(finger->x * SCREEN_WIDTH), (int)(finger->y * SCREEN_HEIGHT), 32);
					}
				}
			}
		}

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	return 0;
}

