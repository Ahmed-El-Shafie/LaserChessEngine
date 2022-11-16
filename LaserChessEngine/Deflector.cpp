#include "Deflector.h"

std::pair<SideInteraction, Common::Vector> Deflector::getLaserInteraction(Common::Vector incomingLaserDir) {
	int incomingOrientation = Common::directionToOrientation.at(incomingLaserDir);
	if (incomingOrientation == this->orientation || Common::mod(incomingOrientation - 1, numOrientations) == this->orientation) {
		return std::make_pair(SideInteraction::DEFLECT, getDeflectedDirection(this->orientation, incomingLaserDir));
	}
	else {
		return std::make_pair(SideInteraction::HIT, Common::zeroVector);
	}
}

std::string Deflector::getRepr() {
	std::string pieceStr = this->color == Common::PieceColor::BLUE ? "B" : "b";
	return pieceStr + std::string(this->orientation, '+');
}