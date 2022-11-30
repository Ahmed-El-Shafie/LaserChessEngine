#include <stdexcept>

#include "Laser.h"

std::pair<SideInteraction, Common::Vector> Laser::getLaserInteraction(Common::Vector incomingLaserDir) const {
	return std::make_pair(SideInteraction::BLOCK, Common::zeroVector);
}

bool Laser::canBeSwitched() const {
	return false;
}

int Laser::getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const {
	return 0;
}

Common::Vector Laser::getLaserDir() {
	if (this->color == Common::PieceColor::BLUE) {
		return this->orientation == 0 ? Common::leftVector : Common::upVector;
	}
	else {
		return this->orientation == 0 ? Common::rightVector : Common::downVector;
	}
}