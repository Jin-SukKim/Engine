#pragma once

#ifdef _DEBUG
#pragma comment(lib, "Math\\Debug\\Math.lib")
#else
#pragma comment(lib, "Math\\Release\\Math.lib")
#endif

#include <Windows.h>
#include <memory>
#include <string>
#include <memory>
#include <algorithm>

#include "Math/MathHeaders.h"

#include "EngineHeaders.h"
