#include "catch.hpp"
#include <sqlite/SQLite.h>
#include <calculator/motor/EstimateFLA.h>


TEST_CASE( "Estimate FLA", "[EstimateFLA]" ) {
 CHECK(EstimateFLA(200, 1780, Motor::LineFrequency::FREQ60, Motor::EfficiencyClass::SPECIFIED, 94, 460).calculate() == Approx(227.3));
 CHECK(EstimateFLA(200, 1780, Motor::LineFrequency::FREQ60, Motor::EfficiencyClass::SPECIFIED, 95, 460).calculate() == Approx(228.4));
}