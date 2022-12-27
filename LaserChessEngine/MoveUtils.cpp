#include <array>
#include <algorithm>

#include "MoveUtils.h"
#include "PosUtils.h"
#include "Square.h"
#include "Laser.h"
#include <functional>

namespace {
	static const std::array<Common::Vector, 8> possibleMoveVectors = {
		Common::Vector{-1, -1}, Common::upVector, Common::Vector{1, -1},
		Common::leftVector, Common::rightVector,
		Common::Vector{-1, 1}, Common::downVector, Common::Vector{1, 1}
	};

	std::string getTranspositionKey(const Board& board, Common::PieceColor player) {
		std::string turnString = player == Common::PieceColor::BLUE ? "blue" : "red";
		return PosUtils::getPositionStringFromBoard(board) + turnString;
	}

	std::pair<Square*, std::shared_ptr<Piece>> makeMove(Board& board, GameState& gameState, std::shared_ptr<Moves::Move> move) {
		move->applyMove(board, &gameState);
		Square* hitSquare = MoveUtils::laserHit(board, gameState.turn);
		std::shared_ptr<Piece> hitPiece = nullptr;
		if (hitSquare) {
			hitPiece = gameState.removeHitPiece(hitSquare);
		}
		gameState.turn = gameState.turn == Common::PieceColor::BLUE ? Common::PieceColor::RED : Common::PieceColor::BLUE;
		return { hitSquare, hitPiece };
	}

	void makeOppositeMove(Board& board, GameState& gameState, std::shared_ptr<Moves::Move> originalMove, Square* hitSquare, std::shared_ptr<Piece> hitPiece) {
		gameState.turn = gameState.turn == Common::PieceColor::BLUE ? Common::PieceColor::RED : Common::PieceColor::BLUE;;
		if (hitSquare) {
			gameState.returnHitPiece(hitSquare, hitPiece);
		}
		std::unique_ptr<Moves::Move> oppositeMove = originalMove->getOppositeMove();
		oppositeMove->applyMove(board, &gameState);
	}

	MoveUtils::EvaluationResult findBestMovePruning(Board& board, GameState& gameState, size_t movesRemaining, const size_t maxDepth, const float alpha, const float beta, std::unordered_map<std::string, MoveUtils::EvaluationResult>& transpositionTable) {
		const int numPieces = gameState.bluePieceSquares.size() + gameState.redPieceSquares.size();
		if (movesRemaining == 0 || gameState.winner != Common::PieceColor::NONE || (numPieces > 20 && maxDepth - movesRemaining == 4)) {
			// Inserting this into transposition table doesn't seem to improve performance
			return { nullptr, PosUtils::evaluatePosition(board, gameState), movesRemaining };
		}
		Common::PieceColor currentTurn = gameState.turn;
		std::string transpositionKey = getTranspositionKey(board, currentTurn);
		// A higher remainingMoves means that the position was analyzed more deeply
		if (transpositionTable.find(transpositionKey) != transpositionTable.end() && transpositionTable.at(transpositionKey).movesRemaining >= movesRemaining) {
			return transpositionTable.at(transpositionKey);
		}
		std::unordered_set<Square, Square::HashFunction> pieceSquares;
		float bestEvaluation;
		std::function<bool(MoveUtils::EvaluationResult, MoveUtils::EvaluationResult)> minimaximizer;
		float alphaLocal = alpha;
		float betaLocal = beta;
		float* pruningParam;
		if (currentTurn == Common::PieceColor::BLUE) {
			pieceSquares = gameState.bluePieceSquares;
			bestEvaluation = std::numeric_limits<float>::lowest();
			minimaximizer = [currentTurn](MoveUtils::EvaluationResult current, MoveUtils::EvaluationResult overallBest) {
				return current.evaluation > overallBest.evaluation;
			};
			pruningParam = &alphaLocal;
		}
		else {
			pieceSquares = gameState.redPieceSquares;
			bestEvaluation = std::numeric_limits<float>::max();
			minimaximizer = [currentTurn](MoveUtils::EvaluationResult current, MoveUtils::EvaluationResult overallBest) {
				return current.evaluation < overallBest.evaluation;
			};
			pruningParam = &betaLocal;
		}
		std::vector<MoveUtils::EvaluationResult> depth1Evaluations;
		for (const Square& square : pieceSquares) {
			for (std::shared_ptr<Moves::Move> move : MoveUtils::getLegalMoves(board, square)) {
				std::pair<Square*, std::shared_ptr<Piece>> hitSquareAndPiece = makeMove(board, gameState, move);
				float evalResult = PosUtils::evaluatePosition(board, gameState);
				depth1Evaluations.push_back({ move, evalResult, movesRemaining - 1});
				makeOppositeMove(board, gameState, move, hitSquareAndPiece.first, hitSquareAndPiece.second);
			}
		}
		std::sort(std::begin(depth1Evaluations), std::end(depth1Evaluations), minimaximizer);
		MoveUtils::EvaluationResult bestEvaluationResult = { nullptr, bestEvaluation, movesRemaining };
		int i = 0;
		for (MoveUtils::EvaluationResult eval : depth1Evaluations) {
			if (numPieces > 10 && maxDepth - movesRemaining >= 6 && i > 5) {
				break;
			}
			std::pair<Square*, std::shared_ptr<Piece>> hitSquareAndPiece = makeMove(board, gameState, eval.move);
			MoveUtils::EvaluationResult moveEval = findBestMovePruning(board, gameState, movesRemaining - 1, maxDepth, alphaLocal, betaLocal, transpositionTable);
			if (!bestEvaluationResult.move || minimaximizer(moveEval, bestEvaluationResult)) {
				bestEvaluationResult = { eval.move, moveEval.evaluation, moveEval.movesRemaining + 1 };
				*pruningParam = moveEval.evaluation;
			}
			makeOppositeMove(board, gameState, eval.move, hitSquareAndPiece.first, hitSquareAndPiece.second);
			if (beta <= alpha) {
				break;
			}
			i++;
		}
		transpositionTable.insert({ transpositionKey, bestEvaluationResult });
		return bestEvaluationResult;
	}
}

