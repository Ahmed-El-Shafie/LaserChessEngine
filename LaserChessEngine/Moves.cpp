#include <cstdlib>

#include "Moves.h"
#include "King.h"
#include "Laser.h"
#include "Switch.h"
#include "GameState.h"

namespace {
	int getRow(char rowChar) {
		if (!isdigit(rowChar)) {
			return -1;
		}
		return numRows - (rowChar - '0');
	}

	char getRowChar(int rowNum) {
		return numRows - rowNum + '0';
	}

	int getCol(char colChar) {
		if (!isalpha(colChar)) {
			return -1;
		}
		return colChar - 'a';
	}

	char getColChar(int colNum) {
		return colNum + 'a';
	}

	bool validatePos(Common::Vector pos) {
		return pos.x >= 0 && pos.x < numCols &&
			pos.y >= 0 && pos.y < numRows;
	}

	bool validateMove(Common::Vector startPos, Common::Vector endPos) {
		return abs(endPos.y - startPos.y) <= 1 &&
			abs(endPos.x - startPos.x) <= 1 &&
			!(startPos == endPos);
	}
}

bool Moves::ShiftMove::equals(const Move& move) const {
	if (typeid(*this) != typeid(move)) {
		return false;
	}
	const ShiftMove& shiftMove = dynamic_cast<const ShiftMove&>(move);
	return this->startPos == shiftMove.startPos && this->endPos == shiftMove.endPos;
};

bool Moves::ShiftMove::getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString) {
	if (moveString.length() != 4) { return false; }
	int startCol = getCol(moveString.at(0));
	int startRow = getRow(moveString.at(1));
	int endCol = getCol(moveString.at(2));
	int endRow = getRow(moveString.at(3));
	Common::Vector startPos = { startCol, startRow };
	Common::Vector endPos = { endCol, endRow };
	move = std::make_unique<ShiftMove>(startPos, endPos);
	return true;
}

std::string Moves::ShiftMove::getMoveString() const {
	char startColChar = getColChar(this->startPos.x);
	char startRowChar = getRowChar(this->startPos.y);
	char endColChar = getColChar(this->endPos.x);
	char endRowChar = getRowChar(this->endPos.y);
	return std::string{ startColChar, startRowChar, endColChar, endRowChar };
}

bool Moves::ShiftMove::isLegalMove(const Board& board, Common::PieceColor player) const {
	if (!validatePos(this->startPos) || !validatePos(this->endPos) || !validateMove(this->startPos, this->endPos)) {
		return false;
	}
	Square startSquare = board[this->startPos.y][this->startPos.x];
	if (!startSquare.piece) {
		return false;
	}
	Piece* piece = startSquare.piece.get();
	if (piece->getColor() != player || tolower(piece->getPieceChar()) == 'l') {
		return false;
	}
	Square endSquare = board[this->endPos.y][this->endPos.x];
	Common::PieceColor oppositePieceColor = player == Common::PieceColor::BLUE ? Common::PieceColor::RED : Common::PieceColor::BLUE;
	if (endSquare.piece || endSquare.reservedColor == oppositePieceColor) {
		return false;
	}
	return true;
}

void Moves::ShiftMove::applyMove(Board& board, GameState* gamePtr) {
	Square& startSquare = board[this->startPos.y][this->startPos.x];
	std::shared_ptr<Piece> piece = startSquare.piece;
	Square& endSquare = board[this->endPos.y][this->endPos.x];
	std::unordered_set<Square, Square::HashFunction>& pieceSquares = piece->getColor() == Common::PieceColor::BLUE ? gamePtr->bluePieceSquares : gamePtr->redPieceSquares;
	startSquare.piece = nullptr;
	endSquare.piece = piece;
	pieceSquares.erase(startSquare);
	pieceSquares.insert(endSquare);
}

std::unique_ptr<Moves::Move> Moves::ShiftMove::getOppositeMove() const {
	return std::make_unique<Moves::ShiftMove>(this->endPos, this->startPos);
}

bool Moves::RotateMove::equals(const Move& move) const {
	if (typeid(*this) != typeid(move)) {
		return false;
	}
	const RotateMove& rotateMove = dynamic_cast<const RotateMove&>(move);
	return this->piecePos == rotateMove.piecePos && this->rotationDirection == rotateMove.rotationDirection;
};

bool Moves::RotateMove::getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString) {
	if (moveString.length() != 3) {
		return false;
	}
	int pieceCol = getCol(moveString.at(0));
	int pieceRow = getRow(moveString.at(1));
	Common::Vector piecePos = { pieceCol, pieceRow };
	Common::RotationDirection rotationDirection;
	switch (moveString.at(2)) {
	case '+':
		rotationDirection = Common::RotationDirection::RIGHT;
		break;
	case '-':
		rotationDirection = Common::RotationDirection::LEFT;
		break;
	default:
		return false;
	}
	move = std::make_unique<RotateMove>(piecePos, rotationDirection);
	return true;
}

