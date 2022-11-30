#pragma once

#include "Piece.h"

class Deflector : public Piece
{
public:
	Deflector(Common::PieceColor color, int orientation) : Piece(color, orientation) {
		this->numOrientations = 4;
		this->pieceChar = this->color == Common::PieceColor::BLUE ? 'B' : 'b';
	}
	std::pair<SideInteraction, Common::Vector> getLaserInteraction(Common::Vector incomingLaserDir) const override;
	bool canBeSwitched() const override;
	int getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const override;
};