#include "Deflector.h"

std::pair<SideInteraction, Common::Vector> Deflector::getLaserInteraction(Common::Vector incomingLaserDir) const {
	int incomingOrientation = Common::directionToOrientation.at(incomingLaserDir);
	if (incomingOrientation == this->orientation || Common::mod(incomingOrientation - 1, numOrientations) == this->orientation) {
		return std::make_pair(SideInteraction::DEFLECT, getDeflectedDirection(this->orientation, incomingLaserDir));
	}
	else {
		return std::make_pair(SideInteraction::HIT, Common::zeroVector);
	}
}

bool Deflector::canBeSwitched() const {
	return true;
}

int Deflector::getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const {
	return 750 / (piecePos - opposingKingPos).getManhattanDistance();
}