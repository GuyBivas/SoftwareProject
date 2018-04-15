#include "Widget.h"

void destroyWidget(Widget* widget)
{
	if (widget != NULL)
		widget->destroy(widget);
}

void widgetHandleEvent(Widget* widget, SDL_Event event)
{
	if (widget != NULL)
		widget->handleEvent(widget, &event);
}

void widgetRender(Widget* widget, SDL_Renderer* rend)
{
	if (widget != NULL)
		widget->draw(widget, rend);
}

void widgetDestroy(Widget* widget)
{
	if (widget != NULL)
		widget->destroy(widget);
}