#pragma once
#include "input.h"
#include <string.h>

void ScreenDrawUpArrow(COORD pos, int color);
void setColor(int, int);
void ScreenDrawKeyInterface();
void UpdateNote();
void GenerateNote();
void UpdateNotePosition(int);
void ScreenPrint(int x, int y, const char* str, int length);

int l_note[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
COORD NotecurPos_l[] = { {8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40},{8,40} };
//COORD NoteprePos_l[] = { {8,40},{8,40},{8,40},{8,40},{8,40},{8,40} };

/// <summary>
/// »ö±ò ÆÈ·¹Æ®
/// </summary>
/// 
enum Color
{
	color_black,
	color_dark_blue,
	color_dark_green,
	color_dark_aqua,
	color_dark_red,
	color_dark_purple,
	color_dark_yellow,
	color_dark_white,
	color_gray,
	color_blue,
	color_green,
	color_aqua,
	color_red,
	color_purple,
	color_yellow,
	color_white
};


//#include <stdio.h>
//#include <windows.h>
//#include <stdbool.h>