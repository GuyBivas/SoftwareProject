#include "SDL_Shared.h"

#define BUTTON_WIDTH 307
#define BUTTON_HEIGHT 84
#define BUTTON_MARGIN 30

Widget* buttonNew;
Widget* buttonLoad;
Widget* buttonExit;

void clickButton(void)
{
	//rend = rend;
	//SDL_ShowSimpleMessageBox(
	//	SDL_MESSAGEBOX_INFORMATION,
	//	"Title",
	//	"We did it",
	//	NULL);

	changeWindow(WINDOW_SETTINGS);
}

void clickButtonExit(void)
{
	exitGameSDL();
}

void mainWindowCreate()
{
	SDL_Rect rect;
	int buttonsX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	int buttonDiffY = BUTTON_HEIGHT + BUTTON_MARGIN;

	createLogo();

	rect = newRect(buttonsX, 220, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButton(rend, "assets/main/newGame.bmp", rect, clickButton));

	rect = newRect(buttonsX, 220 + buttonDiffY, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButton(rend, "assets/main/loadGame.bmp", rect, clickButton));

	rect = newRect(buttonsX, 220 + 2 * buttonDiffY, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButton(rend, "assets/main/exit.bmp", rect, clickButtonExit));

	/*if (logo == NULL || buttonNew == NULL || buttonLoad == NULL || buttonExit == NULL)
	{
		freeResources(true);
		return;
	}*/
}