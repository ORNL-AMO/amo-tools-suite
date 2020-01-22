#include "ssmt/service/high_pressure_header/HighToMediumSteamTurbineCalculator.h"

HighToMediumSteamTurbineCalculationsDomain
HighToMediumSteamTurbineCalculator::calc(const int headerCountInput,
                                         const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                         const HeaderWithHighestPressure &highPressureHeaderInput,
                                         const CondensingTurbine &condensingTurbineInput,
                                         const std::shared_ptr<Turbine> &condensingTurbine,
                                         const PressureTurbine &highToLowTurbineInput,
                                         const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                         const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                         const PressureTurbine &highToMediumTurbineInput,
                                         const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                         const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                         const Boiler &boiler) const {
    const std::string methodName = std::string("HighToMediumSteamTurbineCalculator::") + std::string(__func__) + ": ";

    HighToMediumSteamTurbineCalculationsDomain highToMediumSteamTurbineCalculationsDomain;

    if (headerCountInput == 3 && highToMediumTurbineInput.isUseTurbine()) {
        std::cout << methodName
                  << "medium turbine provided and highToMediumTurbineInput isUseTurbine, calculating highToMediumPressureTurbine"
                  << std::endl;

        const double availableMassFlow =
                calcAvailableMassFlow(highPressureHeaderInput, highPressureHeaderOutput, condensingTurbineInput,
                                      condensingTurbine, highToLowTurbineInput, highToLowPressureTurbine);
        std::cout << methodName << "availableMassFlow=" << availableMassFlow << std::endl;
        highToMediumSteamTurbineCalculationsDomain =
                calc(availableMassFlow, highToLowTurbineInput, highToLowPressureTurbine, highToLowPressureTurbineIdeal,
                     highToMediumTurbineInput, mediumPressureHeaderInput, highPressureHeaderOutput,
                     lowPressureHeaderInput, boiler);
    } else {
        std::cout << methodName << "medium turbine not provided and highToMediumTurbineInput not isUseTurbine, skipping"
                  << std::endl;
        const std::shared_ptr<Turbine> highToMediumPressureTurbine = nullptr;
        const std::shared_ptr<Turbine> highToMediumPressureTurbineIdeal = nullptr;
        highToMediumSteamTurbineCalculationsDomain =
                {highToMediumPressureTurbine, highToMediumPressureTurbineIdeal, highToLowPressureTurbine,
                 highToLowPressureTurbineIdeal};
    }

    std::cout << methodName << "highToMediumSteamTurbineCalculationsDomain="
              << highToMediumSteamTurbineCalculationsDomain << std::endl;

    return highToMediumSteamTurbineCalculationsDomain;
}

HighToMediumSteamTurbineCalculationsDomain
HighToMediumSteamTurbineCalculator::calc(const double availableMassFlow, const PressureTurbine &highToLowTurbineInput,
                                         const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                         const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                         const PressureTurbine &highToMediumTurbineInput,
                                         const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                         const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                         const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                         const Boiler &boiler) const {
    const std::string methodName = std::string("HighToMediumSteamTurbineCalculator::") + std::string(__func__) + ": ";

    HighToMediumSteamTurbineCalculationsDomain highToMediumSteamTurbineCalculationsDomain;

    const PressureTurbineOperation &pressureTurbineOperation = highToMediumTurbineInput.getOperationType();
    std::cout << methodName << "pressureTurbineOperation=" << pressureTurbineOperation << std::endl;

    switch (pressureTurbineOperation) {
        case PressureTurbineOperation::FLOW_RANGE:
            highToMediumSteamTurbineCalculationsDomain =
                    calcFlowRange(availableMassFlow, highToLowTurbineInput, highToLowPressureTurbine,
                                  highToLowPressureTurbineIdeal, highToMediumTurbineInput, highPressureHeaderOutput,
                                  mediumPressureHeaderInput, lowPressureHeaderInput, boiler);
            break;
        case PressureTurbineOperation::POWER_RANGE:
            highToMediumSteamTurbineCalculationsDomain =
                    calcPowerRange(availableMassFlow, highToLowTurbineInput, highToLowPressureTurbine,
                                   highToLowPressureTurbineIdeal, highToMediumTurbineInput, highPressureHeaderOutput,
                                   mediumPressureHeaderInput, lowPressureHeaderInput, boiler);
            break;
        case PressureTurbineOperation::POWER_GENERATION:
            highToMediumSteamTurbineCalculationsDomain =
                    calcPowerGeneration(availableMassFlow, highToLowTurbineInput, highToLowPressureTurbine,
                                        highToLowPressureTurbineIdeal, highToMediumTurbineInput,
                                        highPressureHeaderOutput, mediumPressureHeaderInput, lowPressureHeaderInput,
                                        boiler);
            break;
        case PressureTurbineOperation::STEAM_FLOW:
            highToMediumSteamTurbineCalculationsDomain =
                    calcSteamFlow(availableMassFlow, highToLowTurbineInput, highToLowPressureTurbine,
                                  highToLowPressureTurbineIdeal, highToMediumTurbineInput, highPressureHeaderOutput,
                                  mediumPressureHeaderInput, lowPressureHeaderInput, boiler);
            break;
        case PressureTurbineOperation::BALANCE_HEADER:
            highToMediumSteamTurbineCalculationsDomain =
                    calcBalanceHeader(availableMassFlow, highToLowPressureTurbine, highToLowPressureTurbineIdeal,
                                      highToMediumTurbineInput, highPressureHeaderOutput, mediumPressureHeaderInput);
            break;
        default:
            std::string msg = methodName + "PressureTurbineOperation enum not handled";
            std::cout << msg << std::endl;
            throw std::invalid_argument(msg);
    }

    return highToMediumSteamTurbineCalculationsDomain;
}

