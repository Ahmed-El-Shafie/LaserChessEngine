#include "pch.h"
#include "Defender.h"

TEST(DefenderTests, getLaserInteractionTopBlock) {
	Defender defender = Defender(Common::PieceColor::BLUE, 0);
	EXPECT_EQ(defender.getLaserInteraction(Common::downVector), std::make_pair(SideInteraction::BLOCK, Common::zeroVector));
}

TEST(DefenderTests, getLaserInteractionTopHit) {
	Defender defender = Defender(Common::PieceColor::BLUE, 1);
	EXPECT_EQ(defender.getLaserInteraction(Common::downVector), std::make_pair(SideInteraction::HIT, Common::zeroVector));
}

TEST(DefenderTests, getLaserInteractionRightBlock) {
	Defender defender = Defender(Common::PieceColor::BLUE, 1);
	EXPECT_EQ(defender.getLaserInteraction(Common::leftVector), std::make_pair(SideInteraction::BLOCK, Common::zeroVector));
}

TEST(DefenderTests, getLaserInteractionRightHit) {
	Defender defender = Defender(Common::PieceColor::BLUE, 0);
	EXPECT_EQ(defender.getLaserInteraction(Common::leftVector), std::make_pair(SideInteraction::HIT, Common::zeroVector));
}