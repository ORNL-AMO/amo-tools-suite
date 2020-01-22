#include "ssmt/service/high_pressure_header/HighToLowSteamTurbineCalculator.h"

std::shared_ptr<Turbine>
HighToLowSteamTurbineCalculator::calc(const int headerCountInput, const PressureTurbine &highToLowTurbineInput,
                                      const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                      const HeaderWithHighestPressure &highPressureHeaderInput,
                                      const CondensingTurbine &condensingTurbineInput,
                                      const std::shared_ptr<Turbine> &condensingTurbine,
                                      const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                      const Boiler &boiler, const bool isCalcIdeal) const
{
        const std::string methodName = std::string("HighToLowSteamTurbineCalculator::") + std::string(__func__) + ": ";

        std::shared_ptr<Turbine> highToLowPressureTurbine = nullptr;

        if (headerCountInput > 1 && highToLowTurbineInput.isUseTurbine())
        {
                std::cout << methodName
                          << "low turbine provided and highToLowTurbineInput isUseTurbine, calculating highToLowPressureTurbine"
                          << std::endl;

                //value for inletMassFlow into turbine calculation; mass flow in header - processSteamUsage
                const double availableMassFlow =
                    calcAvailableMassFlow(highPressureHeaderInput, highPressureHeaderOutput, condensingTurbineInput,
                                          condensingTurbine);
                std::cout << methodName << "availableMassFlow=" << availableMassFlow << std::endl;
                highToLowPressureTurbine =
                    calcHighToLowPressureTurbine(availableMassFlow, highToLowTurbineInput, highPressureHeaderOutput,
                                                 lowPressureHeaderInput, boiler, isCalcIdeal);
                std::cout << methodName << "highToLowPressureTurbine=" << *highToLowPressureTurbine << std::endl;
        }
        else
        {
                std::cout << methodName << "low turbine not provided and highToLowTurbineInput not isUseTurbine, skipping"
                          << std::endl;
                std::cout << methodName << "highToLowPressureTurbine=null" << std::endl;
        }

        return highToLowPressureTurbine;
}

std::shared_ptr<Turbine>
HighToLowSteamTurbineCalculator::calcHighToLowPressureTurbine(const double availableMassFlow,
                                                              const PressureTurbine &highToLowTurbineInput,
                                                              const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                              const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                              const Boiler &boiler, const bool isCalcIdeal) const
{
        const std::string methodName = std::string("HighToLowSteamTurbineCalculator::") + std::string(__func__) + ": ";

        std::shared_ptr<Turbine> highToLowPressureTurbine = nullptr;

        const PressureTurbineOperation &pressureTurbineOperation = highToLowTurbineInput.getOperationType();
        std::cout << methodName << "pressureTurbineOperation=" << pressureTurbineOperation << std::endl;

        switch (pressureTurbineOperation)
        {
        case PressureTurbineOperation::FLOW_RANGE:
                highToLowPressureTurbine =
                    calcFlowRange(availableMassFlow, highToLowTurbineInput, highPressureHeaderOutput,
                                  lowPressureHeaderInput, boiler, isCalcIdeal);
                break;
        case PressureTurbineOperation::POWER_RANGE:
                highToLowPressureTurbine =
                    calcPowerRange(availableMassFlow, highToLowTurbineInput, highPressureHeaderOutput,
                                   lowPressureHeaderInput, boiler, isCalcIdeal);
                break;
        case PressureTurbineOperation::POWER_GENERATION:
                highToLowPressureTurbine =
                    calcPowerGeneration(availableMassFlow, highToLowTurbineInput, highPressureHeaderOutput,
                                        lowPressureHeaderInput, boiler, isCalcIdeal);
                break;
        case PressureTurbineOperation::STEAM_FLOW:
                highToLowPressureTurbine =
                    calcSteamFlow(availableMassFlow, highToLowTurbineInput, highPressureHeaderOutput,
                                  lowPressureHeaderInput, boiler, isCalcIdeal);
                break;
        case PressureTurbineOperation::BALANCE_HEADER:
                highToLowPressureTurbine =
                    calcBalanceHeader(availableMassFlow, highToLowTurbineInput, highPressureHeaderOutput,
                                      lowPressureHeaderInput, isCalcIdeal);
                break;
        default:
                std::string msg = methodName + "PressureTurbineOperation enum not handled";
                std::cout << msg << std::endl;
                throw std::invalid_argument(msg);
        }

        return highToLowPressureTurbine;
}

