#include "ssmt/domain/TurbineFactory.h"

Turbine
TurbineFactory::make(const SteamSystemModelerTool::FluidProperties &headerProperties,
                     const Turbine::TurbineProperty &turbineProperty,
                     const PressureTurbine &highToLowTurbine, const double massFlowOrPowerOut,
                     const std::shared_ptr<HeaderNotHighestPressure> &headerWithLowPressure) const {
    double isentropicEfficiency = highToLowTurbine.getIsentropicEfficiency();

    return make(headerProperties, turbineProperty, highToLowTurbine, massFlowOrPowerOut, headerWithLowPressure,
                isentropicEfficiency);
}

Turbine
TurbineFactory::makeIdeal(const SteamSystemModelerTool::FluidProperties &headerProperties,
                          const Turbine::TurbineProperty &turbineProperty,
                          const PressureTurbine &highToLowTurbine, const double massFlowOrPowerOut,
                          const std::shared_ptr<HeaderNotHighestPressure> &headerWithLowPressure) const {
    double isentropicEfficiency = 100;

    return make(headerProperties, turbineProperty, highToLowTurbine, massFlowOrPowerOut, headerWithLowPressure,
                isentropicEfficiency);
}

Turbine
TurbineFactory::make(const SteamSystemModelerTool::FluidProperties &headerProperties,
                     const Turbine::TurbineProperty &turbineProperty,
                     const PressureTurbine &highToLowTurbine, const double massFlowOrPowerOut,
                     const std::shared_ptr<HeaderNotHighestPressure> &headerWithLowPressure,
                     const double isentropicEfficiency) const {
    Turbine::Solve solveFor = Turbine::Solve::OutletProperties;
    double inletPressure = headerProperties.pressure;
    SteamProperties::ThermodynamicQuantity inletQuantity = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    double inletQuantityValue = headerProperties.specificEnthalpy;
    double generatorEfficiency = highToLowTurbine.getGenerationEfficiency();
    double outletSteamPressure = headerWithLowPressure->getPressure();

    return {solveFor, inletPressure, inletQuantity, inletQuantityValue, turbineProperty, isentropicEfficiency,
            generatorEfficiency, massFlowOrPowerOut, outletSteamPressure};
}

Turbine TurbineFactory::make(const SteamSystemModelerTool::FluidProperties &headerProperties,
                             const CondensingTurbine &condensingTurbine, const bool isCalcIdeal) const {
    const std::string methodName = "TurbineFactory::" + std::string(__func__) + " condensingTurbine: ";

    if (isCalcIdeal) {
        // std::cout << methodName << "isCalcIdeal is true, calculating condensingTurbine ideal" << std::endl;

        return makeIdeal(headerProperties, condensingTurbine);
    } else {
        // std::cout << methodName << "isCalcIdeal is false, calculating condensingTurbine normal" << std::endl;

        return make(headerProperties, condensingTurbine);
    }
}

Turbine TurbineFactory::make(const SteamSystemModelerTool::FluidProperties &headerProperties,
                             const CondensingTurbine &condensingTurbine) const {
    const double isentropicEfficiency = condensingTurbine.getIsentropicEfficiency();

    return make(headerProperties, condensingTurbine, isentropicEfficiency);
}

Turbine TurbineFactory::makeIdeal(const SteamSystemModelerTool::FluidProperties &headerProperties,
                                  const CondensingTurbine &condensingTurbine) const {
    const double isentropicEfficiency = 100;

    return make(headerProperties, condensingTurbine, isentropicEfficiency);
}

Turbine TurbineFactory::make(const SteamSystemModelerTool::FluidProperties &headerProperties,
                             const CondensingTurbine &condensingTurbine, const double isentropicEfficiency) const {
    const Turbine::Solve solveFor = Turbine::Solve::OutletProperties;
    const double inletPressure = headerProperties.pressure;
    const SteamProperties::ThermodynamicQuantity inletQuantity = SteamProperties::ThermodynamicQuantity::ENTHALPY;
    const double inletQuantityValue = headerProperties.specificEnthalpy;

    const CondensingTurbineOperation operationType = condensingTurbine.getOperationType();
    const Turbine::TurbineProperty turbineProperty =
            convertCondensingTurbineOperationToTurbineProperty(operationType);

    const double generatorEfficiency = condensingTurbine.getGenerationEfficiency();
    const double massFlowOrPowerOut = condensingTurbine.getOperationValue();
    const double outletSteamPressure = condensingTurbine.getCondenserPressure();

    return {solveFor, inletPressure, inletQuantity, inletQuantityValue, turbineProperty, isentropicEfficiency,
            generatorEfficiency, massFlowOrPowerOut, outletSteamPressure};
}

