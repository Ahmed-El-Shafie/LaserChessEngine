#include "gmock/gmock.h"
#include "Moves.h"
#include "PosUtils.h"

TEST(MovesTests, ShiftMoveGetMoveFromStringValid) {
	std::unique_ptr<Moves::Move> move;
	bool moveResult = Moves::ShiftMove::getMoveFromString(move, "a1j8");
	EXPECT_TRUE(moveResult);
	Moves::ShiftMove* shiftMove = dynamic_cast<Moves::ShiftMove*>(move.get());
	EXPECT_THAT(shiftMove, testing::Field(&Moves::ShiftMove::startPos, Common::Vector{ 0, 7 }));
	EXPECT_THAT(shiftMove, testing::Field(&Moves::ShiftMove::endPos, Common::Vector{ 9, 0 }));
}

TEST(MovesTests, ShiftMoveGetMoveFromStringWrongLength) {
	std::unique_ptr<Moves::Move> move;
	bool moveResult = Moves::ShiftMove::getMoveFromString(move, "a1j8x");
	EXPECT_FALSE(moveResult);
	EXPECT_THAT(move.get(), testing::IsNull());
}

TEST(MovesTests, ShiftMoveGetMoveString) {
	Moves::ShiftMove move = Moves::ShiftMove(Common::Vector{ 0, 7 }, Common::Vector{ 9, 0 });
	EXPECT_EQ(move.getMoveString(), "a1j8");
}

TEST(MovesTests, ShiftMoveIsLegalMoveValid) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1k8/*/*/*/*/1K8/9L");
	Moves::ShiftMove move = Moves::ShiftMove(Common::Vector{ 1, 1 }, Common::Vector{ 2, 2 });
	EXPECT_TRUE(move.isLegalMove(board, Common::PieceColor::RED));
}

void checkInvalidShiftMove(Common::Vector startPos, Common::Vector endPos) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1kd7/*/*/*/*/1K8/9L");
	Moves::ShiftMove move = Moves::ShiftMove(startPos, endPos);
	EXPECT_FALSE(move.isLegalMove(board, Common::PieceColor::RED));
}

TEST(MovesTests, ShiftMoveIsLegalMoveInvalidStartPos) {
	checkInvalidShiftMove(Common::Vector{ -1, 1 }, Common::Vector{ 2, 2 });
}

TEST(MovesTests, ShiftMoveIsLegalMoveInvalidEndPos) {
	checkInvalidShiftMove(Common::Vector{ 1, 1 }, Common::Vector{ -2, 2 });
}

TEST(MovesTests, ShiftMoveIsLegalMoveTooFar) {
	checkInvalidShiftMove(Common::Vector{ 1, 1 }, Common::Vector{ 2, 3 });
}

TEST(MovesTests, ShiftMoveIsLegalMoveSameSquare) {
	checkInvalidShiftMove(Common::Vector{ 1, 1 }, Common::Vector{ 1, 1 });
}

TEST(MovesTests, ShiftMoveIsLegalMoveNoPieceAtSquare) {
	checkInvalidShiftMove(Common::Vector{ 1, 2 }, Common::Vector{ 2, 2 });
}

TEST(MovesTests, ShiftMoveIsLegalMovePieceNotBelongingToPlayer) {
	checkInvalidShiftMove(Common::Vector{ 1, 6 }, Common::Vector{ 2, 6 });
}

TEST(MovesTests, ShiftMoveIsLegalMoveCantMoveLaser) {
	checkInvalidShiftMove(Common::zeroVector, Common::Vector{ 1, 1 });
}

