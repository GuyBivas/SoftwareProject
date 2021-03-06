#ifndef BUTTON_H
#define BUTTON_H

#include "Widget.h"
#include <SDL.h>
#include <SDL_video.h>

typedef struct button_t Button;
struct button_t {
	SDL_Renderer* render;
	SDL_Texture* texture;
	SDL_Rect location;
	void(*action)(void);
	void(*actionParam)(int);
	int param;
};

Widget* createButton(
	SDL_Renderer* renderer,
	const char* image,
	SDL_Rect location,
	void(*action)(void));
Widget* createButtonWithParam(
	SDL_Renderer* renderer,
	const char* image,
	SDL_Rect location,
	void(*action)(int),
	int param);
void destroyButton(Widget* src);
void handleButtonEvent(Widget* src, SDL_Event* event);
void drawButton(Widget*, SDL_Renderer*);

#endif