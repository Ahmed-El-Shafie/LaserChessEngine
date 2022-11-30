#include "gmock/gmock.h"
#include "MoveUtils.h"
#include "PosUtils.h"

TEST(MoveUtilsTests, laserHitBlock) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "*/*/*/*/*/*/*/5D+3L");
	Square* hitSquare = MoveUtils::laserHit(board, Common::PieceColor::BLUE);
	EXPECT_THAT(hitSquare, testing::IsNull());
}

TEST(MoveUtilsTests, laserHitAttack) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "*/*/*/*/*/*/*/5D3L");
	Square* hitSquare = MoveUtils::laserHit(board, Common::PieceColor::BLUE);
	EXPECT_THAT(hitSquare, testing::Field(&Square::pos, Common::Vector{ 5, 7 }));
}

TEST(MoveUtilsTests, laserHitDeflect) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "*/*/*/*/*/*/*/5B3L");
	Square* hitSquare = MoveUtils::laserHit(board, Common::PieceColor::BLUE);
	EXPECT_THAT(hitSquare, testing::IsNull());
}

static const std::array<Common::Vector, 8> possibleMoveVectors = {
	Common::Vector{-1, -1}, Common::upVector, Common::Vector{1, -1},
	Common::leftVector, Common::rightVector,
	Common::Vector{-1, 1}, Common::downVector, Common::Vector{1, 1}
};

TEST(MoveUtilsTests, getLegalMoves) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "*/*/*/4Sd4/*/*/*/*");
	Common::Vector piecePos = { 4, 3 };
	std::vector<std::shared_ptr<Moves::Move>> moves = MoveUtils::getLegalMoves(board, board[piecePos.y][piecePos.x]);
	std::vector<std::shared_ptr<Moves::Move>> expectedMoves;
	for (Common::Vector moveVector : possibleMoveVectors) {
		if (!(moveVector == Common::rightVector)) {
			std::shared_ptr<Moves::Move> shiftMove = std::make_shared<Moves::ShiftMove>(piecePos, piecePos + moveVector);
			expectedMoves.push_back(shiftMove);
		}
		else {
			std::shared_ptr<Moves::Move> specialMove = std::make_shared<Moves::SpecialMove>(piecePos, piecePos + Common::rightVector);
			expectedMoves.push_back(specialMove);
		}
	}
	expectedMoves.push_back(std::make_shared<Moves::RotateMove>(piecePos, Common::RotationDirection::LEFT));

	ASSERT_EQ(moves.size(), expectedMoves.size());
	for (size_t i = 0; i < expectedMoves.size(); i++) {
		EXPECT_EQ(*(moves.at(i)), *(expectedMoves.at(i)));
	}
}

TEST(MoveUtilsTests, findBestMoveZeroDepth) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "lK8/*/*/*/*/*/*/8kL");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 0);
	EXPECT_THAT(result.move, testing::IsNull());
}

TEST(MoveUtilsTests, findBestMoveGameAlreadyOver) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "lK8/*/*/*/*/*/*/8L");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 1);
	EXPECT_THAT(result.move, testing::IsNull());
}

TEST(MoveUtilsTests, findBestMoveMateInOne) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::NONE, Common::PieceColor::BLUE);
	PosUtils::setupPosition(&gameState, board, "l9/1K8/*/*/*/*/7D2/7DkL+");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 1);
	EXPECT_EQ(result.move->getMoveString(), "j1-");
	EXPECT_EQ(result.evaluation, std::numeric_limits<float>::max());
}

TEST(MoveUtilsTests, findBestMoveAvoidMateInOne) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::NONE, Common::PieceColor::RED);
	PosUtils::setupPosition(&gameState, board, "l9/1K8/*/*/*/*/7D2/7DkL+");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 1);
	EXPECT_EQ(result.move->getMoveString(), "i1i2");
	EXPECT_NE(result.evaluation, std::numeric_limits<float>::max());
}

TEST(MoveUtilsTests, findBestMoveUnavoidableMateInOne) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::NONE, Common::PieceColor::RED);
	PosUtils::setupPosition(&gameState, board, "l9/1K8/*/*/*/*/7DD1/7DkL+");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 1);
	EXPECT_THAT(result.move, testing::NotNull()); // Should recommend move even if it's hopeless against best play
	EXPECT_EQ(result.evaluation, std::numeric_limits<float>::max());
}

TEST(MoveUtilsTests, findBestMoveMateInTwo) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::NONE, Common::PieceColor::BLUE);
	PosUtils::setupPosition(&gameState, board, "l9/1K8/*/*/*/*/4DDDD2/5kD2L+");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 2);
	EXPECT_EQ(result.move->getMoveString(), "j1-");
	EXPECT_EQ(result.evaluation, std::numeric_limits<float>::max());
}

TEST(MoveUtilsTests, findBestMoveAvoidMateInTwo) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::NONE, Common::PieceColor::RED);
	PosUtils::setupPosition(&gameState, board, "l9/1K8/*/*/*/*/4DDDD2/5kD2L+");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 2);
	EXPECT_EQ(result.move->getMoveString(), "f1e1");
	EXPECT_NE(result.evaluation, std::numeric_limits<float>::max());
}

TEST(MoveUtilsTests, findBestMoveMateInThree) { // Also tests performance
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::NONE, Common::PieceColor::BLUE);
	PosUtils::setupPosition(&gameState, board, "l+9/4k5/8D1/3s5B++/*/*/*/6K2L+");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 3);
	EXPECT_EQ(result.move->getMoveString(), "j5j6");
	EXPECT_EQ(result.evaluation, std::numeric_limits<float>::max());
}

TEST(MoveUtilsTests, findBestMoveAvoidMateInThree) { // Also tests performance
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::NONE, Common::PieceColor::RED);
	PosUtils::setupPosition(&gameState, board, "l+9/4k5/8D1/3s5B++/*/*/*/6K2L+");
	MoveUtils::EvaluationResult result = MoveUtils::findBestMove(board, gameState, 3);
	EXPECT_EQ(result.move->getMoveString(), "d5e6");
	EXPECT_NE(result.evaluation, std::numeric_limits<float>::max());
}