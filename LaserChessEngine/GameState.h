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
	/*
	* Need the pieces' squares to generate and apply moves and evaluate positions
	*/
	std::unordered_set<Square, Square::HashFunction> bluePieceSquares;
	std::unordered_set<Square, Square::HashFunction> redPieceSquares;

	Piece* removeHitPiece(Square* hitSquare);
};