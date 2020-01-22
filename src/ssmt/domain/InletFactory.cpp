#include "ssmt/domain/InletFactory.h"

std::vector<Inlet> InletFactory::make(const Boiler &boiler) const {
    const std::string methodName = std::string("InletFactory::") + std::string(__func__) + ": ";

    std::cout << methodName << "making inlet from boiler" << std::endl;

    double pressure = boiler.getSteamPressure();
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = boiler.getSteamProperties().specificEnthalpy;
    double massFlow = boiler.getSteamMassFlow();

    const Inlet inlet = {pressure, quantityType, quantityValue, massFlow};
    std::cout << methodName << "inlet=" << inlet << std::endl;

    return {inlet};
}

Inlet InletFactory::make(const std::shared_ptr<PrvWithoutDesuperheating> &prv) const {
    const std::string methodName = std::string("InletFactory::") + std::string(__func__) + ": ";

    std::cout << methodName << "making inlet from PRV" << std::endl;

    double pressure = prv->getOutletPressure();
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = prv->getOutletProperties().specificEnthalpy;
    double massFlow = prv->getOutletMassFlow();

    const Inlet inlet = {pressure, quantityType, quantityValue, massFlow};
    std::cout << methodName << "inlet=" << inlet << std::endl;

    return inlet;
}

Inlet InletFactory::make(const std::shared_ptr<Turbine> &turbine) const {
    const std::string methodName = std::string("InletFactory::") + std::string(__func__) + ": ";
    std::cout << methodName << "making inlet from turbine" << std::endl;

    const SteamSystemModelerTool::SteamPropertiesOutput &properties = turbine->getOutletProperties();
    double pressure = properties.pressure;
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = properties.specificEnthalpy;
    double massFlow = turbine->getMassFlow();

    const Inlet inlet = {pressure, quantityType, quantityValue, massFlow};
    std::cout << methodName << "inlet=" << inlet << std::endl;

    return inlet;
}

Inlet InletFactory::make(const std::shared_ptr<Turbine> &turbine, const double pressure) const {
    const std::string methodName = std::string("InletFactory::") + std::string(__func__) + ": ";
    std::cout << methodName << "making inlet from turbine with specified pressure" << std::endl;

    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::QUALITY;
    double quantityValue = 0;
    double massFlow = turbine->getMassFlow();

    const Inlet inlet = {pressure, quantityType, quantityValue, massFlow};
    std::cout << methodName << "inlet=" << inlet << std::endl;

    return inlet;
}

Inlet InletFactory::makeFromOutletGas(const std::shared_ptr<FlashTank> &flashTank) const {
    const std::string methodName = std::string("InletFactory::") + std::string(__func__) + ": ";
    std::cout << methodName << "making inlet from flash tank outlet gas" << std::endl;

    const SteamSystemModelerTool::FluidProperties &properties = flashTank->getOutletGasSaturatedProperties();

    return makeWithEnthalpy(properties);
}

Inlet InletFactory::makeFromOutletLiquid(const std::shared_ptr<FlashTank> &flashTank) const {
    const std::string methodName = std::string("InletFactory::") + std::string(__func__) + ": ";
    std::cout << methodName << "making inlet from flash tank outlet liquid" << std::endl;

    const SteamSystemModelerTool::FluidProperties &properties = flashTank->getOutletLiquidSaturatedProperties();

    return makeWithEnthalpy(properties);

}

Inlet InletFactory::makeWithEnthalpy(const SteamSystemModelerTool::FluidProperties &properties) const {
    const std::string methodName = std::string("InletFactory::") + std::string(__func__) + ": ";

    double pressure = properties.pressure;
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double quantityValue = properties.specificEnthalpy;
    double massFlow = properties.massFlow;

    const Inlet inlet = {pressure, quantityType, quantityValue, massFlow};
    std::cout << methodName << "inlet=" << inlet << std::endl;

    return inlet;
}

Inlet InletFactory::makeWithTemperature(const std::shared_ptr<HeatExchanger::Output> &output) const {
    const std::string methodName = std::string("InletFactory::") + std::string(__func__) + ": ";

    std::cout << methodName << "making inlet from heat exchanger" << std::endl;

    const SteamSystemModelerTool::FluidProperties &coldOutlet = output->coldOutlet;

    double pressure = coldOutlet.pressure;
    SteamProperties::ThermodynamicQuantity quantityType = SteamProperties::ThermodynamicQuantity::TEMPERATURE;
    double quantityValue = coldOutlet.temperature;
    double massFlow = coldOutlet.massFlow;

    const Inlet inlet = {pressure, quantityType, quantityValue, massFlow};
    std::cout << methodName << "inlet=" << inlet << std::endl;

    return inlet;
}
