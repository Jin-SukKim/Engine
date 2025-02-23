#pragma once

#ifdef _DEBUG
#pragma comment(lib, "Math\\Debug\\Math.lib")
#pragma comment(lib, "Renderer\\Debug\\Renderer.lib")
#else
#pragma comment(lib, "Math\\Release\\Math.lib")
#pragma comment(lib, "Renderer\\Release\\Renderer.lib")
#endif

#include "Math/MathHeaders.h"
#include "Renderer/Vertex.h"

#include "EngineHeaders.h"

