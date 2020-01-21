#include "ssmt/service/SteamReducer.h"

SteamReducerOutput
SteamReducer::reduceSteamThroughHighToLowTurbine(const double additionalSteamNeeded,
                                                 const PressureTurbine &highToLowTurbineInput,
                                                 const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                 const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                                 const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                 const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput) const {
    const std::string methodName = std::string("SteamReducer::") + std::string(__func__) + ": ";

    SteamReducerOutput steamReducerOutput;

    //if the turbine is in use
    if (highToLowTurbineInput.isUseTurbine()) {
        const PressureTurbineOperation &pressureTurbineOperation = highToLowTurbineInput.getOperationType();
        std::cout << methodName << "pressureTurbineOperation=" << pressureTurbineOperation << std::endl;

        switch (pressureTurbineOperation) {
            case PressureTurbineOperation::FLOW_RANGE:
                steamReducerOutput =
                        reduceFlowRange(additionalSteamNeeded, highToLowTurbineInput, highToLowPressureTurbine,
                                        highToLowPressureTurbineIdeal, highPressureHeaderOutput,
                                        lowPressureHeaderInput);
                break;
            case PressureTurbineOperation::POWER_RANGE:
                steamReducerOutput =
                        reducePowerRange(additionalSteamNeeded, highToLowTurbineInput, highToLowPressureTurbine,
                                         highToLowPressureTurbineIdeal, highPressureHeaderOutput,
                                         lowPressureHeaderInput);
                break;
            case PressureTurbineOperation::POWER_GENERATION:
                std::cout << methodName << "pressureTurbineOperation is POWER_GENERATION,"
                         << " skipping reducing as fixed steam cannot reduce steam through turbine" << std::endl;
                steamReducerOutput = {additionalSteamNeeded, highToLowPressureTurbine, highToLowPressureTurbineIdeal};
                break;
            case PressureTurbineOperation::STEAM_FLOW:
                std::cout << methodName << "pressureTurbineOperation is STEAM_FLOW,"
                         << " skipping reducing as fixed steam cannot reduce steam through turbine" << std::endl;
                steamReducerOutput = {additionalSteamNeeded, highToLowPressureTurbine, highToLowPressureTurbineIdeal};
                break;
            case PressureTurbineOperation::BALANCE_HEADER:
                steamReducerOutput =
                        reduceBalanceHeader(additionalSteamNeeded, highToLowTurbineInput, highToLowPressureTurbine,
                                            highToLowPressureTurbineIdeal, highPressureHeaderOutput,
                                            lowPressureHeaderInput);
                break;
            default:
                std::string msg = methodName + "PressureTurbineOperation enum not handled";
                std::cout << msg << std::endl;
                throw std::invalid_argument(msg);
        }
    } else {
        std::cout << methodName << "high to low turbine not provided, skipping reducing" << std::endl;
        steamReducerOutput = {additionalSteamNeeded, highToLowPressureTurbine, highToLowPressureTurbineIdeal};
    }

    std::cout << methodName
             << "remainingAdditionalSteamNeeded=" << steamReducerOutput.remainingAdditionalSteamNeeded << std::endl;

    return steamReducerOutput;
}

