#include "ssmt/service/power_balance/SteamBalanceCalculator.h"

//7. Calculate the balance of the system

double
SteamBalanceCalculator::calc(const int headerCountInput, const HeaderWithHighestPressure &highPressureHeaderInput,
                             const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                             const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                             const BoilerInput &boilerInput, const CondensingTurbine &condensingTurbineInput,
                             const Boiler &boiler, const std::shared_ptr<FlashTank> &blowdownFlashTank,
                             const double deaeratorInletSteamMassFlow,
                             const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                             const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                             const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("SteamBalanceCalculator::") + std::string(__func__) + ": ";


    const std::shared_ptr<Turbine> &condensingTurbine =
            highPressureHeaderCalculationsDomain.condensingTurbine;

    const double steamUse =
            steamUseCalculator.calc(headerCountInput, highPressureHeaderInput, lowPressureHeaderInput,
                                    mediumPressureHeaderInput, deaeratorInletSteamMassFlow, condensingTurbineInput,
                                    condensingTurbine);

    const double steamProduction =
            steamProductionCalculator.calc(headerCountInput, mediumPressureHeaderInput, lowPressureHeaderInput,
                                           boilerInput, blowdownFlashTank, boiler, highPressureHeaderCalculationsDomain,
                                           mediumPressureHeaderCalculationsDomain, lowPressureHeaderCalculationsDomain);

    //steam balance = difference between use and production (we want 0!)
    double steamBalance = steamUse - steamProduction;
    std::cout << methodName
              << "steamUse=" << steamUse << " - steamProduction=" << steamProduction
              << ": steamBalance=" << steamBalance << std::endl;
    return steamBalance;
}
