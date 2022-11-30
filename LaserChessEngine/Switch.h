#pragma once

#include "Piece.h"

class Switch : public Piece
{
public:
	Switch(Common::PieceColor color, int orientation) : Piece(color, orientation) {
		this->numOrientations = 2;
		this->pieceChar = this->color == Common::PieceColor::BLUE ? 'S' : 's';
	}
	std::pair<SideInteraction, Common::Vector> getLaserInteraction(Common::Vector incomingLaserDir) const override;
	bool canBeSwitched() const override;
	int getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const override;
};