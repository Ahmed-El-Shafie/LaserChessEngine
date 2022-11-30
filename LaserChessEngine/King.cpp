#include "King.h"

std::pair<SideInteraction, Common::Vector> King::getLaserInteraction(Common::Vector incomingLaserDir) const {
	return std::make_pair(SideInteraction::HIT, Common::zeroVector);
}

bool King::canBeSwitched() const {
	return false;
}

int King::getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const {
	return 0;
}