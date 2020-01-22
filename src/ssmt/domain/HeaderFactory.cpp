#include "ssmt/domain/HeaderFactory.h"

const Header HeaderFactory::make(const double &headerPressure, const Boiler &boiler) const {
    const std::string methodName = "HeaderFactory::" + std::string(__func__) + ": ";

    std::cout << methodName << "making header" << std::endl;

    std::vector<Inlet> inlets = inletFactory.make(boiler);

    const Header header = {headerPressure, inlets};
    std::cout << methodName << "header=" << header << std::endl;

    return header;
}

const Header HeaderFactory::make(const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                 const std::shared_ptr<PrvWithoutDesuperheating> &prvWithoutDesuperheating,
                                 const PressureTurbine &highToMediumTurbineInput,
                                 const std::shared_ptr<Turbine> &highToMediumPressureTurbine,
                                 const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank) const {
    const std::string methodName = "HeaderFactory::" + std::string(__func__) + ": ";

    std::cout << methodName << "making header" << std::endl;

    const double headerPressure = mediumPressureHeaderInput->getPressure();

    //High to medium PRV
    std::cout << methodName << "adding highToMediumPrv inlet" << std::endl;
    const Inlet highToMediumPrvInlet = inletFactory.make(prvWithoutDesuperheating);
    std::vector<Inlet> inlets = {highToMediumPrvInlet};

    //High to medium turbine
    const bool isUseTurbine = highToMediumTurbineInput.isUseTurbine();
    std::cout << methodName << "highToMediumTurbineInput.isUseTurbine=" << isUseTurbine << std::endl;

    if (isUseTurbine) {
        std::cout << methodName << "isUseTurbine=true, adding highToMediumPressureTurbine inlet" << std::endl;

        Inlet highToMediumTurbineInlet = inletFactory.make(highToMediumPressureTurbine);
        std::cout << methodName << "highToMediumTurbineInlet=" << highToMediumTurbineInlet << std::endl;
        inlets.push_back(highToMediumTurbineInlet);
    } else {
        std::cout << methodName << "isUseTurbine=false, skipping highToMediumPressureTurbine inlet" << std::endl;
    }

    //High pressure flashed condensate
    const bool isFlashCondensate = mediumPressureHeaderInput->isFlashCondensate();
    std::cout << methodName << "mediumPressureHeaderInput->isFlashCondensate=" << isFlashCondensate << std::endl;

    if (isFlashCondensate) {
        std::cout << methodName << "isFlashCondensate=true, adding highPressureFlashedCondensate inlet" << std::endl;
        Inlet highPressureFlashedCondensateInlet = inletFactory.makeFromOutletGas(highPressureCondensateFlashTank);
        std::cout << methodName << "highPressureFlashedCondensateInlet=" << highPressureFlashedCondensateInlet
                  << std::endl;
        inlets.push_back(highPressureFlashedCondensateInlet);
    } else {
        std::cout << methodName << "isFlashCondensate=false, skipping highPressureFlashedCondensate inlet" << std::endl;
    }

    const Header header = {headerPressure, inlets};
    std::cout << methodName << "header=" << header << std::endl;

    return header;
}

const Header HeaderFactory::make(const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                 const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank,
                                 const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate) const {
    const std::string methodName = "HeaderFactory::" + std::string(__func__) + ": ";

    std::cout << methodName << "making header" << std::endl;

    const double headerPressure = lowPressureHeaderInput->getPressure();

    Inlet highPressureFlashedCondensateInlet = inletFactory.makeFromOutletLiquid(highPressureCondensateFlashTank);
    std::cout << methodName << "highPressureFlashedCondensateInlet=" << highPressureFlashedCondensateInlet << std::endl;

    Inlet mediumPressureCondensateInlet = inletFactory.makeWithEnthalpy(mediumPressureCondensate);
    std::cout << methodName << "mediumPressureCondensateInlet=" << mediumPressureCondensateInlet << std::endl;

    std::vector<Inlet> inlets = {highPressureFlashedCondensateInlet, mediumPressureCondensateInlet};

    const Header header = {headerPressure, inlets};
    std::cout << methodName << "header=" << header << std::endl;

    return header;
}

