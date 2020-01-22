#include <ssmt/domain/FlashTankFactory.h>

std::shared_ptr<FlashTank>
FlashTankFactory::make(const HeaderInput &headerInput, const BoilerInput &boilerInput, const Boiler &boiler) const {
    const std::string methodName = "FlashTankFactory::" + std::string(__func__) + ": ";

    std::shared_ptr<FlashTank> flashTankPtr = nullptr;

    if (boilerInput.isBlowdownFlashed()) {
        const double pressure = headerInput.getPressureFromLowestPressureHeader();

        const FlashTank &flashTank = make(pressure, boiler);
        flashTankPtr = std::make_shared<FlashTank>(flashTank);
    } else {
        std::cout << methodName << "boilerInput.isBlowdownFlashed() is false, skipping flash tank creation" << std::endl;
    }

    return flashTankPtr;
}

FlashTank FlashTankFactory::make(const double pressure, const Boiler &boiler) const {
    //flash the blowdown from the boiler
    const SteamSystemModelerTool::FluidProperties &blowdownProperties = boiler.getBlowdownProperties();
    double inletWaterPressure = blowdownProperties.pressure;
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = blowdownProperties.specificEnthalpy;
    double inletWaterMassFlow = blowdownProperties.massFlow;

    return {inletWaterPressure, quantityType, quantityValue, inletWaterMassFlow, pressure};
}

FlashTank FlashTankFactory::make(const double pressure,
                                 const SteamSystemModelerTool::FluidProperties &condensate) const {
    double inletWaterPressure = condensate.pressure;
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = condensate.specificEnthalpy;
    double inletWaterMassFlow = condensate.massFlow;
    double tankPressure = pressure;

    return {inletWaterPressure, quantityType, quantityValue, inletWaterMassFlow, tankPressure};
}

FlashTank FlashTankFactory::make(const std::shared_ptr<Header> &header,
                                 const std::shared_ptr<HeaderNotHighestPressure> &headerNotHighestPressure) const {
    double inletWaterPressure = header->getHeaderPressure();
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = header->getSpecificEnthalpy();
    double inletWaterMassFlow = header->getInletMassFlow();
    double tankPressure = headerNotHighestPressure->getPressure();

    return {inletWaterPressure, quantityType, quantityValue, inletWaterMassFlow, tankPressure};
}

FlashTank FlashTankFactory::make(const BoilerInput &boilerInput,
                                 const SteamSystemModelerTool::FluidProperties &properties) const {
    double inletWaterPressure = properties.pressure;
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = properties.specificEnthalpy;
    double inletWaterMassFlow = properties.massFlow;
    double tankPressure = boilerInput.getDeaeratorPressure();

    return {inletWaterPressure, quantityType, quantityValue, inletWaterMassFlow, tankPressure};
}
