#pragma once

#include <vector>
#include <unordered_map>

#include "Common.h"

enum class SideInteraction {
	DEFLECT, BLOCK, HIT
};

class Piece
{
protected:
	int numOrientations;
	Common::PieceColor color;
	int orientation;
	static Common::Vector getDeflectedDirection(int orientation, Common::Vector incomingLaserDir);
public:
	Piece(Common::PieceColor color, int orientation) : numOrientations(0), color(color), orientation(orientation) {}
	virtual ~Piece() = default;
	virtual std::pair<SideInteraction, Common::Vector> getLaserInteraction(Common::Vector incomingLaserDir) = 0;
	int getOrientation() const { return this->orientation; }
	Common::PieceColor getColor() const { return this->color; }
	virtual std::string getRepr() = 0;
	void rotateLeft();
	void rotateRight();
};

