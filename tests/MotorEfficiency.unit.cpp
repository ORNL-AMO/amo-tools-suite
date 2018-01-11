#include "catch.hpp"
#include <calculator/motor/MotorEfficiency.h>
#include <calculator/motor/MotorEfficiency25.h>

TEST_CASE( "Calculate Motor Efficiency - 200 HP", "[MotorEfficiency]" ) {
    MotorEfficiency motorEff(Motor::LineFrequency::FREQ60, 1785, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 200, 0);

    {
        motorEff.setLoadFactor( 0.0 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.0 ) );
    }

    {
        motorEff.setLoadFactor( 0.25 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.930393384 ) );
    }

    {
        motorEff.setLoadFactor( 0.5 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.949504131 ) );
    }

    {
        motorEff.setLoadFactor( 0.75 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.9569034 ) );
    }

    {
        motorEff.setLoadFactor( 1.0 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.956259245 ) );
    }

    {
        motorEff.setLoadFactor( 1.25 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.946696653 ) );
    }

}

TEST_CASE( "Calculate Motor Efficiency - 100 HP", "[MotorEfficiency]" ) {
    MotorEfficiency motorEff(Motor::LineFrequency::FREQ60, 1785, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 100, 0);

    {
        motorEff.setLoadFactor( 0.0 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.0 ) );
    }

    {
        motorEff.setLoadFactor( 0.25 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.921519665 ) );
    }

    {
        motorEff.setLoadFactor( 0.5 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.946047805 ) );
    }

    {
        motorEff.setLoadFactor( 0.75 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.951033931 ) );
    }

    {
        motorEff.setLoadFactor( 1.0 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.949413679 ) );
    }

    {
        motorEff.setLoadFactor( 1.25 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.939919542 ) );
    }

}

TEST_CASE( "Calculate Motor Efficiencies", "[MotorEfficiency]" ) {
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1785, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 100, 0.5).calculate() == Approx(0.9460478053));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1985, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 100, 0.5).calculate() == Approx(0.936657639));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 2285, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 110, 0.5).calculate() == Approx(0.9382004043));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 2585, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 120, 0.5).calculate() == Approx(0.9395868071));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 2885, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 150, 0.5).calculate() == Approx( 0.9404486692));

    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1285, Motor::EfficiencyClass::STANDARD, 0, 100, 0.5).calculate() == Approx(0.9205681925));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::STANDARD, 0, 120, 0.5).calculate() == Approx(0.9213249265));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::STANDARD, 0, 150, 0.1).calculate() == Approx(0.780716097));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::STANDARD, 0, 150, 0.9).calculate() == Approx(0.9350877877));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::STANDARD, 0, 150, 0.2).calculate() == Approx(0.8671487126));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::STANDARD, 0, 150, 0.25).calculate() == Approx(0.8867837736));

    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 25, 150, 0.25).calculate() == Approx(0.2427959928));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 35, 150, 0.25).calculate() == Approx(0.3399143899));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 55, 150, 0.25).calculate() == Approx(0.5341511841));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 75, 150, 0.25).calculate() == Approx(0.7283879783));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 85, 150, 0.25).calculate() == Approx(0.8255063754));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 95, 150, 0.25).calculate() == Approx(0.9226247725));

    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 25, 150, 0.05).calculate() == Approx(0.0709296658));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 35, 150, 0.15).calculate() == Approx(0.25140747859));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 55, 150, 0.35).calculate() == Approx(0.5403781513));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 85, 150, 0.55).calculate() == Approx(0.84636808254));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 95, 150, 0.75).calculate() == Approx(0.95082302895));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 95, 150, 1.0).calculate() == Approx(0.95));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 95, 150, 1.25).calculate() == Approx(0.9405));
    CHECK(MotorEfficiency(Motor::LineFrequency::FREQ60, 1600, Motor::EfficiencyClass::SPECIFIED, 95, 150, 1.5).calculate() == Approx(0.9223230289));
}

