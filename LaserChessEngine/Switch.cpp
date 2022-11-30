#include "Switch.h"

std::pair<SideInteraction, Common::Vector> Switch::getLaserInteraction(Common::Vector incomingLaserDir) const {
	SideInteraction interaction = SideInteraction::DEFLECT;
	Common::Vector newLaserDir = getDeflectedDirection(this->orientation, incomingLaserDir);
	return std::make_pair(interaction, newLaserDir);
}

bool Switch::canBeSwitched() const {
	return false;
}

int Switch::getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const {
	return 1500 / (piecePos - opposingKingPos).getManhattanDistance();
}