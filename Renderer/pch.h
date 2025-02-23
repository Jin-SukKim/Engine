#pragma once

#ifdef _DEBUG
#pragma comment(lib, "Math\\Debug\\Math.lib")
#pragma comment(lib, "Engine\\Debug\\Engine.lib")
#else
#pragma comment(lib, "Math\\Release\\Math.lib")
#pragma comment(lib, "Engine\\Release\\Engine.lib")
#endif

#include "Math/MathHeaders.h"
#include "RendererHeaders.h"
#include "Engine/EngineHeaders.h"