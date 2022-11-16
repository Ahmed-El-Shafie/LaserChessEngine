#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <memory>

#include "Common.h"
#include "PosUtils.h"
#include "Moves.h"
#include "MoveUtils.h"
#include "GameState.h"

void handleTurnInput(std::string input, GameState* gamePtr, Board& board) {
	if (input == "PLAY") {
		std::string moveString;
		while (true) {
			std::cout << "Please input move string:";
			std::getline(std::cin, moveString);
			std::unique_ptr<Moves::Move> move;
			if (MoveUtils::getMoveFromString(move, moveString) && move->isLegalMove(board, gamePtr->turn)) {
				move->applyMove(board, gamePtr);
				Square* hitSquare = MoveUtils::laserHit(board, gamePtr->turn);
				if (hitSquare) {
					Piece* hitPiece = gamePtr->removeHitPiece(hitSquare);
					std::cout << "Piece " + hitPiece->getRepr() << " at Row " << std::to_string(hitSquare->pos.y + 1) << " Column " << std::to_string(hitSquare->pos.x + 1) << " has been destroyed\n";
					delete hitPiece;
				}
				gamePtr->turn = gamePtr->turn == Common::PieceColor::BLUE ? Common::PieceColor::RED : Common::PieceColor::BLUE;
				break;
			}
			std::cout << "Invalid string or illegal move" << std::endl;
		}
	}
	else if (input == "AI") {
		std::cout << "Not yet implemented\n"; //TODO
	}
	else {
		std::cout << "Input must be PLAY or AI\n";
	}
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		std::cout << "Need at least 2 arguments";
		return 1;
	}

	Board board = PosUtils::createBoard();
	std::string posString = argv[1];
	std::vector<std::string> posStringErrors = PosUtils::validatePosString(board, posString);
	if (!posStringErrors.empty()) {
		std::cout << "Invalid position string\n";
		for (std::string error : posStringErrors) {
			std::cout << error << "\n";
		}
		return 1;
	}

	std::string playerTurnString = argv[2];
	std::transform(playerTurnString.begin(), playerTurnString.end(), playerTurnString.begin(), ::tolower);
	std::unordered_map<std::string, Common::PieceColor> pieceColorEnumMap = {
		{ "blue", Common::PieceColor::BLUE }, {"red", Common::PieceColor::RED }
	};
	if (pieceColorEnumMap.find(playerTurnString) == pieceColorEnumMap.end()) {
		std::cout << "Player string must be blue or red";
		return 1;
	}
	Common::PieceColor playerTurn = pieceColorEnumMap.at(playerTurnString);

	GameState game = GameState(Common::PieceColor::NONE, playerTurn);
	GameState* gamePtr = &game;
	PosUtils::setupPosition(gamePtr, board, posString);
	std::cout << "Current position is: " + PosUtils::getPositionStringFromBoard(board) << std::endl;

	while (gamePtr->winner == Common::PieceColor::NONE) {
		std::string turnString = gamePtr->turn == Common::PieceColor::BLUE ? "BLUE" : "RED";
		std::cout << turnString << " to play.\nWould you like to input a move (PLAY), or let the engine play it for you (AI):";
		std::string line;
		std::getline(std::cin, line);
		handleTurnInput(line, gamePtr, board);
		std::cout << std::endl;
		std::cout << "Current position is: " + PosUtils::getPositionStringFromBoard(board) << std::endl;
	}

	std::string winnerString = gamePtr->winner == Common::PieceColor::BLUE ? "BLUE" : "RED";
	std::cout << "The winner is " << winnerString;
	return 0;
}
