#include "ssmt/service/water_and_condensate/ReturnCondensateCalculator.h"
#include <ssmt/FlashTank.h>

SteamSystemModelerTool::FluidProperties
ReturnCondensateCalculator::calc(const HeaderWithHighestPressure &highPressureHeaderInput,
                                 const Header &combinedCondensateHeader) const {
    return fluidPropertiesFactory.make(highPressureHeaderInput, combinedCondensateHeader);
}

ReturnCondensateCalculationsDomain
ReturnCondensateCalculator::flash(const HeaderWithHighestPressure &highPressureHeaderInput, const BoilerInput &boilerInput,
      const SteamSystemModelerTool::FluidProperties &returnCondensate) const {
    const std::string methodName = std::string("ReturnCondensateCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<FlashTank> condensateFlashTankPtr = nullptr;
    SteamSystemModelerTool::FluidProperties returnCondensateFlashed = returnCondensate;

    if (highPressureHeaderInput.isFlashCondensate()) {
        std::cout << methodName
                  << "highPressureHeaderInput isFlashCondensate, calculating condensateFlashTank & returnCondensate"
                  << std::endl;
        const FlashTank &condensateFlashTank = flashTankFactory.make(boilerInput, returnCondensate);
        condensateFlashTankPtr = std::make_shared<FlashTank>(condensateFlashTank);
        returnCondensateFlashed = condensateFlashTank.getOutletLiquidSaturatedProperties();
    } else {
        std::cout << methodName << "highPressureHeaderInput not isFlashCondensate, skipping" << std::endl;
    }

    return {condensateFlashTankPtr, returnCondensateFlashed};
}
