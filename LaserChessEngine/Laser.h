#pragma once

#include "Piece.h"

class Laser : public Piece
{
public:
	Laser(Common::PieceColor color, int orientation) : Piece(color, orientation) { numOrientations = 2; };
	std::pair<SideInteraction, Common::Vector> getLaserInteraction(Common::Vector incomingLaserDir) override;
	std::string getRepr() override;
	Common::Vector getLaserDir();
};