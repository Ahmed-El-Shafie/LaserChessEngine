#pragma once

#include "Piece.h"

class King : public Piece
{
public:
	King(Common::PieceColor color, int orientation) : Piece(color, orientation) { numOrientations = 1; };
	std::pair<SideInteraction, Common::Vector> getLaserInteraction(Common::Vector incomingLaserDir) override;
	std::string getRepr() override;
};

