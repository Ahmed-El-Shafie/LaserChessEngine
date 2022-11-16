#include <string>
#include <memory>

#include "MoveUtils.h"
#include "Square.h"
#include "Laser.h"

bool MoveUtils::getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString) {
	return Moves::ShiftMove::getMoveFromString(move, moveString) ||
		Moves::RotateMove::getMoveFromString(move, moveString) ||
		Moves::SpecialMove::getMoveFromString(move, moveString);
}

Square* MoveUtils::laserHit(Board& board, Common::PieceColor turn) {
	Common::Vector pos = turn == Common::PieceColor::BLUE ? Common::Vector{ numCols - 1, numRows - 1 } : Common::zeroVector;
	Laser* laser = dynamic_cast<Laser*>(board[pos.y][pos.x].piece);
	Common::Vector laserDir = laser->getLaserDir();
	pos = pos + laserDir; // Skipping laser position handling
	while (0 <= pos.x && pos.x < numCols && 0 <= pos.y && pos.y < numRows) {
		Square& currentSquare = board[pos.y][pos.x];
		Piece* contactPiece = currentSquare.piece;
		if (contactPiece) {
			std::pair<SideInteraction, Common::Vector> laserInteraction = contactPiece->getLaserInteraction(laserDir);
			switch (laserInteraction.first) {
			case SideInteraction::DEFLECT:
				laserDir = laserInteraction.second;
				break;
			case SideInteraction::HIT:
				return &currentSquare;
				break;
			case SideInteraction::BLOCK:
				return nullptr;
			default:
				break;
			}
		}
		pos = pos + laserDir;
	}
	return nullptr;
}