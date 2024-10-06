#pragma once

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// 컴퓨터는 실수를 정확하게 저장하지 못하므로 아주 작은 값의 차이를 감안한 보정값
#define SMALL_NUMBER		(1.e-8f) 