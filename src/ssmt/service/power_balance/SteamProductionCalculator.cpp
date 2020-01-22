#include "ssmt/service/power_balance/SteamProductionCalculator.h"

double
SteamProductionCalculator::calc(const int headerCountInput,
                                const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                const BoilerInput &boilerInput, const std::shared_ptr<FlashTank> &blowdownFlashTank,
                                const Boiler &boiler,
                                const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                                const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("SteamProductionCalculator::") + std::string(__func__) + ": ";

    std::cout << methodName << "calculating steamProduction" << std::endl;

    const double boilerOutputMassFlow = boiler.getSteamProperties().massFlow;

    double flashTankAdditionalSteam =
            calcFlashTankAdditionalSteam(headerCountInput, boilerInput,
                                         mediumPressureHeaderInput, lowPressureHeaderInput, blowdownFlashTank,
                                         highPressureHeaderCalculationsDomain,
                                         lowPressureHeaderCalculationsDomain);
    double prvAdditionalSteam =
            calcPrvAdditionalSteam(headerCountInput, mediumPressureHeaderInput, lowPressureHeaderInput,
                                   mediumPressureHeaderCalculationsDomain,
                                   lowPressureHeaderCalculationsDomain);

    //steam production = steam produced by (boiler) + (flash tanks) + (PRV feedwater)
    const double steamProduction = boilerOutputMassFlow + flashTankAdditionalSteam + prvAdditionalSteam;

    std::cout << methodName << "boilerOutputMassFlow=" << boilerOutputMassFlow
              << ", flashTankAdditionalSteam=" << flashTankAdditionalSteam
              << ", prvAdditionalSteam=" << prvAdditionalSteam
              << ": steamProduction=" << steamProduction << std::endl;

    return steamProduction;
}

/** Calc added steam from flash tanks (outletGasMassFlow). */
double
SteamProductionCalculator::calcFlashTankAdditionalSteam(const int headerCountInput, const BoilerInput &boilerInput,
                                                        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                        const std::shared_ptr<FlashTank> &blowdownFlashTank,
                                                        const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                                        const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("SteamProductionCalculator::") + std::string(__func__) + ": ";

    std::cout << methodName << "calculating flashTankAdditionalSteam" << std::endl;

    double flashTankAdditionalSteam = 0;

    if (headerCountInput > 1) {
        const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank =
                highPressureHeaderCalculationsDomain.highPressureCondensateFlashTank;

        //steam added by blowdown flash tank
        if (boilerInput.isBlowdownFlashed()) {
            const SteamSystemModelerTool::FluidProperties &outletGasSaturatedProperties =
                    blowdownFlashTank->getOutletGasSaturatedProperties();
            const double massFlow = outletGasSaturatedProperties.massFlow;
            std::cout << methodName
                     << "boiler blowdown is flashed, adding blowdownFlashTank massFlow=" << massFlow << std::endl;
            flashTankAdditionalSteam += massFlow;
        } else {
            std::cout << methodName << "blowdown is not flashed, not adding blowdownFlashTank massFlow" << std::endl;
        }

        //steam added by flash tank into low pressure header
        if (lowPressureHeaderInput->isFlashCondensate()) {
            if (headerCountInput == 2) {
                const SteamSystemModelerTool::FluidProperties &outletGasSaturatedProperties =
                        highPressureCondensateFlashTank->getOutletGasSaturatedProperties();
                const double massFlow = outletGasSaturatedProperties.massFlow;
                std::cout << methodName
                          << "lowPressureHeader condensate is flashed and headerCountInput==2"
                          << ", adding highPressureCondensateFlashTank massFlow=" << massFlow << std::endl;
                flashTankAdditionalSteam += massFlow;
            } else if (headerCountInput == 3) {
                const LowPressureFlashedSteamIntoHeaderCalculatorDomain &lowPressureFlashedSteamIntoHeaderCalculatorDomain =
                        lowPressureHeaderCalculationsDomain->lowPressureFlashedSteamIntoHeaderCalculatorDomain;
                const std::shared_ptr<FlashTank> &mediumPressureCondensateFlashTank =
                        lowPressureFlashedSteamIntoHeaderCalculatorDomain.mediumPressureCondensateFlashTank;
                const SteamSystemModelerTool::FluidProperties &outletGasSaturatedProperties =
                        mediumPressureCondensateFlashTank->getOutletGasSaturatedProperties();
                const double massFlow = outletGasSaturatedProperties.massFlow;
                std::cout << methodName
                          << "lowPressureHeader condensate is flashed and headerCountInput==3"
                          << ", adding mediumPressureCondensateFlashTank massFlow=" << massFlow << std::endl;
                flashTankAdditionalSteam += massFlow;
            }
        } else {
            std::cout << methodName
                      << "lowPressureHeader condensate is not flashed, not adding highPressureCondensateFlashTank massFlow"
                      << std::endl;
        }

        if (headerCountInput == 3) {
            //steam added by flash tank into medium pressure header
            if (mediumPressureHeaderInput->isFlashCondensate()) {
                const SteamSystemModelerTool::FluidProperties &outletGasSaturatedProperties =
                        highPressureCondensateFlashTank->getOutletGasSaturatedProperties();
                const double massFlow = outletGasSaturatedProperties.massFlow;
                std::cout << methodName
                          << "headerCountInput==3, adding highPressureCondensateFlashTank massFlow=" << massFlow
                          << std::endl;
                flashTankAdditionalSteam += massFlow;
            }
        }
    } else {
        std::cout << methodName << "headerCountInput=" << headerCountInput
                  << " (not > 1), not adding boiler, medium, or high pressureCondensateFlashTank massFlow" << std::endl;
    }

    std::cout << methodName << "flashTankAdditionalSteam=" << flashTankAdditionalSteam << std::endl;

    return flashTankAdditionalSteam;
}