HighToMediumSteamTurbineCalculationsDomain
HighToMediumSteamTurbineCalculator::calcFlowRange(const double availableMassFlow,
                                                  const PressureTurbine &highToLowTurbineInput,
                                                  const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                  const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                  const PressureTurbine &highToMediumTurbineInput,
                                                  const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                  const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                  const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                  const Boiler &boiler) const {
    const std::string methodName = std::string("HighToMediumSteamTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> highToMediumPressureTurbine = nullptr;
    std::shared_ptr<Turbine> highToMediumPressureTurbineIdeal = nullptr;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double highToMediumTurbineInputOperationValue1 = highToMediumTurbineInput.getOperationValue1();
    const double highToMediumTurbineInputOperationValue2 = highToMediumTurbineInput.getOperationValue2();

    //if more steam needed for minimum than is available
    if (highToMediumTurbineInputOperationValue1 > availableMassFlow) {
        std::cout << methodName
                  << "highToMediumTurbineInputOperationValue1=" << highToMediumTurbineInputOperationValue1
                  << " > availableMassFlow=" << availableMassFlow
                  << ", calculating highToMediumPressureTurbine with amount needed (highToMediumTurbineInputOperationValue1)"
                  << " instead of amount available" << std::endl;
        //calculate turbine with amount needed
        highToMediumPressureTurbine =
                turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   highToMediumTurbineInputOperationValue1, mediumPressureHeaderInput,
                                                   false);
        highToMediumPressureTurbineIdeal =
                turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   highToMediumTurbineInputOperationValue1, mediumPressureHeaderInput,
                                                   true);

        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("highToMediumPressureTurbine", highToLowTurbineInput,
                                                 lowPressureHeaderInput, boiler, highToLowPressureTurbine,
                                                 highToLowPressureTurbineIdeal, highToMediumPressureTurbine,
                                                 highPressureHeaderOutput, availableMassFlow);
        std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;

        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    } else if (highToMediumTurbineInputOperationValue2 < availableMassFlow) {
        std::cout << methodName
                  << "highToMediumTurbineInputOperationValue2=" << highToMediumTurbineInputOperationValue2
                  << " < availableMassFlow=" << availableMassFlow
                  << ", calculating highToMediumPressureTurbine with max amount allowed (highToMediumTurbineInputOperationValue2)"
                  << " instead of the greater amount available" << std::endl;
        highToMediumPressureTurbine =
                turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   highToMediumTurbineInputOperationValue2, mediumPressureHeaderInput,
                                                   false);
        highToMediumPressureTurbineIdeal =
                turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   highToMediumTurbineInputOperationValue2, mediumPressureHeaderInput,
                                                   true);
    } else {
        std::cout << methodName
                  << "availableMassFlow=" << availableMassFlow << " is between needed and max amounts,"
                  << " calculating highToMediumPressureTurbine with availableMassFlow" << std::endl;
        highToMediumPressureTurbine =
                turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   availableMassFlow, mediumPressureHeaderInput, false);
        highToMediumPressureTurbineIdeal =
                turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   availableMassFlow, mediumPressureHeaderInput, true);
    }

    std::cout << methodName << "highToMediumPressureTurbine=" << highToMediumPressureTurbine << std::endl;
    std::cout << methodName << "highToMediumPressureTurbineIdeal=" << highToMediumPressureTurbineIdeal << std::endl;

    return {highToMediumPressureTurbine, highToMediumPressureTurbineIdeal, highToLowPressureTurbineUpdated,
            highToLowPressureTurbineIdealUpdated};
}

