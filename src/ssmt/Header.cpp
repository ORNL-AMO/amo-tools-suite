#include "ssmt/Header.h"

Inlet::Inlet(const double pressure, const SteamProperties::ThermodynamicQuantity quantityType,
             const double quantityValue, const double massFlow)
    : pressure(pressure), quantityValue(quantityValue), massFlow(massFlow),
      quantityType(quantityType)
{
    calculate();
}

Header::Header(const double headerPressure, std::vector<Inlet> &inletVec)
    : headerPressure(headerPressure), inlets(std::move(inletVec))
{
    calculate();
}

void Header::calculate()
{
    inletEnergyFlow = 0.0, inletMassFlow = 0.0;

    for (auto const &inlet : inlets)
    {
        inletEnergyFlow += inlet.getInletEnergyFlow();
        inletMassFlow += inlet.getMassFlow();
    }

    specificEnthalpy = (inletMassFlow == 0.0) ? 0.0 : inletEnergyFlow / inletMassFlow;
    std::cout << "SPECIFIC ENTHALPY!!!!!!!!!" << specificEnthalpy << std::endl;
    headerProperties = SteamProperties(headerPressure, SteamProperties::ThermodynamicQuantity::ENTHALPY,
                                       specificEnthalpy)
                           .calculate();
    std::cout << "PROPERTIES ENTHALPY!!!!!!!!!" << headerProperties.specificEnthalpy << std::endl;
}

std::ostream &operator<<(std::ostream &stream, const Inlet &inlet)
{
    stream << "Inlet["
           << "pressure=" << inlet.pressure << ", quantityType=" << static_cast<int>(inlet.quantityType)
           << ", quantityValue=" << inlet.quantityValue << ", massFlow=" << inlet.massFlow
           << ", inletEnergyFlow=" << inlet.inletEnergyFlow
           << ", inletProperties=" << inlet.inletProperties << "]";

    return stream;
}

std::ostream &operator<<(std::ostream &stream, const Header &header)
{
    stream << "Header["
           << "headerPressure=" << header.getHeaderPressure()
           << ", specificEnthalpy=" << header.getSpecificEnthalpy()
           << ", inletEnergyFlow=" << header.getInletEnergyFlow()
           << ", inletMassFlow=" << header.getInletMassFlow()
           << ", headerProperties=" << header.getHeaderProperties() << "]";

    return stream;
}

void Header::setHeaderPressure(const double headerPressure)
{
    this->headerPressure = headerPressure;
    headerProperties = SteamProperties(this->headerPressure, SteamProperties::ThermodynamicQuantity::ENTHALPY,
                                       specificEnthalpy)
                           .calculate();
}

void Header::setInlets(std::vector<Inlet> &inlets)
{
    this->inlets = std::move(inlets);
    calculate();
}

void Inlet::calculate()
{
    inletProperties = SteamProperties(pressure, quantityType, quantityValue).calculate();
    inletEnergyFlow = inletProperties.specificEnthalpy * massFlow;
}

void Inlet::setPressure(const double pressure)
{
    this->pressure = pressure;
    calculate();
}

void Inlet::setQuantityValue(const double quantityValue)
{
    this->quantityValue = quantityValue;
    calculate();
}

void Inlet::setMassFlow(const double massFlow)
{
    this->massFlow = massFlow;
    inletEnergyFlow = inletProperties.specificEnthalpy * massFlow;
}

void Inlet::setQuantityType(const SteamProperties::ThermodynamicQuantity quantityType)
{
    this->quantityType = quantityType;
    calculate();
}
