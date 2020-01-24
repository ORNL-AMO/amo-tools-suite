#include "ssmt/service/SteamModelCalculator.h"

SteamModelCalculationsDomain
SteamModelCalculator::calc(const bool isBaselineCalc, const double baselinePowerDemand, const HeaderInput &headerInput,
                           const BoilerInput &boilerInput, const TurbineInput &turbineInput,
                           const OperationsInput &operationsInput, const double initialMassFlow) const
{
        const std::string methodName = "SteamModelCalculator::" + std::string(__func__) + ": ";

        const HeaderWithHighestPressure &highPressureHeaderInput = headerInput.getHighPressureHeader();
        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput = headerInput.getMediumPressureHeader();
        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput = headerInput.getLowPressureHeader();
        const int headerCountInput = headerInput.getHeaderCount();

        const PressureTurbine &highToLowTurbineInput = turbineInput.getHighToLowTurbine();
        const PressureTurbine &highToMediumTurbineInput = turbineInput.getHighToMediumTurbine();
        const CondensingTurbine &condensingTurbineInput = turbineInput.getCondensingTurbine();
        const PressureTurbine &mediumToLowTurbineInput = turbineInput.getMediumToLowTurbine();

        std::cout << methodName << "calculating boiler" << std::endl;
        const Boiler &boiler = boilerFactory.make(headerInput, boilerInput, initialMassFlow);
        std::cout << methodName << "boiler=" << boiler << std::endl;

        std::cout << methodName << "calculating blowdownFlashTank" << std::endl;
        const std::shared_ptr<FlashTank> &blowdownFlashTank = flashTankFactory.make(headerInput, boilerInput, boiler);
        std::cout << methodName << "blowdownFlashTank=" << blowdownFlashTank << std::endl;

        std::cout << methodName << "running highPressureHeaderModeler" << std::endl;
        HighPressureHeaderCalculationsDomain highPressureHeaderCalculationsDomain =
            highPressureHeaderModeler.model(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                            lowPressureHeaderInput, highToMediumTurbineInput, highToLowTurbineInput,
                                            condensingTurbineInput, boiler);
        std::cout << methodName << "highPressureHeaderCalculationsDomain=" << highPressureHeaderCalculationsDomain
                  << std::endl;

        std::cout << methodName << "running mediumPressureHeaderModeler" << std::endl;
        const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain =
            mediumPressureHeaderModeler.model(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                              lowPressureHeaderInput, highToMediumTurbineInput, highToLowTurbineInput,
                                              mediumToLowTurbineInput, condensingTurbineInput, boiler,
                                              highPressureHeaderCalculationsDomain);
        std::cout << methodName << "mediumPressureHeaderCalculationsDomain=" << mediumPressureHeaderCalculationsDomain
                  << std::endl;

        std::cout << methodName << "running lowPressureHeaderModeler" << std::endl;
        const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain =
            lowPressureHeaderModeler.model(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                           lowPressureHeaderInput, highToLowTurbineInput, mediumToLowTurbineInput,
                                           condensingTurbineInput, boilerInput, boiler, blowdownFlashTank,
                                           highPressureHeaderCalculationsDomain,
                                           mediumPressureHeaderCalculationsDomain);
        std::cout << methodName << "lowPressureHeaderCalculationsDomain=" << lowPressureHeaderCalculationsDomain
                  << std::endl;

        std::cout << methodName << "running makeupWaterAndCondensateHeaderModeler" << std::endl;
        MakeupWaterAndCondensateHeaderCalculationsDomain makeupWaterAndCondensateHeaderCalculationsDomain =
            makeupWaterAndCondensateHeaderModeler.model(headerCountInput, highPressureHeaderInput,
                                                        mediumPressureHeaderInput, lowPressureHeaderInput, boilerInput,
                                                        operationsInput, condensingTurbineInput, boiler,
                                                        blowdownFlashTank, highPressureHeaderCalculationsDomain,
                                                        mediumPressureHeaderCalculationsDomain,
                                                        lowPressureHeaderCalculationsDomain);
        std::cout << methodName << "makeupWaterAndCondensateHeaderCalculationsDomain="
                  << makeupWaterAndCondensateHeaderCalculationsDomain << std::endl;

        std::cout << methodName << "running deaeratorModeler" << std::endl;
        Deaerator deaerator =
            deaeratorModeler.model(headerCountInput, boilerInput, boiler, highPressureHeaderCalculationsDomain,
                                   mediumPressureHeaderCalculationsDomain, lowPressureHeaderCalculationsDomain,
                                   makeupWaterAndCondensateHeaderCalculationsDomain);
        std::cout << methodName << "deaerator=" << deaerator << std::endl;

        // if (headerCountInput > 2)
        // {
                const double lowPressureSteamNeed = lowPressureHeaderInput->getProcessSteamUsage() + deaerator.getFeedwaterMassFlow() - lowPressureHeaderCalculationsDomain->lowPressureFlashedSteamIntoHeaderCalculatorDomain.mediumPressureCondensateFlashTank->getOutletGasSaturatedProperties().massFlow - blowdownFlashTank->getOutletGasSaturatedProperties().massFlow;
                std::cout << "LOW PRESSURE STEAM NEED!@#!@#: " << lowPressureSteamNeed << std::endl;
        // }

        std::cout << methodName << "running powerBalanceChecker" << std::endl;
        const double deaeratorInletSteamMassFlow = deaerator.getInletSteamProperties().massFlow;
        const PowerBalanceCheckerCalculationsDomain &powerBalanceCheckerCalculationsDomain =
            powerBalanceChecker.check(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                      lowPressureHeaderInput, highToLowTurbineInput, highToMediumTurbineInput,
                                      mediumToLowTurbineInput, boilerInput, condensingTurbineInput, operationsInput,
                                      boiler, blowdownFlashTank, deaeratorInletSteamMassFlow,
                                      highPressureHeaderCalculationsDomain, mediumPressureHeaderCalculationsDomain,
                                      lowPressureHeaderCalculationsDomain,
                                      makeupWaterAndCondensateHeaderCalculationsDomain);
        std::cout << methodName << "powerBalanceCheckerCalculationsDomain=" << powerBalanceCheckerCalculationsDomain
                  << std::endl;

        const std::shared_ptr<LowPressureVentedSteamCalculationsDomain> &lowPressureVentedSteamCalculationsDomain =
            powerBalanceCheckerCalculationsDomain.lowPressureVentedSteamCalculationsDomain;
        if (lowPressureVentedSteamCalculationsDomain != nullptr)
        {
                deaerator = lowPressureVentedSteamCalculationsDomain->deaerator;
        }

        std::cout << methodName << "running processSteamUsageCalculator" << std::endl;
        const ProcessSteamUsageCalculationsDomain &processSteamUsageCalculationsDomain =
            processSteamUsageModeler.model(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                           lowPressureHeaderInput, highPressureHeaderCalculationsDomain,
                                           mediumPressureHeaderCalculationsDomain, lowPressureHeaderCalculationsDomain);
        std::cout << methodName << "processSteamUsageCalculationsDomain=" << processSteamUsageCalculationsDomain
                  << std::endl;

        std::cout << methodName << "running energyAndCostCalculator" << std::endl;
        const MakeupWaterVolumeFlowCalculationsDomain &makeupWaterVolumeFlowCalculationsDomain =
            makeupWaterAndCondensateHeaderCalculationsDomain.makeupWaterVolumeFlowCalculationsDomain;
        const double makeupWaterVolumeFlowAnnual =
            makeupWaterVolumeFlowCalculationsDomain.makeupWaterVolumeFlowAnnual;
        const EnergyAndCostCalculationsDomain &energyAndCostCalculationsDomain =
            energyAndCostCalculator.calc(isBaselineCalc, baselinePowerDemand, operationsInput, boiler,
                                         highPressureHeaderCalculationsDomain, mediumPressureHeaderCalculationsDomain,
                                         makeupWaterVolumeFlowAnnual);
        std::cout << methodName << "energyAndCostCalculationsDomain=" << energyAndCostCalculationsDomain << std::endl;

        return {boiler, blowdownFlashTank, highPressureHeaderCalculationsDomain, mediumPressureHeaderCalculationsDomain,
                lowPressureHeaderCalculationsDomain, makeupWaterAndCondensateHeaderCalculationsDomain, deaerator,
                powerBalanceCheckerCalculationsDomain, processSteamUsageCalculationsDomain,
                energyAndCostCalculationsDomain};
}
