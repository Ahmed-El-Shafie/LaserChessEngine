#pragma once

#include <array>

#include "Common.h"
#include "Piece.h"

struct Square {
	Square(Common::Vector pos, Common::PieceColor reservedColor) : piece(nullptr), pos(pos), reservedColor(reservedColor) {};
	Square() : piece(nullptr), pos({ -1, -1 }), reservedColor(Common::PieceColor::NONE) {};

	std::shared_ptr<Piece> piece;
	Common::Vector pos;
	Common::PieceColor reservedColor;

	bool operator==(const Square& otherSquare) const {
		return this->pos == otherSquare.pos;
	}
	struct HashFunction {
		size_t operator()(const Square& square) const {
			size_t rowHash = std::hash<int>{}(square.pos.y);
			size_t colHash = std::hash<int>{}(square.pos.x);
			return rowHash * 37 + colHash;
		}
	};
};

constexpr int numRows = 8;
constexpr int numCols = 10;
using Board = std::array<std::array<Square, numCols>, numRows>;