HighToMediumSteamTurbineCalculationsDomain
HighToMediumSteamTurbineCalculator::calcPowerRange(const double availableMassFlow,
                                                   const PressureTurbine &highToLowTurbineInput,
                                                   const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                   const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                   const PressureTurbine &highToMediumTurbineInput,
                                                   const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                   const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                   const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                   const Boiler &boiler) const {
    const std::string methodName = std::string("HighToMediumSteamTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> highToMediumPressureTurbine = nullptr;
    std::shared_ptr<Turbine> highToMediumPressureTurbineIdeal = nullptr;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double highToMediumTurbineInputOperationValue1 = highToMediumTurbineInput.getOperationValue1();
    const double highToMediumTurbineInputOperationValue2 = highToMediumTurbineInput.getOperationValue2();

    std::cout << methodName
              << "calculating highToMediumPressureTurbine with availableMassFlow=" << availableMassFlow << std::endl;
    highToMediumPressureTurbine =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput, availableMassFlow,
                                               mediumPressureHeaderInput, false);
    std::cout << methodName << "highToMediumPressureTurbine=" << highToMediumPressureTurbine << std::endl;

    highToMediumPressureTurbineIdeal =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput, availableMassFlow,
                                               mediumPressureHeaderInput, true);
    std::cout << methodName << "highToMediumPressureTurbineIdeal=" << highToMediumPressureTurbineIdeal << std::endl;

    //check to see if power out is in range
    const double highToMediumPressureTurbinePowerOut = highToMediumPressureTurbine->getPowerOut();
    if (highToMediumTurbineInputOperationValue1 > highToMediumPressureTurbinePowerOut) {
        std::cout << methodName
                  << "highToMediumTurbineInputOperationValue1=" << highToMediumTurbineInputOperationValue1
                  << " > highToMediumPressureTurbinePowerOut=" << highToMediumPressureTurbinePowerOut
                  << "; not enough power out of turbine,"
                  << " calculating highToMediumPressureTurbine with amount needed instead of amount available"
                  << std::endl;
        double currentMassFlowAvailable = highToMediumPressureTurbine->getMassFlow();
        //calculate minimum mass flow needed
        highToMediumPressureTurbine =
                turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   highToMediumTurbineInputOperationValue1, mediumPressureHeaderInput,
                                                   false);
        std::cout << methodName << "highToMediumPressureTurbine=" << highToMediumPressureTurbine << std::endl;

        highToMediumPressureTurbineIdeal =
                turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   highToMediumTurbineInputOperationValue1, mediumPressureHeaderInput,
                                                   true);
        std::cout << methodName << "highToMediumPressureTurbineIdeal=" << highToMediumPressureTurbineIdeal << std::endl;

        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("highToMediumPressureTurbine", highToLowTurbineInput,
                                                 lowPressureHeaderInput, boiler, highToLowPressureTurbine,
                                                 highToLowPressureTurbineIdeal, highToMediumPressureTurbine,
                                                 highPressureHeaderOutput, currentMassFlowAvailable);
        std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;

        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    } else if (highToMediumTurbineInputOperationValue2 < highToMediumPressureTurbinePowerOut) {
        std::cout << methodName
                  << "highToMediumTurbineInputOperationValue2=" << highToMediumTurbineInputOperationValue2
                  << " < highToMediumPressureTurbinePowerOut=" << highToMediumPressureTurbinePowerOut
                  << " not enough power out of turbine,"
                  << " calculating highToMediumPressureTurbine with amount needed instead of amount available"
                  << std::endl;
        //if power out with available mass flow is greater than max, calculate turbine with max power out
        highToMediumPressureTurbine =
                turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   highToMediumTurbineInputOperationValue2, mediumPressureHeaderInput,
                                                   false);
        std::cout << methodName << "highToMediumPressureTurbine=" << highToMediumPressureTurbine << std::endl;

        highToMediumPressureTurbineIdeal =
                turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToMediumTurbineInput,
                                                   highToMediumTurbineInputOperationValue2, mediumPressureHeaderInput,
                                                   true);
        std::cout << methodName << "highToMediumPressureTurbineIdeal=" << highToMediumPressureTurbineIdeal << std::endl;
    }

    return {highToMediumPressureTurbine, highToMediumPressureTurbineIdeal, highToLowPressureTurbineUpdated,
            highToLowPressureTurbineIdealUpdated};
}