const Header HeaderFactory::make(const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                 const SteamSystemModelerTool::FluidProperties &highPressureCondensate,
                                 const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate) const {
    const std::string methodName = "HeaderFactory::" + std::string(__func__) + ": ";

    std::cout << methodName << "making header" << std::endl;

    const double headerPressure = lowPressureHeaderInput->getPressure();

    Inlet highPressureCondensateInlet = inletFactory.makeWithEnthalpy(highPressureCondensate);
    std::cout << methodName << "highPressureCondensateInlet=" << highPressureCondensateInlet << std::endl;

    Inlet mediumPressureCondensateInlet = inletFactory.makeWithEnthalpy(mediumPressureCondensate);
    std::cout << methodName << "mediumPressureCondensateInlet=" << mediumPressureCondensateInlet << std::endl;

    std::vector<Inlet> inlets = {highPressureCondensateInlet, mediumPressureCondensateInlet};

    const Header header = {headerPressure, inlets};
    std::cout << methodName << "header=" << header << std::endl;

    return header;
}

const Header HeaderFactory::make(const double headerCountInput,
                                 const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                 const PressureTurbine &highToLowTurbineInput,
                                 const PressureTurbine &mediumToLowTurbineInput, const BoilerInput &boilerInput,
                                 const std::shared_ptr<PrvWithoutDesuperheating> &lowPressurePrvWithoutDesuperheating,
                                 const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                 const std::shared_ptr<FlashTank> &blowdownFlashTank,
                                 const LowPressureFlashedSteamIntoHeaderCalculatorDomain &lowPressureFlashedSteamIntoHeaderCalculatorDomain,
                                 const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain) const {
    const std::string methodName = "HeaderFactory::" + std::string(__func__) + ": ";

    std::cout << methodName << "making header" << std::endl;

    //Low pressure PRV; PRV always exists
    const double headerPressure = lowPressureHeaderInput->getPressure();

    const Inlet lowPrvInlet = inletFactory.make(lowPressurePrvWithoutDesuperheating);
    std::vector<Inlet> inlets = {lowPrvInlet};

    //High to low pressure turbine
    const bool isUseTurbineHighToLow = highToLowTurbineInput.isUseTurbine();
    std::cout << methodName << "highToLowTurbineInput.isUseTurbine=" << isUseTurbineHighToLow << std::endl;

    if (isUseTurbineHighToLow) {
        std::cout << methodName << "highToLowTurbineInput.isUseTurbine=true, adding highToLowPressureTurbine"
                  << std::endl;
        const Inlet &inlet = inletFactory.make(highToLowPressureTurbine);
        std::cout << methodName << "highToLowPressureTurbineInlet=" << inlet << std::endl;
        inlets.push_back(inlet);
    } else {
        std::cout << methodName << "highToLowTurbineInput.isUseTurbine=false, skipping highToLowPressureTurbine"
                  << std::endl;
    }

    //Medium to low pressure turbine
    const bool isUseTurbineMediumToLow = mediumToLowTurbineInput.isUseTurbine();
    std::cout << methodName << "mediumToLowTurbineInput.isUseTurbine=" << isUseTurbineMediumToLow << std::endl;

    if (headerCountInput == 3 && isUseTurbineMediumToLow) {
        std::cout << methodName
                   << "mediumToLowTurbineInput.isUseTurbineMediumToLow=true, adding mediumToLowPressureTurbine"
                   << std::endl;
        const std::shared_ptr<Turbine> &mediumToLowPressureTurbine =
                mediumPressureHeaderCalculationsDomain->mediumToLowPressureTurbine;
        const Inlet &inlet = inletFactory.make(mediumToLowPressureTurbine);
        std::cout << methodName << "mediumToLowPressureTurbineInlet=" << inlet << std::endl;
        inlets.push_back(inlet);
    } else {
        std::cout << methodName
                   << "mediumToLowTurbineInput.isUseTurbineMediumToLow=false, skipping mediumToLowPressureTurbine"
                   << std::endl;
    }

    //Flashed condensate into header
    const bool isFlashCondensate = lowPressureHeaderInput->isFlashCondensate();
    std::cout << methodName << "lowPressureHeaderInput.isFlashCondensate=" << isFlashCondensate << std::endl;

    if (isFlashCondensate) {
        //if medium pressure header exists, use medium pressure flash tank
        if (headerCountInput == 3) {
            std::cout << methodName
                       << "lowPressureHeaderInput.isFlashCondensate=true & 3 headers, adding mediumPressureCondensateFlashTank"
                       << std::endl;

            const std::shared_ptr<FlashTank> &mediumPressureCondensateFlashTank =
                    lowPressureFlashedSteamIntoHeaderCalculatorDomain.mediumPressureCondensateFlashTank;
            const Inlet &inlet = inletFactory.makeFromOutletGas(mediumPressureCondensateFlashTank);
            std::cout << methodName << "mediumPressureCondensateFlashTankInlet=" << inlet << std::endl;

            inlets.push_back(inlet);
        } else {
            std::cout << methodName
                       << "lowPressureHeaderInput.isFlashCondensate=true & not 3 headers, adding highPressureCondensateFlashTank"
                       << std::endl;

            //if only high and low header, high pressure flash tank
            const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank =
                    lowPressureFlashedSteamIntoHeaderCalculatorDomain.highPressureCondensateFlashTank;
            const Inlet &inlet = inletFactory.makeFromOutletGas(highPressureCondensateFlashTank);
            std::cout << methodName << "highPressureCondensateFlashTankInlet=" << inlet << std::endl;

            inlets.push_back(inlet);
        }
    }

    //Blowdown flash tank outlet gas
    const bool isBlowdownFlashed = boilerInput.isBlowdownFlashed();
    std::cout << methodName << "boilerInput.isBlowdownFlashed=" << isFlashCondensate << std::endl;

    if (isBlowdownFlashed) {
        std::cout << methodName << "boilerInput.isBlowdownFlashed=true, adding blowdownFlashTank" << std::endl;

        const Inlet &inlet = inletFactory.makeFromOutletGas(blowdownFlashTank);
        std::cout << methodName << "blowdownFlashTankInlet=" << inlet << std::endl;

        inlets.push_back(inlet);
    } else {
        std::cout << methodName << "boilerInput.isBlowdownFlashed=false, skipping blowdownFlashTank" << std::endl;
    }

    const Header header = {headerPressure, inlets};
    std::cout << methodName << "header=" << header << std::endl;

    return header;
}

