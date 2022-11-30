#include "gmock/gmock.h"
#include "PosUtils.h"
#include "Laser.h"
#include "King.h"
#include "Switch.h"
#include "Defender.h"
#include "Deflector.h"

static const Board posValidationBoard = PosUtils::createBoard();

void posStringValidationContainsErrors(std::string posString, std::vector<std::string> expectedErrors) {
	std::vector<std::string> posStringErrors = PosUtils::validatePosString(posValidationBoard, posString);
	EXPECT_THAT(posStringErrors, testing::UnorderedElementsAreArray(expectedErrors));
}
TEST(PosUtilsTests, validatePosStringValid) {
	posStringValidationContainsErrors("lK8/*/*/*/*/*/*/8kL", {});
}

TEST(PosUtilsTests, validatePosStringMissingKings) {
	posStringValidationContainsErrors("l9/*/*/*/*/*/*/9L", { "Impossible for both sides' kings to be missing" });
}

TEST(PosUtilsTests, validatePosStringNotEnoughRows) {
	posStringValidationContainsErrors("lK8/*/*/*/*/*/8kL", { "Not enough rows", "Row 7: Blue laser can only be on bottom-right corner" });
}

TEST(PosUtilsTests, validatePosStringTooManyRows) {
	posStringValidationContainsErrors("lK8/*/*/*/*/*/*/8kL/*", { "Too many rows" });
}

TEST(PosUtilsTests, validatePosStringRowStringCantBeEmpty) {
	posStringValidationContainsErrors("lK8//*/*/*/*/*/8kL", { "Row 2: Not enough items in this row" });
}

TEST(PosUtilsTests, validatePosStringMissingRedLaser) {
	posStringValidationContainsErrors("2K7/*/*/*/*/*/*/8kL", { "Row 1: Missing red laser on top-left corner" });
}

TEST(PosUtilsTests, validatePosStringInvalidPieceChar) {
	posStringValidationContainsErrors("lKg7/*/*/*/*/*/*/8kL", { "Row 1: Invalid piece character g" });
}

TEST(PosUtilsTests, validatePosStringOutOfPlaceRedLaser) {
	posStringValidationContainsErrors("lKl7/*/*/*/*/*/*/8kL", { "Row 1: Red laser can only be on top-left corner" });
}

TEST(PosUtilsTests, validatePosStringOutOfPlaceBlueLaser) {
	posStringValidationContainsErrors("lK8/*/*/*/*/*/*/7LkL", { "Row 8: Blue laser can only be on bottom-right corner" });
}

TEST(PosUtilsTests, validatePosStringReservedSquares) {
	posStringValidationContainsErrors("ldK5Db/*/*/*/*/*/*/8kL", {
		"Row 1: Column 2 is reserved for blue pieces only",
		"Row 1: Column 9 is reserved for red pieces only",
		"Row 1: Column 10 is reserved for blue pieces only"});
}

TEST(PosUtilsTests, validatePosStringMisplacedPlus) {
	posStringValidationContainsErrors("lK8+/*/*/*/*/*/*/8kL", { "Row 1: '+' can only come after a piece character"});
}

TEST(PosUtilsTests, validatePosStringTooManyPluses) {
	posStringValidationContainsErrors("lK++++8/*/*/*/*/*/*/8kL", { "Row 1: There can only be at most 3 consecutive '+' characters"});
}

TEST(PosUtilsTests, validatePosStringZeroDigit) {
	posStringValidationContainsErrors("l0K8/*/*/*/*/*/*/8kL", { "Row 1: Digit can't be 0" });
}

TEST(PosUtilsTests, validatePosStringNoConsecutiveDigits) {
	posStringValidationContainsErrors("lK26/*/*/*/*/*/*/8kL", { "Row 1: Can't have 2 consecutive digits" });
}

TEST(PosUtilsTests, validatePosStringInvalidChar) {
	posStringValidationContainsErrors("lK&8/*/*/*/*/*/*/8kL", { "Row 1: Invalid character &" });
}

TEST(PosUtilsTests, validatePosStringTooManyRowItems) {
	posStringValidationContainsErrors("lK9/dddddddddDd/*/*/*/*/*/8kL", { "Row 1: Too many items in this row", "Row 2: Too many items in this row" });
}

TEST(PosUtilsTests, validatePosStringNotEnoughRowItems) {
	posStringValidationContainsErrors("lK7/ddddddddd/*/*/*/*/*/8kL", { "Row 1: Not enough items in this row", "Row 2: Not enough items in this row" });
}

