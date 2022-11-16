#pragma once

#include <unordered_map>
#include <string>
#include <array>
#include <memory>

#include "Common.h"
#include "Square.h"

struct GameState;

namespace Moves {

	struct Move {
		virtual ~Move() = default;
		virtual std::string getMoveString() = 0;
		virtual bool isLegalMove(Board board, Common::PieceColor player) = 0;
		virtual void applyMove(Board& board, GameState* gamePtr) = 0;
	};

	struct ShiftMove : Move {
		ShiftMove(Common::Vector startPos, Common::Vector endPos) : startPos(startPos), endPos(endPos) {}
		~ShiftMove() = default;
		Common::Vector startPos;
		Common::Vector endPos;
		static bool getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString);
		std::string getMoveString() override;
		bool isLegalMove(Board board, Common::PieceColor player) override;
		void applyMove(Board& board, GameState* gamePtr) override;
	};

	struct RotateMove : Move {
		RotateMove(Common::Vector piecePos, Common::RotationDirection rotationDirection) : piecePos(piecePos), rotationDirection(rotationDirection) {}
		~RotateMove() = default;
		Common::Vector piecePos;
		Common::RotationDirection rotationDirection;
		static bool getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString);
		std::string getMoveString() override;
		bool isLegalMove(Board board, Common::PieceColor player) override;
		void applyMove(Board& board, GameState* gamePtr) override;
	};

	struct SpecialMove : Move
	{
		SpecialMove(Common::Vector startPos, Common::Vector endPos) : startPos(startPos), endPos(endPos) {}
		~SpecialMove() = default;
		Common::Vector startPos;
		Common::Vector endPos;
		static bool getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString);
		std::string getMoveString() override;
		bool isLegalMove(Board board, Common::PieceColor player) override;
		void applyMove(Board& board, GameState* gamePtr) override;
	};
}