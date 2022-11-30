#include "gtest/gtest.h"
#include "Deflector.h"

TEST(DeflectorTests, getLaserInteractionTopAndRightDeflect) {
	Deflector deflector = Deflector(Common::PieceColor::BLUE, 0);
	EXPECT_EQ(deflector.getLaserInteraction(Common::downVector), std::make_pair(SideInteraction::DEFLECT, Common::rightVector));
	EXPECT_EQ(deflector.getLaserInteraction(Common::leftVector), std::make_pair(SideInteraction::DEFLECT, Common::upVector));
}

TEST(DeflectorTests, getLaserInteractionTopHit) {
	Deflector deflector = Deflector(Common::PieceColor::BLUE, 1);
	EXPECT_EQ(deflector.getLaserInteraction(Common::downVector), std::make_pair(SideInteraction::HIT, Common::zeroVector));
}

TEST(DeflectorTests, getLaserInteractionBottomAndRightDeflect) {
	Deflector deflector = Deflector(Common::PieceColor::BLUE, 1);
	EXPECT_EQ(deflector.getLaserInteraction(Common::leftVector), std::make_pair(SideInteraction::DEFLECT, Common::downVector));
	EXPECT_EQ(deflector.getLaserInteraction(Common::upVector), std::make_pair(SideInteraction::DEFLECT, Common::rightVector));
}

TEST(DeflectorTests, getLaserInteractionRightHit) {
	Deflector deflector = Deflector(Common::PieceColor::BLUE, 2);
	EXPECT_EQ(deflector.getLaserInteraction(Common::leftVector), std::make_pair(SideInteraction::HIT, Common::zeroVector));
}