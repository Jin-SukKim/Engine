#pragma once

#ifdef _DEBUG
#pragma comment(lib, "Math\\Debug\\Math.lib")
#else
#pragma comment(lib, "Math\\Release\\Math.lib")
#endif

#include <Windows.h>
#include <vector>
#include <string>

#include "Math/MathHeaders.h"