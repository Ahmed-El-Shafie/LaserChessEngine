#include "Common.h"

Common::Vector Common::Vector::operator+(const Vector& v) const {
	return { x + v.x, y + v.y };
}

Common::Vector Common::Vector::operator-(const Vector& v) const {
	return { x - v.x, y - v.y };
}

bool Common::Vector::operator==(const Vector& v) const {
	return x == v.x && y == v.y;
}

size_t Common::Vector::HashFunction::operator()(const Vector& v) const {
	size_t xHash = std::hash<int>{}(v.x);
	size_t yHash = std::hash<int>{}(v.y);
	return xHash * 37 + yHash;
}

int Common::Vector::getManhattanDistance() const {
	return abs(x) + abs(y);
}

const Common::Vector Common::zeroVector = { 0, 0 };
const Common::Vector Common::upVector = { 0, -1 };
const Common::Vector Common::rightVector = { 1, 0 };
const Common::Vector Common::downVector = { 0, 1 };
const Common::Vector Common::leftVector = { -1, 0 };

const std::unordered_map<Common::Vector, int, Common::Vector::HashFunction> Common::directionToOrientation = {
		{Common::downVector, 0}, { Common::leftVector, 1}, { Common::upVector, 2 }, { Common::rightVector, 3}
};

int Common::mod(int a, int b) { return (b + (a % b)) % b; }