Turbine::TurbineProperty TurbineFactory::convertCondensingTurbineOperationToTurbineProperty(
        const CondensingTurbineOperation &operationType) const {
    Turbine::TurbineProperty turbineProperty;

    switch (operationType) {
        case CondensingTurbineOperation::POWER_GENERATION:
            turbineProperty = Turbine::TurbineProperty::PowerOut;
            break;
        case CondensingTurbineOperation::STEAM_FLOW:
            turbineProperty = Turbine::TurbineProperty::MassFlow;
            break;
        default:
            std::string msg = "TurbineFactory::convertCondensingTurbineOperationToTurbineProperty<<: operator enum not handled";
            // std::cout << msg << std::endl;
            throw std::invalid_argument(msg);
    }

    return turbineProperty;
}

Turbine TurbineFactory::makeWithMassFlow(const SteamSystemModelerTool::FluidProperties &headerProperties,
                                         const PressureTurbine &pressureTurbine, const double massFlow,
                                         const std::shared_ptr<HeaderNotHighestPressure> &headerWithLowPressure,
                                         const bool isCalcIdeal) const {
    const std::string methodName = "TurbineFactory::" + std::string(__func__) + " headerWithLowPressure: ";

    const Turbine::TurbineProperty turbineProperty = Turbine::TurbineProperty::MassFlow;

    if (isCalcIdeal) {
        // std::cout << methodName << "isCalcIdeal is true, calculating turbine ideal" << std::endl;

        return makeIdeal(headerProperties, turbineProperty, pressureTurbine, massFlow, headerWithLowPressure);
    } else {
        // std::cout << methodName << "isCalcIdeal is false, calculating turbine normal" << std::endl;

        return make(headerProperties, turbineProperty, pressureTurbine, massFlow, headerWithLowPressure);
    }
}

Turbine TurbineFactory::makeWithPowerOut(const SteamSystemModelerTool::FluidProperties &headerProperties,
                                         const PressureTurbine &pressureTurbine, const double powerOut,
                                         const std::shared_ptr<HeaderNotHighestPressure> &headerWithLowPressure,
                                         const bool isCalcIdeal) const {
    const std::string methodName = "TurbineFactory::" + std::string(__func__) + " headerWithLowPressure: ";

    const Turbine::TurbineProperty turbineProperty = Turbine::TurbineProperty::PowerOut;

    if (isCalcIdeal) {
        // std::cout << methodName << "isCalcIdeal is true, calculating turbine ideal" << std::endl;

        return makeIdeal(headerProperties, turbineProperty, pressureTurbine, powerOut, headerWithLowPressure);
    } else {
        // std::cout << methodName << "isCalcIdeal is false, calculating turbine normal" << std::endl;

        return make(headerProperties, turbineProperty, pressureTurbine, powerOut, headerWithLowPressure);
    }
}

std::shared_ptr<Turbine>
TurbineFactory::makePtrWithMassFlow(const SteamSystemModelerTool::FluidProperties &headerProperties,
                                    const PressureTurbine &pressureTurbine, const double massFlow,
                                    const std::shared_ptr<HeaderNotHighestPressure> &headerWithLowPressure,
                                    const bool isCalcIdeal) const {
    const Turbine turbine =
            makeWithMassFlow(headerProperties, pressureTurbine, massFlow, headerWithLowPressure, isCalcIdeal);
    return std::make_shared<Turbine>(turbine);
}

std::shared_ptr<Turbine>
TurbineFactory::makePtrWithPowerOut(const SteamSystemModelerTool::FluidProperties &headerProperties,
                                    const PressureTurbine &pressureTurbine, double massFlow,
                                    const std::shared_ptr<HeaderNotHighestPressure> &headerWithLowPressure,
                                    const bool isCalcIdeal) const {
    const Turbine turbine =
            makeWithPowerOut(headerProperties, pressureTurbine, massFlow, headerWithLowPressure, isCalcIdeal);
    return std::make_shared<Turbine>(turbine);
}
