#include <stdexcept>

#include "Piece.h"

// To be used with pieces with mirrors only
Common::Vector Piece::getDeflectedDirection(int orientation, Common::Vector incomingLaserDir) {
	orientation = orientation % 2;
	switch (orientation) {
	case 0: // Mirrors facing NE & SW
		if (incomingLaserDir == Common::upVector) {
			return Common::leftVector;
		}
		else if (incomingLaserDir == Common::rightVector) {
			return Common::downVector;
		}
		else if (incomingLaserDir == Common::downVector) {
			return Common::rightVector;
		}
		else if (incomingLaserDir == Common::leftVector) {
			return Common::upVector;
		}
		else {
			throw std::invalid_argument("Invalid incoming laser direction");
		}
	case 1: // Mirrors facing NW & SE
		if (incomingLaserDir == Common::upVector) {
			return Common::rightVector;
		}
		else if (incomingLaserDir == Common::rightVector) {
			return Common::upVector;
		}
		else if (incomingLaserDir == Common::downVector) {
			return Common::leftVector;
		}
		else if (incomingLaserDir == Common::leftVector) {
			return Common::downVector;
		}
		else {
			throw std::invalid_argument("Invalid incoming laser direction");
		}
	default:
		return { -1, -1 };
	}
}

void Piece::rotateLeft() { this->orientation = Common::mod(this->orientation - 1, this->numOrientations); }
void Piece::rotateRight() { this->orientation = Common::mod(this->orientation + 1, this->numOrientations); }