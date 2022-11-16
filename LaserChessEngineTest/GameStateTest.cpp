#include "pch.h"
#include "GameState.h"
#include "PosUtils.h"
#include "Moves.h"

TEST(GameStateTests, removeHitPieceKing) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::NONE, Common::PieceColor::BLUE);
	PosUtils::setupPosition(&gameState, board, "l+K8/*/*/*/*/*/*/8kL");
	EXPECT_THAT(board[7][8].piece, testing::NotNull());
	EXPECT_FALSE(gameState.redPieceSquares.find(board[7][8]) == gameState.redPieceSquares.end());
	EXPECT_EQ(gameState.winner, Common::PieceColor::NONE);
	Piece* hitPiece = gameState.removeHitPiece(&board[7][8]);
	EXPECT_THAT(board[7][8].piece, testing::IsNull());
	EXPECT_TRUE(gameState.redPieceSquares.find(board[7][8]) == gameState.redPieceSquares.end());
	EXPECT_EQ(gameState.winner, Common::PieceColor::BLUE);
}