#pragma once

#define NOMINMAX // Windows.h의 min, max define 매크로 사용 안함

#include <cmath>
#include <algorithm>
#include <array>
#include <string>
#include <cassert>
#include <format>
#include <vector>

#include "Types.h"

#include "MathUtils.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

#include "ScreenPoint.h"
#include "Color32.h"
#include "Color.h"

#include "Rectangle.h"
#include "Circle.h"
#include "Box.h"
#include "Sphere.h"
#include "Plane.h"
#include "Frustum.h"

#include "Rotator.h"
#include "Quaternion.h"