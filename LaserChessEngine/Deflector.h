#pragma once

#include "Piece.h"

class Deflector : public Piece
{
public:
	Deflector(Common::PieceColor color, int orientation) : Piece(color, orientation) { numOrientations = 4; }
	std::pair<SideInteraction, Common::Vector> getLaserInteraction(Common::Vector incomingLaserDir) override;
	std::string getRepr() override;
};

