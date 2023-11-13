#pragma once

#include "targetver.h"
#include "resource.h"

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX

#include <windows.h>

#include <memory>
#include <cassert>

#include <vector>
#include <queue>
#include <string>

using namespace std;

template<class T>
using spvector = vector<shared_ptr<T>>;

template<class T>
using wpvector = vector<weak_ptr<T>>;