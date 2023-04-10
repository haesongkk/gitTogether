#pragma once

#include "main.h"

/// <summary>
/// Å° ÄÚµå
/// </summary>
const int ESCAPE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int UP = 3;
const int DOWN = 4;
const int SPACE = 5;

const int MAX_KEY = 6;




void SetKeyTable(const int keyIndex, bool isOn);

bool GetKeyTable(const int keyIndex);

void UpdateInput();