const Header
HeaderFactory::make(const int headerCountInput, const double headerPressure,
                    const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank,
                    const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                    const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                    const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName = "HeaderFactory::" + std::string(__func__) + ": ";

    std::cout << methodName << "making header" << std::endl;

    std::vector<Inlet> inlets = {};

    const bool isFlashTankNull = isMediumPressureCondensateFlashTankNull(lowPressureHeaderCalculationsDomain);
    if (highPressureCondensateFlashTank == nullptr && isFlashTankNull) {
        std::cout << methodName
                   << "highPressureCondensateFlashTank not specified & mediumPressureCondensateFlashTank not specified"
                   << ", adding highPressureCondensate" << std::endl;
        const SteamSystemModelerTool::FluidProperties &highPressureCondensate =
                highPressureHeaderCalculationsDomain.highPressureCondensate;
        const Inlet &inlet = inletFactory.makeWithEnthalpy(highPressureCondensate);
        std::cout << methodName << "highPressureCondensateInlet=" << inlet << std::endl;

        inlets.push_back(inlet);
    } else if (isFlashTankNull) {
        std::cout << methodName
                   << "highPressureCondensateFlashTank specified & mediumPressureCondensateFlashTank not specified"
                   << ", adding highPressureCondensateFlashTank" << std::endl;
        const Inlet &inlet = inletFactory.makeFromOutletLiquid(highPressureCondensateFlashTank);
        std::cout << methodName << "highPressureCondensateFlashTankInlet=" << inlet << std::endl;

        inlets.push_back(inlet);
    }

    if (headerCountInput > 1) {
        std::cout << methodName << "lowPressureHeader specified, adding lowPressureCondensate" << std::endl;

        const SteamSystemModelerTool::FluidProperties &lowPressureCondensate =
                lowPressureHeaderCalculationsDomain->lowPressureCondensate;
        const Inlet &inlet = inletFactory.makeWithEnthalpy(lowPressureCondensate);
        std::cout << methodName << "lowPressureCondensateInlet=" << inlet << std::endl;

        inlets.push_back(inlet);
    } else {
        std::cout << methodName << "lowPressureHeader not exists, skipping lowPressureCondensate" << std::endl;
    }

    if (headerCountInput == 3) {
        if (isFlashTankNull) {
            std::cout << methodName
                       << "mediumPressureHeader specified & mediumPressureCondensateFlashTank not specified"
                       << ", adding mediumPressureCondensate" << std::endl;

            const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate =
                    mediumPressureHeaderCalculationsDomain->mediumPressureCondensate;
            const Inlet &inlet = inletFactory.makeWithEnthalpy(mediumPressureCondensate);
            std::cout << methodName << "mediumPressureCondensateInlet=" << inlet << std::endl;

            inlets.push_back(inlet);
        } else {
            std::cout << methodName
                       << "mediumPressureHeader specified & mediumPressureCondensateFlashTank specified"
                       << ", adding mediumPressureCondensateFlashTank" << std::endl;

            const LowPressureFlashedSteamIntoHeaderCalculatorDomain &lowPressureFlashedSteamIntoHeaderCalculatorDomain =
                    lowPressureHeaderCalculationsDomain->lowPressureFlashedSteamIntoHeaderCalculatorDomain;
            const std::shared_ptr<FlashTank> &mediumPressureCondensateFlashTank =
                    lowPressureFlashedSteamIntoHeaderCalculatorDomain.mediumPressureCondensateFlashTank;
            const Inlet &inlet = inletFactory.makeFromOutletLiquid(mediumPressureCondensateFlashTank);
            std::cout << methodName << "mediumPressureCondensateFlashTankInlet=" << inlet << std::endl;

            inlets.push_back(inlet);
        }
    }

    const Header header = {headerPressure, inlets};
    std::cout << methodName << "header=" << header << std::endl;

    return header;
}

