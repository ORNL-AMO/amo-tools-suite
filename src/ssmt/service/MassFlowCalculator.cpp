#include <string>
#include "ssmt/service/MassFlowCalculator.h"

double MassFlowCalculator::calcInitialMassFlow(const HeaderInput &headerInput) const {
    const std::string methodName = std::string("MassFlowCalculator::") + std::string(__func__) + ": ";

    double massFlow = 0;

    const HeaderWithHighestPressure &highPressureHeaderInput = headerInput.getHighPressureHeader();
    const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput = headerInput.getMediumPressureHeader();
    const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput = headerInput.getLowPressureHeader();

    const int headerCount = headerInput.getHeaderCount();
    switch (headerCount) {
        case 3: {
            const double mediumProcessSteamUsage = mediumPressureHeaderInput->getProcessSteamUsage();
            massFlow = addToMassFlow("mediumPressureHeaderInput", mediumProcessSteamUsage, massFlow);
        }
//            [[fallthrough]];
        case 2: {
            const double lowProcessSteamUsage = lowPressureHeaderInput->getProcessSteamUsage();
            massFlow = addToMassFlow("lowProcessSteamUsage", lowProcessSteamUsage, massFlow);
        }
//            [[fallthrough]];
        case 1: {
            const double highProcessSteamUsage = highPressureHeaderInput.getProcessSteamUsage();
            massFlow = addToMassFlow("highProcessSteamUsage", highProcessSteamUsage, massFlow);
        }
            break;
        default:
            std::string msg = methodName + "headerCount=" + std::to_string(headerCount) + " not handled";
            // std::cout << msg << std::endl;
            throw std::out_of_range(msg);
    }

    std::cout << methodName << "massFlow=" << massFlow << std::endl;

    return massFlow;
}

double
MassFlowCalculator::addToMassFlow(const std::string &objectName, const double processSteamUsage,
                                  const double massFlow) const {
    const std::string methodName = std::string("MassFlowCalculator::") + std::string(__func__) + ": ";

    double massFlowUpdated = massFlow;

    // handle NaN
    if (processSteamUsage > 0) {
        std::cout << methodName << "adding " << objectName << " processSteamUsage=" << processSteamUsage
                  << " to massFlow=" << massFlow << std::endl;
        massFlowUpdated += processSteamUsage;
    } else {
        std::cout << methodName << objectName << " processSteamUsage=" << processSteamUsage
                  << ", not adding to massFlow=" << massFlow << std::endl;
    }

    return massFlowUpdated;
}

double MassFlowCalculator::calc(const HeaderWithHighestPressure &header) const {
    const double processSteamUsage = header.getProcessSteamUsage();
    const double condensationRecoveryRate = header.getCondensationRecoveryRate();
    return calc(processSteamUsage, condensationRecoveryRate);
}

double MassFlowCalculator::calc(const std::shared_ptr<HeaderNotHighestPressure> &header) const {
    const double processSteamUsage = header->getProcessSteamUsage();
    const double condensationRecoveryRate = header->getCondensationRecoveryRate();
    return calc(processSteamUsage, condensationRecoveryRate);
}

double MassFlowCalculator::calc(const double processSteamUsage, const double condensationRecoveryRate) const {
    return processSteamUsage * (condensationRecoveryRate / 100);
}
