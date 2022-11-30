#pragma once

#include "Piece.h"

class King : public Piece
{
public:
	King(Common::PieceColor color, int orientation) : Piece(color, orientation) {
		this->numOrientations = 1;
		this->pieceChar = this->color == Common::PieceColor::BLUE ? 'K' : 'k';
	};
	std::pair<SideInteraction, Common::Vector> getLaserInteraction(Common::Vector incomingLaserDir) const override;
	bool canBeSwitched() const override;
	int getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const override;
};