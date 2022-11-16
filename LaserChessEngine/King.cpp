#include "King.h"

std::pair<SideInteraction, Common::Vector> King::getLaserInteraction(Common::Vector incomingLaserDir) {
	return std::make_pair(SideInteraction::HIT, Common::zeroVector);
}

std::string King::getRepr() {
	std::string pieceStr = this->color == Common::PieceColor::BLUE ? "K" : "k";
	return pieceStr + std::string(this->orientation, '+');
}