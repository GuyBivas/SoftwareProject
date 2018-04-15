#include "SDL.h"
#include "SDL_Video.h"
#include "SDL_MainWindow.h"

void createWindow()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_SHOWN);
	SDL_Surface* screenSurface = NULL;

	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Get window surface
		screenSurface = SDL_GetWindowSurface(window);

		//Fill the surface white
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

		//Update the surface
		SDL_UpdateWindowSurface(window);

		//Wait two seconds
		SDL_Delay(2000);
	}

	SDL_Quit();
}