SteamReducerOutput
SteamReducer::reduceBalanceHeader(const double additionalSteamNeeded,
                                  const PressureTurbine &highToLowTurbineInput,
                                  const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                  const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                  const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                  const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput) const {
    const std::string methodName = std::string("SteamReducer::") + std::string(__func__) + ": ";

    double remainingAdditionalSteamNeeded = additionalSteamNeeded;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    //balance header, all steam is available to be taken
    const double availableSteam = highToLowPressureTurbine->getMassFlow();
    const double remainingSteam = availableSteam - additionalSteamNeeded;
    std::cout << methodName << "availableSteam=" << availableSteam << " - additionalSteamNeeded="
             << additionalSteamNeeded << "; resulting remainingSteam=" << remainingSteam << std::endl;

    double massFlow = remainingSteam;
    //if all steam can be taken,
    if (remainingSteam >= 0) {
        //all additional steam needed was taken from turbine, no additional steam needed
        remainingAdditionalSteamNeeded = 0;
    } else {
        //take all steam from turbine
        massFlow = 0;
        //steam still needed = needed steam - amount taken (which is everything)
        remainingAdditionalSteamNeeded = additionalSteamNeeded - availableSteam;
    }

    std::cout << methodName << "remainingAdditionalSteamNeeded=" << remainingAdditionalSteamNeeded << std::endl;

    std::cout << methodName
             << "calculating highToLowPressureTurbine with massFlow=" << massFlow << std::endl;
    highToLowPressureTurbineUpdated =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput, massFlow,
                                               lowPressureHeaderInput, false);
    std::cout << methodName << "highToLowPressureTurbineUpdated=" << highToLowPressureTurbineUpdated << std::endl;

    std::cout << methodName
             << "calculating highToLowPressureTurbineIdeal with massFlow=" << massFlow << std::endl;
    highToLowPressureTurbineIdealUpdated =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput, massFlow,
                                               lowPressureHeaderInput, true);
    std::cout << methodName << "highToLowPressureTurbineIdealUpdated=" << highToLowPressureTurbineIdealUpdated
             << std::endl;

    return {remainingAdditionalSteamNeeded, highToLowPressureTurbineUpdated, highToLowPressureTurbineIdealUpdated};
}

SteamReducerOutput
SteamReducer::reducePowerRange(const double additionalSteamNeeded,
                               const PressureTurbine &highToLowTurbineInput,
                               const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                               const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                               const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                               const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput) const {
    const std::string methodName = std::string("SteamReducer::") + std::string(__func__) + ": ";

    double remainingAdditionalSteamNeeded = additionalSteamNeeded;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    //if current power out is greater than the minimum amount needed there is steam available
    const double highToLowPressureTurbinePowerOut = highToLowPressureTurbine->getPowerOut();
    const double highToLowTurbineInputOperationValue1 = highToLowTurbineInput.getOperationValue1();

    if (highToLowPressureTurbinePowerOut > highToLowTurbineInputOperationValue1) {
        //calculate amount of steam taken when reducing power out to minimum
        const double previousMassFlow = highToLowPressureTurbine->getMassFlow();

        //calculate header using minimum power out needed
        std::cout << methodName
                  << "calculating highToLowPressureTurbine with minimum power out needed (highToLowTurbineInputOperationValue1)="
                  << highToLowTurbineInputOperationValue1 << std::endl;
        highToLowPressureTurbineUpdated =
                turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToLowTurbineInput,
                                                   highToLowTurbineInputOperationValue1, lowPressureHeaderInput, false);
        std::cout << methodName << "highToLowPressureTurbineUpdated=" << highToLowPressureTurbineUpdated << std::endl;

        std::cout << methodName
                  << "calculating highToLowPressureTurbineIdeal with minimum power out needed (highToLowTurbineInputOperationValue1)="
                  << highToLowTurbineInputOperationValue1 << std::endl;
        highToLowPressureTurbineIdealUpdated =
                turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToLowTurbineInput,
                                                   highToLowTurbineInputOperationValue1, lowPressureHeaderInput, true);
        std::cout << methodName << "highToLowPressureTurbineIdealUpdated=" << highToLowPressureTurbineIdealUpdated
                  << std::endl;

        //amount reduced = previous mass flow - mass flow at min need
        const double highToLowPressureTurbineMassFlow = highToLowPressureTurbineUpdated->getMassFlow();
        const double massFlowReduction = previousMassFlow - highToLowPressureTurbineMassFlow;

        //amount of steam take = needed steam - amount reduced
        const double newSteamNeed = additionalSteamNeeded - massFlowReduction;

        //if excess amount of steam was taken than needed when reducing, put excess steam taken back into turbine
        if (newSteamNeed < 0) {
            const double massFlow = highToLowPressureTurbineMassFlow + fabs(newSteamNeed);
            std::cout << methodName
                      << "calculating highToLowPressureTurbine, returning excess steam amount, with mass flow="
                      << massFlow << std::endl;
            highToLowPressureTurbineUpdated =
                    turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput, massFlow,
                                                       lowPressureHeaderInput, false);
            std::cout << methodName << "highToLowPressureTurbineUpdated=" << highToLowPressureTurbineUpdated << std::endl;

            std::cout << methodName
                      << "calculating highToLowPressureTurbineIdeal, returning excess steam amount, with mass flow="
                      << massFlow << std::endl;
            highToLowPressureTurbineIdealUpdated =
                    turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput, massFlow,
                                                       lowPressureHeaderInput, true);
            std::cout << methodName << "highToLowPressureTurbineIdealUpdated=" << highToLowPressureTurbineIdealUpdated
                      << std::endl;

            remainingAdditionalSteamNeeded = 0;
        } else {
            //return new steam need
            remainingAdditionalSteamNeeded = newSteamNeed;
        }
    } else {
        //no steam available from turbine
        remainingAdditionalSteamNeeded = additionalSteamNeeded;
    }

    return {remainingAdditionalSteamNeeded, highToLowPressureTurbineUpdated, highToLowPressureTurbineIdealUpdated};
}