const Header
HeaderFactory::make(const int headerCountInput,
                    const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                    const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                    const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const double lowHeaderPressure =
            determineLowHeaderPressure(headerCountInput, highPressureHeaderCalculationsDomain,
                                       lowPressureHeaderCalculationsDomain);

    const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank =
            selectHighPressureCondensateFlashTank(mediumPressureHeaderCalculationsDomain,
                                                  lowPressureHeaderCalculationsDomain);

    return make(headerCountInput, lowHeaderPressure, highPressureCondensateFlashTank,
                highPressureHeaderCalculationsDomain, mediumPressureHeaderCalculationsDomain,
                lowPressureHeaderCalculationsDomain);
}

/** Pressure from lowest pressure condensate. */
double
HeaderFactory::determineLowHeaderPressure(const int headerCountInput,
                                          const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                          const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    double pressure = 0;

    if (headerCountInput == 1) {
        const SteamSystemModelerTool::FluidProperties &highPressureCondensate =
                highPressureHeaderCalculationsDomain.highPressureCondensate;
        pressure = highPressureCondensate.pressure;
    } else {
        const SteamSystemModelerTool::FluidProperties &lowPressureCondensate =
                lowPressureHeaderCalculationsDomain->lowPressureCondensate;
        pressure = lowPressureCondensate.pressure;
    }

    return pressure;
}

