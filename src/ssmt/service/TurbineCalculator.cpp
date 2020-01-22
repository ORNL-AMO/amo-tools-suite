#include "ssmt/service/TurbineCalculator.h"

Turbine TurbineCalculator::calc(const SteamSystemModelerTool::FluidProperties &headerProperties,
                                const HeaderWithHighestPressure &highPressureHeaderInput, const int headerCountInput,
                                const double connectedHeaderProcessSteamUsage,
                                const std::shared_ptr<HeaderNotHighestPressure> &headerWithNotHighPressure,
                                const PressureTurbine &pressureTurbine) const {
    const Turbine::TurbineProperty &turbineProperty = determineTurbineProperty(pressureTurbine);

    double massFlowOrPowerOut =
            calcMassFlowOrPowerOut(headerProperties, highPressureHeaderInput, headerCountInput,
                                   connectedHeaderProcessSteamUsage);
    massFlowOrPowerOut = adjustMassFlowOrPowerOut(massFlowOrPowerOut, pressureTurbine);

    return turbineFactory.make(headerProperties, turbineProperty, pressureTurbine, massFlowOrPowerOut,
                               headerWithNotHighPressure);
}

Turbine
TurbineCalculator::calc(const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                        const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput,
                        const PressureTurbine &mediumToLowTurbineInput,
                        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput) const {
    const Turbine::TurbineProperty &turbineProperty = determineTurbineProperty(mediumToLowTurbineInput);

    double massFlowOrPowerOut = calcMassFlowOrPowerOut(mediumPressureHeaderInput, mediumPressureHeaderOutput);
    massFlowOrPowerOut = adjustMassFlowOrPowerOut(massFlowOrPowerOut, mediumToLowTurbineInput);

    return turbineFactory.make(mediumPressureHeaderOutput, turbineProperty, mediumToLowTurbineInput, massFlowOrPowerOut,
                               lowPressureHeaderInput);
}

Turbine::TurbineProperty TurbineCalculator::determineTurbineProperty(const PressureTurbine &pressureTurbine) const {
    Turbine::TurbineProperty turbineProperty = Turbine::TurbineProperty::MassFlow;

    const PressureTurbineOperation &operationType = pressureTurbine.getOperationType();
    switch (operationType) {
        case PressureTurbineOperation::STEAM_FLOW:
            break;
        case PressureTurbineOperation::POWER_GENERATION:
            turbineProperty = Turbine::TurbineProperty::PowerOut;
            break;
        case PressureTurbineOperation::BALANCE_HEADER:
            break;
        case PressureTurbineOperation::POWER_RANGE:
            turbineProperty = Turbine::TurbineProperty::PowerOut;
            break;
        case PressureTurbineOperation::FLOW_RANGE:
            break;
        default:
            std::string msg = "TurbineCalculator::determineTurbineProperty: operationType enum not handled=";
            std::cout << msg << operationType << std::endl;
            throw std::invalid_argument(msg);
    }

    return turbineProperty;
}


double TurbineCalculator::calcMassFlowOrPowerOut(const SteamSystemModelerTool::FluidProperties &headerProperties,
                                                 const HeaderWithHighestPressure &highPressureHeaderInput,
                                                 const int headerCountInput,
                                                 const double connectedHeaderProcessSteamUsage) const {
    double massFlowOrPowerOut = headerProperties.massFlow - highPressureHeaderInput.getProcessSteamUsage();

    if (headerCountInput == 3) {
        massFlowOrPowerOut -= connectedHeaderProcessSteamUsage;
    }
    return massFlowOrPowerOut;
}

double
TurbineCalculator::calcMassFlowOrPowerOut(const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                          const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput) const {
    const double inletMassFlow = mediumPressureHeaderOutput.massFlow;
    const double processSteamUsage = mediumPressureHeaderInput->getProcessSteamUsage();

    return inletMassFlow - processSteamUsage;
}

double TurbineCalculator::adjustMassFlowOrPowerOut(const double massFlowOrPowerOut,
                                                   const PressureTurbine &pressureTurbine) const {
    double adjustedMassFlowOrPowerOut = massFlowOrPowerOut;

    //mass flow can be adjusted depending on operation type of the turbine
    const PressureTurbineOperation &operationType = pressureTurbine.getOperationType();
    switch (operationType) {
        case PressureTurbineOperation::STEAM_FLOW:
            adjustedMassFlowOrPowerOut = pressureTurbine.getOperationValue1();
            break;
        case PressureTurbineOperation::POWER_GENERATION:
            adjustedMassFlowOrPowerOut = pressureTurbine.getOperationValue1();
            break;
        case PressureTurbineOperation::BALANCE_HEADER:
            break;
        case PressureTurbineOperation::POWER_RANGE:
            adjustedMassFlowOrPowerOut = pressureTurbine.getOperationValue2();
            break;
        case PressureTurbineOperation::FLOW_RANGE: {
            const double operationValue1 = pressureTurbine.getOperationValue1();
            if (massFlowOrPowerOut < operationValue1) {
                adjustedMassFlowOrPowerOut = operationValue1;
            } else if (massFlowOrPowerOut > pressureTurbine.getOperationValue2()) {
                adjustedMassFlowOrPowerOut = pressureTurbine.getOperationValue2();
            }
        }
            break;
        default:
            std::string msg = "TurbineCalculator::adjustMassFlowOrPowerOut: operationType enum not handled";
           std::cout << msg << std::endl;
            throw std::invalid_argument(msg);
    }

    return adjustedMassFlowOrPowerOut;
}
