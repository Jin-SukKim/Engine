#pragma once

namespace JE {

struct Color
{
	Color() : R(0.f), G(0.f), B(0.f), A(0.f) {}
	Color(float R, float G, float B, float A) : R(R), G(G), B(B), A(A) {}
	Color(const Color& color) : R(color.R), G(color.G), B(color.B), A(color.A) {}
	Color(const Color32& color) {
		R = static_cast<float>(color.R) * ratio;
		G = static_cast<float>(color.G) * ratio;
		B = static_cast<float>(color.B) * ratio;
		A = static_cast<float>(color.A) * ratio;
	}

	Color32 ToColor32() const {
		// float -> int�� ����ȯ �� �Ҽ����� �������⿡ 0.999�� 254�� �Ǵ°� �����ϱ� ���� 255���� ũ�� 256���� ���� ���� ����� ���
		return Color32(
			static_cast<uint8>(R * 255.9999f),
			static_cast<uint8>(G * 255.9999f),
			static_cast<uint8>(B * 255.9999f),
			static_cast<uint8>(A * 255.9999f)
		);
	}

	bool Equal(const Color& color) {
		// float�̱⿡ �������� ����
		return (std::abs(R - color.R) < SMALL_NUMBER) &&
				(std::abs(G - color.G) < SMALL_NUMBER) &&
				(std::abs(B - color.B) < SMALL_NUMBER) &&
				(std::abs(A - color.A) < SMALL_NUMBER);
	}
	bool operator==(const Color& color) {
		return Equal(color);
	}
	bool operator!=(const Color& color) {
		return !Equal(color);
	}
	Color operator+(const Color& color) const {
		return Color(R + color.R, G + color.G, B + color.B, A + color.A);
	}
	Color& operator+=(const Color& color) {
		*this = *this + color;
		return *this;
	}
	Color operator-(const Color& color) const {
		return Color(R - color.R, G - color.G, B - color.B, A - color.A);
	}
	Color& operator-=(const Color& color) {
		*this = *this - color;
		return *this;
	}
	Color operator*(const float& scale) const {
		return Color(R * scale, G * scale, B * scale, A * scale);
	}
	Color operator*(const Color& color) const {
		return Color(R * color.R, G * color.G, B * color.B, A * color.A);
	}
	Color& operator*=(const float& scale) {
		*this = *this * scale;
		return *this;
	}
	Color& operator*=(const Color& color) {
		*this = *this * color;
		return *this;
	}
	Color operator/(const float& scale) const {
		return Color(R / scale, G / scale, B / scale, A / scale);
	}
	Color& operator/=(const float& scale) {
		*this = *this / scale;
		return *this;
	}

	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Magenta;
	static const Color Gray;
	static const Color Error;

	// [0, 255] ������ [0.0, 1.0]���� ��ȯ�ϱ� ���� ��
	static constexpr float ratio = { 1.f / 255.f }; // constexpr�� ������� static ���� �ʱ�ȭ ����
	
	float R, G, B, A;
};

}

