#ifndef WIDGET_H
#define WIDGET_H

#include <SDL.h>
#include <SDL_video.h>

typedef struct widget_t Widget;
struct widget_t {
	void(*draw)(Widget*, SDL_Renderer*);
	void(*handleEvent)(Widget*, SDL_Event*);
	void(*destroy)(Widget*);
	void* data;
};

void destroyWidget(Widget* src);
void widgetHandleEvent(Widget* widget, SDL_Event event);
void widgetRender(Widget* widget, SDL_Renderer* rend);
void widgetDestroy(Widget* widget);

#endif