HighToMediumSteamTurbineCalculationsDomain
HighToMediumSteamTurbineCalculator::calcPowerGeneration(const double availableMassFlow,
                                                        const PressureTurbine &highToLowTurbineInput,
                                                        const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                        const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                        const PressureTurbine &highToMediumTurbineInput,
                                                        const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                        const Boiler &boiler) const {
    const std::string methodName = std::string("HighToMediumSteamTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> highToMediumPressureTurbine = nullptr;
    std::shared_ptr<Turbine> highToMediumPressureTurbineIdeal = nullptr;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double highToMediumTurbineInputOperationValue1 = highToMediumTurbineInput.getOperationValue1();

    std::cout << methodName
              << "calculating highToMediumPressureTurbine with power out (highToMediumTurbineInputOperationValue1)="
              << highToMediumTurbineInputOperationValue1 << std::endl;
    highToMediumPressureTurbine =
            turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToMediumTurbineInput,
                                               highToMediumTurbineInputOperationValue1, mediumPressureHeaderInput,
                                               false);
    std::cout << methodName << "highToMediumPressureTurbine=" << highToMediumPressureTurbine << std::endl;

    highToMediumPressureTurbineIdeal =
            turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToMediumTurbineInput,
                                               highToMediumTurbineInputOperationValue1, mediumPressureHeaderInput,
                                               true);
    std::cout << methodName << "highToMediumPressureTurbineIdeal=" << highToMediumPressureTurbineIdeal << std::endl;


    //check that there is enough mass flow available to meet need for given power out
    const double highToMediumPressureTurbineMassFlow = highToMediumPressureTurbine->getMassFlow();
    if (highToMediumPressureTurbineMassFlow > availableMassFlow) {
        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("highToMediumPressureTurbine", highToLowTurbineInput,
                                                 lowPressureHeaderInput, boiler, highToLowPressureTurbine,
                                                 highToLowPressureTurbineIdeal, highToMediumPressureTurbine,
                                                 highPressureHeaderOutput, availableMassFlow);
        std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;

        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    }

    return {highToMediumPressureTurbine, highToMediumPressureTurbineIdeal, highToLowPressureTurbineUpdated,
            highToLowPressureTurbineIdealUpdated};
}

