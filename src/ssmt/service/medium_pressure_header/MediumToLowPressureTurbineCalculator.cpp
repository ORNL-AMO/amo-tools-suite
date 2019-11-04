#include <ssmt/service/medium_pressure_header/MediumToLowPressureTurbineCalculator.h>

MediumToLowPressureTurbineCalculatorOutput
MediumToLowPressureTurbineCalculator::calc(const PressureTurbine &highToLowTurbineInput,
                                           const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                           const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                           const PressureTurbine &mediumToLowTurbineInput,
                                           const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                           const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                           const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput,
                                           const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                           const Boiler &boiler) const {
    const std::string methodName = std::string("MediumToLowPressureTurbineCalculator::") + std::string(__func__) + ": ";

    MediumToLowPressureTurbineCalculatorOutput mediumToLowPressureTurbineCalculatorOutput;

    if (mediumToLowTurbineInput.isUseTurbine()) {
        // std::cout << methodName
                //   << "medium to low turbine input provided and mediumToLowTurbineInput isUseTurbine,"
                //   << " calculating mediumToLowPressureTurbine" << std::endl;

        double availableMassFlow = calcAvailableMassFlow(mediumPressureHeaderInput, mediumPressureHeaderOutput);
        // std::cout << methodName << "availableMassFlow" << availableMassFlow << std::endl;

        mediumToLowPressureTurbineCalculatorOutput =
                calc(availableMassFlow, highToLowTurbineInput, highToLowPressureTurbine, highToLowPressureTurbineIdeal,
                     highPressureHeaderOutput, mediumToLowTurbineInput, mediumPressureHeaderOutput,
                     lowPressureHeaderInput, boiler);
    } else {
        // std::cout << methodName
                //   << "medium to low turbine input not provided and mediumToLowTurbineInput not isUseTurbine, skipping"
                //   << std::endl;
        const std::shared_ptr<Turbine> mediumToLowPressureTurbine = nullptr;
        const std::shared_ptr<Turbine> mediumToLowPressureTurbineIdeal = nullptr;
        mediumToLowPressureTurbineCalculatorOutput =
                {mediumToLowPressureTurbine, mediumToLowPressureTurbineIdeal, highToLowPressureTurbine,
                 highToLowPressureTurbineIdeal};
    }

    return mediumToLowPressureTurbineCalculatorOutput;
}

MediumToLowPressureTurbineCalculatorOutput
MediumToLowPressureTurbineCalculator::calc(const double availableMassFlow,
                                           const PressureTurbine &highToLowTurbineInput,
                                           const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                           const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                           const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                           const PressureTurbine &mediumToLowTurbineInput,
                                           const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput,
                                           const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                           const Boiler &boiler) const {
    const std::string methodName = std::string("MediumToLowPressureTurbineCalculator::") + std::string(__func__) + ": ";

    MediumToLowPressureTurbineCalculatorOutput mediumToLowPressureTurbineCalculatorOutput;

    const PressureTurbineOperation &pressureTurbineOperation = mediumToLowTurbineInput.getOperationType();
//     std::cout << methodName << "pressureTurbineOperation=" << pressureTurbineOperation << std::endl;

    switch (pressureTurbineOperation) {
        case PressureTurbineOperation::FLOW_RANGE:
            mediumToLowPressureTurbineCalculatorOutput =
                    calcFlowRange(availableMassFlow, mediumToLowTurbineInput, mediumPressureHeaderOutput,
                                  lowPressureHeaderInput, boiler, highToLowTurbineInput, highToLowPressureTurbine,
                                  highToLowPressureTurbineIdeal, highPressureHeaderOutput);
            break;
        case PressureTurbineOperation::POWER_RANGE:
            mediumToLowPressureTurbineCalculatorOutput =
                    calcPowerRange(availableMassFlow, mediumToLowTurbineInput, mediumPressureHeaderOutput,
                                   lowPressureHeaderInput, boiler, highToLowTurbineInput, highToLowPressureTurbine,
                                   highToLowPressureTurbineIdeal, highPressureHeaderOutput);
            break;
        case PressureTurbineOperation::POWER_GENERATION:
            mediumToLowPressureTurbineCalculatorOutput =
                    calcPowerGeneration(availableMassFlow, mediumToLowTurbineInput, mediumPressureHeaderOutput,
                                        lowPressureHeaderInput, boiler, highToLowTurbineInput, highToLowPressureTurbine,
                                        highToLowPressureTurbineIdeal, highPressureHeaderOutput);
            break;
        case PressureTurbineOperation::STEAM_FLOW:
            mediumToLowPressureTurbineCalculatorOutput =
                    calcSteamFlow(availableMassFlow, mediumToLowTurbineInput, mediumPressureHeaderOutput,
                                  lowPressureHeaderInput, boiler, highToLowTurbineInput, highToLowPressureTurbine,
                                  highToLowPressureTurbineIdeal, highPressureHeaderOutput);
            break;
        case PressureTurbineOperation::BALANCE_HEADER:
            mediumToLowPressureTurbineCalculatorOutput =
                    calcBalanceHeader(availableMassFlow, mediumToLowTurbineInput, mediumPressureHeaderOutput,
                                      lowPressureHeaderInput, highToLowPressureTurbine, highToLowPressureTurbineIdeal);
            break;
        default:
            std::string msg = methodName + "PressureTurbineOperation enum not handled";
        //     std::cout << msg << std::endl;
            throw std::invalid_argument(msg);
    }

    return mediumToLowPressureTurbineCalculatorOutput;
}

