#include "GameState.h"
#include "MoveUtils.h"
#include "King.h"

Piece* GameState::removeHitPiece(Square* hitSquare) {
	Piece* hitPiece = hitSquare->piece;
	std::unordered_set<Square, Square::HashFunction>& pieceSquares = hitPiece->getColor() == Common::PieceColor::BLUE ? this->bluePieceSquares : this->redPieceSquares;
	pieceSquares.erase((*hitSquare));
	if (dynamic_cast<King*>(hitPiece)) {
		this->winner = hitPiece->getColor() == Common::PieceColor::BLUE ? Common::PieceColor::RED : Common::PieceColor::BLUE;
	}
	hitSquare->piece = nullptr;
	return hitPiece;
}