TEST(PosUtilsTests, validatePosStringMissingBlueLaser) {
	posStringValidationContainsErrors("lK8/*/*/*/*/*/*/8k1", { "Row 8: Missing blue laser on bottom-right corner" });
}

TEST(PosUtilsTests, validatePosStringEmpty) {
	std::vector<std::string> posStringErrors = PosUtils::validatePosString(posValidationBoard, "");
	EXPECT_THAT(posStringErrors, testing::Not(testing::IsEmpty()));
}

TEST(PosUtilsTests, validatePosStringNoSlashes) {
	std::vector<std::string> posStringErrors = PosUtils::validatePosString(posValidationBoard, "lK8******8kL");
	EXPECT_THAT(posStringErrors, testing::Not(testing::IsEmpty()));
}


void expectPieceProperties(Piece* piece, char pieceChar, int orientation, Common::PieceColor color) {
	switch (pieceChar) {
	case 'l':
		EXPECT_THAT(dynamic_cast<Laser*>(piece), testing::NotNull());
		break;
	case 'k':
		EXPECT_THAT(dynamic_cast<King*>(piece), testing::NotNull());
		break;
	case 'd':
		EXPECT_THAT(dynamic_cast<Defender*>(piece), testing::NotNull());
		break;
	case 'b':
		EXPECT_THAT(dynamic_cast<Deflector*>(piece), testing::NotNull());
		break;
	case 's':
		EXPECT_THAT(dynamic_cast<Switch*>(piece), testing::NotNull());
		break;
	}
	if (piece != nullptr) {
		EXPECT_THAT(piece, testing::Property(&Piece::getColor, color));
		EXPECT_THAT(piece, testing::Property(&Piece::getOrientation, orientation));
	}
}

TEST(PosUtilsTests, setupPositionAllPieces) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "lkbds/L+K+B+D+S+S++/*/*/*/*/*/*");
	std::string pieceLetters = "lkbds";
	for (size_t i = 0; i < pieceLetters.length(); i++) {
		expectPieceProperties(board[0][i].piece.get(), pieceLetters.at(i), 0, Common::PieceColor::RED);
	}
	for (size_t i = 0; i < pieceLetters.length(); i++) {
		if (pieceLetters.at(i) == 'k') {
			expectPieceProperties(board[1][i].piece.get(), 'k', 0, Common::PieceColor::BLUE);
		}
		else {
			expectPieceProperties(board[1][i].piece.get(), pieceLetters.at(i), 1, Common::PieceColor::BLUE);
		}
	}
	expectPieceProperties(board[1][5].piece.get(), 's', 0, Common::PieceColor::BLUE);
	EXPECT_THAT(gameState.winner, Common::PieceColor::NONE);
}

TEST(PosUtilsTests, setupPositionBlueWinner) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "K9/*/*/*/*/*/*/*");
	EXPECT_THAT(gameState.winner, Common::PieceColor::BLUE);
}

TEST(PosUtilsTests, setupPositionRedWinner) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, "k9/*/*/*/*/*/*/*");
	EXPECT_THAT(gameState.winner, Common::PieceColor::RED);
}

TEST(PosUtilsTests, getPositionStringFromBoard) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	std::string startPosString = "*/k9/d8b++/SSSSSSSSSS/*/*/*/*";
	PosUtils::setupPosition(&gameState, board, startPosString);
	std::string resultPosString = PosUtils::getPositionStringFromBoard(board);
	EXPECT_THAT(resultPosString, startPosString);
}

TEST(PosUtilsTests, evaluatePositionBlueWins) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::BLUE, Common::PieceColor::NONE);
	EXPECT_EQ(PosUtils::evaluatePosition(board, gameState), std::numeric_limits<float>::max());
}

TEST(PosUtilsTests, evaluatePositionRedWins) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState(Common::PieceColor::RED, Common::PieceColor::NONE);
	EXPECT_EQ(PosUtils::evaluatePosition(board, gameState), std::numeric_limits<float>::lowest());
}

/*
* Sanity tests. Positional evaluations are complicated and there's no "correct" answer. These tests are based mainly
* on my understanding of Laser Chess positions and intuition.
* Also, a lot of these positions are ones that wouldn't appear in an actual game (missing switches), but they
* still serve to guide the evaluation function's implementation
*/