MediumToLowPressureTurbineCalculatorOutput
MediumToLowPressureTurbineCalculator::calcFlowRange(const double availableMassFlow,
                                                    const PressureTurbine &mediumToLowTurbineInput,
                                                    const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput,
                                                    const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                    const Boiler &boiler,
                                                    const PressureTurbine &highToLowTurbineInput,
                                                    const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                    const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                    const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput) const {
    const std::string methodName = std::string("MediumToLowPressureTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> mediumToLowPressureTurbine = nullptr;
    std::shared_ptr<Turbine> mediumToLowPressureTurbineIdeal = nullptr;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double mediumToLowTurbineInputOperationValue1 = mediumToLowTurbineInput.getOperationValue1();
    const double mediumToLowTurbineInputOperationValue2 = mediumToLowTurbineInput.getOperationValue2();

    //if minimum amount needed is greater than available amount
    if (mediumToLowTurbineInputOperationValue1 > availableMassFlow) {
        // std::cout << methodName
                //   << "mediumToLowTurbineInputOperationValue1=" << mediumToLowTurbineInputOperationValue1
                //   << " > availableMassFlow=" << availableMassFlow
                //   << ", calculating mediumToLowPressureTurbine with amount needed (mediumToLowTurbineInputOperationValue1)"
                //   << " instead of amount available" << std::endl;
        //calculate turbine with amount needed
        mediumToLowPressureTurbine =
                turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   mediumToLowTurbineInputOperationValue1, lowPressureHeaderInput,
                                                   false);
        mediumToLowPressureTurbineIdeal =
                turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   mediumToLowTurbineInputOperationValue1, lowPressureHeaderInput,
                                                   true);

        const double neededMassFlow = mediumToLowPressureTurbine->getMassFlow();
        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("mediumToLowPressureTurbine", highToLowTurbineInput,
                                                 lowPressureHeaderInput, boiler, highToLowPressureTurbine,
                                                 highToLowPressureTurbineIdeal, highPressureHeaderOutput,
                                                 neededMassFlow, availableMassFlow);
        // std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;
        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    } else if (mediumToLowTurbineInputOperationValue2 < availableMassFlow) {
        // std::cout << methodName
                //   << "mediumToLowTurbineInputOperationValue2=" << mediumToLowTurbineInputOperationValue2
                //   << " < availableMassFlow=" << availableMassFlow
                //   << ", calculating mediumToLowPressureTurbine with max amount allowed (mediumToLowTurbineInputOperationValue2)"
                //   << " instead of the greater amount available" << std::endl;
        mediumToLowPressureTurbine =
                turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   mediumToLowTurbineInputOperationValue2, lowPressureHeaderInput,
                                                   false);
        mediumToLowPressureTurbineIdeal =
                turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   mediumToLowTurbineInputOperationValue2, lowPressureHeaderInput,
                                                   true);
    } else {
        // std::cout << methodName
                //   << "availableMassFlow=" << availableMassFlow << " is between needed and max amounts,"
                //   << " calculating mediumToLowPressureTurbine with availableMassFlow" << std::endl;
        mediumToLowPressureTurbine =
                turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   availableMassFlow, lowPressureHeaderInput, false);
        mediumToLowPressureTurbineIdeal =
                turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   availableMassFlow, lowPressureHeaderInput, true);
    }

//     std::cout << methodName << "mediumToLowPressureTurbine=" << mediumToLowPressureTurbine << std::endl;
//     std::cout << methodName << "mediumToLowPressureTurbineIdeal=" << mediumToLowPressureTurbineIdeal << std::endl;

    return {mediumToLowPressureTurbine, mediumToLowPressureTurbineIdeal, highToLowPressureTurbineUpdated,
            highToLowPressureTurbineIdealUpdated};
}