std::shared_ptr<Turbine>
HighToLowSteamTurbineCalculator::calcFlowRange(const double availableMassFlow,
                                               const PressureTurbine &highToLowTurbineInput,
                                               const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                               const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                               const Boiler &boiler, const bool isCalcIdeal) const
{
        const std::string methodName = std::string("HighToLowSteamTurbineCalculator::") + std::string(__func__) + ": ";

        std::shared_ptr<Turbine> highToLowPressureTurbine = nullptr;

        const double highToLowTurbineInputOperationValue1 = highToLowTurbineInput.getOperationValue1();
        const double highToLowTurbineInputOperationValue2 = highToLowTurbineInput.getOperationValue2();

        //if less steam available then minimum needed
        if (highToLowTurbineInputOperationValue1 > availableMassFlow)
        {
                std::cout << methodName
                          << "highToLowTurbineInputOperationValue1=" << highToLowTurbineInputOperationValue1
                          << " > availableMassFlow=" << availableMassFlow
                          << ", calculating highToLowPressureTurbine with amount needed (highToLowTurbineInputOperationValue1)"
                          << " instead of amount available" << std::endl;
                highToLowPressureTurbine =
                    turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput,
                                                       highToLowTurbineInputOperationValue1, lowPressureHeaderInput,
                                                       isCalcIdeal);
                restarter.restartIfNotEnoughSteam(highToLowPressureTurbine, availableMassFlow, boiler);
        }
        else if (highToLowTurbineInputOperationValue2 < availableMassFlow)
        {
                std::cout << methodName
                          << "highToLowTurbineInputOperationValue2=" << highToLowTurbineInputOperationValue2
                          << " < availableMassFlow=" << availableMassFlow
                          << ", calculating highToLowPressureTurbine with max amount allowed (highToLowTurbineInputOperationValue2)"
                          << " instead of amount available" << std::endl;
                highToLowPressureTurbine =
                    turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput,
                                                       highToLowTurbineInputOperationValue2, lowPressureHeaderInput,
                                                       isCalcIdeal);
        }
        else
        {
                std::cout << methodName
                          << "availableMassFlow=" << availableMassFlow << " is between needed and max amounts,"
                          << " calculating highToLowPressureTurbine with availableMassFlow" << std::endl;
                highToLowPressureTurbine =
                    turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput, availableMassFlow,
                                                       lowPressureHeaderInput, isCalcIdeal);
        }

        return highToLowPressureTurbine;
}

std::shared_ptr<Turbine>
HighToLowSteamTurbineCalculator::calcPowerRange(const double availableMassFlow,
                                                const PressureTurbine &highToLowTurbineInput,
                                                const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                const Boiler &boiler, const bool isCalcIdeal) const
{
        const std::string methodName = std::string("HighToLowSteamTurbineCalculator::") + std::string(__func__) + ": ";

        std::shared_ptr<Turbine> highToLowPressureTurbine = nullptr;

        const double highToLowTurbineInputOperationValue1 = highToLowTurbineInput.getOperationValue1();
        const double highToLowTurbineInputOperationValue2 = highToLowTurbineInput.getOperationValue2();

        std::cout << methodName
                  << "calculating highToLowPressureTurbine with availableMassFlow=" << availableMassFlow << std::endl;
        highToLowPressureTurbine =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput, availableMassFlow,
                                               lowPressureHeaderInput, isCalcIdeal);

        //check that power out is in range
        const double highToLowPressureTurbinePowerOut = highToLowPressureTurbine->getPowerOut();
        if (highToLowTurbineInputOperationValue1 > highToLowPressureTurbinePowerOut)
        {
                std::cout << methodName
                          << "highToLowTurbineInputOperationValue1=" << highToLowTurbineInputOperationValue1
                          << " > highToLowPressureTurbinePowerOut=" << highToLowPressureTurbinePowerOut
                          << " not enough power out of turbine,"
                          << " calculating highToLowPressureTurbine with amount needed instead of amount available"
                          << std::endl;
                double currentSteamAvailable = highToLowPressureTurbine->getMassFlow();
                highToLowPressureTurbine =
                    turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToLowTurbineInput,
                                                       highToLowTurbineInputOperationValue1, lowPressureHeaderInput,
                                                       isCalcIdeal);
                restarter.restartIfNotEnoughSteam(highToLowPressureTurbine, currentSteamAvailable, boiler);
        }
        else if (highToLowTurbineInputOperationValue2 < highToLowPressureTurbinePowerOut)
        {
                std::cout << methodName
                          << "highToLowTurbineInputOperationValue2=" << highToLowTurbineInputOperationValue2
                          << " < highToLowPressureTurbinePowerOut=" << highToLowPressureTurbinePowerOut
                          << ", calculating highToLowPressureTurbine with max amount allowed (highToLowTurbineInputOperationValue2)"
                          << " instead of amount available" << std::endl;
                //calculate turbine with max power out value
                highToLowPressureTurbine =
                    turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToLowTurbineInput,
                                                       highToLowTurbineInputOperationValue2, lowPressureHeaderInput,
                                                       isCalcIdeal);
        }

        return highToLowPressureTurbine;
}

