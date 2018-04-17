#include "SDL_Shared.h"

bool exitGameFlag;

void createWindow(WINDOW_TYPE newType)
{
	switch (newType)
	{
	case WINDOW_MAIN:
		mainWindowCreate();
		break;
	case WINDOW_SETTINGS:
		settingsWindowCreate();
		break;
	case WINDOW_LOAD:
		break;
	case WINDOW_SAVE:
		break;
	case WINDOW_GAME:
		break;
	default:
		break;
	}
}

//void destroyCurrWindow()
//{
//	widgetListClear(widgets);
//}

void changeWindow(WINDOW_TYPE newType)
{
	if (rend != NULL)
	{
		widgetListClear(widgets);
		SDL_DestroyRenderer(rend);
	}

	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (rend == NULL)
	{
		freeResources(true);
		return;
	}

	currWindowType = newType;
	createWindow(newType);
}

void createLogo()
{
	int logoX = (WINDOW_WIDTH - LOGO_WIDTH) / 2;
	SDL_Rect rectLogo = { .x = logoX,.y = 10,.w = LOGO_WIDTH,.h = 155 };
	widgetListAddLast(widgets, createButton(rend, "assets/logo.bmp", rectLogo, NULL));
}

void freeResources(bool isError)
{
	if (isError)
		printf("SDL ERROR: %s\n", SDL_GetError());

	if (window != NULL)
		SDL_DestroyWindow(window);

	if (rend != NULL)
		SDL_DestroyRenderer(rend);

	widgetListClear(widgets);
}

void SDL_Loop()
{
	// event handling loop
	exitGameFlag = false;
	SDL_Event e;
	while (!exitGameFlag)
	{
		while (SDL_PollEvent(&e))
		{
			for (int i = 0; i < widgets->actualSize; i++)
			{
				widgetHandleEvent(widgetListGetAt(widgets, i), e);
			}

			switch (e.type)
			{
			case SDL_QUIT:
				exitGameFlag = true;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE)
					exitGameFlag = true;
				break;

			}
		}

		SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
		SDL_RenderClear(rend);

		for (int i = 0; i < widgets->actualSize; i++)
		{
			widgetRender(widgetListGetAt(widgets, i), rend);
		}

		// present changes to user
		SDL_RenderPresent(rend);

		// small delay
		SDL_Delay(10);
	}
}

void exitGameSDL()
{
	exitGameFlag = true;
}

void initSDL()
{
	//SP_BUFF_SET();
	atexit(SDL_Quit);

	// initialize SDL2 for video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		freeResources(true);
		return;
	}

	// create an SDL window
	window = SDL_CreateWindow(
		"Chess Master",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL);

	// make sure window was created successfully
	if (window == NULL) {
		freeResources(true);
		return;
	}

	widgets = widgetListCreate(100);
}

int main_SDL()
{
	manager = gameManagerCreate(HISTORY_SIZE_SDL);

	initSDL();
	changeWindow(WINDOW_MAIN);
	SDL_Loop();

	freeResources(false);
	return 0;
}

SDL_Rect newRect(int x, int y, int width, int height)
{
	SDL_Rect rect = { .x = x,.y = y,.w = width,.h = height };
	return rect;
}