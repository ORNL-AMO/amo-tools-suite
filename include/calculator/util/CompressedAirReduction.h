#ifndef AMO_LIBRARY_COMPRESSEDAIRREDUCTION_H
#define AMO_LIBRARY_COMPRESSEDAIRREDUCTION_H

#include <exception>
#include <stdexcept>
#include <vector>

class CompressedAirFlowMeterMethodData
{
  public:
    CompressedAirFlowMeterMethodData(const double meterReading)
        : meterReading(meterReading) {}

    double getMeterReading() const { return meterReading; }
    void setMeterReading(double meterReading);

  private:
    double meterReading;
};

class BagMethodData
{
  public:
    BagMethodData(const double height, const double diameter, const double fillTime)
        : height(height), diameter(diameter), fillTime(fillTime) {}

    double getHeight() const { return height; }
    double getDiameter() const { return diameter; }
    double getFillTime() const { return fillTime; }
    void setHeight(double height);
    void setDiameter(double diameter);
    void setFillTime(double fillTime);

  private:
    double height, diameter, fillTime;
};

class PressureMethodData
{
  public:
    PressureMethodData(const int nozzleType, const int numberOfNozzles, const double supplyPressure)
        : nozzleType(nozzleType), numberOfNozzles(numberOfNozzles), supplyPressure(supplyPressure) {}

    int getNozzleType() const { return nozzleType; }
    int getNumberOfNozzles() const { return numberOfNozzles; }
    double getSupplyPressure() const { return supplyPressure; }
    void setNozzleType(int nozzleType);
    void setNumberOfNozzles(int numberOfNozzles);
    void setSupplyPressure(double supplyPressure);

    double calculate();

  private:
    int nozzleType, numberOfNozzles;
    double supplyPressure;
};

class DecibelsMethodData
{
  public:
    DecibelsMethodData(const double linePressure, const double decibels, const double decibelRatingA, 
		  const double pressureA, const double firstFlowA, const double secondFlowA, const double decibelRatingB, const double pressureB,
		  const double firstFlowB, const double secondFlowB)
		  : linePressure(linePressure), decibels(decibels), decibelRatingA(decibelRatingA), 
		    pressureA(pressureA), firstFlowA(firstFlowA), secondFlowA(secondFlowA), decibelRatingB(decibelRatingB),
		    pressureB(pressureB), firstFlowB(firstFlowB), secondFlowB(secondFlowB) {}

    double getLinePressure() const { return linePressure; }
    double getDecibels() const { return decibels; }
    double getDecibelRatingA() const { return decibelRatingA; }
    double getPressureA() const { return pressureA; }
    double getFirstFlowA() const { return firstFlowA; }
    double getSecondFlowA() const { return secondFlowA; }
    double getDecibelRatingB() const { return decibelRatingB; }
    double getPressureB() const { return pressureB; }
    double getFirstFlowB() const { return firstFlowB; }
    double getSecondFlowB() const { return secondFlowB; }

    void setLinePressure(double linePressure);
    void setDecibels(double decibels);
    void setDecibelRatingA(double decibelRatingA);
    void setPressureA(double pressureA);
    void setFirstFlowA(double firstFlowA);
    void setSecondFlowA(double secondFlowA);
    void setDecibelRatingB(double decibelRatingB);
    void setPressureB(double pressureB);
    void setFirstFlowB(double firstFlowB);
    void setSecondFlowB(double secondFlowB);

    double calculate();

  private:
    double linePressure; // X
    double decibels; // Y
    double decibelRatingA; // Y1
    double pressureA; // X1
    double firstFlowA; // Q11
    double secondFlowA; // Q21
    double decibelRatingB; // Y2
    double pressureB; // X2
    double firstFlowB; // Q12
    double secondFlowB; // Q22
};

class EstimateMethodData
{
  public:
    EstimateMethodData(const double leakRateEstimate) : leakRateEstimate(leakRateEstimate) {}

    double getLeakRateEstimate() const {return leakRateEstimate; }
    void setLeakRateEstimate(double leakRateEstimate);

  private:
    double leakRateEstimate;
};

class OrificeMethodData
{
  public:
    OrificeMethodData(const double airTemp, const double atmPressure, const double dischargeCoef, // removed operatingTime
			const double diameter, const double supplyPressure, const int numOrifices)
			: airTemp(airTemp), atmPressure(atmPressure), dischargeCoef(dischargeCoef), diameter(diameter),
			  supplyPressure(supplyPressure), numOrifices(numOrifices) {}

    double getAirTemp() const { return airTemp; }
    double getAtmPressure() const { return atmPressure; }
    double getDischargeCoef() const { return dischargeCoef; }
    double getDiameter() const { return diameter; }
    double getSupplyPressure() const { return supplyPressure; }
    int getNumOrifices() const { return numOrifices; }