MediumToLowPressureTurbineCalculatorOutput
MediumToLowPressureTurbineCalculator::calcPowerRange(const double availableMassFlow,
                                                     const PressureTurbine &mediumToLowTurbineInput,
                                                     const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput,
                                                     const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                     const Boiler &boiler,
                                                     const PressureTurbine &highToLowTurbineInput,
                                                     const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                     const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                     const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput) const {
    const std::string methodName = std::string("MediumToLowPressureTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> mediumToLowPressureTurbine = nullptr;
    std::shared_ptr<Turbine> mediumToLowPressureTurbineIdeal = nullptr;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double mediumToLowTurbineInputOperationValue1 = mediumToLowTurbineInput.getOperationValue1();
    const double mediumToLowTurbineInputOperationValue2 = mediumToLowTurbineInput.getOperationValue2();

//     std::cout << methodName
          //    << "calculating mediumToLowPressureTurbine with availableMassFlow=" << availableMassFlow << std::endl;
    mediumToLowPressureTurbine =
            turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                               availableMassFlow, lowPressureHeaderInput, false);
    mediumToLowPressureTurbineIdeal =
            turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                               availableMassFlow, lowPressureHeaderInput, true);
    //check that power out is in range
    const double mediumToLowPressureTurbinePowerOut = mediumToLowPressureTurbine->getPowerOut();
    if (mediumToLowTurbineInputOperationValue1 > mediumToLowPressureTurbinePowerOut) {
        // std::cout << methodName
                //   << "mediumToLowTurbineInputOperationValue1=" << mediumToLowTurbineInputOperationValue1
                //   << " > mediumToLowPressureTurbinePowerOut=" << mediumToLowPressureTurbinePowerOut
                //   << "; not enough power out of turbine,"
                //   << " calculating mediumToLowPressureTurbine with amount needed instead of amount available"
                //   << std::endl;
        //calculate minimum mass flow needed
        mediumToLowPressureTurbine =
                turbineFactory.makePtrWithPowerOut(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   mediumToLowTurbineInputOperationValue1, lowPressureHeaderInput,
                                                   false);
        mediumToLowPressureTurbineIdeal =
                turbineFactory.makePtrWithPowerOut(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   mediumToLowTurbineInputOperationValue1, lowPressureHeaderInput,
                                                   true);
        const double neededMassFlow = mediumToLowPressureTurbine->getMassFlow();
        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("mediumToLowPressureTurbine", highToLowTurbineInput,
                                                 lowPressureHeaderInput, boiler, highToLowPressureTurbine,
                                                 highToLowPressureTurbineIdeal, highPressureHeaderOutput,
                                                 neededMassFlow, availableMassFlow);
        // std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;
        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    } else if (mediumToLowTurbineInputOperationValue2 < mediumToLowPressureTurbinePowerOut) {
        // std::cout << methodName
                //   << "mediumToLowTurbineInputOperationValue2=" << mediumToLowTurbineInputOperationValue2
                //   << " < mediumToLowPressureTurbinePowerOut=" << mediumToLowPressureTurbinePowerOut
                //   << " not enough power out of turbine,"
                //   << " calculating mediumToLowPressureTurbine with amount needed instead of amount available"
                //   << std::endl;
        //if power out with available mass flow is greater than max, calculate turbine with max power out
        mediumToLowPressureTurbine =
                turbineFactory.makePtrWithPowerOut(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   mediumToLowTurbineInputOperationValue2, lowPressureHeaderInput,
                                                   false);
        mediumToLowPressureTurbineIdeal =
                turbineFactory.makePtrWithPowerOut(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                                   mediumToLowTurbineInputOperationValue2, lowPressureHeaderInput,
                                                   true);
    }

//     std::cout << methodName << "mediumToLowPressureTurbine=" << mediumToLowPressureTurbine << std::endl;
//     std::cout << methodName << "mediumToLowPressureTurbineIdeal=" << mediumToLowPressureTurbineIdeal << std::endl;

    return {mediumToLowPressureTurbine, mediumToLowPressureTurbineIdeal, highToLowPressureTurbineUpdated,
            highToLowPressureTurbineIdealUpdated};
}