SteamReducerOutput
SteamReducer::reduceFlowRange(const double additionalSteamNeeded,
                              const PressureTurbine &highToLowTurbineInput,
                              const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                              const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                              const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                              const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput) const {
    const std::string methodName = std::string("SteamReducer::") + std::string(__func__) + ": ";

    double remainingAdditionalSteamNeeded = additionalSteamNeeded;
    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

    const double highToLowPressureTurbineMassFlow = highToLowPressureTurbine->getMassFlow();
    const double highToLowTurbineInputOperationValue1 = highToLowTurbineInput.getOperationValue1();

    //if current mass flow is greater than minimum needed
    if (highToLowPressureTurbineMassFlow > highToLowTurbineInputOperationValue1) {
        //calculate amount of steam that can be taken
        const double currentMassFlow = highToLowPressureTurbineMassFlow;

        //calculate turbine at minimum value
        std::cout << methodName
                  << "calculating highToLowPressureTurbine with amount needed (highToLowTurbineInputOperationValue1)="
                  << highToLowTurbineInputOperationValue1 << std::endl;
        highToLowPressureTurbineUpdated =
                turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput,
                                                   highToLowTurbineInputOperationValue1, lowPressureHeaderInput, false);
        std::cout << methodName << "highToLowPressureTurbineUpdated=" << highToLowPressureTurbineIdealUpdated
                  << std::endl;

        std::cout << methodName
                  << "calculating highToLowPressureTurbineIdeal with amount needed (highToLowTurbineInputOperationValue1)="
                  << highToLowTurbineInputOperationValue1 << std::endl;
        highToLowPressureTurbineIdealUpdated =
                turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput,
                                                   highToLowTurbineInputOperationValue1, lowPressureHeaderInput, true);
        std::cout << methodName << "highToLowPressureTurbineIdealUpdated=" << highToLowPressureTurbineIdealUpdated
                  << std::endl;

        //calculate amount of mass flow reduced
        const double massFlowReduction = currentMassFlow - highToLowPressureTurbineUpdated->getMassFlow();

        //calculate new steam need
        double newSteamNeed = additionalSteamNeeded - massFlowReduction;

        //if more steam taken than needed when reducing, put excess steam taken back into turbine
        if (newSteamNeed < 0) {
            const double massFlow = highToLowPressureTurbineUpdated->getMassFlow() + fabs(newSteamNeed);
            std::cout << methodName
                      << "calculating highToLowPressureTurbine, returning excess steam amount, with mass flow="
                      << massFlow << std::endl;
            highToLowPressureTurbineUpdated =
                    turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput,
                                                       massFlow, lowPressureHeaderInput, false);
            std::cout << methodName << "highToLowPressureTurbineUpdated=" << highToLowPressureTurbineUpdated
                      << std::endl;

            std::cout << methodName
                      << "calculating highToLowPressureTurbineIdeal, returning excess steam amount, with mass flow="
                      << massFlow << std::endl;
            highToLowPressureTurbineIdealUpdated =
                    turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput,
                                                       massFlow, lowPressureHeaderInput, true);
            std::cout << methodName << "highToLowPressureTurbineIdealUpdated=" << highToLowPressureTurbineIdealUpdated
                     << std::endl;

            remainingAdditionalSteamNeeded = 0;
        } else {
            remainingAdditionalSteamNeeded = newSteamNeed;
        }
    }

    return {remainingAdditionalSteamNeeded, highToLowPressureTurbineUpdated, highToLowPressureTurbineIdealUpdated};
}
