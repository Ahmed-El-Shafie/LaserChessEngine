#pragma once

#include <string>

#include "Common.h"
#include "GameState.h"

namespace PosUtils {
	std::vector<std::string> validatePosString(Board board, std::string posString);
	Board createBoard();
	// setupPosition doesn't perform validation. Use validatePosString to validate if necessary.
	void setupPosition(GameState* gamePtr, Board& board, std::string posString);
	std::string getPositionStringFromBoard(Board board);
}