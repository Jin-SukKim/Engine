#include "pch.h"
#include "MathUtils.h"

const void JE::MathUtils::GetSinCosRad(float& outSin, float& outCos, float radian)
{
	outSin = std::sinf(radian);
	outCos = std::cosf(radian);
}

const void JE::MathUtils::GetSinCos(float& outSin, float& outCos, float degree)
{
	if (degree == 0.f)
	{
		outSin = 0.f;
		outCos = 1.f;
	}
	else if (degree == 90.f)
	{
		outSin = 1.f;
		outCos = 0.f;
	}
	else if (degree == 180.f)
	{
		outSin = 0.f;
		outCos = -1.f;
	}
	else if (degree == 270.f)
	{
		outSin = -1.f;
		outCos = 0.f;
	}
	else {
		GetSinCosRad(outSin, outCos, Deg2Rad(degree));
	}
}
