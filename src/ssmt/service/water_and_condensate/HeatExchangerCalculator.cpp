#include "ssmt/service/water_and_condensate/HeatExchangerCalculator.h"

std::shared_ptr<HeatExchanger::Output>
HeatExchangerCalculator::calc(const BoilerInput &boilerInput, const Boiler &boiler,
                              const SteamSystemModelerTool::FluidProperties &makeupWaterAndMassFlow,
                              const std::shared_ptr<FlashTank> &blowdownFlashTank) const {
    const std::string methodName = std::string("HeatExchangerCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<HeatExchanger::Output> heatExchangerOutput = nullptr;

    const bool isPreheatMakeupWater = boilerInput.isPreheatMakeupWater();
    std::cout << methodName << "boilerInput.isPreheatMakeupWater=" << isPreheatMakeupWater << std::endl;

    if (isPreheatMakeupWater) {
        std::cout << methodName << "isPreheatMakeupWater is true, calculating heatExchangerOutput" << std::endl;
        SteamSystemModelerTool::FluidProperties coldInlet = fluidPropertiesFactory.make(makeupWaterAndMassFlow);
        const double approachTemp = boilerInput.getApproachTemperature();

        SteamSystemModelerTool::FluidProperties hotInlet;
        if (boilerInput.isBlowdownFlashed()) {
            std::cout << methodName << "boilerInput isBlowdownFlashed is true, calculating hotInlet from blowdownFlashTank"
                     << std::endl;
            const SteamSystemModelerTool::FluidProperties &outletLiquidSaturatedProperties =
                    blowdownFlashTank->getOutletLiquidSaturatedProperties();
            hotInlet = fluidPropertiesFactory.make(outletLiquidSaturatedProperties);
        } else {
            std::cout << methodName << "boilerInput not isBlowdownFlashed is false, calculating hotInlet from boiler blowdown"
                     << std::endl;
            const SteamSystemModelerTool::FluidProperties &blowdownProperties = boiler.getBlowdownProperties();
            hotInlet = fluidPropertiesFactory.make(blowdownProperties);
        }

        std::cout << methodName << "calculating heatExchanger" << std::endl;
        HeatExchanger heatExchanger = {hotInlet, coldInlet, approachTemp};
        const HeatExchanger::Output &output = heatExchanger.calculate();
        heatExchangerOutput = std::make_shared<HeatExchanger::Output>(output);
    } else {
        std::cout << methodName << "isPreheatMakeupWater is false, skipping heat exchanger" << std::endl;
    }

    return heatExchangerOutput;
}
