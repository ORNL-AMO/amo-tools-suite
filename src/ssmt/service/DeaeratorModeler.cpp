#include "ssmt/service/DeaeratorModeler.h"

Deaerator
DeaeratorModeler::model(const int headerCountInput, const BoilerInput &boilerInput, const Boiler &boiler,
                        const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                        const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                        const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain,
                        const MakeupWaterAndCondensateHeaderCalculationsDomain &makeupWaterAndCondensateHeaderCalculationsDomain) const {
    const std::string methodName = std::string("DeaeratorModeler::") + std::string(__func__) + ": ";

    // std::cout << methodName << "calculating deaerator" << std::endl;

    const double feedwaterMassFlow =
            calcFeedwaterMassFlow(headerCountInput, boiler, mediumPressureHeaderCalculationsDomain,
                                  lowPressureHeaderCalculationsDomain);
    const Deaerator &deaerator =
            makeDeaerator(headerCountInput, boilerInput, highPressureHeaderCalculationsDomain,
                          lowPressureHeaderCalculationsDomain, makeupWaterAndCondensateHeaderCalculationsDomain,
                          feedwaterMassFlow);

    return deaerator;
}

double DeaeratorModeler::calcFeedwaterMassFlow(const int headerCountInput, const Boiler &boiler,
                                               const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                                               const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("DeaeratorModeler::") + std::string(__func__) + ": ";

    //6. Calculate Deaerator
    //6A. Get Feedwater Details and Inlet header
    // std::cout << methodName << "calculating feedwaterMassFlow from boiler" << std::endl;

    const SteamSystemModelerTool::FluidProperties &feedwaterProperties = boiler.getFeedwaterProperties();
    double feedwaterMassFlow = feedwaterProperties.massFlow;

    if (headerCountInput > 1) {
        const std::shared_ptr<PrvWithoutDesuperheating> &lowPressurePrv =
                lowPressureHeaderCalculationsDomain->lowPressurePrv;
        const double lowOutletMassFlow = getFeedwaterMassFlow(lowPressurePrv);
        // std::cout << methodName
                //   << "lowPressureHeader exists, adding feedwater mass flow from lowPressurePrv=" << lowOutletMassFlow
                //   << " to feedwaterMassFlow" << std::endl;
        feedwaterMassFlow += lowOutletMassFlow;

        if (headerCountInput == 3) {
            const std::shared_ptr<PrvWithoutDesuperheating> &highToMediumPressurePrv =
                    mediumPressureHeaderCalculationsDomain->highToMediumPressurePrv;
            const double highOutletMassFlow = getFeedwaterMassFlow(highToMediumPressurePrv);
            // std::cout << methodName
                    //   << "mediumPressureHeader exists, adding feedwater mass flow from highToMediumPressurePrv="
                    //   << highOutletMassFlow << " to feedwaterMassFlow" << std::endl;
            feedwaterMassFlow += highOutletMassFlow;
        }
    } else {
        // std::cout << methodName
                //   << "lowPressureHeader does not exist, skipping lowPressurePrv feedwaterMassFlow" << std::endl;
    }

    // std::cout << methodName << "feedwaterMassFlow=" << feedwaterMassFlow << std::endl;

    return feedwaterMassFlow;
}

double DeaeratorModeler::getFeedwaterMassFlow(const std::shared_ptr<PrvWithoutDesuperheating> &prv) const {
    double massFlow = 0;

    if (prv != nullptr) {
        if (prv->isWithDesuperheating()) {
            std::shared_ptr<PrvWithDesuperheating> with =
                    std::static_pointer_cast<PrvWithDesuperheating>(prv);
            massFlow = with->getFeedwaterMassFlow();
        }
    }

    return massFlow;
}

Deaerator DeaeratorModeler::makeDeaerator(const int headerCountInput, const BoilerInput &boilerInput,
                                          const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                          const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain,
                                          const MakeupWaterAndCondensateHeaderCalculationsDomain &makeupWaterAndCondensateHeaderCalculationsDomain,
                                          const double feedwaterMassFlow) const {
    const std::string methodName = std::string("DeaeratorModeler::") + std::string(__func__) + ": ";


    //6B. Calculate Deaerator
    // std::cout << methodName << "making deaerator" << std::endl;

    const SteamSystemModelerTool::FluidProperties &makeupWaterAndCondensateHeaderOutput =
            makeupWaterAndCondensateHeaderCalculationsDomain.makeupWaterAndCondensateHeaderOutput;
    const SteamSystemModelerTool::FluidProperties &inletHeaderOutput =
            headerCountInput == 1 ? highPressureHeaderCalculationsDomain.highPressureHeaderOutput
                                  : lowPressureHeaderCalculationsDomain->lowPressureHeaderOutput;

    return deaeratorFactory.make(boilerInput, feedwaterMassFlow, makeupWaterAndCondensateHeaderOutput,
                                 inletHeaderOutput);
}
