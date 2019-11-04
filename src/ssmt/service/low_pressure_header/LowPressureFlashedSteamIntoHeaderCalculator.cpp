#include "ssmt/service/low_pressure_header/LowPressureFlashedSteamIntoHeaderCalculator.h"

LowPressureFlashedSteamIntoHeaderCalculatorDomain LowPressureFlashedSteamIntoHeaderCalculator::calc(
        const int headerCountInput,
        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
        const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
        const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain) const {
    const std::string methodName =
            std::string("LowPressureFlashedSteamIntoHeaderCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<FlashTank> mediumPressureCondensateFlashTank = nullptr;
    std::shared_ptr<FlashTank> highPressureCondensateFlashTank =
            (mediumPressureHeaderCalculationsDomain == nullptr)
            ? nullptr
            : mediumPressureHeaderCalculationsDomain->highPressureCondensateFlashTank;

//     std::cout << methodName << "calculating flashedSteamIntoLowPressureHeader" << std::endl;
    if (lowPressureHeaderInput->isFlashCondensate()) {
        // std::cout << methodName << "lowPressureHeaderInput isFlashCondensate, processing" << std::endl;
        const SteamSystemModelerTool::FluidProperties &highPressureCondensate =
                highPressureHeaderCalculationsDomain.highPressureCondensate;
        if (headerCountInput == 3) {
        //     std::cout << methodName << "medium pressure header provided" << std::endl;
        //     std::cout << methodName << "calculating mediumPressureCondensateFlashTank" << std::endl;
            const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate =
                    mediumPressureHeaderCalculationsDomain->mediumPressureCondensate;
            highPressureCondensateFlashTank = mediumPressureHeaderCalculationsDomain->highPressureCondensateFlashTank;
            mediumPressureCondensateFlashTank =
                    makeMediumPressureCondensateFlashTank(lowPressureHeaderInput, mediumPressureHeaderInput,
                                                          mediumPressureCondensate, highPressureCondensate,
                                                          highPressureCondensateFlashTank);
        } else {
        //     std::cout << methodName << "medium pressure header not provided" << std::endl;
        //     std::cout << methodName
                //       << "calculating highPressureCondensateFlashTank with lowPressureHeaderInput pressure"
                //       << std::endl;
            highPressureCondensateFlashTank =
                    makeHighPressureCondensateFlashTank(lowPressureHeaderInput, highPressureCondensate);
        }
    } else {
        // std::cout << methodName << "lowPressureHeaderInput not isFlashCondensate, skipping" << std::endl;
    }

    return {mediumPressureCondensateFlashTank, highPressureCondensateFlashTank};
}

std::shared_ptr<FlashTank> LowPressureFlashedSteamIntoHeaderCalculator::makeMediumPressureCondensateFlashTank(
        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
        const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate,
        const SteamSystemModelerTool::FluidProperties &highPressureCondensate,
        const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank) const {
    const std::string methodName =
            std::string("LowPressureFlashedSteamIntoHeaderCalculator::") + std::string(__func__) + ": ";

    //4B. Calculate Medium Pressure Flash Tank
    //mix inlet condensate using header calculate
//     std::cout << methodName << "calculating highAndMediumPressureMixHeader" << std::endl;
    std::shared_ptr<Header> highAndMediumPressureMixHeader =
            makeHighAndMediumPressureMixHeader(lowPressureHeaderInput, mediumPressureHeaderInput,
                                               mediumPressureCondensate, highPressureCondensate,
                                               highPressureCondensateFlashTank);
//     std::cout << methodName << "highAndMediumPressureMixHeader=" << *highAndMediumPressureMixHeader << std::endl;

    //run the mixed condensate through the flash tank
//     std::cout << methodName << "calculating mediumPressureCondensateFlashTank" << std::endl;
    const FlashTank &flashTank =
            flashTankFactory.make(highAndMediumPressureMixHeader, lowPressureHeaderInput);
//     std::cout << methodName << "mediumPressureCondensateFlashTank=" << flashTank << std::endl;

    return std::make_shared<FlashTank>(flashTank);
}

std::shared_ptr<Header> LowPressureFlashedSteamIntoHeaderCalculator::makeHighAndMediumPressureMixHeader(
        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
        const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate,
        const SteamSystemModelerTool::FluidProperties &highPressureCondensate,
        const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank) const {
    const std::string methodName =
            std::string("LowPressureFlashedSteamIntoHeaderCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Header> highAndMediumPressureMixHeader;

    if (mediumPressureHeaderInput->isFlashCondensate()) {
        // std::cout << methodName
                //   << "mediumPressureHeaderInput isFlashCondensate,"
                //   << " calculating highAndMediumPressureMixHeader with flash tank"
                //   << " (highPressureCondensateFlashTank)"
                //   << std::endl;
        //if high pressure condensate has been flashed into medium pressure header,
        //inlets will be leftover condensate from flash tank and medium pressure condensate
        const Header &header =
                headerFactory.make(lowPressureHeaderInput, highPressureCondensateFlashTank, mediumPressureCondensate);
        highAndMediumPressureMixHeader = std::make_shared<Header>(header);
    } else {
        // std::cout << methodName
                //   << "mediumPressureHeaderInput not isFlashCondensate,"
                //   << " calculating highAndMediumPressureMixHeader without flash tank"
                //   << std::endl;
        //if not, inlets will be high pressure condensate and medium pressure condensate
        const Header &header =
                headerFactory.make(lowPressureHeaderInput, highPressureCondensate, mediumPressureCondensate);
        highAndMediumPressureMixHeader = std::make_shared<Header>(header);
    }

    return highAndMediumPressureMixHeader;
}

//if two headers, flashing into low pressure header
std::shared_ptr<FlashTank> LowPressureFlashedSteamIntoHeaderCalculator::makeHighPressureCondensateFlashTank(
        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
        const SteamSystemModelerTool::FluidProperties &highPressureCondensate) const {
    const std::string methodName =
            std::string("LowPressureFlashedSteamIntoHeaderCalculator::") + std::string(__func__) + ": ";

    const double pressure = lowPressureHeaderInput->getPressure();

    const FlashTank &flashTank = flashTankFactory.make(pressure, highPressureCondensate);
//     std::cout << methodName << "highPressureCondensateFlashTank=" << flashTank << std::endl;

    return std::make_shared<FlashTank>(flashTank);
}