bool MoveUtils::getMoveFromString(std::unique_ptr<Moves::Move>& move, std::string moveString) {
	return Moves::ShiftMove::getMoveFromString(move, moveString) ||
		Moves::RotateMove::getMoveFromString(move, moveString) ||
		Moves::SpecialMove::getMoveFromString(move, moveString);
}

Square* MoveUtils::laserHit(Board& board, Common::PieceColor turn) {
	Common::Vector pos = turn == Common::PieceColor::BLUE ? Common::Vector{ numCols - 1, numRows - 1 } : Common::zeroVector;
	Laser* laser = dynamic_cast<Laser*>(board[pos.y][pos.x].piece.get());
	Common::Vector laserDir = laser->getLaserDir();
	pos = pos + laserDir; // Skipping laser position handling
	while (0 <= pos.x && pos.x < numCols && 0 <= pos.y && pos.y < numRows) {
		Square& currentSquare = board[pos.y][pos.x];
		Piece* contactPiece = currentSquare.piece.get();
		if (contactPiece) {
			std::pair<SideInteraction, Common::Vector> laserInteraction = contactPiece->getLaserInteraction(laserDir);
			switch (laserInteraction.first) {
			case SideInteraction::DEFLECT:
				laserDir = laserInteraction.second;
				break;
			case SideInteraction::HIT:
				return &currentSquare;
				break;
			case SideInteraction::BLOCK:
				return nullptr;
			default:
				break;
			}
		}
		pos = pos + laserDir;
	}
	return nullptr;
}

std::vector<std::shared_ptr<Moves::Move>> MoveUtils::getLegalMoves(const Board& board, const Square& square) {
	Piece* piece = square.piece.get();
	if (!piece) {
		return {};
	}
	std::vector<std::shared_ptr<Moves::Move>> moves;
	for (Common::Vector moveVector : possibleMoveVectors) {
		std::shared_ptr<Moves::Move> shiftMove = std::make_shared<Moves::ShiftMove>(square.pos, square.pos + moveVector);
		if (shiftMove->isLegalMove(board, piece->getColor())) {
			moves.push_back(shiftMove);
		}
		std::shared_ptr<Moves::Move> specialMove = std::make_shared<Moves::SpecialMove>(square.pos, square.pos + moveVector);
		if (specialMove->isLegalMove(board, piece->getColor())) {
			moves.push_back(specialMove);
		}
	}
	int leftRotateOrientation = Common::mod(piece->getOrientation() - 1, piece->getNumOrientations());
	int rightRotateOrientation = Common::mod(piece->getOrientation() + 1, piece->getNumOrientations());
	moves.push_back(std::make_shared<Moves::RotateMove>(square.pos, Common::RotationDirection::LEFT));
	if (leftRotateOrientation != rightRotateOrientation) {
		moves.push_back(std::make_shared<Moves::RotateMove>(square.pos, Common::RotationDirection::RIGHT));
	}
	return moves;
}

MoveUtils::EvaluationResult MoveUtils::findBestMove(Board& board, GameState& gameState, size_t depth) {
	/* Even though findBestMove modifies board and gameState, it returns it to original state. But it's better to make
	a copy anyway to reduce likelihood of bugs.*/
	GameState gameStateCopy = GameState(gameState.winner, gameState.turn);
	Board boardCopy = PosUtils::createBoard();
	PosUtils::setupPosition(&gameStateCopy, boardCopy, PosUtils::getPositionStringFromBoard(board));
	std::unordered_map<std::string, MoveUtils::EvaluationResult> transpositionTable{};
	return findBestMovePruning(boardCopy, gameStateCopy, depth * 2, depth * 2, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), transpositionTable);
}