#include "gtest/gtest.h"
#include "Switch.h"

TEST(SwitchTests, getLaserInteractionTopAndRightDeflect) {
	Switch switchPiece = Switch(Common::PieceColor::BLUE, 0);
	EXPECT_EQ(switchPiece.getLaserInteraction(Common::downVector), std::make_pair(SideInteraction::DEFLECT, Common::rightVector));
	EXPECT_EQ(switchPiece.getLaserInteraction(Common::leftVector), std::make_pair(SideInteraction::DEFLECT, Common::upVector));
}

TEST(SwitchTests, getLaserInteractionTopAndLeftDeflect) {
	Switch switchPiece = Switch(Common::PieceColor::BLUE, 1);
	EXPECT_EQ(switchPiece.getLaserInteraction(Common::leftVector), std::make_pair(SideInteraction::DEFLECT, Common::downVector));
	EXPECT_EQ(switchPiece.getLaserInteraction(Common::upVector), std::make_pair(SideInteraction::DEFLECT, Common::rightVector));
}