HighToMediumSteamTurbineCalculationsDomain
HighToMediumSteamTurbineCalculator::calcSteamFlow(const double availableMassFlow,
                                                  const PressureTurbine &highToLowTurbineInput,
                                                  const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                  const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                  const PressureTurbine &highToMediumTurbineInput,
                                                  const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                  const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                  const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                  const Boiler &boiler) const {
    const std::string methodName = std::string("HighToMediumSteamTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> highToMediumPressureTurbine = nullptr;
    std::shared_ptr<Turbine> highToMediumPressureTurbineIdeal = nullptr;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double highToMediumTurbineInputOperationValue1 = highToMediumTurbineInput.getOperationValue1();

    std::cout << methodName
              << "calculating highToMediumPressureTurbine with mass flow (highToMediumTurbineInputOperationValue1)="
              << highToMediumTurbineInputOperationValue1
              << std::endl;
    highToMediumPressureTurbine =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput,
                                               highToMediumTurbineInputOperationValue1, mediumPressureHeaderInput,
                                               false);
    std::cout << methodName << "highToMediumPressureTurbine=" << highToMediumPressureTurbine << std::endl;

    highToMediumPressureTurbineIdeal =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput,
                                               highToMediumTurbineInputOperationValue1, mediumPressureHeaderInput,
                                               true);
    std::cout << methodName << "highToMediumPressureTurbineIdeal=" << highToMediumPressureTurbineIdeal << std::endl;

    //check enough mass flow exists for set mass flow
    const double highToMediumPressureTurbineMassFlow = highToMediumPressureTurbine->getMassFlow();
    if (highToMediumPressureTurbineMassFlow > availableMassFlow) {
        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("highToMediumPressureTurbine", highToLowTurbineInput,
                                                 lowPressureHeaderInput, boiler, highToLowPressureTurbine,
                                                 highToLowPressureTurbineIdeal, highToMediumPressureTurbine,
                                                 highPressureHeaderOutput, availableMassFlow);
        std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;

        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    }

    return {highToMediumPressureTurbine, highToMediumPressureTurbineIdeal, highToLowPressureTurbineUpdated,
            highToLowPressureTurbineIdealUpdated};
}

HighToMediumSteamTurbineCalculationsDomain
HighToMediumSteamTurbineCalculator::calcBalanceHeader(const double availableMassFlow,
                                                      const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                      const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                      const PressureTurbine &highToMediumTurbineInput,
                                                      const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                      const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput) const {
    const std::string methodName = std::string("HighToMediumSteamTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> highToMediumPressureTurbine = nullptr;
    std::shared_ptr<Turbine> highToMediumPressureTurbineIdeal = nullptr;

    std::cout << methodName
              << "calculating highToMediumPressureTurbine with availableMassFlow=" << availableMassFlow << std::endl;
    highToMediumPressureTurbine =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput, availableMassFlow,
                                               mediumPressureHeaderInput, false);
    std::cout << methodName << "highToMediumPressureTurbine=" << highToMediumPressureTurbine << std::endl;

    highToMediumPressureTurbineIdeal =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToMediumTurbineInput, availableMassFlow,
                                               mediumPressureHeaderInput, true);
    std::cout << methodName << "highToMediumPressureTurbineIdeal=" << highToMediumPressureTurbineIdeal << std::endl;

    return {highToMediumPressureTurbine, highToMediumPressureTurbineIdeal, highToLowPressureTurbine,
            highToLowPressureTurbineIdeal};
}

double
HighToMediumSteamTurbineCalculator::calcAvailableMassFlow(const HeaderWithHighestPressure &highPressureHeaderInput,
                                                          const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                          const CondensingTurbine &condensingTurbineInput,
                                                          const std::shared_ptr<Turbine> &condensingTurbine,
                                                          const PressureTurbine &highToLowTurbineInput,
                                                          const std::shared_ptr<Turbine> &highToLowPressureTurbine) const {
    const std::string methodName = std::string("HighToMediumSteamTurbineCalculator::") + std::string(__func__) + ": ";

    double availableMassFlow = highPressureHeaderOutput.massFlow - highPressureHeaderInput.getProcessSteamUsage();

    //remove steam that goes through condensing turbine
    if (condensingTurbineInput.isUseTurbine()) {
        const double massFlow = condensingTurbine->getMassFlow();
        std::cout << methodName
                  << "condensingTurbineInput.isUseTurbine() is true, subtracting condensingTurbine->getMassFlow()="
                  << massFlow << " from availableMassFlow=" << availableMassFlow << std::endl;
        availableMassFlow -= massFlow;
    } else {
        std::cout << methodName << "condensingTurbineInput.isUseTurbine() is false, skipping" << std::endl;
    }

    //remove steam that goes through high to low turbine
    if (highToLowTurbineInput.isUseTurbine()) {
        const double massFlow = highToLowPressureTurbine->getMassFlow();
        std::cout << methodName
                  << "highToLowTurbineInput.isUseTurbine() is true, subtracting highToLowPressureTurbine->getMassFlow()="
                  << massFlow << " from availableMassFlow=" << availableMassFlow << std::endl;
        availableMassFlow -= massFlow;
    } else {
        std::cout << methodName << "highToLowTurbineInput.isUseTurbine() is false, skipping" << std::endl;
    }

    return availableMassFlow;
}
