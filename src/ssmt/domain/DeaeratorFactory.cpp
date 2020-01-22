#include "ssmt/domain/DeaeratorFactory.h"

const Deaerator DeaeratorFactory::make(const BoilerInput &boilerInput, const double feedwaterMassFlow,
                                       const SteamSystemModelerTool::FluidProperties &makeupWaterAndCondensateHeaderOutput,
                                       const SteamSystemModelerTool::FluidProperties &inletHeaderOutput) const {
    const std::string methodName = std::string("DeaeratorFactory::") + std::string(__func__) + ": ";

    double deaeratorPressure = boilerInput.getDeaeratorPressure();
    double ventRate = boilerInput.getDeaeratorVentRate();
    double waterPressure = makeupWaterAndCondensateHeaderOutput.pressure;
    SteamProperties::ThermodynamicQuantity waterQuantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double waterQuantityValue = makeupWaterAndCondensateHeaderOutput.specificEnthalpy;
    double steamPressure = inletHeaderOutput.pressure;
    SteamProperties::ThermodynamicQuantity steamQuantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double steamQuantityValue = inletHeaderOutput.specificEnthalpy;

    std::cout << methodName << "deaerator inputs: "
              << "deaeratorPressure=" << deaeratorPressure
              << ", ventRate=" << ventRate
              << ", waterPressure=" << waterPressure
              //    << ", waterQuantityType=" << waterQuantityType
              << ", waterQuantityValue=" << waterQuantityValue
              << ", steamPressure=" << steamPressure
              //    << ", steamQuantityType=" << steamQuantityType
              << ", steamQuantityValue=" << steamQuantityValue
              << std::endl;


    const Deaerator &deaerator =
            {deaeratorPressure, ventRate, feedwaterMassFlow, waterPressure, waterQuantityType, waterQuantityValue,
             steamPressure, steamQuantityType, steamQuantityValue};

    std::cout << methodName << "deaerator=" << deaerator << std::endl;

    return deaerator;
}
