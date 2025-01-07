#pragma once

#ifdef _DEBUG
#pragma comment(lib, "Math\\Debug\\Math.lib")
#pragma comment(lib, "Renderer\\Debug\\Renderer.lib")
#pragma comment(lib, "Engine\\Debug\\Engine.lib")
#else
#pragma comment(lib, "Math\\Release\\Math.lib")
#pragma comment(lib, "Renderer\\Release\\Renderer.lib")
#pragma comment(lib, "Engine\\Release\\Engine.lib")
#endif

#include <Windows.h>
#include <memory>
#include <string>

#include "Math/MathHeaders.h"
#include "Renderer/RendererHeaders.h"
#include "Engine/EngineHeaders.h"

using namespace JE;