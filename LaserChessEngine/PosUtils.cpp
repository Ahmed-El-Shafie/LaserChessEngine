#include "PosUtils.h"
#include "Laser.h"
#include "Defender.h"
#include "Deflector.h"
#include "King.h"
#include "Switch.h"
#include "Piece.h"
#include "Square.h"

#include <stdexcept>
#include <unordered_set>

namespace {
	std::vector<std::string> validateRowString(std::array<Square, numCols> row, int rowNum, std::string rowString) {
		if (rowString == "*") {
			return {};
		}
		std::vector<std::string> rowStringErrors;
		std::string rowNumString = "Row " + std::to_string(rowNum + 1) + ": ";
		if (rowNum == 0 && (rowString.empty() || rowString.at(0) != 'l')) {
			rowStringErrors.push_back(rowNumString + "Missing red laser on top-left corner");
		}
		bool blueLaserInCorner = false;
		std::string pieceLetters = "lkdbs";
		bool scanningPiece = false;
		int rotationPluses = 0;
		bool digitScanned = false;
		int colNum = 0;
		for (size_t i = 0; i < rowString.length(); i++) {
			char c = rowString.at(i);
			if (isalpha(c)) {
				if (pieceLetters.find_first_of(tolower(c)) == std::string::npos) {
					rowStringErrors.push_back(rowNumString + "Invalid piece character " + c);
				}
				else if (c == 'l' && (rowNum != 0 || colNum != 0)) {
					rowStringErrors.push_back(rowNumString + "Red laser can only be on top-left corner");
				}
				else if (c == 'L') {
					if (rowNum == numRows - 1 && colNum == numCols - 1) {
						blueLaserInCorner = true;
					}
					else {
						rowStringErrors.push_back(rowNumString + "Blue laser can only be on bottom-right corner");
					}
				}
				Common::PieceColor oppositePieceColor = isupper(c) ? Common::PieceColor::RED : Common::PieceColor::BLUE;
				if (colNum < numCols && row[colNum].reservedColor == oppositePieceColor) {
					std::string oppositeColorString = oppositePieceColor == Common::PieceColor::RED ? "red" : "blue";
					rowStringErrors.push_back(rowNumString + "Column " + std::to_string(colNum + 1) + " is reserved for " + oppositeColorString + " pieces only");
				}
				scanningPiece = true;
				rotationPluses = 0;
				digitScanned = false;
				colNum++;
			}
			else if (c == '+') {
				if (!scanningPiece) {
					rowStringErrors.push_back(rowNumString + "'+' can only come after a piece character");
				}
				if (rotationPluses == 3) {
					rowStringErrors.push_back(rowNumString + "There can only be at most 3 consecutive '+' characters");
				}
				rotationPluses += 1;
			}
			else if (isdigit(c)) {
				if (c == '0') {
					rowStringErrors.push_back(rowNumString + "Digit can't be 0");
				}
				if (digitScanned) {
					rowStringErrors.push_back(rowNumString + "Can't have 2 consecutive digits");
				}
				scanningPiece = false;
				digitScanned = true;
				rotationPluses = 0;
				colNum += c - '0';
			}
			else {
				rowStringErrors.push_back(rowNumString + "Invalid character " + c);
			}
			if (colNum > numCols) {
				rowStringErrors.push_back(rowNumString + "Too many items in this row");
				break; // To avoid wasting time parsing the rest of this row and repeating this error message many times
			}
		}
		if (colNum < numCols) {
			rowStringErrors.push_back(rowNumString + "Not enough items in this row");
		}
		if (rowNum == numRows - 1 && !blueLaserInCorner) {
			rowStringErrors.push_back(rowNumString + "Missing blue laser on bottom-right corner");
		}
		return rowStringErrors;
	}

