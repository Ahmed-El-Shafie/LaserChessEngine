#include <stdexcept>

#include "Laser.h"

std::pair<SideInteraction, Common::Vector> Laser::getLaserInteraction(Common::Vector incomingLaserDir) {
	return std::make_pair(SideInteraction::BLOCK, Common::zeroVector);
}

std::string Laser::getRepr() {
	std::string pieceStr = this->color == Common::PieceColor::BLUE ? "L" : "l";
	return pieceStr + std::string(this->orientation, '+');
}

Common::Vector Laser::getLaserDir() {
	if (this->color == Common::PieceColor::BLUE) {
		return this->orientation == 0 ? Common::leftVector : Common::upVector;
	}
	else {
		return this->orientation == 0 ? Common::rightVector : Common::downVector;
	}
}