/** @return selected one, possibly null. */
const std::shared_ptr<FlashTank>
HeaderFactory::selectHighPressureCondensateFlashTank(
        const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
        const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    std::shared_ptr<FlashTank> flashTank = nullptr;

    if (lowPressureHeaderCalculationsDomain == nullptr) {
        if (mediumPressureHeaderCalculationsDomain != nullptr) {
            flashTank = mediumPressureHeaderCalculationsDomain->highPressureCondensateFlashTank;
        }
    } else {
        const LowPressureFlashedSteamIntoHeaderCalculatorDomain &lowPressureFlashedSteamIntoHeaderCalculatorDomain =
                lowPressureHeaderCalculationsDomain->lowPressureFlashedSteamIntoHeaderCalculatorDomain;
        flashTank = lowPressureFlashedSteamIntoHeaderCalculatorDomain.highPressureCondensateFlashTank;
    }

    return flashTank;
}

bool HeaderFactory::isMediumPressureCondensateFlashTankNull(
        const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    bool isNull = false;

    if (lowPressureHeaderCalculationsDomain == nullptr) {
        isNull = true;
    } else {
        const LowPressureFlashedSteamIntoHeaderCalculatorDomain &lowPressureFlashedSteamIntoHeaderCalculatorDomain =
                lowPressureHeaderCalculationsDomain->lowPressureFlashedSteamIntoHeaderCalculatorDomain;
        if (lowPressureFlashedSteamIntoHeaderCalculatorDomain.mediumPressureCondensateFlashTank == nullptr) {
            isNull = true;
        }
    }

    return isNull;
}

const Header
HeaderFactory::make(const double headerPressure, const SteamSystemModelerTool::FluidProperties &returnCondensate,
                    const BoilerInput &boilerInput, const std::shared_ptr<HeatExchanger::Output> &heatExchangerOutput,
                    const SteamSystemModelerTool::FluidProperties &makeupWater,
                    const CondensingTurbine &condensingTurbineInput,
                    const std::shared_ptr<Turbine> &condensingTurbine) const {
    const std::string methodName = "HeaderFactory::" + std::string(__func__) + ": ";

    std::cout << methodName << "making header" << std::endl;

    std::cout << methodName << "adding returnCondensate inlet" << std::endl;
    const Inlet &returnCondensateInlet = inletFactory.makeWithEnthalpy(returnCondensate);

    std::vector<Inlet> inlets = {returnCondensateInlet};

    //makeup water
    const bool isPreheatMakeupWater = boilerInput.isPreheatMakeupWater();
    std::cout << methodName << "boilerInput.isPreheatMakeupWater=" << isPreheatMakeupWater << std::endl;

    if (isPreheatMakeupWater) {
        std::cout << methodName << "isPreheatMakeupWater is true, adding heatExchangerOutput inlet" << std::endl;

        const Inlet &makeupWaterInlet = inletFactory.makeWithTemperature(heatExchangerOutput);
        inlets.push_back(makeupWaterInlet);
    } else {
        std::cout << methodName << "isPreheatMakeupWater is false, adding makeupWater inlet" << std::endl;

        const Inlet &makeupWaterInlet = inletFactory.makeWithEnthalpy(makeupWater);
        inlets.push_back(makeupWaterInlet);
    }

    const bool isUseTurbine = condensingTurbineInput.isUseTurbine();
    std::cout << methodName << "condensingTurbineInput.isUseTurbine=" << isUseTurbine << std::endl;
    if (isUseTurbine) {
        std::cout << methodName << "isUseTurbine=true, adding condensingTurbine inlet" << std::endl;

        const double condenserPressure = condensingTurbineInput.getCondenserPressure();
        const Inlet &condensingTurbineInlet = inletFactory.make(condensingTurbine, condenserPressure);
        inlets.push_back(condensingTurbineInlet);
    } else {
        std::cout << methodName << "isUseTurbine=false, skipping condensingTurbine" << std::endl;
    }

    const Header header = {headerPressure, inlets};
    std::cout << methodName << "header=" << header << std::endl;

    return header;
}