TEST_CASE( "Calculate Motor25 Efficiencies", "[MotorEfficiency25]" ) {
    const std::vector< std::vector<double> > expected {
            // ENERGY_EFFICIENT
            {0.9215196653, 0.9460478053, 0.9510339314, 0.9494136792, 0.9399195424},
            {0.9241840076, 0.9475057386, 0.9529291531, 0.9518554564, 0.9423369019},
            {0.9353383999, 0.9516372151, 0.9584322705, 0.9578040944, 0.9482260535},
            {0.9045636369, 0.9366576390, 0.9445673713, 0.9436768567, 0.9342400881},
            {0.9045636369, 0.9366576390, 0.9445673713, 0.9436768567, 0.9342400881},

            // STANDARD
            {0.8509115247, 0.9036358682, 0.9142885586, 0.9138935683, 0.9047546326},
            {0.8731490357, 0.9184557213, 0.9281311155, 0.9293990866, 0.9201050957},
            {0.8867837736, 0.9242760124, 0.9338556716, 0.9347855116, 0.9254376565},
            {0.9071879002, 0.9326659464, 0.9416871700, 0.9424751293, 0.933050378},
            {0.8476517473, 0.9070133723, 0.9196222911, 0.9189355966, 0.9097462406},
    };

    std::size_t unitTestNumber = 0;
	auto const compare = [&unitTestNumber, &expected](const std::array<double, 5> & results) {
        INFO("Unit test Number " + std::to_string(unitTestNumber));
        CHECK(expected.at(unitTestNumber).at(0) == Approx(results.at(0)));
        CHECK(expected.at(unitTestNumber).at(1) == Approx(results.at(1)));
        CHECK(expected.at(unitTestNumber).at(2) == Approx(results.at(2)));
        CHECK(expected.at(unitTestNumber).at(3) == Approx(results.at(3)));
        CHECK(expected.at(unitTestNumber).at(4) == Approx(results.at(4)));
        unitTestNumber++;
    };

	compare(MotorEfficiency25(Motor::EfficiencyClass::ENERGY_EFFICIENT, 100, 1780, Motor::LineFrequency::FREQ60).calculate());
    compare(MotorEfficiency25(Motor::EfficiencyClass::ENERGY_EFFICIENT, 120, 1780, Motor::LineFrequency::FREQ60).calculate());
    compare(MotorEfficiency25(Motor::EfficiencyClass::ENERGY_EFFICIENT, 290, 1780, Motor::LineFrequency::FREQ60).calculate());
    compare(MotorEfficiency25(Motor::EfficiencyClass::ENERGY_EFFICIENT, 100, 2400, Motor::LineFrequency::FREQ60).calculate());
    compare(MotorEfficiency25(Motor::EfficiencyClass::ENERGY_EFFICIENT, 100, 3880, Motor::LineFrequency::FREQ60).calculate());

    compare(MotorEfficiency25(Motor::EfficiencyClass::STANDARD, 50, 1780, Motor::LineFrequency::FREQ60).calculate());
    compare(MotorEfficiency25(Motor::EfficiencyClass::STANDARD, 100, 1780, Motor::LineFrequency::FREQ60).calculate());
    compare(MotorEfficiency25(Motor::EfficiencyClass::STANDARD, 150, 1780, Motor::LineFrequency::FREQ60).calculate());
    compare(MotorEfficiency25(Motor::EfficiencyClass::STANDARD, 250, 1780, Motor::LineFrequency::FREQ60).calculate());
    compare(MotorEfficiency25(Motor::EfficiencyClass::STANDARD, 125, 3200, Motor::LineFrequency::FREQ60).calculate());
}

TEST_CASE( "MotorEfficiency25 Premium", "[MotorEfficiency][PREMIUM]" ) {
    MotorEfficiency25 me25(Motor::EfficiencyClass::PREMIUM, 200, 1780, Motor::LineFrequency::FREQ60);
    auto const results = me25.calculate();
    CHECK(results[0] == Approx(0.9199840391));
    CHECK(results[1] == Approx(0.9514158668));
    CHECK(results[2] == Approx(0.9610411879));
    CHECK(results[3] == Approx(0.962));
    CHECK(results[4] == Approx(0.9298570145));
}
