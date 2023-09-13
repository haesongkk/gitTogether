#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

#include <vector>
#include <string>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxtk/Simplemath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;