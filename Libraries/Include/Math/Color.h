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
		// float -> int로 현변환 시 소수점은 버려지기에 0.999가 254가 되는걸 방지하기 위해 255보다 크고 256보다 작은 값을 사용해 계산
		return Color32(
			static_cast<uint8>(R * 255.9999f),
			static_cast<uint8>(G * 255.9999f),
			static_cast<uint8>(B * 255.9999f),
			static_cast<uint8>(A * 255.9999f)
		);
	}

	bool Equal(const Color& color) {
		// float이기에 오차범위 감안
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

	// [0, 255] 범위를 [0.0, 1.0]으로 변환하기 위한 값
	static constexpr float ratio = { 1.f / 255.f }; // constexpr로 헤더에서 static 변수 초기화 가능
	
	float R, G, B, A;
};

}

