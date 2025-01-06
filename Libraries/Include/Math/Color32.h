#pragma once

namespace JE {

// Color [0, 255]
struct Color32
{
	Color32() : R(0), G(0), B(0), A(255) {}
	Color32(uint8 R, uint8 G, uint8 B, uint8 A = 255) : R(R), G(G), B(B), A(A) {}
	Color32(uint32 color) : Color(color) {}

	bool operator==(const Color32& c) const {
		return Color == c.Color;
	}
	bool operator!=(const Color32& c) const {
		return Color != c.Color;
	}
	// Color의 범위 [0, 255]
	Color32 operator+(const Color32& c) {
		return Color32(
				std::clamp(R + c.R, 0, 255),
				std::clamp(G + c.G, 0, 255),
				std::clamp(B + c.B, 0, 255),
				std::clamp(A + c.A, 0, 255)
			);
	}
	Color32 operator-(const Color32& c) {
		return Color32(
				std::clamp(R - c.R, 0, 255),
				std::clamp(G - c.G, 0, 255),
				std::clamp(B - c.B, 0, 255),
				std::clamp(A - c.A, 0, 255)
			);
	}

	Color32& operator+=(const Color32& c) {
		*this = *this + c;
		return *this;
	}

	Color32& operator-=(const Color32& c) {
		*this = *this - c;
		return *this;
	}

	union {
		// Bitmap은 BGR순서
		struct {
			uint8 B, G, R, A;
		};
		uint32 Color;
	};
};

}
