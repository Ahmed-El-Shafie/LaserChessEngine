#pragma once

#include <unordered_set>
#include <memory>

#include "Common.h"
#include "Square.h"
#include "Moves.h"

struct GameState {
	GameState(Common::PieceColor winner, Common::PieceColor turn) : winner(winner), turn(turn) {};
	GameState() : winner(Common::PieceColor::NONE), turn(Common::PieceColor::NONE) {};

	Common::PieceColor winner; // TODO: Implement draw
	Common::PieceColor turn;
	std::unordered_set<Square, Square::HashFunction> bluePieceSquares;
	std::unordered_set<Square, Square::HashFunction> redPieceSquares;

	std::shared_ptr<Piece> removeHitPiece(Square* hitSquare);
	void returnHitPiece(Square* pieceSquare, std::shared_ptr<Piece> hitPiece);
};