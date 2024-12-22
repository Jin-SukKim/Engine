#pragma once

#ifdef _DEBUG
#pragma comment(lib, "Renderer\\Debug\\Renderer.lib")
#else
#pragma comment(lib, "Renderer\\Release\\Renderer.lib")
#endif

#include "MathHeaders.h"
#include "Renderer/Vertex.h"