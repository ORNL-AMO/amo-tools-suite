#include <ssmt/service/RestarterService.h>
#include <ssmt/service/SteamBalanceException.h>

void RestarterService::restartIfNotEnoughSteam(const std::shared_ptr<Turbine> &turbine, const double availableMassFlow,
                                               const Boiler &boiler) const {
    const std::string methodName =
            std::string("RestarterService::") + std::string(__func__) + "(turbine, steamAvailable, boiler): ";

    // additional steam needed = amount needed - current amount
    const double neededMassFlow = turbine->getMassFlow();
    const double additionalSteamNeeded = neededMassFlow - availableMassFlow;

    std::cout << methodName
              << "neededMassFlow=" << neededMassFlow << ", availableMassFlow=" << availableMassFlow
              << ", additionalSteamNeeded=" << additionalSteamNeeded << std::endl;

    restartIfNotEnoughSteam(additionalSteamNeeded, boiler);
}

void RestarterService::restartIfNotEnoughSteam(const double additionalSteamNeeded, const Boiler &boiler) const {
    const std::string methodName = std::string("RestarterService::") + std::string(__func__) + "(steamNeed, boiler): ";

    if (std::isnan(additionalSteamNeeded)) {
        std::string msg =
                methodName + "Internal Error: additionalSteamNeeded=" + std::to_string(additionalSteamNeeded) +
                ", cannot continue";
        logMessage(msg);
        throw std::runtime_error(msg);
    }

    const double absAdditionalSteamNeeded = fabs(additionalSteamNeeded);

    const double tolerance = 1e-3;

    std::cout << methodName
              << "checking if the steam amount shortage is within tolerance; additionalSteamNeeded="
              << additionalSteamNeeded << "; tolerance=+-" << tolerance << std::endl;

    //if need more than .0001
    if (absAdditionalSteamNeeded > tolerance) {
        logMessage(
                "additionalSteamNeeded=" + std::to_string(additionalSteamNeeded) + " is outside tolerance=" +
                std::to_string(tolerance) + "; starting over with adjusted steam starting value");
        //re-run model with additional needed steam added
        const double steamMassFlow = boiler.getSteamMassFlow();
        const double adjustedSteam = steamMassFlow + additionalSteamNeeded;
        logMessage("steamMassFlow=" + std::to_string(steamMassFlow) + ", additionalSteamNeeded=" +
                   std::to_string(additionalSteamNeeded) + ", adjustedSteam=" +
                   std::to_string(adjustedSteam));
        throw SteamBalanceException(additionalSteamNeeded, adjustedSteam);
    } else {
        logMessage(
                "additionalSteamNeeded=" + std::to_string(additionalSteamNeeded) + " is within tolerance=" +
                std::to_string(tolerance) + "; continuing/not restarting");
    }
}

void RestarterService::logMessage(const std::string &message) const {
    const std::string delimeter = "======== ";
    std::cout << delimeter << std::endl;
    std::cout << delimeter << message << std::endl;
    std::cout << delimeter << std::endl;
}