	void createPiece(std::shared_ptr<Piece>& piecePtr, char pieceChar, Common::PieceColor color) {
		pieceChar = tolower(pieceChar);
		switch (pieceChar) {
		case 'l': {
			piecePtr = std::make_shared<Laser>(color, 0);
			break;
		}
		case 'k': {
			piecePtr = std::make_shared<King>(color, 0);
			break;
		}
		case 'b': {
			piecePtr = std::make_shared<Deflector>(color, 0);
			break;
		}
		case 'd': {
			piecePtr = std::make_shared<Defender>(color, 0);
			break;
		}
		case 's': {
			piecePtr = std::make_shared<Switch>(color, 0);
			break;
		}
		default:
			throw std::invalid_argument("No piece for this character");
		}
	}

	void fillRowFromRowString(GameState* gamePtr, std::array<Square, numCols>& row, std::string rowString) {
		std::shared_ptr<Piece> currentPiece = nullptr;
		int colNum = 0;
		for (size_t i = 0; i < rowString.length(); i++) {
			char c = rowString.at(i);
			if (isalpha(c)) {
				Common::PieceColor color = isupper(c) ? Common::PieceColor::BLUE : Common::PieceColor::RED;
				Square& square = row[colNum];
				createPiece(currentPiece, c, color);
				square.piece = currentPiece;
				std::unordered_set<Square, Square::HashFunction>& pieceSquares = color == Common::PieceColor::BLUE ? gamePtr->bluePieceSquares : gamePtr->redPieceSquares;
				pieceSquares.insert(square);
				colNum++;
			}
			else if (c == '+' && currentPiece /*Checking for linter warning*/) {
				currentPiece->rotateRight();
			}
			else if (isdigit(c)) {
				colNum += c - '0';
			}
		}
	}

	std::string getRowStringFromRow(std::array<Square, numCols> row) {
		std::string rowString = "";
		int emptySquares = 0;
		for (int j = 0; j < numCols; j++) {
			Square square = row[j];
			if (square.piece) {
				if (emptySquares > 0) {
					rowString += std::to_string(emptySquares);
					emptySquares = 0;
				}
				rowString += square.piece->getRepr();
			}
			else {
				emptySquares++;
			}
		}
		if (emptySquares == numCols) {
			rowString += "*";
		}
		else if (emptySquares > 0) {
			rowString += std::to_string(emptySquares);
		}
		return rowString;
	}

	Common::Vector findKingPos(const std::unordered_set<Square, Square::HashFunction>& pieceSquares) {
		for (const Square& pieceSquare : pieceSquares) {
			if (tolower(pieceSquare.piece->getPieceChar()) == 'k') {
				return pieceSquare.pos;
			}
		}
		return { -1, -1 };
	}

	int getBestinFrontOfLaserOrientation(char pieceChar, size_t laserCol) {
		pieceChar = tolower(pieceChar);
		if (laserCol == 0) {
			return 0;
		}
		else if (pieceChar == 'b') {
			return 2;
		}
		else if (pieceChar == 's') {
			return 0;
		}
		else {
			return -1;
		}
	}

	float evaluatePieces(const std::unordered_set<Square, Square::HashFunction>& pieceSquares, Common::Vector ownKingPos, Common::Vector opposingKingPos, size_t laserCol) {
		float score = 0;
		float deflectingPieceInFrontOfLaserDivider = 1;
		for (const Square& pieceSquare : pieceSquares) {
			std::shared_ptr<Piece> piece = pieceSquare.piece;
			char pieceChar = piece->getPieceChar();
			score += piece->getDistanceScore(pieceSquare.pos, ownKingPos, opposingKingPos);
			if (tolower(pieceChar) == 'b' || tolower(pieceChar) == 's') {
				if (pieceSquare.pos.x == opposingKingPos.x || pieceSquare.pos.y == opposingKingPos.y) {
					score += 300;
				}
				if (pieceSquare.pos.x == laserCol) {
					score += 400 / deflectingPieceInFrontOfLaserDivider;
					if (piece->getOrientation() == getBestinFrontOfLaserOrientation(pieceChar, laserCol)) {
						score += 200 / deflectingPieceInFrontOfLaserDivider;
					}
					deflectingPieceInFrontOfLaserDivider *= 2;
				}
			}
			Common::Vector defenderToKing = ownKingPos - pieceSquare.pos;
			if (tolower(pieceChar) == 'd' && (defenderToKing.x == 0 || defenderToKing.y == 0)) {
				Common::Vector unitVector = defenderToKing.x == 0 ? Common::Vector{0, defenderToKing.y / abs(defenderToKing.y)} : Common::Vector{defenderToKing.x / abs(defenderToKing.x), 0};
				int bestOrientation = Common::directionToOrientation.at(unitVector);
				if (piece->getOrientation() == bestOrientation) {
					score += 80;
				}
			}
		}
		return score;
	}
}

