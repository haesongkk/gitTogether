#pragma once
#include "input.h"
#include <string.h>

void ScreenDrawUpArrow(COORD pos, int color);
void setColor(int, int);
void ScreenDrawKeyInterface();
void UpdateNote();
void GenerateNote();
void UpdateNotePosition_left(int);
void ScreenPrint(int x, int y, const char* str, int length);
void PrintAsciiArt(const char*);
void HitBox(int);


int l_note[] = { 1,1,1,1,1,1,1,1 };
int d_note[] = { 1,1,1,1,1,1,1,1 };
int u_note[] = { 1,1,1,1,1,1,1,1 };
int r_note[] = { 1,1,1,1,1,1,1,1 };


//COORD NoteprePos_l[] = { {8,40},{8,40},{8,40},{8,40},{8,40},{8,40} };
COORD NotecurPos_l[] = { {8,45},{8,45},{8,45},{8,45},{8,45},{8,45},{8,45},{8,45} };
COORD NotecurPos_d[] = { {24,45},{24,45},{24,45},{24,45},{24,45},{24,45},{24,45},{24,45} };
COORD NotecurPos_u[] = { {40,45},{40,45},{40,45},{40,45},{40,45},{40,45},{40,45},{40,45} };
COORD NotecurPos_r[] = { {56,45},{56,45},{56,45},{56,45},{56,45},{56,45},{56,45},{56,45} };


const char* asciiArtFilePath2 = "D:\\SVN\\GA2023\\DataStructureAndAlgorithm Assignment\\Console Mini Project\\FEEL_MY_RHYTHM\\4_Resources\\ascii_art\\fanclub\\ascii_art_";
const char* asciiArtFilePath1 = "D:\\SVN\\GA2023\\DataStructureAndAlgorithm Assignment\\Console Mini Project\\FEEL_MY_RHYTHM\\4_Resources\\fanclub_ascii\\ascii_art_ ";


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