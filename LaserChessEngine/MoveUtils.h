#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Common.h"
#include "Moves.h"
#include "GameState.h"

namespace MoveUtils {
	bool getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString);
	Square* laserHit(Board& board, Common::PieceColor turn);
	std::vector<std::shared_ptr<Moves::Move>> getLegalMoves(const Board& board, const Square& square);

	struct EvaluationResult {
		std::shared_ptr<Moves::Move> move;
		float evaluation;
		size_t movesRemaining;
	};
	// 1 depth = 2 moves, so that engine can evaluate opponent's response to each move
	EvaluationResult findBestMove(Board& board, GameState& gameState, size_t depth);
}