MediumToLowPressureTurbineCalculatorOutput
MediumToLowPressureTurbineCalculator::calcPowerGeneration(const double availableMassFlow,
                                                          const PressureTurbine &mediumToLowTurbineInput,
                                                          const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput,
                                                          const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                          const Boiler &boiler,
                                                          const PressureTurbine &highToLowTurbineInput,
                                                          const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                          const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                          const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput) const {
    const std::string methodName = std::string("MediumToLowPressureTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> mediumToLowPressureTurbine = nullptr;
    std::shared_ptr<Turbine> mediumToLowPressureTurbineIdeal = nullptr;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double mediumToLowTurbineInputOperationValue1 = mediumToLowTurbineInput.getOperationValue1();

    //calculate turbine with set amount of power out
    mediumToLowPressureTurbine =
            turbineFactory.makePtrWithPowerOut(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                               mediumToLowTurbineInputOperationValue1, lowPressureHeaderInput, false);
    mediumToLowPressureTurbineIdeal =
            turbineFactory.makePtrWithPowerOut(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                               mediumToLowTurbineInputOperationValue1, lowPressureHeaderInput, true);

    //check enough available steam for set power out
    const double mediumToLowPressureTurbineMassFlow = mediumToLowPressureTurbine->getMassFlow();
    if (mediumToLowPressureTurbineMassFlow > availableMassFlow) {
        const double neededMassFlow = mediumToLowPressureTurbine->getMassFlow();
        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("mediumToLowPressureTurbine", highToLowTurbineInput,
                                                 lowPressureHeaderInput, boiler, highToLowPressureTurbine,
                                                 highToLowPressureTurbineIdeal, highPressureHeaderOutput,
                                                 neededMassFlow, availableMassFlow);
        // std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;
        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    }

//     std::cout << methodName << "mediumToLowPressureTurbine=" << mediumToLowPressureTurbine << std::endl;
//     std::cout << methodName << "mediumToLowPressureTurbineIdeal=" << mediumToLowPressureTurbineIdeal << std::endl;

    return {mediumToLowPressureTurbine, mediumToLowPressureTurbineIdeal, highToLowPressureTurbineUpdated,
            highToLowPressureTurbineIdealUpdated};
}


MediumToLowPressureTurbineCalculatorOutput
MediumToLowPressureTurbineCalculator::calcSteamFlow(const double availableMassFlow,
                                                    const PressureTurbine &mediumToLowTurbineInput,
                                                    const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput,
                                                    const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                    const Boiler &boiler,
                                                    const PressureTurbine &highToLowTurbineInput,
                                                    const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                    const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                    const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput) const {
    const std::string methodName = std::string("MediumToLowPressureTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> mediumToLowPressureTurbine = nullptr;
    std::shared_ptr<Turbine> mediumToLowPressureTurbineIdeal = nullptr;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double mediumToLowTurbineInputOperationValue1 = mediumToLowTurbineInput.getOperationValue1();

    //calculate turbine with set mass flow
    mediumToLowPressureTurbine =
            turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                               mediumToLowTurbineInputOperationValue1, lowPressureHeaderInput, false);
    mediumToLowPressureTurbineIdeal =
            turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                               mediumToLowTurbineInputOperationValue1, lowPressureHeaderInput, true);

    //check enough available steam for set mass flow
    const double mediumToLowPressureTurbineMassFlow = mediumToLowPressureTurbine->getMassFlow();
    if (mediumToLowPressureTurbineMassFlow > availableMassFlow) {
        const double neededMassFlow = mediumToLowPressureTurbine->getMassFlow();
        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("mediumToLowPressureTurbine", highToLowTurbineInput,
                                                 lowPressureHeaderInput, boiler, highToLowPressureTurbine,
                                                 highToLowPressureTurbineIdeal, highPressureHeaderOutput,
                                                 neededMassFlow, availableMassFlow);
        // std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;
        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    }

//     std::cout << methodName << "mediumToLowPressureTurbine=" << mediumToLowPressureTurbine << std::endl;
//     std::cout << methodName << "mediumToLowPressureTurbineIdeal=" << mediumToLowPressureTurbineIdeal << std::endl;

    return {mediumToLowPressureTurbine, mediumToLowPressureTurbineIdeal, highToLowPressureTurbineUpdated,
            highToLowPressureTurbineIdealUpdated};
}

MediumToLowPressureTurbineCalculatorOutput
MediumToLowPressureTurbineCalculator::calcBalanceHeader(const double availableMassFlow,
                                                        const PressureTurbine &mediumToLowTurbineInput,
                                                        const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput,
                                                        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                        const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                        const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal) const {
    const std::string methodName = std::string("MediumToLowPressureTurbineCalculator::") + std::string(__func__) + ": ";

    //balance header send through what is available
    std::shared_ptr<Turbine> mediumToLowPressureTurbine =
            turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                               availableMassFlow, lowPressureHeaderInput, false);
    std::shared_ptr<Turbine> mediumToLowPressureTurbineIdeal =
            turbineFactory.makePtrWithMassFlow(mediumPressureHeaderOutput, mediumToLowTurbineInput,
                                               availableMassFlow, lowPressureHeaderInput, true);

//     std::cout << methodName << "mediumToLowPressureTurbine=" << mediumToLowPressureTurbine << std::endl;
//     std::cout << methodName << "mediumToLowPressureTurbineIdeal=" << mediumToLowPressureTurbineIdeal << std::endl;

    return {mediumToLowPressureTurbine, mediumToLowPressureTurbineIdeal, highToLowPressureTurbine,
            highToLowPressureTurbineIdeal};
}

double MediumToLowPressureTurbineCalculator::calcAvailableMassFlow(
        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
        const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput) const {
    return mediumPressureHeaderOutput.massFlow - mediumPressureHeaderInput->getProcessSteamUsage();
}