std::shared_ptr<Turbine>
HighToLowSteamTurbineCalculator::calcPowerGeneration(const double availableMassFlow,
                                                     const PressureTurbine &highToLowTurbineInput,
                                                     const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                     const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                     const Boiler &boiler, const bool isCalcIdeal) const
{
        const std::string methodName = std::string("HighToLowSteamTurbineCalculator::") + std::string(__func__) + ": ";

        std::shared_ptr<Turbine> highToLowPressureTurbine = nullptr;

        const double highToLowTurbineInputOperationValue1 = highToLowTurbineInput.getOperationValue1();

        std::cout << methodName
                  << "calculating highToLowPressureTurbine with power out (highToLowTurbineInputOperationValue1)="
                  << highToLowTurbineInputOperationValue1 << std::endl;
        highToLowPressureTurbine =
            turbineFactory.makePtrWithPowerOut(highPressureHeaderOutput, highToLowTurbineInput,
                                               highToLowTurbineInputOperationValue1, lowPressureHeaderInput,
                                               isCalcIdeal);
        steamBalanceCheckerService.check(highToLowPressureTurbine, availableMassFlow, boiler);

        return highToLowPressureTurbine;
}

std::shared_ptr<Turbine>
HighToLowSteamTurbineCalculator::calcSteamFlow(const double availableMassFlow,
                                               const PressureTurbine &highToLowTurbineInput,
                                               const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                               const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                               const Boiler &boiler, const bool isCalcIdeal) const
{
        const std::string methodName = std::string("HighToLowSteamTurbineCalculator::") + std::string(__func__) + ": ";

        std::shared_ptr<Turbine> highToLowPressureTurbine = nullptr;

        const double highToLowTurbineInputOperationValue1 = highToLowTurbineInput.getOperationValue1();

        std::cout << methodName
                  << "calculating highToLowPressureTurbine with mass flow (highToLowTurbineInputOperationValue1)="
                  << highToLowTurbineInputOperationValue1 << std::endl;
        highToLowPressureTurbine =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput,
                                               highToLowTurbineInputOperationValue1, lowPressureHeaderInput,
                                               isCalcIdeal);
        steamBalanceCheckerService.check(highToLowPressureTurbine, availableMassFlow, boiler);

        return highToLowPressureTurbine;
}

std::shared_ptr<Turbine>
HighToLowSteamTurbineCalculator::calcBalanceHeader(const double availableMassFlow,
                                                   const PressureTurbine &highToLowTurbineInput,
                                                   const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                   const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                   const bool isCalcIdeal) const
{
        const std::string methodName = std::string("HighToLowSteamTurbineCalculator::") + std::string(__func__) + ": ";

        std::shared_ptr<Turbine> highToLowPressureTurbine = nullptr;

        std::cout << methodName
                  << "calculating highToLowPressureTurbine with availableMassFlow=" << availableMassFlow << std::endl;
        highToLowPressureTurbine =
            turbineFactory.makePtrWithMassFlow(highPressureHeaderOutput, highToLowTurbineInput, availableMassFlow,
                                               lowPressureHeaderInput, isCalcIdeal);

        return highToLowPressureTurbine;
}

double
HighToLowSteamTurbineCalculator::calcAvailableMassFlow(const HeaderWithHighestPressure &highPressureHeaderInput,
                                                       const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                       const CondensingTurbine &condensingTurbineInput,
                                                       const std::shared_ptr<Turbine> &condensingTurbine) const
{
        const std::string methodName = std::string("HighToLowSteamTurbineCalculator::") + std::string(__func__) + ": ";

        const double highPressureHeaderOutputMassFlow = highPressureHeaderOutput.massFlow;
        const double highPressureHeaderInputProcessSteamUsage = highPressureHeaderInput.getProcessSteamUsage();
        double availableMassFlow = highPressureHeaderOutputMassFlow - highPressureHeaderInputProcessSteamUsage;
        std::cout << methodName
                  << "highPressureHeaderOutputMassFlow=" << highPressureHeaderOutputMassFlow
                  << " - highPressureHeaderInputProcessSteamUsage=" << highPressureHeaderInputProcessSteamUsage
                  << " = availableMassFlow=" << availableMassFlow
                  << std::endl;

        //remove steam that goes through condensing turbine
        if (condensingTurbineInput.isUseTurbine())
        {
                const double condensingTurbineMassFlow = condensingTurbine->getMassFlow();
                availableMassFlow -= condensingTurbineMassFlow;
                std::cout << methodName
                          << "subtracting condensingTurbineMassFlow=" << condensingTurbineMassFlow
                          << " = availableMassFlow=" << availableMassFlow
                          << std::endl;
        }

        return availableMassFlow;
}
