#include "Switch.h"

std::pair<SideInteraction, Common::Vector> Switch::getLaserInteraction(Common::Vector incomingLaserDir) {
	SideInteraction interaction = SideInteraction::DEFLECT;
	Common::Vector newLaserDir = getDeflectedDirection(this->orientation, incomingLaserDir);
	return std::make_pair(interaction, newLaserDir);
}

std::string Switch::getRepr() {
	std::string pieceStr = this->color == Common::PieceColor::BLUE ? "S" : "s";
	return pieceStr + std::string(this->orientation, '+');
}