std::vector<std::string> PosUtils::validatePosString(Board board, std::string posString) {
	std::vector<std::string> posStringErrors;
	if (posString.find('k') == std::string::npos && posString.find('K') == std::string::npos) {
		posStringErrors.push_back("Impossible for both sides' kings to be missing");
	}
	posString += "/"; // Adding slash to make last row parsing easier
	size_t slashSearchStart = 0;
	for (int i = 0; i < numRows; i++) {
		int slashIndex = posString.find('/', slashSearchStart);
		if (slashIndex == std::string::npos) {
			posStringErrors.push_back("Not enough rows");
			break;
		}
		std::string rowString = posString.substr(slashSearchStart, slashIndex - slashSearchStart);
		std::vector<std::string> rowStringErrors = validateRowString(board[i], i, rowString);
		posStringErrors.insert(posStringErrors.end(), rowStringErrors.begin(), rowStringErrors.end());
		slashSearchStart = slashIndex + 1;
	}
	if (slashSearchStart < posString.length()) {
		posStringErrors.push_back("Too many rows");
	}
	return posStringErrors;
}

Board PosUtils::createBoard() {
	Board board;
	// Top left in array is [0][0], but coordinate "a1" is bottom left
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			Common::PieceColor reservedColor = Common::PieceColor::NONE;
			if (j == 0 || (j == numCols - 2 && (i == 0 || i == numCols - 1))) {
				reservedColor = Common::PieceColor::RED;
			}
			else if (j == numCols - 1 || (j == 1 && (i == 0 || i == numCols - 1))) {
				reservedColor = Common::PieceColor::BLUE;
			}
			Square square = Square({ j, i }, reservedColor);
			board[i][j] = square;
		}
	}
	return board;
}

void PosUtils::setupPosition(GameState* gamePtr, Board& board, std::string posString) {
	posString += "/"; // Adding slash to make last row parsing easier
	int slashSearchStart = 0;
	for (int i = 0; i < numRows; i++) {
		int slashIndex = posString.find('/', slashSearchStart);
		std::string rowString = posString.substr(slashSearchStart, slashIndex - slashSearchStart);
		fillRowFromRowString(gamePtr, board[i], rowString);
		slashSearchStart = slashIndex + 1;
	}
	if (posString.find('k') == std::string::npos) {
		gamePtr->winner = Common::PieceColor::BLUE;
	}
	else if (posString.find('K') == std::string::npos) {
		gamePtr->winner = Common::PieceColor::RED;
	}
}

std::string PosUtils::getPositionStringFromBoard(const Board& board) {
	std::string posString = "";
	for (int i = 0; i < numRows; i++) {
		posString += getRowStringFromRow(board[i]);
		if (i != numRows - 1) {
			posString += "/";
		}
	}
	return posString;
}

float PosUtils::evaluatePosition(const Board& board, const GameState& gameState) {
	if (gameState.winner == Common::PieceColor::BLUE) {
		return std::numeric_limits<float>::max();
	}
	else if (gameState.winner == Common::PieceColor::RED) {
		return std::numeric_limits<float>::lowest();
	}
	float score = 0;
	Common::Vector blueKingPos = findKingPos(gameState.bluePieceSquares);
	Common::Vector redKingPos = findKingPos(gameState.redPieceSquares);
	score += evaluatePieces(gameState.bluePieceSquares, blueKingPos, redKingPos, numCols - 1);
	score -= evaluatePieces(gameState.redPieceSquares, redKingPos, blueKingPos, 0);
	return score / 100;
}