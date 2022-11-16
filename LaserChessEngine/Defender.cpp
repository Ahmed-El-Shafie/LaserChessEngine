#include "Defender.h"

std::pair<SideInteraction, Common::Vector> Defender::getLaserInteraction(Common::Vector incomingLaserDir) {
	if (Common::directionToOrientation.at(incomingLaserDir) == this->orientation) {
		return std::make_pair(SideInteraction::BLOCK, Common::zeroVector);
	}
	else {
		return std::make_pair(SideInteraction::HIT, Common::zeroVector);
	}
}

std::string Defender::getRepr() {
	std::string pieceStr = this->color == Common::PieceColor::BLUE ? "D" : "d";
	return pieceStr + std::string(this->orientation, '+');
}