TEST(MovesTests, ShiftMoveIsLegalMovePieceAtEndSquare) {
	checkInvalidShiftMove(Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
}

TEST(MovesTests, ShiftMoveIsLegalMoveEndSquareReserved) {
	checkInvalidShiftMove(Common::Vector{ 1, 1 }, Common::Vector{ 1, 0 });
}

TEST(MovesTests, ShiftMoveApplyMove) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1k8/*/*/*/*/1K8/9L");
	Moves::ShiftMove move = Moves::ShiftMove(Common::Vector{ 1, 1 }, Common::Vector{ 2, 2 });
	std::shared_ptr<Piece> piece = board[1][1].piece;
	move.applyMove(board, &gameState);
	EXPECT_THAT(board[1][1].piece, testing::IsNull());
	EXPECT_THAT(gameState.redPieceSquares, testing::Not(testing::Contains(board[1][1])));
	EXPECT_EQ(board[2][2].piece, piece);
	EXPECT_THAT(gameState.redPieceSquares, testing::Contains(board[2][2]));
}

TEST(MovesTests, ShiftMoveGetOppositeMove) {
	Moves::ShiftMove move = Moves::ShiftMove(Common::Vector{ 3, 3 }, Common::Vector{ 4, 4 });
	std::unique_ptr<Moves::Move> oppositeMove = move.getOppositeMove();
	Moves::ShiftMove* oppositeShiftMove = dynamic_cast<Moves::ShiftMove*>(oppositeMove.get());
	EXPECT_EQ(move.startPos, oppositeShiftMove->endPos);
	EXPECT_EQ(move.endPos, oppositeShiftMove->startPos);
}

TEST(MovesTests, RotateMoveGetMoveFromStringValid) {
	std::unique_ptr<Moves::Move> move;
	bool moveResult = Moves::RotateMove::getMoveFromString(move, "a1+");
	EXPECT_TRUE(moveResult);
	Moves::RotateMove* rotateMove = dynamic_cast<Moves::RotateMove*>(move.get());
	EXPECT_THAT(rotateMove, testing::Field(&Moves::RotateMove::piecePos, Common::Vector{ 0, 7 }));
	EXPECT_THAT(rotateMove, testing::Field(&Moves::RotateMove::rotationDirection, Common::RotationDirection::RIGHT));
}

TEST(MovesTests, RotateMoveGetMoveFromStringWrongLength) {
	std::unique_ptr<Moves::Move> move;
	bool moveResult = Moves::RotateMove::getMoveFromString(move, "a1+x");
	EXPECT_FALSE(moveResult);
	EXPECT_THAT(move.get(), testing::IsNull());
}

TEST(MovesTests, RotateMoveGetMoveFromStringInvalidRotationChar) {
	std::unique_ptr<Moves::Move> move;
	bool moveResult = Moves::RotateMove::getMoveFromString(move, "a1=");
	EXPECT_FALSE(moveResult);
	EXPECT_THAT(move.get(), testing::IsNull());
}

TEST(MovesTests, RotateMoveGetMoveString) {
	Moves::RotateMove move = Moves::RotateMove(Common::Vector{ 0, 7 }, Common::RotationDirection::RIGHT);
	EXPECT_EQ(move.getMoveString(), "a1+");
}

TEST(MovesTests, RotateMoveIsLegalMoveValid) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1d8/*/*/*/*/*/9L");
	Moves::RotateMove move = Moves::RotateMove(Common::Vector{ 1, 1 }, Common::RotationDirection::RIGHT);
	EXPECT_TRUE(move.isLegalMove(board, Common::PieceColor::RED));
}

void checkInvalidRotateMove(Common::Vector piecePos) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1d8/1D8/*/*/*/*/9L");
	Moves::RotateMove move = Moves::RotateMove(piecePos, Common::RotationDirection::LEFT);
	EXPECT_FALSE(move.isLegalMove(board, Common::PieceColor::RED));
}

TEST(MovesTests, RotateMoveIsLegalMoveInvalidPos) {
	checkInvalidRotateMove(Common::Vector{-1, 1});
}

TEST(MovesTests, RotateMoveIsLegalMoveNoPiece) {
	checkInvalidRotateMove(Common::Vector{ 2, 1 });
}

TEST(MovesTests, RotateMoveIsLegalMovePieceNotBelongingToPlayer) {
	checkInvalidRotateMove(Common::Vector{ 1, 2 });
}

TEST(MovesTests, RotateMoveApplyMove) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1d8/*/*/*/*/*/9L");
	Moves::RotateMove move = Moves::RotateMove(Common::Vector{ 1, 1 }, Common::RotationDirection::RIGHT);
	move.applyMove(board, &gameState);
	EXPECT_EQ(board[1][1].piece->getOrientation(), 1);
}

