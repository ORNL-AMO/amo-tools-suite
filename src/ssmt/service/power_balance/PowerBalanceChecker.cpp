#include "ssmt/service/power_balance/PowerBalanceChecker.h"

//7. Check system steam balance

PowerBalanceCheckerCalculationsDomain
PowerBalanceChecker::check(const int headerCountInput, const HeaderWithHighestPressure &highPressureHeaderInput,
                           const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                           const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                           const PressureTurbine &highToLowTurbineInput,
                           const PressureTurbine &highToMediumTurbineInput,
                           const PressureTurbine &mediumToLowTurbineInput, const BoilerInput &boilerInput,
                           const CondensingTurbine &condensingTurbineInput, const OperationsInput &operationsInput,
                           const Boiler &boiler, const std::shared_ptr<FlashTank> &blowdownFlashTank,
                           const double deaeratorInletSteamMassFlow,
                           const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                           const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                           const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain,
                           MakeupWaterAndCondensateHeaderCalculationsDomain &makeupWaterAndCondensateHeaderCalculationsDomain) const {
    const std::string methodName = std::string("PowerBalanceChecker::") + std::string(__func__) + ": ";

    std::cout << methodName << "calculating steamBalance" << std::endl;
    double steamBalance =
            steamBalanceCalculator.calc(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                        lowPressureHeaderInput, boilerInput, condensingTurbineInput, boiler,
                                        blowdownFlashTank, deaeratorInletSteamMassFlow,
                                        highPressureHeaderCalculationsDomain, mediumPressureHeaderCalculationsDomain,
                                        lowPressureHeaderCalculationsDomain);
    std::cout << methodName << "steamBalance=" << steamBalance << std::endl;

    std::shared_ptr<LowPressureVentedSteamCalculationsDomain> lowPressureVentedSteamCalculationsDomainPtr = nullptr;
    std::shared_ptr<SteamSystemModelerTool::FluidProperties> lowPressureVentedSteamPtr = nullptr;

    //TODO refactor this
    if (headerCountInput > 1 && steamBalance < 0) {
        std::cout << methodName << "headerCountInput > 1 and steamBalance < 0 so calculating lowPressureVentedSteam"
                 << std::endl;

        double deaeratorInletSteamMassFlowUpdated = deaeratorInletSteamMassFlow;

        const bool isVentingOnlyOption =
                isVentingOnlyExcessSteamOption(headerCountInput, highToLowTurbineInput, highToMediumTurbineInput,
                                               mediumToLowTurbineInput);
        if (isVentingOnlyOption) {
            const bool recalcMakeupWaterAndMassFlow = false;
            LowPressureVentedSteamCalculationsDomain lowPressureVentedSteamCalculationsDomain =
                    lowPressureVentedSteamCalculator.calc(headerCountInput, highPressureHeaderInput,
                                                          mediumPressureHeaderInput, lowPressureHeaderInput,
                                                          condensingTurbineInput, operationsInput, boilerInput, boiler,
                                                          highPressureHeaderCalculationsDomain,
                                                          mediumPressureHeaderCalculationsDomain,
                                                          lowPressureHeaderCalculationsDomain,
                                                          makeupWaterAndCondensateHeaderCalculationsDomain,
                                                          deaeratorInletSteamMassFlowUpdated,
                                                          recalcMakeupWaterAndMassFlow);
            std::cout << methodName << "lowPressureVentedSteamCalculationsDomain="
                      << lowPressureVentedSteamCalculationsDomain << std::endl;
            lowPressureVentedSteamCalculationsDomainPtr =
                    std::make_shared<LowPressureVentedSteamCalculationsDomain>(
                            lowPressureVentedSteamCalculationsDomain);
            const double lowPressureVentedSteam = lowPressureVentedSteamCalculationsDomain.lowPressureVentedSteam;
            deaeratorInletSteamMassFlowUpdated =
                    lowPressureVentedSteamCalculationsDomain.deaerator.getInletSteamProperties().massFlow;

            steamBalance += lowPressureVentedSteam;
            std::cout << methodName << "updated steamBalance=" << steamBalance
                      << " (added lowPressureVentedSteam=" << lowPressureVentedSteam << " to it)" << std::endl;
        }

        //Steam balance will be positive, vented steam amount ends up negative.
        //If it gets it close enough to zero then venting the steam will result in a balanced system.
        const double absSteamBalance = fabs(steamBalance);
        if (absSteamBalance < 1e-2) {
            std::cout << methodName << "steamBalance < " << 1e-2 << " so calculating lowPressureVentedSteam again"
                      << " with recalcMakeupWaterAndMassFlow" << std::endl;
            const bool recalcMakeupWaterAndMassFlow = true;
            LowPressureVentedSteamCalculationsDomain lowPressureVentedSteamCalculationsDomain =
                    lowPressureVentedSteamCalculator.calc(headerCountInput, highPressureHeaderInput,
                                                          mediumPressureHeaderInput, lowPressureHeaderInput,
                                                          condensingTurbineInput, operationsInput, boilerInput, boiler,
                                                          highPressureHeaderCalculationsDomain,
                                                          mediumPressureHeaderCalculationsDomain,
                                                          lowPressureHeaderCalculationsDomain,
                                                          makeupWaterAndCondensateHeaderCalculationsDomain,
                                                          deaeratorInletSteamMassFlowUpdated,
                                                          recalcMakeupWaterAndMassFlow);
            std::cout << methodName << "lowPressureVentedSteamCalculationsDomain="
                      << lowPressureVentedSteamCalculationsDomain << std::endl;
            lowPressureVentedSteamCalculationsDomainPtr =
                    std::make_shared<LowPressureVentedSteamCalculationsDomain>(
                            lowPressureVentedSteamCalculationsDomain);

            std::cout << methodName << "calculating final lowPressureVentedSteam"
                      << " from lowPressureHeaderOutput and lowPressureVentedSteam" << std::endl;
            const SteamSystemModelerTool::FluidProperties &lowPressureHeaderOutput =
                    lowPressureHeaderCalculationsDomain->lowPressureHeaderOutput;
            const double lowPressureVentedSteamAmount = lowPressureVentedSteamCalculationsDomain.lowPressureVentedSteam;
            SteamSystemModelerTool::FluidProperties lowPressureVentedSteam =
                    fluidPropertiesFactory.makeWithMassFlow(lowPressureHeaderOutput, lowPressureVentedSteamAmount);
            lowPressureVentedSteamPtr =
                    std::make_shared<SteamSystemModelerTool::FluidProperties>(lowPressureVentedSteam);
        }
    } else {
        std::cout << methodName << "condition not true (headerCountInput > 1 and steamBalance < 0)"
                  << ", so skip calculating lowPressureVentedSteam" << std::endl;
    }

    restarter.restartIfNotEnoughSteam(steamBalance, boiler);

    return {steamBalance, lowPressureVentedSteamCalculationsDomainPtr, lowPressureVentedSteamPtr};
}

