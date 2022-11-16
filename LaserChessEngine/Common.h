#pragma once

#include <unordered_map>

namespace Common {
	enum class PieceColor { NONE, BLUE, RED };
	enum class RotationDirection { LEFT, RIGHT };

	struct Vector {
		int x;
		int y;
		Vector operator+(const Vector& v) const;
		bool operator==(const Vector& v) const;
		struct HashFunction {
			size_t operator()(const Vector& v) const;
		};
	};

	extern const Vector zeroVector;
	extern const Vector upVector;
	extern const Vector rightVector;
	extern const Vector downVector;
	extern const Vector leftVector;

	extern const std::unordered_map<Vector, int, Vector::HashFunction> directionToOrientation;

	extern int mod(int a, int b);
}