#include "ssmt/domain/HeatLossFactory.h"

const HeatLoss HeatLossFactory::make(const HeaderWithHighestPressure &highestPressureHeaderInput,
                                     const SteamSystemModelerTool::FluidProperties &headerOutput) const {
    double percentHeatLoss = highestPressureHeaderInput.getHeatLoss();
    return make(headerOutput, percentHeatLoss);
}

const HeatLoss HeatLossFactory::make(const std::shared_ptr<HeaderNotHighestPressure> &notHighestPressureHeaderInput,
                                     const SteamSystemModelerTool::FluidProperties &headerOutput) const {
    double percentHeatLoss = notHighestPressureHeaderInput->getHeatLoss();
    return make(headerOutput, percentHeatLoss);
}

const HeatLoss
HeatLossFactory::make(const SteamSystemModelerTool::FluidProperties &headerOutput, const double percentHeatLoss) const {
    const std::string methodName = "HeatLossFactory::" + std::string(__func__) + ": ";

    double inletPressure = headerOutput.pressure;
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = headerOutput.specificEnthalpy;
    double inletMassFlow = headerOutput.massFlow;

    HeatLoss heatLoss = {inletPressure, quantityType, quantityValue, inletMassFlow, percentHeatLoss};

    //std::cout << methodName << "heatLoss=" << heatLoss << std::endl;

    return heatLoss;
}