TEST(MovesTests, RotateMoveGetOppositeMove) {
	Moves::RotateMove move = Moves::RotateMove(Common::Vector{ 3, 3 }, Common::RotationDirection::LEFT);
	std::unique_ptr<Moves::Move> oppositeMove = move.getOppositeMove();
	Moves::RotateMove* oppositeRotateMove = dynamic_cast<Moves::RotateMove*>(oppositeMove.get());
	EXPECT_EQ(move.piecePos, oppositeRotateMove->piecePos);
	EXPECT_EQ(Common::RotationDirection::RIGHT, oppositeRotateMove->rotationDirection);
}

TEST(MovesTests, SpecialMoveGetMoveFromStringValid) {
	std::unique_ptr<Moves::Move> move;
	bool moveResult = Moves::SpecialMove::getMoveFromString(move, "a1uj8");
	EXPECT_TRUE(moveResult);
	Moves::SpecialMove* specialMove = dynamic_cast<Moves::SpecialMove*>(move.get());
	EXPECT_THAT(specialMove, testing::Field(&Moves::SpecialMove::startPos, Common::Vector{ 0, 7 }));
	EXPECT_THAT(specialMove, testing::Field(&Moves::SpecialMove::endPos, Common::Vector{ 9, 0 }));
}

TEST(MovesTests, SpecialMoveGetMoveFromStringWrongLength) {
	std::unique_ptr<Moves::Move> move;
	bool moveResult = Moves::SpecialMove::getMoveFromString(move, "a1uj8x");
	EXPECT_FALSE(moveResult);
	EXPECT_THAT(move.get(), testing::IsNull());
}

TEST(MovesTests, SpecialMoveGetMoveFromStringMissingU) {
	std::unique_ptr<Moves::Move> move;
	bool moveResult = Moves::SpecialMove::getMoveFromString(move, "a1nj8");
	EXPECT_FALSE(moveResult);
	EXPECT_THAT(move.get(), testing::IsNull());
}

TEST(MovesTests, SpecialMoveGetMoveString) {
	Moves::SpecialMove move = Moves::SpecialMove(Common::Vector{ 0, 7 }, Common::Vector{ 9, 0 });
	EXPECT_EQ(move.getMoveString(), "a1uj8");
}

TEST(MovesTests, SpecialMoveIsLegalMoveValid) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1sb7/*/*/*/*/*/9L");
	Moves::SpecialMove move = Moves::SpecialMove(Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
	EXPECT_TRUE(move.isLegalMove(board, Common::PieceColor::RED));
}

void checkInvalidSpecialMove(std::string posString, Common::Vector startPos, Common::Vector endPos) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, posString);
	Moves::SpecialMove move = Moves::SpecialMove(startPos, endPos);
	EXPECT_FALSE(move.isLegalMove(board, Common::PieceColor::RED));
}

