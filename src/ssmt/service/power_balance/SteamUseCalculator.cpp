#include "ssmt/service/power_balance/SteamUseCalculator.h"

double
SteamUseCalculator::calc(const int headerCountInput, const HeaderWithHighestPressure &highPressureHeaderInput,
                         const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                         const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                         const double deaeratorInletSteamMassFlow, const CondensingTurbine &condensingTurbineInput,
                         const std::shared_ptr<Turbine> &condensingTurbine) const {
    const std::string methodName = std::string("SteamUseCalculator::") + std::string(__func__) + ": ";

    // std::cout << methodName << "calculating steamUse" << std::endl;

    //steam use = steam used by (header process usage) + (deaerator) + (condensing turbine)
    //steam used by condensing turbine
    double processSteamUsage =
            calcProcessSteamUsage(headerCountInput, highPressureHeaderInput, lowPressureHeaderInput,
                                  mediumPressureHeaderInput);
    const double condensingTurbineMassFlow =
            condensingTurbineInput.isUseTurbine() ? condensingTurbine->getMassFlow() : 0;

    const double steamUse = processSteamUsage + deaeratorInletSteamMassFlow + condensingTurbineMassFlow;
    // std::cout << methodName << "processSteamUsage=" << processSteamUsage
            //   << " + deaeratorInletSteamMassFlow=" << deaeratorInletSteamMassFlow
            //   << " + condensingTurbineMassFlow=" << condensingTurbineMassFlow
            //   << ": steamUse=" << steamUse << std::endl;

    return steamUse;
}

/** Calc amount of steam used by the system. */
double
SteamUseCalculator::calcProcessSteamUsage(const int headerCountInput,
                                          const HeaderWithHighestPressure &highPressureHeaderInput,
                                          const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                          const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput) const {
    const std::string methodName = std::string("SteamUseCalculator::") + std::string(__func__) + ": ";

    // std::cout << methodName << "calculating steamUse" << std::endl;

    double processSteamUsage = highPressureHeaderInput.getProcessSteamUsage();

    // std::cout << methodName << "highPressureHeaderInput processSteamUsage=" << processSteamUsage << std::endl;

    if (headerCountInput > 1) {
        //steam used by low pressure header
        const double lowPressureProcessSteamUsage = lowPressureHeaderInput->getProcessSteamUsage();
        // std::cout << methodName << "headerCountInput > 1, adding lowPressureProcessSteamUsage="
                //   << lowPressureProcessSteamUsage << std::endl;

        processSteamUsage += lowPressureProcessSteamUsage;

        if (headerCountInput == 3) {
            const double mediumPressureProcessSteamUsage = mediumPressureHeaderInput->getProcessSteamUsage();
            // std::cout << methodName << "headerCountInput == 3, adding mediumPressureProcessSteamUsage="
                    //   << mediumPressureProcessSteamUsage << std::endl;
            //steam used by medium pressure header
            processSteamUsage += mediumPressureProcessSteamUsage;
        }
    } else {
        // std::cout << methodName << "headerCountInput=" << headerCountInput
                //   << " (not > 1), not adding low or medium pressureProcessSteamUsage" << std::endl;
    }

    // std::cout << methodName << "processSteamUsage=" << processSteamUsage << std::endl;

    return processSteamUsage;
}
