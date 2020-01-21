#include <ssmt/service/medium_pressure_header/ReducedSteamException.h>
#include "ssmt/service/medium_pressure_header/SteamBalanceCheckerService.h"

SteamReducerOutput
SteamBalanceCheckerService::check(const std::string &itemName, const PressureTurbine &highToLowTurbineInput,
                                  const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                  const Boiler &boiler, const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                  const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                  const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                  const double neededMassFlow, const double availableMassFlow) const {
    const std::string methodName = std::string("SteamBalanceCheckerService::") + std::string(__func__) + ": ";

    //calculate additional steam needed to meet minimum requirement
    const double additionalSteamNeeded = neededMassFlow - availableMassFlow;
    const double absAdditionalSteamNeeded = fabs(additionalSteamNeeded);

    //if need more than .0001
    if (absAdditionalSteamNeeded > 1e-3) {
        std::cout << methodName
                  << itemName << " additionalSteamNeeded=" << additionalSteamNeeded << " > " << 1e-3
                  << "; attempting to take steam needed from highToLowPressureTurbine" << std::endl;
        const SteamReducerOutput &steamReducerOutput =
                steamReducer.reduceSteamThroughHighToLowTurbine(additionalSteamNeeded, highToLowTurbineInput,
                                                                highToLowPressureTurbine, highToLowPressureTurbineIdeal,
                                                                highPressureHeaderOutput, lowPressureHeaderInput);
        std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;

        const double remainingAdditionalSteamNeeded = steamReducerOutput.remainingAdditionalSteamNeeded;

        //if high to low turbine was reduced and more steam is available
        if (remainingAdditionalSteamNeeded < additionalSteamNeeded) {
            const std::shared_ptr<Turbine> &highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
            const std::shared_ptr<Turbine> &highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;

            throw ReducedSteamException("Reduced steam from highToLowPressureTurbine for " + itemName,
                                        highToLowPressureTurbineUpdated, highToLowPressureTurbineIdealUpdated);
        } else {
            restarter.restartIfNotEnoughSteam(remainingAdditionalSteamNeeded, boiler);
            return steamReducerOutput;
        }
    } else {
        return {0, highToLowPressureTurbine, highToLowPressureTurbineIdeal};
    }
}

SteamReducerOutput
SteamBalanceCheckerService::check(const std::string &itemName, const PressureTurbine &highToLowTurbineInput,
                                  const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                  const Boiler &boiler, const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                  const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                  const std::shared_ptr<Turbine> &highToMediumPressureTurbine,
                                  const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                  const double availableMassFlow) const {
    const std::string methodName = std::string("SteamBalanceCheckerService::") + std::string(__func__) + ": ";

    //calculate additional steam needed to meet minimum requirement
    const double additionalSteamNeeded = highToMediumPressureTurbine->getMassFlow() - availableMassFlow;
    const double absAdditionalSteamNeeded = fabs(additionalSteamNeeded);

    //if need more than .0001
    if (absAdditionalSteamNeeded > 1e-3) {
        std::cout << methodName
                  << itemName << " additionalSteamNeeded=" << additionalSteamNeeded << " > " << 1e-3
                  << "; attempting to take steam needed from high to low turbine" << std::endl;
        const SteamReducerOutput &steamReducerOutput =
                steamReducer.reduceSteamThroughHighToLowTurbine(additionalSteamNeeded, highToLowTurbineInput,
                                                                highToLowPressureTurbine, highToLowPressureTurbineIdeal,
                                                                highPressureHeaderOutput, lowPressureHeaderInput);
        std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;

        const double remainingAdditionalSteamNeeded = steamReducerOutput.remainingAdditionalSteamNeeded;

        restarter.restartIfNotEnoughSteam(remainingAdditionalSteamNeeded, boiler);
        return steamReducerOutput;
    } else {
        return {0, highToLowPressureTurbine, highToLowPressureTurbineIdeal};
    }
}

void
SteamBalanceCheckerService::check(const std::shared_ptr<Turbine> &turbine, const double availableMassFlow,
                                  const Boiler &boiler) const {
    const std::string methodName = std::string("SteamBalanceCheckerService::") + std::string(__func__) + ": ";

    //check that enough mass flow is available for set amount
    const double highToLowPressureTurbineMassFlow = turbine->getMassFlow();
    if (highToLowPressureTurbineMassFlow > availableMassFlow) {
        std::cout << methodName << "highToLowPressureTurbineMassFlow=" << highToLowPressureTurbineMassFlow
                  << " > availableMassFlow=" << availableMassFlow << ", not enough power out of turbine" << std::endl;
        restarter.restartIfNotEnoughSteam(turbine, availableMassFlow, boiler);
    }
}
