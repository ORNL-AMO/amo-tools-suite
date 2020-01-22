#include <ssmt/service/high_pressure_header/HighPressureHeaderCalculator.h>
#include <ssmt/Header.h>

SteamSystemModelerTool::FluidProperties
HighPressureHeaderCalculator::calc(const HeaderWithHighestPressure &highPressureHeaderInput, const Boiler &boiler) const {
    const std::string methodName = std::string("HighPressureHeaderCalculator::") + std::string(__func__) + ": ";

    const double headerPressure = highPressureHeaderInput.getPressure();
    Header highPressureHeader = headerFactory.make(headerPressure, boiler);
    std::cout << methodName << "highPressureHeader=" << highPressureHeader << std::endl;

    return fluidPropertiesFactory.make(highPressureHeader);
}
