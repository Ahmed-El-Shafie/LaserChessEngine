#pragma once

#include <string>

#include "Common.h"
#include "Moves.h"

namespace MoveUtils {
	bool getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString);
	Square* laserHit(Board& board, Common::PieceColor turn);
}