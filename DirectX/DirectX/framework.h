#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX
#include <windows.h>

#include <vector>
#include <string>
#include <map>

#include <filesystem>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxtk/Simplemath.h>
#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;