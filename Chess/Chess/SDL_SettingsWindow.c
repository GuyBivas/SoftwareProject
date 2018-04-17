#include "SDL_Shared.h"

#define BUTTON_WIDTH 110
#define BUTTON_HEIGHT 31
#define LABEL_HEIGHT 28
#define START_WIDTH 210
#define START_HEIGHT 74
#define BACK_WIDTH 151
#define BACK_HEIGHT 52

#define BUTTON_MARGIN_X 15
#define BUTTON_MARGIN_Y 15
#define LABEL_MARGIN_Y 90
#define MARGIN_LEFT 40
#define MARGIN_UP 200

Widget* frame1;
Widget* frame2;
Widget* frame3;


void moveFrame(Widget* frame, int param)
{
	int xStepButtons = BUTTON_WIDTH + BUTTON_MARGIN_X;
	int firstFrameX = MARGIN_LEFT - 4;

	SDL_Rect rect = ((Button*)frame->data)->location;
	((Button*)frame->data)->location = newRect(firstFrameX + (param - 1) * xStepButtons, rect.y, rect.w, rect.h);
}

void clickSettingMode(int param)
{
	manager->mode = (PLAYER_MODE)(param - 1);
	moveFrame(frame1, param);
}

void clickSettingDifficulty(int param)
{
	manager->difficulty = (GAME_DIFFICULTY)(param - 1);
	moveFrame(frame2, param);
}

void clickSettingColor(int param)
{
	manager->computerColor = (PLAYER_COLOR)(param - 1);
	moveFrame(frame3, param);
}


void clickButtonBack(void)
{
	changeWindow(WINDOW_MAIN);
}

void clickButton1(void)
{
	changeWindow(WINDOW_MAIN);
}

void settingsWindowCreate()
{
	SDL_Rect rect;
	int xStepButtons = BUTTON_WIDTH + BUTTON_MARGIN_X;
	int yStep = LABEL_HEIGHT + LABEL_MARGIN_Y;
	int firstButtonY = MARGIN_UP + LABEL_HEIGHT + BUTTON_MARGIN_Y;
	int firstFrameX = MARGIN_LEFT - 4;
	int firstFrameY = firstButtonY - 4;

	createLogo();

	// frames
	rect = newRect(0, firstFrameY, 118, 39);
	frame1 = createButton(rend, "assets/settings/setting_frame.bmp", rect, NULL);

	rect = newRect(0, firstFrameY + yStep, 118, 39);
	frame2 = createButton(rend, "assets/settings/setting_frame.bmp", rect, NULL);

	rect = newRect(0, firstFrameY + 2 * yStep, 118, 39);
	frame3 = createButton(rend, "assets/settings/setting_frame.bmp", rect, NULL);

	moveFrame(frame1, manager->mode + 1);
	moveFrame(frame2, manager->difficulty + 1);
	moveFrame(frame3, manager->computerColor + 1);
	widgetListAddLast(widgets, frame1);
	widgetListAddLast(widgets, frame2);
	widgetListAddLast(widgets, frame3);

	// buttons
	rect = newRect(15, 15, BACK_WIDTH, BACK_HEIGHT);
	widgetListAddLast(widgets, createButton(rend, "assets/settings/button_back.bmp", rect, clickButtonBack));

	rect = newRect(WINDOW_WIDTH - START_WIDTH - 15, WINDOW_HEIGHT - START_HEIGHT - 15, START_WIDTH, START_HEIGHT);
	widgetListAddLast(widgets, createButton(rend, "assets/settings/button_start.bmp", rect, clickButton1));

	// labels
	rect = newRect(MARGIN_LEFT, MARGIN_UP, 200, 27);
	widgetListAddLast(widgets, createButton(rend, "assets/settings/label_gameMode.bmp", rect, NULL));

	rect = newRect(MARGIN_LEFT, MARGIN_UP + yStep, 179, 28);
	widgetListAddLast(widgets, createButton(rend, "assets/settings/label_difficulty.bmp", rect, NULL));

	rect = newRect(MARGIN_LEFT, MARGIN_UP + 2 * yStep, 204, 28);
	widgetListAddLast(widgets, createButton(rend, "assets/settings/label_userColor.bmp", rect, NULL));

	// settings
	rect = newRect(MARGIN_LEFT, firstButtonY, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_mode1.bmp", rect, clickSettingMode, 1));

	rect = newRect(MARGIN_LEFT + xStepButtons, firstButtonY, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_mode2.bmp", rect, clickSettingMode, 2));

	rect = newRect(MARGIN_LEFT, firstButtonY + yStep, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_difficulty1.bmp", rect, clickSettingDifficulty, 1));

	rect = newRect(MARGIN_LEFT + xStepButtons, firstButtonY + yStep, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_difficulty2.bmp", rect, clickSettingDifficulty, 2));

	rect = newRect(MARGIN_LEFT + 2 * xStepButtons, firstButtonY + yStep, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_difficulty3.bmp", rect, clickSettingDifficulty, 3));

	rect = newRect(MARGIN_LEFT + 3 * xStepButtons, firstButtonY + yStep, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_difficulty4.bmp", rect, clickSettingDifficulty, 4));

	rect = newRect(MARGIN_LEFT + 4 * xStepButtons, firstButtonY + yStep, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_difficulty5.bmp", rect, clickSettingDifficulty, 5));

	rect = newRect(MARGIN_LEFT, firstButtonY + 2 * yStep, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_color1.bmp", rect, clickSettingColor, 1));

	rect = newRect(MARGIN_LEFT + xStepButtons, firstButtonY + 2 * yStep, BUTTON_WIDTH, BUTTON_HEIGHT);
	widgetListAddLast(widgets, createButtonWithParam(rend, "assets/settings/setting_color2.bmp", rect, clickSettingColor, 2));

	/*if (logo == NULL || buttonBack == NULL || buttonStart == NULL || labelMode == NULL || labelDifficulty == NULL || labelColor == NULL ||
		settingDifficulty1 == NULL || settingDifficulty2 == NULL || settingDifficulty3 == NULL || settingDifficulty4 == NULL || settingDifficulty5 == NULL ||
		settingMode1 == NULL || settingMode2 == NULL || settingColor1 == NULL || settingColor2 == NULL)
	{
		freeResources(true);
		return;
	}*/
}