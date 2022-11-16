#include "pch.h"
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