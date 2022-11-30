#include "Defender.h"

std::pair<SideInteraction, Common::Vector> Defender::getLaserInteraction(Common::Vector incomingLaserDir) const {
	if (Common::directionToOrientation.at(incomingLaserDir) == this->orientation) {
		return std::make_pair(SideInteraction::BLOCK, Common::zeroVector);
	}
	else {
		return std::make_pair(SideInteraction::HIT, Common::zeroVector);
	}
}

bool Defender::canBeSwitched() const {
	return true;
}

int Defender::getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const {
	return  100 / (piecePos - ownKingPos).getManhattanDistance() - 100 / (piecePos - opposingKingPos).getManhattanDistance();
}