/** Calc added steam from PRVs (outletMassFlow - inletMassFlow). */
double
SteamProductionCalculator::calcPrvAdditionalSteam(const int headerCountInput,
                                                  const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                  const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                  const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                                                  const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("SteamProductionCalculator::") + std::string(__func__) + ": ";

    std::cout << methodName << "calculating prvAdditionalSteam" << std::endl;

    double prvAdditionalSteam = 0;

    if (headerCountInput > 1) {
        //steam added by desuperheating feedwater
        if (lowPressureHeaderInput->isDesuperheatSteamIntoNextHighest()) {
            std::cout << methodName
                      << "headerCountInput > 1 and lowPressureHeader isDesuperheatSteamIntoNextHighest is true"
                      << ", adding lowPressurePrv massFlow" << std::endl;
            const std::shared_ptr<PrvWithoutDesuperheating> &lowPressurePrv =
                    lowPressureHeaderCalculationsDomain->lowPressurePrv;
            prvAdditionalSteam = addPrvMassFlow(prvAdditionalSteam, lowPressurePrv);
        } else {
            std::cout << methodName
                      << "headerCountInput not > 1 or lowPressureHeader isDesuperheatSteamIntoNextHighest is false"
                      << ", not adding lowPressurePrv massFlow" << std::endl;
        }

        if (headerCountInput == 3) {
            //steam added by desuperheating feedwater
            if (mediumPressureHeaderInput->isDesuperheatSteamIntoNextHighest()) {
                std::cout << methodName
                          << "headerCountInput == 3 and mediumPressureHeader isDesuperheatSteamIntoNextHighest is true"
                          << ", adding highToMediumPressurePrv massFlow" << std::endl;
                const std::shared_ptr<PrvWithoutDesuperheating> &highToMediumPressurePrv =
                        mediumPressureHeaderCalculationsDomain->highToMediumPressurePrv;
                prvAdditionalSteam = addPrvMassFlow(prvAdditionalSteam, highToMediumPressurePrv);
            } else {
                std::cout << methodName
                          << "headerCountInput not == 3 or mediumPressureHeader isDesuperheatSteamIntoNextHighest is false"
                          << ", not adding highToMediumPressurePrv massFlow" << std::endl;
            }
        }
    } else {
        std::cout << methodName << "headerCountInput=" << headerCountInput
                  << " (not > 1), no PRVs to calc additional steam for" << std::endl;
    }

    std::cout << methodName << "prvAdditionalSteam=" << prvAdditionalSteam << std::endl;

    return prvAdditionalSteam;
}

double SteamProductionCalculator::addPrvMassFlow(double prvAdditionalSteam,
                                                 const std::shared_ptr<PrvWithoutDesuperheating> &prv) const {
    const std::string methodName = std::string("SteamProductionCalculator::") + std::string(__func__) + ": ";

    const double outletMassFlow = prv->getOutletMassFlow();
    const double inletMassFlow = prv->getInletMassFlow();
    const double diff = outletMassFlow - inletMassFlow;
    const double result = prvAdditionalSteam + diff;

    std::cout << methodName << "outletMassFlow=" << outletMassFlow << ", inletMassFlow=" << inletMassFlow
             << ", diff=" << diff << ": result=" << result << std::endl;

    return result;
}