/** If nowhere else to put excess steam, must vent it. */
bool
PowerBalanceChecker::isVentingOnlyExcessSteamOption(const int headerCountInput,
                                                    const PressureTurbine &highToLowTurbineInput,
                                                    const PressureTurbine &highToMediumTurbineInput,
                                                    const PressureTurbine &mediumToLowTurbineInput) const {
    const std::string methodName = std::string("PowerBalanceChecker::") + std::string(__func__) + ": ";
    bool isOnlyOption = false;

    if (headerCountInput > 1) {
        if (highToLowTurbineInput.isUseTurbine() &&
            highToLowTurbineInput.getOperationType() != PressureTurbineOperation::BALANCE_HEADER) {
            isOnlyOption = true;
        }
        if (headerCountInput == 3) {
            if (highToMediumTurbineInput.isUseTurbine() &&
                highToMediumTurbineInput.getOperationType() != PressureTurbineOperation::BALANCE_HEADER) {
                isOnlyOption = true;
            }
            if (mediumToLowTurbineInput.isUseTurbine() &&
                mediumToLowTurbineInput.getOperationType() != PressureTurbineOperation::BALANCE_HEADER) {
                isOnlyOption = true;
            }
        }
    }

    std::cout << methodName << "isOnlyOption=" << isOnlyOption << std::endl;

    return isOnlyOption;
}
