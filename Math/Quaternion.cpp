#include "pch.h"
#include "Quaternion.h"

namespace JE {
	const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };

	Quaternion::Quaternion(const Vector3& axis, float angleDegree)
	{
		float sin = 0.f, cos = 0.f;
		MathUtils::GetSinCos(sin, cos, angleDegree * 0.5f);
		// W = cos(각도 / 2)
		W = cos;
		// sin(각도 / 2)와 회전축 n을 곱해 계산
		X = sin * axis.X;
		Y = sin * axis.Y;
		Z = sin * axis.Z;
	}

	Quaternion::Quaternion(const Rotator& rotator)
	{
		FromRotator(rotator);
	}

	Quaternion::Quaternion(const Matrix3x3& mat)
	{
		FromMatrix(mat);
	}

	Quaternion Quaternion::operator-() const
	{
		return Quaternion(-X, -Y, -Z, -W);
	}
	Quaternion Quaternion::operator+(const Quaternion& q) const
	{
		return Quaternion(X + q.X, Y + q.Y, Z + q.Z, W + q.W);
	}
	Quaternion Quaternion::operator-(const Quaternion& q) const
	{
		return Quaternion(X - q.X, Y - q.Y, Z - q.Z, W - q.W);
	}
	Quaternion Quaternion::operator+=(const Quaternion& q) 
	{
		X += q.X;
		Y += q.Y;
		Z += q.Z;
		W += q.W;
		return *this;
	}
	Quaternion Quaternion::operator-=(const Quaternion& q) 
	{
		X -= q.X;
		Y -= q.Y;
		Z -= q.Z;
		W -= q.W;
		return *this;
	}
	Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		// 모든 요소를 교차해 곱하는 방식 (사원수는 총 4가지 요소로 구성되므로 곱의 결과는 총 16가지)
		Quaternion result;
		
		// q = a + bi + cj + dk (사원수 곱셈)
		/*
			result.a = a * q.a - b * q.b - c * q.c - d * q.d;
			result.b = a * q.b + q.a * b + c * q.d - d * q.c;
			result.c = a * q.c + q.a * c + q.b * d - b * q.d;
			result.d = a * q.d + q.a * d + b * q.c - c * q.b;

			1개의 실수와 3차원 벡터로 사원수 관리할 때의 곱셈은 벡터의 내적과 외적을 사용해 정리 가능

			사원수 q = w + xi + yj + zk = (w, v)
			사원수의 3차원 벡터 v = xi + yj + zk = (x, y, z)
			q1 * q2 = (w1*w2 - (v1.Dot(v2)), w1*v2 + w2*v1 + v1.Cross(v2))
		*/	

		// 두 사원수의 허수부를 벡터로 묶기
		Vector3 v1 = this->ImaginaryPart(), v2 = q.ImaginaryPart();

		// 사원수 곱셈의 실수부 계산, w = w1*w2 - (v1.Dot(v2))
		result.W = W * q.W - v1.Dot(v2);

		// 허수부 계산 (외적은 교환법칙이 성립하지 않기에 사원수의 곱셈은 교환법칙이 성립하지 않음)
		// v = w1*v2 + w2*v1 + v1.Cross(v2)
		Vector3 v = v2 * W + v1 * q.W + v1.Cross(v2);
		result.X = v.X;
		result.Y = v.Y;
		result.Z = v.Z;

		// 순허수 사원수 q1, q2의 곱셈은 더 간단하게 정리 가능
		// q1 * q2 = ((-v1).Dot(v2), v1.Cross(v2))

		return result;
	}
	Quaternion Quaternion::operator*=(const Quaternion& q) 
	{
		// 두 사원수의 허수부를 벡터로 묶기
		Vector3 v1 = this->ImaginaryPart(), v2 = q.ImaginaryPart();

		// 사원수 곱셈의 실수부 계산, w = w1*w2 - (v1.Dot(v2))
		W = W * q.W - v1.Dot(v2);

		// 허수부 계산, v = w1*v2 + w2*v1 + v1.Cross(v2)
		Vector3 v = v2 * W + v1 * q.W + v1.Cross(v2);
		X = v.X;
		Y = v.Y;
		Z = v.Z;

		return *this;
	}
	float Quaternion::NormSize() const
	{
		const float squareSum = X * X + Y * Y + Z * Z + W * W;

		// 0보다 크다면
		if (squareSum >= SMALL_NUMBER)
			return std::sqrtf(squareSum);
		return 0.0f;
	}
	void Quaternion::Normalize()
	{
		const float sqrt = NormSize();

		// 0보다 크면
		if (sqrt) {
			const float scale = 1.f / sqrt;

			X *= scale;
			Y *= scale;
			Z *= scale;
			W *= scale;
		}
		else
			*this = Quaternion::Identity;
	}
	bool Quaternion::IsUnitQuaternion() const
	{
		// 크기가 1.f에 아주 가깝거나 1.f이라면
		return MathUtils::EqualsInTolerance(NormSize(), 1.f);
	}
	Quaternion Quaternion::Conjugate() const
	{
		// q* = w - (xi + yj + zk), 즉 허수부의 부호가 반전
		return Quaternion(-X, -Y, -Z, W);
	}
	Quaternion Quaternion::Inverse() const
	{
		/*
			임의의 사원수에 그 켤레를 곱하면 크기의 제곱 값이 나온다.
			q*q = qq* = a^2 + b^2 + c^2 + d^2 = |q|^2
			float sizeSquare = x * x + y * y + z * z + w * w;
    
			따라서 단위 사원수와 그 켤레 사원수를 곱한 값은 언제나 1이 된다. (단위 사원수 크기 = 1)
			이 성질을 사용해 역원을 유도
			q^-1 = 1 / q = q* / qq* = q* / |q|^2
			Quaternion inverse = Conjugate() / sizeSquare;
    
			만약 사원수가 단위 사원수라면 결국 분모인 크기의 제곱 값은 1이 되고 켤레 사원수가 역원이 된다.
		*/
		return Conjugate();
	}
	Vector3 Quaternion::RotateVector(const Vector3& v) const
	{
		/*
			3차원 공간에서 벡터를 회전시키는 용도로 사원수를 사용하기 위해서는 사원수 곱의 결과가 항상 순허수 사원수가 되는 수식을 사용
			순허수 사원수를 사용하면 4차원 공간에서 회전축 n인 허수부 벡터를 중심으로 회전 변환하는 성직을 이용

			벡터를 회전시키는 수식의 r 벡터는 sin(각도 / 2)와 회전축 n을 곱해 얻기
			스칼라 w 값은 cos(각도 / 2)
		*/

		// 실수부는 항상 0이 되기에 계산하지 않고 허수부인 벡터만 계산 (사원수의 회전 사원수의 벡터부를 벡터로 저장)
		Vector3 q = this->ImaginaryPart();
		// 계산량을 줄이기 위해 회전축과 벡터를 외적한 결과를 2배한 후 저장
		Vector3 t = q.Cross(v) * 2.f;
		// 수식을 이용해 회전된 벡터의 값
		Vector3 result = v + (t * W) + q.Cross(t);

		return result;
	}
	Vector3 Quaternion::operator*(const Vector3& v) const
	{
		return RotateVector(v);
	}
	void Quaternion::FromRotator(const Rotator& rotator)
	{
		/*
			오일러 각의 각 회전은 x, y, z 기저 축이 회전축 역할을 수행 -> 이를 사원수로 표현
			기저축을 중심으로 각도만큼 회전시키는데 사용하는 회전 사원수 q :
				q = (cos(각도/2), sin(각도/2) * 기저축) = cos(각도/2) + sin(각도/2) * 기저축

			이를 오일러 각을 구성하는 roll, pitch, yaw에 대응하는 각 사원수를 표기하고
			세 사원수를 Rotator의 회전 순서에 맞게 곱해주면 오일러 각에 대응하는 사원수가 만들어진다.
		*/

		// s = sin, c = cost
		// p = pitch, y = yaw, r = roll
		float sp = 0.f, sy = 0.f, sr = 0.f; 
		float cp = 0.f, cy = 0.f, cr = 0.f;

		// 사원수는 절반의 각도를 사용
		MathUtils::GetSinCos(sy, cy, rotator.Yaw * 0.5f);
		MathUtils::GetSinCos(sr, cr, rotator.Roll * 0.5f);
		MathUtils::GetSinCos(sp, cp, rotator.Pitch * 0.5f);
		
		W = sy * sp * sr + cy * cp * cr;
		X = sy * sr * cp + sp * cy * cr;
		Y = sy * cp * cr - sp * sr * cy;
		Z = sr * cp * cy - cr * sp * sy;
	}
	Rotator Quaternion::ToRotator() const
	{
		Rotator result;
		// Roll 회전 각을 구하기 위한 분자의 식 2(wz + xy) 계산 (사원수는 절반의 각도를 사용하므로 2 곱하기)
		float sr = 2 * (W * Z + X * Y);
		// Roll 회전 각을 구하기 위한 분모의 식 (1 - 2(z^2 + x^2) 계산
		float cr = 1 - 2 * (Z * Z + X * X);

		// arctan 함수를 사용해 Roll 회전각을 구하기 (Radian 값을 반환하므로 Degree로 변환)
		result.Roll = MathUtils::Rad2Deg(std::atan2f(sr, cr));

		// Pitch 회전 각을 구하기 위한 식 (wx - yz)를 계산 (사원수는 절반의 각도를 사용하므로 2 곱하기)
		float pitchTest = W * X - Y * Z; 
		// 오차 범위 내에서 arcsin 함수를 구하기 위한 범위 설정 (arcsin 범위 [-1, 1])
		float asinThreshold = 0.499995f;
		// Pitch 회전 각을 구하기 위한 식의 값에 2를 곱해 arcsin 함수에 입력할 최종 값 구하기
		float sp = 2 * pitchTest;

		// arcsin 함수의 입력 범위를 넘어서는 경우가 나온다면 고정 값으로 지정 (범위를 벗어날일은 없지만 안전성을 위해 확인)
		if (pitchTest < -asinThreshold)
			result.Pitch = -90.f;
		else if (pitchTest > asinThreshold)
			result.Pitch = 90.f;
		else
			// arcsin 함수를 사용해 Pitch 회전각 구하기
			result.Pitch = MathUtils::Rad2Deg(std::asinf(sp));

		// Yaw 회전 각을 구하는 분자의 식 2(wy + xz)를 계산
		float sy = 2 * (W * Y + X * Z);
		// Yaw 회전 각을 구하는 분모의 식 1 - 2(x^2 + y^2)를 계산
		float cy = 1.f - 2 * (X * X + Y * Y);

		// arctan 함수를 사용해 Yaw 회전각을 구하기
		result.Yaw = MathUtils::Rad2Deg(std::atan2f(sy, cy));

		// 최종 오일러 각을 반환
		return result;
	}
	Matrix4x4 Quaternion::GetRotationTransform(const Quaternion& q)
	{
		// 사원수로 회전된 세 로컬축 벡터를 사용해 회전 변환행렬 생성
		return Matrix4x4::RotationTransform(GetLocalX(q), GetLocalY(q), GetLocalZ(q));;
	}
	void Quaternion::FromMatrix(const Matrix3x3& mat)
	{
		// 사원수에서 회전변환 행렬을 구하려면 행렬의 요소를 조합해서 사원수를 구성하는 x, y, z, w값을 개별로 계산

		float root = 0.f; // 제곱근을 저장할 변수
		float trace = mat[0][0] + mat[1][1] + mat[2][2]; // 정방행렬의 대각 성분을 모두 더한 값(Trace)

		// 회전하지 않는 회전 변환행렬인 경우
		if (!MathUtils::EqualsInTolerance(mat[0].SizeSquared(), 1.f) ||
			!MathUtils::EqualsInTolerance(mat[1].SizeSquared(), 1.f) ||
			!MathUtils::EqualsInTolerance(mat[1].SizeSquared(), 1.f))
			*this = Quaternion::Identity;

		/*
			예외 사항 : Trace 값 <= -1인 경우 제곱근을 구하는 값이 0보다 작거나 같아져 w를 구하는데 필요한 r(root)의 해가 존재하지 않는다.
			이러한 경우, w가 아닌 다른 요소부터 계산한 후에 이로부터 다른 성분을 구하도록 계산 방법을 우회 - Ken Shoemake 알고리듬 사용

			켄 슈메이크(Ken Shoemake) 알고리듬 : 예외 상황을 감안해 회전변환 행렬로부터 사원수를 구하는 알고리듬
				Trace값 > -1이라면 w값 계산 가능, 하지만 -1에 가까울수록 오차가 발생하므로 0보다 큰 경우에만 로직을 적용
			
			Trace값 > 0 : 기존 방법을 사용
			Trace값 <= 0 : 예외 상황으로 다른 값부터 계산
		*/ 

		// Ken Shoemake 알고리듬
		if (trace > 0.f) {
			// trace 값으로부터 실수부 W 값 계산
			root = std::sqrtf(trace + 1.f);
			W = 0.5f * root;
			// 나머지 x, y, z 값을 구하기 위해 w의 역수를 계산하고 변수 root에 저장
			root = 0.5f / root;

			X = (mat[1][2] - mat[2][1]) * root;
			Y = (mat[2][0] - mat[0][2]) * root;
			Z = (mat[0][1] - mat[1][0]) * root;
		}
		// 예외 상황
		else {
			// X, Y, Z 중에서 가장 큰 요소를 파악
			uint8 i = 0;
			if (mat[1][1] > mat[0][0]) i = 1;
			if (mat[2][2] > mat[i][i]) i = 2;

			// i를 사용해 나머지 순서 j, k를 파악
			static const uint8 next[3] = { 1, 2, 0 };
			uint8 j = next[i];
			uint8 k = next[j];

			// 가장 큰 요소의 값 구하기
			root = std::sqrtf(mat[i][i] - mat[j][j] - mat[k][k] + 1.f);

			// 가장 큰 요소와 나머지를 순서대로 가르킬 수 있도록 포인터 배열로 x, y, z를 가르키기
			float* qt[3] = { &X, &Y, &Z }; // index값을 활용해 바로 x, y, z에 값을 입력
			// 가장 큰 요소의 값을 포인터 배열 i에 저장
			*qt[i] = 0.5f * root;

			// 나머지 두 요소의 값을 구하기 위한 가장 큰 요소의 역수를 계산해 저장
			root = 0.5f / root;

			// 나머지 두 요소의 값 계산
			*qt[j] = (mat[i][j] + mat[j][i]) * root;
			*qt[k] = (mat[i][k] + mat[k][i]) * root;

			// 마지막 w값 계산
			W = (mat[j][k] - mat[k][j]) * root;
		}
	}
	Quaternion Quaternion::Slerp(const Quaternion& quat1, const Quaternion& quat2, float ratio)
	{
		Quaternion q1 = quat1, q2 = quat2; // 입력된 두 사원수 저장

		// 두 사원수의 내적 구하기
		float dot = q1.X * q2.X + q1.Y * q2.Y + q1.Z * q2.Z + q1.W * q2.W;

		// 내적의 값이 0보다 작으면 최단 거리를 사용하도록 방향을 전환 (두 사원수 각도가 90도보다 크면, 즉 원래대로면 180도보다 크면)
		// 일반적으로 최단 거리를 사용, 만약 긴 거리를 사용하고 싶으면 조건이 달라져야 함
		if (dot < 0.f) {
			// 사원수를 반전시켜 최단 각도로 전환 (사원수를 반전시키면 반대방향 회전)
			q1 = -q1;
			dot = -dot; // 반전시킨 결과를 반영해 내적의 값도 반전
		}

		// 선형 보간 식의 두 계수, q3 = a * q1 + b * q2 = (1 - t)*q1 + t * q2
		float alpha = 1.f, beta = 0.f;

		// 두 사원수의 사이각이 작으면 선형 보간으로 수행
		// 내적의 값이 1에 근접한지 검사 (두 벡터가 거의 평행한지 확인, 평행하면 선형보간)
		if (dot > 0.9995f) {
			alpha = 1.f - ratio;
			beta = ratio;
		}
		// 구면 선형 보간 (각도를 이용해 선형 보간하므로 회전 보간을 수행)
		else {
			const float theta = std::acosf(dot); // arcsin으로 내적 값으로부터 각도 계산
			const float invSin = 1.f / std::sinf(theta); // sin 함수의 역수로 분모의 값

			// 선형 보간의 두 계수 계산 (각도를 활용)
			alpha = std::sinf((1.f - ratio) * theta) * invSin;
			beta = std::sinf(ratio * theta) * invSin;
		}

		// 선형 보간식을 적용해 보간된 사원수 계산
		Quaternion result;
		result.X = alpha * q1.X + beta * q2.X;
		result.Y = alpha * q1.Y + beta * q2.Y;
		result.Z = alpha * q1.Z + beta * q2.Z;
		result.W = alpha * q1.W + beta * q2.W;

		return result;
	}
	std::wstring Quaternion::ToString() const
	{
		return ToRotator().ToString();
	}
}