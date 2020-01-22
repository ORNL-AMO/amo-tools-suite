#include "ssmt/service/high_pressure_header/HighPressureCondensateCalculator.h"

const SteamSystemModelerTool::FluidProperties
HighPressureCondensateCalculator::calc(const HeaderWithHighestPressure &highPressureHeaderInput,
                                       const Boiler &boiler) const {
    const std::string methodName = std::string("HighPressureCondensateCalculator::") + std::string(__func__) + ": ";

    //has same properties as blowdown with updated mass and energy flows
    std::cout << methodName << "calculating highPressureCondensate" << std::endl;
    double massFlow = massFlowCalculator.calc(highPressureHeaderInput);
    double energyFlow = energyFlowCalculator.calc(massFlow, boiler);
    std::cout << methodName << "massFlow=" << massFlow << ", energyFlow=" <<energyFlow << std::endl;

    return fluidPropertiesFactory.makeFromBlowdown(boiler, massFlow, energyFlow);
}