    void setAirTemp(double airTemp);
    void setAtmPressure(double atmPressure);
    void setDischargeCoef(double dischargeCoef);
    void setDiameter(double diameter);
    void setSupplyPressure(double supplyPressure);
    void setNumOrifices(int numOrifices);

    double calculate();

  private:
    double airTemp, atmPressure, dischargeCoef, diameter, supplyPressure;
    int numOrifices;
};

class CompressedAirOtherMethodData
{
  public:
    CompressedAirOtherMethodData(const double consumption)
        : consumption(consumption) {}

    double getConsumption() const { return consumption; }
    void setConsumption(double consumption);

  private:
    double consumption;
};

//calculates electricity use for a compressor if utilityType == electricity
class CompressorElectricityData
{
  public:
    CompressorElectricityData(const double compressorControlAdjustment, const double compressorSpecificPower)
        : compressorControlAdjustment(compressorControlAdjustment), compressorSpecificPower(compressorSpecificPower) {}

    double calculate();

  private:
    double compressorControlAdjustment, compressorSpecificPower;
};

class CompressedAirReductionInput
{
  public:
    CompressedAirReductionInput(const int hoursPerYear, const int utilityType, const double utilityCost, const int measurementMethod,
                                const CompressedAirFlowMeterMethodData flowMeterMethodData, const BagMethodData bagMethodData,
                                const PressureMethodData pressureMethodData, const CompressedAirOtherMethodData otherMethodData,
                                const OrificeMethodData orificeMethodData, const DecibelsMethodData decibelsMethodData,
                                const EstimateMethodData estimateMethodData, const CompressorElectricityData compressorElectricityData,
                                const int units)
        : hoursPerYear(hoursPerYear), utilityType(utilityType), utilityCost(utilityCost), measurementMethod(measurementMethod),
          flowMeterMethodData(flowMeterMethodData), bagMethodData(bagMethodData),
          pressureMethodData(pressureMethodData), otherMethodData(otherMethodData), orificeMethodData(orificeMethodData),
          decibelsMethodData(decibelsMethodData), estimateMethodData(estimateMethodData),
          compressorElectricityData(compressorElectricityData), units(units) {}

    int getHoursPerYear() const { return hoursPerYear; } // operating time
    int getUtilityType() const { return utilityType; }
    int getMeasurementMethod() const { return measurementMethod; }
    int getUnits() const { return units; }
    double getUtilityCost() const { return utilityCost; }
    CompressedAirFlowMeterMethodData getFlowMeterMethodData() const { return flowMeterMethodData; }
    BagMethodData getBagMethodData() const { return bagMethodData; }
    PressureMethodData getPressureMethodData() const { return pressureMethodData; }
    CompressedAirOtherMethodData getOtherMethodData() const { return otherMethodData; }
    OrificeMethodData getOrificeMethodData() const { return orificeMethodData; }
    DecibelsMethodData getDecibelsMethodData() const { return decibelsMethodData; }
    EstimateMethodData getEstimateMethodData() const { return estimateMethodData; }
    CompressorElectricityData getCompressorElectricityData() const { return compressorElectricityData; }

  private:
    int hoursPerYear, utilityType;
    double utilityCost;
    int measurementMethod;
    CompressedAirFlowMeterMethodData flowMeterMethodData;
    BagMethodData bagMethodData;
    PressureMethodData pressureMethodData;
    CompressedAirOtherMethodData otherMethodData;
    OrificeMethodData orificeMethodData;
    DecibelsMethodData decibelsMethodData;
    EstimateMethodData estimateMethodData;
    CompressorElectricityData compressorElectricityData;
    int units;
};

class CompressedAirReduction
{
  public:
    struct Output
    {
        Output(double energyUse, double energyCost, double flowRate, double singleNozzleFlowRate, double consumption)
            : energyUse(energyUse), energyCost(energyCost), flowRate(flowRate), singleNozzleFlowRate(singleNozzleFlowRate), consumption(consumption) {}

        Output() = default;
        double energyUse = 0, energyCost = 0, flowRate = 0, singleNozzleFlowRate, consumption = 0;
    };

    CompressedAirReduction(std::vector<CompressedAirReductionInput> &compressedAirReductionInputVec) : compressedAirReductionInputVec(compressedAirReductionInputVec)
    {
    }

    CompressedAirReduction::Output calculate();
    std::vector<CompressedAirReductionInput> const &getCompressedAirReductionInputVec() const
    {
        return compressedAirReductionInputVec;
    }
    void setCompressedAirReductionInputVec(std::vector<CompressedAirReductionInput> &compressedAirReductionInputVec);

  private:
    std::vector<CompressedAirReductionInput> compressedAirReductionInputVec;
    CompressedAirReduction::Output output;
};

#endif // AMO_LIBRARY_COMPRESSEDAIRREDUCTION_H