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

	void createPiece(Piece* &piecePtr, char pieceChar, Common::PieceColor color) {
		pieceChar = tolower(pieceChar);
		switch (pieceChar) {
		case 'l': {
			piecePtr = new Laser(color, 0);
			break;
		}
		case 'k': {
			piecePtr = new King(color, 0);
			break;
		}
		case 'b': {
			piecePtr = new Deflector(color, 0);
			break;
		}
		case 'd': {
			piecePtr = new Defender(color, 0);
			break;
		}
		case 's': {
			piecePtr = new Switch(color, 0);
			break;
		}
		default:
			throw std::invalid_argument("No piece for this character");
		}
	}

	void fillRowFromRowString(GameState* gamePtr, std::array<Square, numCols>& row, std::string rowString) {
		Piece* currentPiece = nullptr;
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

std::string PosUtils::getPositionStringFromBoard(const Board board) {
	std::string posString = "";
	for (int i = 0; i < numRows; i++) {
		int emptySquares = 0;
		for (int j = 0; j < numCols; j++) {
			Square square = board[i][j];
			if (square.piece) {
				if (emptySquares > 0) {
					posString += std::to_string(emptySquares);
					emptySquares = 0;
				}
				posString += square.piece->getRepr();
			}
			else {
				emptySquares++;
			}
		}
		if (emptySquares == numCols) {
			posString += "*";
		}
		else if (emptySquares > 0) {
			posString += std::to_string(emptySquares);
		}
		if (i != numRows - 1) {
			posString += "/";
		}
	}
	return posString;
}