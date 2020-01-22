#include <ssmt/api/HeaderInput.h>

HeaderWithPressure::HeaderWithPressure(double pressure, double processSteamUsage, double condensationRecoveryRate,
                                       double heatLoss, bool flashCondensate)
        : pressure(pressure), processSteamUsage(processSteamUsage), condensationRecoveryRate(condensationRecoveryRate),
          heatLoss(heatLoss), flashCondensate(flashCondensate) {}

double HeaderWithPressure::getPressure() const {
    return pressure;
}

double HeaderWithPressure::getProcessSteamUsage() const {
    return processSteamUsage;
}

double HeaderWithPressure::getCondensationRecoveryRate() const {
    return condensationRecoveryRate;
}

double HeaderWithPressure::getHeatLoss() const {
    return heatLoss;
}

bool HeaderWithPressure::isFlashCondensate() const {
    return flashCondensate;
}

HeaderWithHighestPressure::HeaderWithHighestPressure(double pressure, double processSteamUsage,
                                                     double condensationRecoveryRate, double heatLoss,
                                                     double condensateReturnTemperature, bool flashCondensate)
        : HeaderWithPressure(pressure, processSteamUsage, condensationRecoveryRate, heatLoss, flashCondensate),
          condensateReturnTemperature(condensateReturnTemperature) {
}

std::ostream &operator<<(std::ostream &stream, const HeaderWithHighestPressure &header) {
    return stream << "HeaderWithHighestPressure["
                  << "pressure=" << header.pressure
                  << ", processSteamUsage=" << header.processSteamUsage
                  << ", condensationRecoveryRate=" << header.condensationRecoveryRate
                  << ", heatLoss=" << header.heatLoss
                  << ", flashCondensate=" << header.flashCondensate
                  << ", condensateReturnTemperature=" << header.condensateReturnTemperature << "]";
}

double HeaderWithHighestPressure::getCondensateReturnTemperature() const {
    return condensateReturnTemperature;
}

HeaderNotHighestPressure::HeaderNotHighestPressure(double pressure, double processSteamUsage,
                                                   double condensationRecoveryRate, double heatLoss,
                                                   bool flashCondensate, bool desuperheatSteamIntoNextHighest,
                                                   double desuperheatSteamTemperature)
        : HeaderWithPressure(pressure, processSteamUsage, condensationRecoveryRate, heatLoss, flashCondensate),
          desuperheatSteamIntoNextHighest(desuperheatSteamIntoNextHighest),
          desuperheatSteamTemperature(desuperheatSteamTemperature) {
}

std::ostream &operator<<(std::ostream &stream, const HeaderNotHighestPressure &header) {
    return stream << "HeaderNotHighestPressure["
                  << "pressure=" << header.pressure
                  << ", processSteamUsage=" << header.processSteamUsage
                  << ", condensationRecoveryRate=" << header.condensationRecoveryRate
                  << ", heatLoss=" << header.heatLoss
                  << ", flashCondensate=" << header.flashCondensate
                  << ", desuperheatSteamIntoNextHighest=" << header.desuperheatSteamIntoNextHighest
                  << ", desuperheatSteamTemperature=" << header.desuperheatSteamTemperature << "]";
}

bool HeaderNotHighestPressure::isDesuperheatSteamIntoNextHighest() const {
    return desuperheatSteamIntoNextHighest;
}

double HeaderNotHighestPressure::getDesuperheatSteamTemperature() const {
    return desuperheatSteamTemperature;
}

HeaderInput::HeaderInput(const HeaderWithHighestPressure &highPressureHeader,
                         const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeader,
                         const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeader)
        : highPressureHeader(highPressureHeader), mediumPressureHeader(mediumPressureHeader),
          lowPressureHeader(lowPressureHeader) {
}

std::ostream &operator<<(std::ostream &stream, const HeaderInput &headerInput) {
    return stream << "HeaderInput["
                  << "highPressureHeader=" << headerInput.highPressureHeader
                  << ", mediumPressureHeader=" << headerInput.mediumPressureHeader
                  << ", lowPressureHeader=" << headerInput.lowPressureHeader << "]";
}

int HeaderInput::getHeaderCount() const {
    int headerCount = 1;    // always at least 1 header (the high)
    if (mediumPressureHeader != nullptr) headerCount++;
    if (lowPressureHeader != nullptr) headerCount++;

    std::cout << "HeaderInput::getHeaderCount: headerCount=" << headerCount << std::endl;

    return headerCount;
}

double HeaderInput::getPressureFromLowestPressureHeader() const {
    return getHeaderCount() == 1 ? highPressureHeader.getPressure() : lowPressureHeader->getPressure();
}

HeaderWithHighestPressure HeaderInput::getHighPressureHeader() const {
    return highPressureHeader;
}

std::shared_ptr<HeaderNotHighestPressure> HeaderInput::getLowPressureHeader() const {
    return lowPressureHeader;
}

std::shared_ptr<HeaderNotHighestPressure> HeaderInput::getMediumPressureHeader() const {
    return mediumPressureHeader;
}
