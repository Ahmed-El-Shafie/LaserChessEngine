#pragma once

#include <string>
#include <vector>

#include "Common.h"
#include "Moves.h"
#include "GameState.h"

namespace PosUtils {
	std::vector<std::string> validatePosString(Board board, std::string posString);
	Board createBoard();
	// setupPosition doesn't perform validation. Use validatePosString to validate if necessary.
	void setupPosition(GameState* gamePtr, Board& board, std::string posString);
	std::string getPositionStringFromBoard(const Board& board);
	float evaluatePosition(const Board& board, const GameState& gameState);
}