#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "Button.h"
#include "Enums.h"
#include "ChessGameManager.h"
#include "WidgetList.h"

#define HISTORY_SIZE_SDL 6
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LOGO_WIDTH 299

typedef enum {
	WINDOW_MAIN,
	WINDOW_SETTINGS,
	WINDOW_LOAD,
	WINDOW_SAVE,
	WINDOW_GAME,
} WINDOW_TYPE;

ChessGameManager* manager;
SDL_Window* window;
SDL_Renderer* rend;
WINDOW_TYPE currWindowType;
//Widget* logo;
WidgetList* widgets;

// general
int main_SDL();
void changeWindow(WINDOW_TYPE newType);
void freeResources(bool isError);
void createLogo();
SDL_Rect newRect(int x, int y, int width, int height);
void exitGameSDL();

// main window
void mainWindowCreate();
//void mainWindowHandleButtons(SDL_Event e);
//void mainWindowRender();
//void mainWindowDestroy();

// settings window
void settingsWindowCreate();
void settingsWindowHandleButtons(SDL_Event e);
void settingsWindowRender();
void settingsWindowDestroy();

// load window
// save window
// game window

#endif