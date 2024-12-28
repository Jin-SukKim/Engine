#include "pch.h"
#include "Quaternion.h"

namespace JE {
	const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };

	Quaternion::Quaternion(const Vector3& axis, float angleDegree)
	{
		float sin = 0.f, cos = 0.f;
		MathUtils::GetSinCos(sin, cos, angleDegree * 0.5f);
		// W = cos(���� / 2)
		W = cos;
		// sin(���� / 2)�� ȸ���� n�� ���� ���
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
		// ��� ��Ҹ� ������ ���ϴ� ��� (������� �� 4���� ��ҷ� �����ǹǷ� ���� ����� �� 16����)
		Quaternion result;
		
		// q = a + bi + cj + dk (����� ����)
		/*
			result.a = a * q.a - b * q.b - c * q.c - d * q.d;
			result.b = a * q.b + q.a * b + c * q.d - d * q.c;
			result.c = a * q.c + q.a * c + q.b * d - b * q.d;
			result.d = a * q.d + q.a * d + b * q.c - c * q.b;

			1���� �Ǽ��� 3���� ���ͷ� ����� ������ ���� ������ ������ ������ ������ ����� ���� ����

			����� q = w + xi + yj + zk = (w, v)
			������� 3���� ���� v = xi + yj + zk = (x, y, z)
			q1 * q2 = (w1*w2 - (v1.Dot(v2)), w1*v2 + w2*v1 + v1.Cross(v2))
		*/	

		// �� ������� ����θ� ���ͷ� ����
		Vector3 v1 = this->ImaginaryPart(), v2 = q.ImaginaryPart();

		// ����� ������ �Ǽ��� ���, w = w1*w2 - (v1.Dot(v2))
		result.W = W * q.W - v1.Dot(v2);

		// ����� ��� (������ ��ȯ��Ģ�� �������� �ʱ⿡ ������� ������ ��ȯ��Ģ�� �������� ����)
		// v = w1*v2 + w2*v1 + v1.Cross(v2)
		Vector3 v = v2 * W + v1 * q.W + v1.Cross(v2);
		result.X = v.X;
		result.Y = v.Y;
		result.Z = v.Z;

		// ����� ����� q1, q2�� ������ �� �����ϰ� ���� ����
		// q1 * q2 = ((-v1).Dot(v2), v1.Cross(v2))

		return result;
	}
	Quaternion Quaternion::operator*=(const Quaternion& q) 
	{
		// �� ������� ����θ� ���ͷ� ����
		Vector3 v1 = this->ImaginaryPart(), v2 = q.ImaginaryPart();

		// ����� ������ �Ǽ��� ���, w = w1*w2 - (v1.Dot(v2))
		W = W * q.W - v1.Dot(v2);

		// ����� ���, v = w1*v2 + w2*v1 + v1.Cross(v2)
		Vector3 v = v2 * W + v1 * q.W + v1.Cross(v2);
		X = v.X;
		Y = v.Y;
		Z = v.Z;

		return *this;
	}
	float Quaternion::NormSize() const
	{
		const float squareSum = X * X + Y * Y + Z * Z + W * W;

		// 0���� ũ�ٸ�
		if (squareSum >= SMALL_NUMBER)
			return std::sqrtf(squareSum);
		return 0.0f;
	}
	void Quaternion::Normalize()
	{
		const float sqrt = NormSize();

		// 0���� ũ��
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
		// ũ�Ⱑ 1.f�� ���� �����ų� 1.f�̶��
		return MathUtils::EqualsInTolerance(NormSize(), 1.f);
	}
	Quaternion Quaternion::Conjugate() const
	{
		// q* = w - (xi + yj + zk), �� ������� ��ȣ�� ����
		return Quaternion(-X, -Y, -Z, W);
	}
	Quaternion Quaternion::Inverse() const
	{
		/*
			������ ������� �� �ӷ��� ���ϸ� ũ���� ���� ���� ���´�.
			q*q = qq* = a^2 + b^2 + c^2 + d^2 = |q|^2
			float sizeSquare = x * x + y * y + z * z + w * w;
    
			���� ���� ������� �� �ӷ� ������� ���� ���� ������ 1�� �ȴ�. (���� ����� ũ�� = 1)
			�� ������ ����� ������ ����
			q^-1 = 1 / q = q* / qq* = q* / |q|^2
			Quaternion inverse = Conjugate() / sizeSquare;
    
			���� ������� ���� �������� �ᱹ �и��� ũ���� ���� ���� 1�� �ǰ� �ӷ� ������� ������ �ȴ�.
		*/
		return Conjugate();
	}
	Vector3 Quaternion::RotateVector(const Vector3& v) const
	{
		/*
			3���� �������� ���͸� ȸ����Ű�� �뵵�� ������� ����ϱ� ���ؼ��� ����� ���� ����� �׻� ����� ������� �Ǵ� ������ ���
			����� ������� ����ϸ� 4���� �������� ȸ���� n�� ����� ���͸� �߽����� ȸ�� ��ȯ�ϴ� ������ �̿�

			���͸� ȸ����Ű�� ������ r ���ʹ� sin(���� / 2)�� ȸ���� n�� ���� ���
			��Į�� w ���� cos(���� / 2)
		*/

		// �Ǽ��δ� �׻� 0�� �Ǳ⿡ ������� �ʰ� ������� ���͸� ��� (������� ȸ�� ������� ���ͺθ� ���ͷ� ����)
		Vector3 q = this->ImaginaryPart();
		// ��귮�� ���̱� ���� ȸ����� ���͸� ������ ����� 2���� �� ����
		Vector3 t = q.Cross(v) * 2.f;
		// ������ �̿��� ȸ���� ������ ��
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
			���Ϸ� ���� �� ȸ���� x, y, z ���� ���� ȸ���� ������ ���� -> �̸� ������� ǥ��
			�������� �߽����� ������ŭ ȸ����Ű�µ� ����ϴ� ȸ�� ����� q :
				q = (cos(����/2), sin(����/2) * ������) = cos(����/2) + sin(����/2) * ������

			�̸� ���Ϸ� ���� �����ϴ� roll, pitch, yaw�� �����ϴ� �� ������� ǥ���ϰ�
			�� ������� Rotator�� ȸ�� ������ �°� �����ָ� ���Ϸ� ���� �����ϴ� ������� ���������.
		*/

		// s = sin, c = cost
		// p = pitch, y = yaw, r = roll
		float sp = 0.f, sy = 0.f, sr = 0.f; 
		float cp = 0.f, cy = 0.f, cr = 0.f;

		// ������� ������ ������ ���
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
		// Roll ȸ�� ���� ���ϱ� ���� ������ �� 2(wz + xy) ��� (������� ������ ������ ����ϹǷ� 2 ���ϱ�)
		float sr = 2 * (W * Z + X * Y);
		// Roll ȸ�� ���� ���ϱ� ���� �и��� �� (1 - 2(z^2 + x^2) ���
		float cr = 1 - 2 * (Z * Z + X * X);

		// arctan �Լ��� ����� Roll ȸ������ ���ϱ� (Radian ���� ��ȯ�ϹǷ� Degree�� ��ȯ)
		result.Roll = MathUtils::Rad2Deg(std::atan2f(sr, cr));

		// Pitch ȸ�� ���� ���ϱ� ���� �� (wx - yz)�� ��� (������� ������ ������ ����ϹǷ� 2 ���ϱ�)
		float pitchTest = W * X - Y * Z; 
		// ���� ���� ������ arcsin �Լ��� ���ϱ� ���� ���� ���� (arcsin ���� [-1, 1])
		float asinThreshold = 0.499995f;
		// Pitch ȸ�� ���� ���ϱ� ���� ���� ���� 2�� ���� arcsin �Լ��� �Է��� ���� �� ���ϱ�
		float sp = 2 * pitchTest;

		// arcsin �Լ��� �Է� ������ �Ѿ�� ��찡 ���´ٸ� ���� ������ ���� (������ ������� ������ �������� ���� Ȯ��)
		if (pitchTest < -asinThreshold)
			result.Pitch = -90.f;
		else if (pitchTest > asinThreshold)
			result.Pitch = 90.f;
		else
			// arcsin �Լ��� ����� Pitch ȸ���� ���ϱ�
			result.Pitch = MathUtils::Rad2Deg(std::asinf(sp));

		// Yaw ȸ�� ���� ���ϴ� ������ �� 2(wy + xz)�� ���
		float sy = 2 * (W * Y + X * Z);
		// Yaw ȸ�� ���� ���ϴ� �и��� �� 1 - 2(x^2 + y^2)�� ���
		float cy = 1.f - 2 * (X * X + Y * Y);

		// arctan �Լ��� ����� Yaw ȸ������ ���ϱ�
		result.Yaw = MathUtils::Rad2Deg(std::atan2f(sy, cy));

		// ���� ���Ϸ� ���� ��ȯ
		return result;
	}
	Matrix4x4 Quaternion::GetRotationTransform(const Quaternion& q)
	{
		// ������� ȸ���� �� ������ ���͸� ����� ȸ�� ��ȯ��� ����
		return Matrix4x4::RotationTransform(GetLocalX(q), GetLocalY(q), GetLocalZ(q));;
	}
	void Quaternion::FromMatrix(const Matrix3x3& mat)
	{
		// ��������� ȸ����ȯ ����� ���Ϸ��� ����� ��Ҹ� �����ؼ� ������� �����ϴ� x, y, z, w���� ������ ���

		float root = 0.f; // �������� ������ ����
		float trace = mat[0][0] + mat[1][1] + mat[2][2]; // ��������� �밢 ������ ��� ���� ��(Trace)

		// ȸ������ �ʴ� ȸ�� ��ȯ����� ���
		if (!MathUtils::EqualsInTolerance(mat[0].SizeSquared(), 1.f) ||
			!MathUtils::EqualsInTolerance(mat[1].SizeSquared(), 1.f) ||
			!MathUtils::EqualsInTolerance(mat[1].SizeSquared(), 1.f))
			*this = Quaternion::Identity;

		/*
			���� ���� : Trace �� <= -1�� ��� �������� ���ϴ� ���� 0���� �۰ų� ������ w�� ���ϴµ� �ʿ��� r(root)�� �ذ� �������� �ʴ´�.
			�̷��� ���, w�� �ƴ� �ٸ� ��Һ��� ����� �Ŀ� �̷κ��� �ٸ� ������ ���ϵ��� ��� ����� ��ȸ - Ken Shoemake �˰��� ���

			�� ������ũ(Ken Shoemake) �˰��� : ���� ��Ȳ�� ������ ȸ����ȯ ��ķκ��� ������� ���ϴ� �˰���
				Trace�� > -1�̶�� w�� ��� ����, ������ -1�� �������� ������ �߻��ϹǷ� 0���� ū ��쿡�� ������ ����
			
			Trace�� > 0 : ���� ����� ���
			Trace�� <= 0 : ���� ��Ȳ���� �ٸ� ������ ���
		*/ 

		// Ken Shoemake �˰���
		if (trace > 0.f) {
			// trace �����κ��� �Ǽ��� W �� ���
			root = std::sqrtf(trace + 1.f);
			W = 0.5f * root;
			// ������ x, y, z ���� ���ϱ� ���� w�� ������ ����ϰ� ���� root�� ����
			root = 0.5f / root;

			X = (mat[1][2] - mat[2][1]) * root;
			Y = (mat[2][0] - mat[0][2]) * root;
			Z = (mat[0][1] - mat[1][0]) * root;
		}
		// ���� ��Ȳ
		else {
			// X, Y, Z �߿��� ���� ū ��Ҹ� �ľ�
			uint8 i = 0;
			if (mat[1][1] > mat[0][0]) i = 1;
			if (mat[2][2] > mat[i][i]) i = 2;

			// i�� ����� ������ ���� j, k�� �ľ�
			static const uint8 next[3] = { 1, 2, 0 };
			uint8 j = next[i];
			uint8 k = next[j];

			// ���� ū ����� �� ���ϱ�
			root = std::sqrtf(mat[i][i] - mat[j][j] - mat[k][k] + 1.f);

			// ���� ū ��ҿ� �������� ������� ����ų �� �ֵ��� ������ �迭�� x, y, z�� ����Ű��
			float* qt[3] = { &X, &Y, &Z }; // index���� Ȱ���� �ٷ� x, y, z�� ���� �Է�
			// ���� ū ����� ���� ������ �迭 i�� ����
			*qt[i] = 0.5f * root;

			// ������ �� ����� ���� ���ϱ� ���� ���� ū ����� ������ ����� ����
			root = 0.5f / root;

			// ������ �� ����� �� ���
			*qt[j] = (mat[i][j] + mat[j][i]) * root;
			*qt[k] = (mat[i][k] + mat[k][i]) * root;

			// ������ w�� ���
			W = (mat[j][k] - mat[k][j]) * root;
		}
	}
	Quaternion Quaternion::Slerp(const Quaternion& quat1, const Quaternion& quat2, float ratio)
	{
		Quaternion q1 = quat1, q2 = quat2; // �Էµ� �� ����� ����

		// �� ������� ���� ���ϱ�
		float dot = q1.X * q2.X + q1.Y * q2.Y + q1.Z * q2.Z + q1.W * q2.W;

		// ������ ���� 0���� ������ �ִ� �Ÿ��� ����ϵ��� ������ ��ȯ (�� ����� ������ 90������ ũ��, �� ������θ� 180������ ũ��)
		// �Ϲ������� �ִ� �Ÿ��� ���, ���� �� �Ÿ��� ����ϰ� ������ ������ �޶����� ��
		if (dot < 0.f) {
			// ������� �������� �ִ� ������ ��ȯ (������� ������Ű�� �ݴ���� ȸ��)
			q1 = -q1;
			dot = -dot; // ������Ų ����� �ݿ��� ������ ���� ����
		}

		// ���� ���� ���� �� ���, q3 = a * q1 + b * q2 = (1 - t)*q1 + t * q2
		float alpha = 1.f, beta = 0.f;

		// �� ������� ���̰��� ������ ���� �������� ����
		// ������ ���� 1�� �������� �˻� (�� ���Ͱ� ���� �������� Ȯ��, �����ϸ� ��������)
		if (dot > 0.9995f) {
			alpha = 1.f - ratio;
			beta = ratio;
		}
		// ���� ���� ���� (������ �̿��� ���� �����ϹǷ� ȸ�� ������ ����)
		else {
			const float theta = std::acosf(dot); // arcsin���� ���� �����κ��� ���� ���
			const float invSin = 1.f / std::sinf(theta); // sin �Լ��� ������ �и��� ��

			// ���� ������ �� ��� ��� (������ Ȱ��)
			alpha = std::sinf((1.f - ratio) * theta) * invSin;
			beta = std::sinf(ratio * theta) * invSin;
		}

		// ���� �������� ������ ������ ����� ���
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