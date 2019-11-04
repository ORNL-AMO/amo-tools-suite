#include "ssmt/service/high_pressure_header/HighPressureFlashTankCalculator.h"

const std::shared_ptr<FlashTank> HighPressureFlashTankCalculator::calc(const int headerCountInput,
                                                                       const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                                       const SteamSystemModelerTool::FluidProperties &highPressureCondensate) const {
    const std::string methodName = std::string("HighPressureFlashTankCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<FlashTank> highPressureCondensateFlashTank = nullptr;
    if (headerCountInput == 3 && mediumPressureHeaderInput->isFlashCondensate()) {
        // std::cout << methodName << "mediumPressureHeaderInput isUseTurbine, calculating highPressureCondensateFlashTank"
        //           << std::endl;
        const double pressure = mediumPressureHeaderInput->getPressure();
        const FlashTank &flashTank = flashTankFactory.make(pressure, highPressureCondensate);
        highPressureCondensateFlashTank = std::make_shared<FlashTank>(flashTank);
    } else {
        // std::cout << methodName
        //           << "mediumPressureHeaderInput not provided or mediumPressureHeaderInput not isFlashCondensate, skipping"
        //           << std::endl;
    }

    return highPressureCondensateFlashTank;
}