std::string Moves::RotateMove::getMoveString() const {
	char pieceCol = getColChar(this->piecePos.x);
	char pieceRow = getRowChar(this->piecePos.y);
	char direction = this->rotationDirection == Common::RotationDirection::RIGHT ? '+' : '-';
	return std::string{ pieceCol, pieceRow, direction };
}

bool Moves::RotateMove::isLegalMove(const Board& board, Common::PieceColor player) const {
	if (!validatePos(this->piecePos)) {
		return false;
	}
	Square pieceSquare = board[this->piecePos.y][this->piecePos.x];
	if (!pieceSquare.piece) {
		return false;
	}
	Piece* piece = pieceSquare.piece.get();
	if (piece->getColor() != player) {
		return false;
	}
	return true;
}

void Moves::RotateMove::applyMove(Board& board, GameState* gamePtr) {
	std::shared_ptr<Piece> piece = board[this->piecePos.y][this->piecePos.x].piece;
	if (this->rotationDirection == Common::RotationDirection::RIGHT) {
		piece->rotateRight();
	}
	else {
		piece->rotateLeft();
	}
}

std::unique_ptr<Moves::Move> Moves::RotateMove::getOppositeMove() const {
	Common::RotationDirection oppositeRotation = this->rotationDirection == Common::RotationDirection::RIGHT ? Common::RotationDirection::LEFT : Common::RotationDirection::RIGHT;
	return std::make_unique<Moves::RotateMove>(this->piecePos, oppositeRotation);
}

bool Moves::SpecialMove::equals(const Move& move) const {
	if (typeid(*this) != typeid(move)) {
		return false;
	}
	const SpecialMove& specialMove = dynamic_cast<const SpecialMove&>(move);
	return this->startPos == specialMove.startPos && this->endPos == specialMove.endPos;
};

bool Moves::SpecialMove::getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString) {
	if (moveString.length() != 5) { return false; }
	int startCol = getCol(moveString.at(0));
	int startRow = getRow(moveString.at(1));
	int endCol = getCol(moveString.at(3));
	int endRow = getRow(moveString.at(4));
	Common::Vector startPos = { startCol, startRow };
	Common::Vector endPos = { endCol, endRow };
	if (moveString.at(2) != 'u') {
		return false;
	}
	move = std::make_unique<SpecialMove>(startPos, endPos);
	return true;
}

std::string Moves::SpecialMove::getMoveString() const {
	char startColChar = getColChar(this->startPos.x);
	char startRowChar = getRowChar(this->startPos.y);
	char endColChar = getColChar(this->endPos.x);
	char endRowChar = getRowChar(this->endPos.y);
	return std::string{ startColChar, startRowChar, 'u', endColChar, endRowChar};
}

bool Moves::SpecialMove::isLegalMove(const Board& board, Common::PieceColor player) const {
	if (!validatePos(this->startPos) || !validatePos(this->endPos) || !validateMove(this->startPos, this->endPos)) {
		return false;
	}
	Square startSquare = board[this->startPos.y][this->startPos.x];
	if (!startSquare.piece) {
		return false;
	}
	Piece* piece = startSquare.piece.get();
	if (piece->getColor() != player || tolower(piece->getPieceChar()) != 's') {
		return false;
	}
	Square endSquare = board[this->endPos.y][this->endPos.x];
	Common::PieceColor oppositePieceColor = player == Common::PieceColor::BLUE ? Common::PieceColor::RED : Common::PieceColor::BLUE;
	if (!endSquare.piece || !(endSquare.piece->canBeSwitched()) || endSquare.reservedColor == oppositePieceColor || (startSquare.reservedColor == player && endSquare.piece->getColor() != player)) {
		return false;
	}
	return true;
}

void Moves::SpecialMove::applyMove(Board& board, GameState* gamePtr) {
	Square& startSquare = board[this->startPos.y][this->startPos.x];
	std::shared_ptr<Piece> startPiece = startSquare.piece;
	Square& endSquare = board[this->endPos.y][this->endPos.x];
	std::shared_ptr<Piece> endPiece = endSquare.piece;
	std::unordered_set<Square, Square::HashFunction>& startPieceSquares = startPiece->getColor() == Common::PieceColor::BLUE ? gamePtr->bluePieceSquares : gamePtr->redPieceSquares;
	std::unordered_set<Square, Square::HashFunction>& endPieceSquares = endPiece->getColor() == Common::PieceColor::BLUE ? gamePtr->bluePieceSquares : gamePtr->redPieceSquares;
	std::shared_ptr<Piece> temp = endSquare.piece;
	endSquare.piece = startSquare.piece;
	startSquare.piece = temp;
	startPieceSquares.erase(startSquare);
	endPieceSquares.erase(endSquare);
	startPieceSquares.insert(endSquare);
	endPieceSquares.insert(startSquare);
}

std::unique_ptr<Moves::Move> Moves::SpecialMove::getOppositeMove() const {
	return std::make_unique<Moves::SpecialMove>(this->endPos, this->startPos);
}