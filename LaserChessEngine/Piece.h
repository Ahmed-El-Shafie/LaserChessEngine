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
	char pieceChar;
	static Common::Vector getDeflectedDirection(int orientation, Common::Vector incomingLaserDir);
public:
	Piece(Common::PieceColor color, int orientation) : numOrientations(0), pieceChar(), color(color), orientation(orientation) {}
	virtual ~Piece() = default;
	virtual std::pair<SideInteraction, Common::Vector> getLaserInteraction(Common::Vector incomingLaserDir) const = 0;
	virtual bool canBeSwitched() const = 0;
	virtual int getDistanceScore(Common::Vector piecePos, Common::Vector ownKingPos, Common::Vector opposingKingPos) const = 0;
	int getNumOrientations() const { return this->numOrientations; }
	int getOrientation() const { return this->orientation; }
	Common::PieceColor getColor() const { return this->color; }
	char getPieceChar() const { return this->pieceChar; }
	void rotateLeft();
	void rotateRight();
	std::string getRepr() const;
};