TEST(MovesTests, SpecialMoveIsLegalMoveInvalidStartPos) {
	checkInvalidSpecialMove("l9/1sb7/*/*/*/*/*/9L", Common::Vector{ -1, 1 }, Common::Vector{ 2, 2 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveInvalidEndPos) {
	checkInvalidSpecialMove("l9/1sb7/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ -2, 2 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveTooFar) {
	checkInvalidSpecialMove("l9/1sb7/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ 2, 3 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveSameSquare) {
	checkInvalidSpecialMove("l9/1sb7/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ 1, 1 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveNoPieceAtSquare) {
	checkInvalidSpecialMove("l9/1sb7/*/*/*/*/*/9L", Common::Vector{ 1, 2 }, Common::Vector{ 2, 2 });
}

TEST(MovesTests, SpecialMoveIsLegalMovePieceNotBelongingToPlayer) {
	checkInvalidSpecialMove("l9/1Sb7/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveNotSwitch) {
	checkInvalidSpecialMove("l9/1db7/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveNoPieceAtEndSquare) {
	checkInvalidSpecialMove("l9/1s8/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveAnotherSwitch) {
	checkInvalidSpecialMove("l9/1ss7/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveNoSubKing) {
	checkInvalidSpecialMove("l9/1sk7/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveNoSubLaser) {
	checkInvalidSpecialMove("l9/1sk7/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::zeroVector);
}

TEST(MovesTests, SpecialMoveIsLegalMoveReservedEndSquare) {
	checkInvalidSpecialMove("lB8/1s8/*/*/*/*/*/9L", Common::Vector{ 1, 1 }, Common::Vector{ 0, 1 });
}

TEST(MovesTests, SpecialMoveIsLegalMoveReservedStartSquare) {
	checkInvalidSpecialMove("l9/sB8/*/*/*/*/*/9L", Common::Vector{ 1, 0 }, Common::Vector{ 1, 1 });
}

TEST(MovesTests, SpecialMoveApplyMoveSameColour) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1sb7/*/*/*/*/*/9L");
	Moves::SpecialMove move = Moves::SpecialMove(Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
	std::shared_ptr<Piece> startPiece = board[1][1].piece;
	std::shared_ptr<Piece> endPiece = board[1][2].piece;
	move.applyMove(board, &gameState);
	EXPECT_EQ(board[1][1].piece, endPiece);
	EXPECT_EQ(board[1][2].piece, startPiece);
	EXPECT_TRUE(gameState.redPieceSquares.find(board[1][1]) != gameState.redPieceSquares.end());
	EXPECT_TRUE(gameState.redPieceSquares.find(board[1][2]) != gameState.redPieceSquares.end());
}

TEST(MovesTests, SpecialMoveApplyMoveDifferentColours) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "l9/1sB7/*/*/*/*/*/9L");
	Moves::SpecialMove move = Moves::SpecialMove(Common::Vector{ 1, 1 }, Common::Vector{ 2, 1 });
	std::shared_ptr<Piece> startPiece = board[1][1].piece;
	std::shared_ptr<Piece> endPiece = board[1][2].piece;
	move.applyMove(board, &gameState);
	EXPECT_EQ(board[1][1].piece, endPiece);
	EXPECT_EQ(board[1][2].piece, startPiece);
	EXPECT_FALSE(gameState.bluePieceSquares.find(board[1][1]) == gameState.bluePieceSquares.end());
	EXPECT_TRUE(gameState.bluePieceSquares.find(board[1][2]) == gameState.bluePieceSquares.end());
	EXPECT_FALSE(gameState.redPieceSquares.find(board[1][2]) == gameState.redPieceSquares.end());
	EXPECT_TRUE(gameState.redPieceSquares.find(board[1][1]) == gameState.redPieceSquares.end());
}

TEST(MovesTests, SpecialMoveGetOppositeMove) {
	Moves::SpecialMove move = Moves::SpecialMove(Common::Vector{ 3, 3 }, Common::Vector{ 4, 4 });
	std::unique_ptr<Moves::Move> oppositeMove = move.getOppositeMove();
	Moves::SpecialMove* oppositeSpecialMove = dynamic_cast<Moves::SpecialMove*>(oppositeMove.get());
	EXPECT_EQ(move.startPos, oppositeSpecialMove->endPos);
	EXPECT_EQ(move.endPos, oppositeSpecialMove->startPos);
}