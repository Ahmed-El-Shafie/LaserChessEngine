#include "GameState.h"
#include "MoveUtils.h"

std::shared_ptr<Piece> GameState::removeHitPiece(Square* hitSquare) {
	std::shared_ptr<Piece> hitPiece = hitSquare->piece;
	std::unordered_set<Square, Square::HashFunction>& pieceSquares = hitPiece->getColor() == Common::PieceColor::BLUE ? this->bluePieceSquares : this->redPieceSquares;
	pieceSquares.erase((*hitSquare));
	if (tolower(hitPiece->getPieceChar()) == 'k') {
		this->winner = hitPiece->getColor() == Common::PieceColor::BLUE ? Common::PieceColor::RED : Common::PieceColor::BLUE;
	}
	hitSquare->piece = nullptr;
	return hitPiece;
}

void GameState::returnHitPiece(Square* pieceSquare, std::shared_ptr<Piece> hitPiece) {
	pieceSquare->piece = hitPiece;
	std::unordered_set<Square, Square::HashFunction>& pieceSquares = hitPiece->getColor() == Common::PieceColor::BLUE ? this->bluePieceSquares : this->redPieceSquares;
	pieceSquares.insert(*(pieceSquare));
	this->winner = Common::PieceColor::NONE;
}