float setupAndEvaluate(std::string posString) {
	Board board = PosUtils::createBoard();
	GameState gameState = GameState();
	PosUtils::setupPosition(&gameState, board, posString);
	return PosUtils::evaluatePosition(board, gameState);
}
TEST(PosUtilsTests, evaluatePositionBlueMaterialAdvantage) {
	float eval1 = setupAndEvaluate("l9/1k8/*/5s4/9B/4S5/8K1/9L+");  // Extra blue deflector only
	EXPECT_THAT(eval1, testing::Gt(0));
	float eval2 = setupAndEvaluate("l9/1k8/*/5s4/8SB/4S5/8K1/9L+"); // Extra blue deflector and switch
	EXPECT_THAT(eval2, testing::Gt(eval1));
	float eval3 = setupAndEvaluate("l9/1k8/*/5s4/8SB/4S3D1/8K1/9L+"); // Extra blue deflector, switch, and defender
	EXPECT_THAT(eval3, testing::Gt(eval2));
}

TEST(PosUtilsTests, evaluatePositionRedKingExposed) {
	float eval1 = setupAndEvaluate("l9/1s8/*/5k4/*/4S5/8K1/9L+"); // Red king is in the middle of the board with nothing to protect it
	EXPECT_THAT(eval1, testing::Gt(0));
	float eval2 = setupAndEvaluate("l9/1k8/3s6/*/*/4S5/8K1/9L+"); // Red king is in the corner and switch is nearby to defend if necessary
	EXPECT_THAT(eval2, testing::Lt(eval1));
}

TEST(PosUtilsTests, evaluatePositionLaserUsage) {
	// Blue switches are closer to red king than vice versa
	float eval1 = setupAndEvaluate("l1k7/2ss1/7S+2/7S+2/*/*/8K1/9L+");
	EXPECT_THAT(eval1, testing::Gt(0));
	// Blue switchers are closer to red king than vice cersa, but one of them is in front of blue laser
	float eval2 = setupAndEvaluate("l1k7/2ss1/7S+2/9S+2/*/*/8K1/9L+");
	EXPECT_THAT(eval2, testing::Gt(eval1));
	// One of the switches is closer to red king than before
	float eval3 = setupAndEvaluate("l1k7/2ss1/4S+5/9S+/*/*/8K1/9L+");
	EXPECT_THAT(eval3, testing::Gt(eval2));
	// Having one piece in front of the laser while the other is on the same column as the king is even better
	float eval4 = setupAndEvaluate("l1k7/2ss1/2S+7/9S+/*/*/8K1/9L+");
	EXPECT_THAT(eval4, testing::Gt(eval3));
	// Having one piece in front of the laser and the other on the same column as the king AND the same row as the other deflecting piece is better
	float eval5 = setupAndEvaluate("l1k7/2ss1/2S+6S+/*/*/*/8K1/9L+");
	EXPECT_THAT(eval5, testing::Gt(eval4));
	// Bonus points if the switch in front of the laser would direct the laser into the battlefield instead of outside
	float eval6 = setupAndEvaluate("l1k7/2ss1/2S6S/*/*/*/8K1/9L+");
	EXPECT_THAT(eval6, testing::Gt(eval5));
}

TEST(PosUtilsTests, evaluatePositionPieceDistances) {
	// Blue's defenders too far from the blue king while red's defenders are close
	float eval1 = setupAndEvaluate("l1k7/1dd7/*/*/4D5/4D5/8K1/9L+");
	EXPECT_THAT(eval1, testing::Lt(0));
	// Blue's deflectors are closer to red's king than red's deflectors are to blue king
	float eval2 = setupAndEvaluate("l1k7/1bb7/*/*/4B5/4B5/8K1/9L+");
	EXPECT_THAT(eval2, testing::Gt(0));
	// Blue's switches are closer to red's king than red's switches are to blue king, and switches are stronger than deflectors
	float eval3 = setupAndEvaluate("l1k7/1ss7/*/*/4S5/4S5/8K1/9L+");
	EXPECT_THAT(eval3, testing::Gt(eval2));
}

TEST(PosUtilsTests, evaluatePositionDefenderRotationBonus) {
	// Blue has 2 defenders near king
	float eval1 = setupAndEvaluate("l1k7/*/*/*/*/*/7DKD1/9L+");
	// Blue's 2 defenders are facing away from king to block possible incoming lasers
	float eval2 = setupAndEvaluate("l1k7/*/*/*/*/*/7D+++KD+1/9L+");
	EXPECT_THAT(eval2, testing::Gt(eval1));
}
TEST(PosUtilsTests, evaluatePositionStartPosition) {
	float eval1 = setupAndEvaluate("l+3d++kd++b+2/2b++7/3B+++6/b1B++1ss+1b+1B+++/b+1B+++1S+S1b1B++/6b+3/7B2/2B+++DKD3L+");
	EXPECT_THAT(abs(eval1), testing::Le(0.5));
}