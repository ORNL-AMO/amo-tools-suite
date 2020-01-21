#include "ssmt/service/SteamModelRunner.h"

SteamModelCalculationsDomain
SteamModelRunner::run(const bool isBaselineCalc, const double baselinePowerDemand, const HeaderInput &headerInput,
                      const BoilerInput &boilerInput, const TurbineInput &turbineInput,
                      const OperationsInput &operationsInput) const {
    const std::string methodName = std::string("SteamModelRunner::") + std::string(__func__) + ": ";

    // adjust max iterations as desired; mainly to prevent runaway modeling from unexpected issues
    const int maxIterationCount = 25;

    double initialMassFlow = massFlowCalculator.calcInitialMassFlow(headerInput);

    int iterationCount = 0;
    while (iterationCount < maxIterationCount) {
        iterationCount++;
        logSection(methodName + "iterationCount=" + std::to_string(iterationCount));

        try {
            return steamModelCalculator.calc(isBaselineCalc, baselinePowerDemand, headerInput, boilerInput,
                                             turbineInput, operationsInput, initialMassFlow);
        } catch (const SteamBalanceException &e) {
            initialMassFlow = handleSteamBalanceException(e, iterationCount, initialMassFlow);
        }
    }

    std::string msg =
            methodName + "ran " + std::to_string(maxIterationCount) + " times and did not balance system, aborting";
    std::cout << msg << std::endl;
    throw std::logic_error(msg);
}

double SteamModelRunner::handleSteamBalanceException(const SteamBalanceException &e, const int iterationCount,
                                                     const double initialMassFlow) const {
    const std::string methodName = std::string("SteamModelRunner::") + std::string(__func__) + ": ";

    const double adjustedInitialSteam = e.getAdjustedInitialSteam();
    std::cout << methodName << e << std::endl;
    std::cout << methodName << "iterationCount=" << iterationCount
              << " with initialMassFlow=" << initialMassFlow
              << " failed; trying again with initialMassFlow=" << adjustedInitialSteam
              << std::endl;
    return adjustedInitialSteam;
}


void SteamModelRunner::logSection(const std::string &message) const {
    std::cout << "-------- " << std::endl;
    std::cout << "-------- " << message << std::endl;
    std::cout << "-------- " << std::endl;
}
