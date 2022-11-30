#pragma once

#include <string>
#include <memory>
#include <unordered_set>

#include "Common.h"
#include "Square.h"

struct GameState;

namespace Moves {

	struct Move {
	protected:
		virtual bool equals(const Move& move) const = 0;
	public:
		virtual ~Move() = default;
		virtual std::string getMoveString() const = 0;
		virtual bool isLegalMove(const Board& board, Common::PieceColor player) const = 0;
		virtual void applyMove(Board& board, GameState* gamePtr) = 0;
		virtual std::unique_ptr<Move> getOppositeMove() const = 0;
		bool operator==(const Move& move) const {
			return this->equals(move);
		}
	};

	struct ShiftMove : Move {
	protected:
		bool equals(const Move& move) const override;
	public:
		ShiftMove(Common::Vector startPos, Common::Vector endPos) : startPos(startPos), endPos(endPos) {}
		~ShiftMove() = default;
		Common::Vector startPos;
		Common::Vector endPos;
		static bool getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString);
		std::string getMoveString() const override;
		bool isLegalMove(const Board& board, Common::PieceColor player) const override;
		void applyMove(Board& board, GameState* gamePtr) override;
		std::unique_ptr<Move> getOppositeMove() const override;
	};

	struct RotateMove : Move {
	protected:
		bool equals(const Move& move) const override;
	public:
		RotateMove(Common::Vector piecePos, Common::RotationDirection rotationDirection) : piecePos(piecePos), rotationDirection(rotationDirection) {}
		~RotateMove() = default;
		Common::Vector piecePos;
		Common::RotationDirection rotationDirection;
		static bool getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString);
		std::string getMoveString() const override;
		bool isLegalMove(const Board& board, Common::PieceColor player) const override;
		void applyMove(Board& board, GameState* gamePtr) override;
		std::unique_ptr<Move> getOppositeMove() const override;
	};

	struct SpecialMove : Move
	{
	protected:
		bool equals(const Move& move) const override;
	public:
		SpecialMove(Common::Vector startPos, Common::Vector endPos) : startPos(startPos), endPos(endPos) {}
		~SpecialMove() = default;
		Common::Vector startPos;
		Common::Vector endPos;
		static bool getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString);
		std::string getMoveString() const override;
		bool isLegalMove(const Board& board, Common::PieceColor player) const override;
		void applyMove(Board& board, GameState* gamePtr) override;
		std::unique_ptr<Move> getOppositeMove() const override;
	};
}