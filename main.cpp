#include <iostream>
#include <cmath>
#include <emscripten/bind.h>
#include "calculator/motor/MotorPower.h"
#include "calculator/pump/HeadTool.h"

int main() {
    std::cout << "Run 'doxygen Doxyfile.in' from the root project directory to generate documentation." << std::endl;
	std::cout << "View the documentation by opening docs/html/index.html in a web browser." << std::endl;
	return 0;
}

double motor_power_calculate(double ratedVoltage, double motorCurrent, double powerFactor) {
    return ratedVoltage * motorCurrent * sqrt(3) * powerFactor / 1000;
}

double head_tool_calculate(
	const double specificGravity,
	const double flowRate,
	const double suctionPipeDiameter,
	const double suctionTankGasOverPressure,
	const double suctionTankFluidSurfaceElevation,
	const double suctionLineLossCoefficients,
	const double dischargePipeDiameter,
	const double dischargeGaugePressure,
	const double dischargeGaugeElevation,
	const double dischargeLineLossCoefficients)
{
	auto output = HeadToolSuctionTank(
		specificGravity,
		flowRate,
		suctionPipeDiameter,
		suctionTankGasOverPressure,
		suctionTankFluidSurfaceElevation,
		suctionLineLossCoefficients,
		dischargePipeDiameter,
		dischargeGaugePressure,
		dischargeGaugeElevation,
		dischargeLineLossCoefficients
		).calculate();

	return output["elevationHead"];
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("motor_power_calculate", &motor_power_calculate);
    emscripten::function("head_tool_calculate", &head_tool_calculate);

}
