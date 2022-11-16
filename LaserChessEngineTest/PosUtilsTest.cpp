#include "pch.h"
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
		expectPieceProperties(board[0][i].piece, pieceLetters.at(i), 0, Common::PieceColor::RED);
	}
	for (size_t i = 0; i < pieceLetters.length(); i++) {
		if (pieceLetters.at(i) == 'k') {
			expectPieceProperties(board[1][i].piece, 'k', 0, Common::PieceColor::BLUE);
		}
		else {
			expectPieceProperties(board[1][i].piece, pieceLetters.at(i), 1, Common::PieceColor::BLUE);
		}
	}
	expectPieceProperties(board[1][5].piece, 's', 0, Common::PieceColor::BLUE);
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