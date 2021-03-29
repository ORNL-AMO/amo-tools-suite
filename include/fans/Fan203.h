/**
 * @brief Contains some of the Fan related classes
 *
 * @author Preston Shires (pshires)
 * @author Allie Ledbetter (Aeledbetter)
 * @author Colin Causey (causeyc)
 * @bug No known bugs.
 * 
 */

#ifndef AMO_TOOLS_SUITE_FAN_H
#define AMO_TOOLS_SUITE_FAN_H

#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <functional>
#include "Planar.h"
#include "FanShaftPower.h"

#include <fstream>
#include <iostream>

class FanRatedInfo;
class Planar;
class FlangePlane;
class TraversePlane;
class MstPlane;
/**
 * Base Gas Density Class
 * Calculates base gas density
 */

class BaseGasDensity
{
public:
	/**
  * enum class for Gas Type
  * 
  */
	enum class GasType
	{
		AIR,
		STANDARDAIR,
		OTHERGAS
	};
	/**
 * enum class for Input Type
 * 
 */
	enum class InputType
	{
		DewPoint,
		RelativeHumidity,
		WetBulbTemp
	};

	/**
 * @param dryBulbTemp double, temperature of inputted air in °F
 * @param staticPressure const, double, pressure in Hg 
 * @param barometricPressure,const, double in Hg
 * @param gasDensity const, double, density of a gas in pounds per sqft,lb/scf
 * @param gasType, double, gas, type of gas
 */
	// used for method 1
	BaseGasDensity(const double dryBulbTemp, const double staticPressure, const double barometricPressure,
				   const double gasDensity, const GasType gasType)
		: tdo(dryBulbTemp), pso(staticPressure), pbo(barometricPressure), gasDensity(gasDensity), gasType(gasType)
	{
	}
	/**
 * @param dryBulbTemp double, const, temperature of inputted air in °F
 * @param staticPressure double, const, pressure in inches of water (in WC)
 * @param barometricPressure, double, const,  pressure in Hg
 * @param relativeHumidityOrDewPoint double, const, relative humidity in % or Dewpoint in °F
 * @param gasType, double, gas, type of gas, unitless
 * @param inputType const, type of input, unitless
 * @param specificGravity, double, const, specific gravity, unitless
 * @return gasDensity double, density of the gas in pounds per sqft, lb/scf
 * @return absolutePressure double, Absolute Pressure In in in Hg
 * @return saturatedHumidity double, Saturated Humidity Ratio, unitless
 * @return saturationDegree double, Degree of Saturation, unitless
 * @return humidityRatio double, Humidity Ratio, unitless
 * @return specificVolume double, Specific Volume, ft^3/lb
 * @return entropy double, Entropy, B/lb
 * @return dewPoint double, Dewpoint, deg F
 * @return relativeHumidity double, Relative Humidity, %
 * @return saturationPressure double, Saturation Pressure, in Hg
 */
	// TODO ensure correctness
	BaseGasDensity(double const dryBulbTemp, double const staticPressure, double const barometricPressure,
				   double const relativeHumidityOrDewPoint, GasType const gasType,
				   InputType const inputType, double const specificGravity)
		: tdo(dryBulbTemp), pso(staticPressure), pbo(barometricPressure), g(specificGravity), gasType(gasType)
	{
		saturationPressure = calculateSaturationPressure(tdo);
		relativeHumidity = 0;
		if (inputType == InputType::RelativeHumidity)
		{
			relativeHumidity = relativeHumidityOrDewPoint / 100;
		}
		else if (inputType == InputType::DewPoint)
		{
			relativeHumidity = calculateSaturationPressure(relativeHumidityOrDewPoint) / saturationPressure;
		}
		else if (inputType == InputType::WetBulbTemp)
		{
			throw std::runtime_error("The wrong constructor for BaseGasDensity was called here - check inputType field");
		}

		calculateFanAttributes(inputType, relativeHumidityOrDewPoint);

		/*
		std::ofstream fout;
    	fout.open("debug.txt", std::ios::app);
		fout << "gasDensity:       " << gasDensity << std::endl;
		fout << "absolutePressure:      " << absolutePressure << std::endl;
		fout << "saturatedHumidity:     " << saturatedHumidity << std::endl;
		fout << "saturationDegree:   " << saturationDegree << std::endl;
		fout << "humidityRatio:     " << humidityRatio << std::endl;
		fout << "specificVolume:  " << specificVolume << std::endl;
		fout << "enthalpy: " << enthalpy << std::endl;
		fout << "dewPoint: " << dewPoint << std::endl;
		fout << "relativeHumidity:       " << relativeHumidity << std::endl;
		fout << "saturationPressure: " << saturationPressure << std::endl;
		fout << "------------------------------" << std::endl << std::endl;
		fout.close();
		*/
	}

	BaseGasDensity(double const dryBulbTemp, double const staticPressure, double const barometricPressure,
				   double const wetBulbTemp, GasType const gasType,
				   InputType const inputType, double const specificGravity, const double cpGas)
		: tdo(dryBulbTemp), pso(staticPressure), pbo(barometricPressure), wetBulbTemp(wetBulbTemp), g(specificGravity), gasType(gasType)
	{
		if (inputType != InputType::WetBulbTemp)
			throw std::runtime_error("The wrong constructor for BaseGasDensity was called - check inputType field");
		saturationPressure = calculateSaturationPressure(tdo);
		relativeHumidity = calculateRelativeHumidityFromWetBulb(tdo, wetBulbTemp, cpGas);

		calculateFanAttributes(inputType);

		/*
		std::ofstream fout;
    	fout.open("debug.txt", std::ios::app);
		fout << "Wet Bulb" << std::endl;
		fout << "gasDensity:       " << gasDensity << std::endl;
		fout << "absolutePressure:      " << absolutePressure << std::endl;
		fout << "saturatedHumidity:     " << saturatedHumidity << std::endl;
		fout << "saturationDegree:   " << saturationDegree << std::endl;
		fout << "humidityRatio:     " << humidityRatio << std::endl;
		fout << "specificVolume:  " << specificVolume << std::endl;
		fout << "enthalpy: " << enthalpy << std::endl;
		fout << "dewPoint: " << dewPoint << std::endl;
		fout << "relativeHumidity:       " << relativeHumidity << std::endl;
		fout << "saturationPressure: " << saturationPressure << std::endl;
		fout << "------------------------------" << std::endl << std::endl;
		fout.close();
		*/
	}

	double getGasDensity() const
	{
		return gasDensity; // po
	}
	double getAbsolutePressureIn() const
	{
		return absolutePressure; // pIn
	}
	double getSaturatedHumidityRatio() const
	{
		return saturatedHumidity; // satW
	}
	double getDegreeOfSaturation() const
	{
		return saturationDegree; // satDeg
	}
	double getHumidityRatio() const
	{
		return humidityRatio; // humW
	}
	double getSpecificVolume() const
	{
		return specificVolume; // specVol
	}
	double getEnthalpy() const
	{
		return enthalpy;
	}
	double getDewPoint() const
	{
		return dewPoint;
	}
	double getRelativeHumidity() const
	{
		return relativeHumidity; // rh
	}
	double getSaturationPressure() const
	{
		return saturationPressure; // satPress
	}
	double getWetBulbTemp() const
	{
		return wetBulbTemp; // Tdb
	}

private:
	/**
 * @brief Calculates Wet Bulb Temperature
 * 
 * @param dryBulbTemp double, temperature of inputted air in °F
 * @param relativeHumidity double, Relative Humidity, %
 * @param barometricPressure, double, const,  pressure in Hg
 * @return wetBulbTemp double, Wet Bulb Temperature, °F
 */
	double calculateWetBulbTemperature(double dryBulbTemp, double relativeHumidity, double absolutePressure) const
	{
		// Newton-Raphson iteration (solve to within 0.001% accuracy)

		double humidityRatioNormal = calculateRatioRH(dryBulbTemp, relativeHumidity, absolutePressure, 1);
		double wetBulbTemp = dryBulbTemp; // set initial guess
		double humidityRatioNew = calculateHumidityRatioFromWetBulb(dryBulbTemp, wetBulbTemp, 0.24);
		if (humidityRatioNormal > 0)
		{
			while (fabs((humidityRatioNew - humidityRatioNormal) / humidityRatioNormal) > 0.00001)
			{
				double humidityRatioNew2 = calculateHumidityRatioFromWetBulb(dryBulbTemp, wetBulbTemp - 0.001, 0.24);
				double dw_dtwb = (humidityRatioNew - humidityRatioNew2) / 0.001;
				wetBulbTemp = wetBulbTemp - (humidityRatioNew - humidityRatioNormal) / dw_dtwb;
				humidityRatioNew = calculateHumidityRatioFromWetBulb(dryBulbTemp, wetBulbTemp, 0.24);
			}
		}
		else
		{
			while (fabs(humidityRatioNew) > 0.00001)
			{
				double humidityRatioNew2 = calculateHumidityRatioFromWetBulb(dryBulbTemp, wetBulbTemp - 0.001, 0.24);
				double dw_dtwb = (humidityRatioNew - humidityRatioNew2) / 0.001;
				wetBulbTemp = wetBulbTemp - (humidityRatioNew - humidityRatioNormal) / dw_dtwb;
				humidityRatioNew = calculateHumidityRatioFromWetBulb(dryBulbTemp, wetBulbTemp, 0.24);
			}
		}
		return wetBulbTemp;
	}
	/**
 * @brief Calculates Saturation Pressure
 * 
 * @param dryBulbTemp double, temperature of inputted air in °F
 * @return saturationPressure double, Saturation Pressure, in Hg
 */
	double calculateSaturationPressure(double dryBulbTemp) const
	{
		double const C1 = -5674.5359;
		double const C2 = 6.3925247;
		double const C3 = -0.009677843;
		double const C4 = 0.00000062215701;
		double const C5 = 2.0747825 * std::pow(10, -9);
		double const C6 = -9.484024 * std::pow(10, -13);
		double const C7 = 4.1635019;
		double const C8 = -5800.2206;
		double const C9 = 1.3914993;
		double const C10 = -0.048640239;
		double const C11 = 0.000041764768;
		double const C12 = -0.000000014452093;
		double const C13 = 6.5459673;

		double const tKelvin = (dryBulbTemp + 459.67) * 0.555556;

		if (tKelvin < 273.15)
		{
			double const p = std::exp(C1 / tKelvin + C2 + tKelvin * C3 + tKelvin * tKelvin * (C4 + tKelvin * (C5 + C6 * tKelvin)) + C7 * std::log(tKelvin));
			return p * (29.9216 / 101325);
		}
		double const p = std::exp(C8 / tKelvin + C9 + tKelvin * (C10 + tKelvin * (C11 + tKelvin * C12)) + C13 * std::log(tKelvin));

		return p * (29.9216 / 101325);
	}
	/**
 * @brief Calculates Relative Humidity Ratio
 * 
 * @param dryBulbTemp double, temperature of inputted air in °F 
 * @param relativeHumidity double, relative humidity as %
 * @param barometricPressure double, pressure in Hg 
 * @param specificGravity double, specific gravity, unitless
 * @return humidityRatio double, Humidity Ratio, unitless
 */
	double calculateRatioRH(const double dryBulbTemp, const double relativeHumidity, const double barometricPressure,
							const double specificGravity) const
	{
		auto const pw = (calculateSaturationPressure(dryBulbTemp) * relativeHumidity);
		return (18.02 / (specificGravity * 28.98)) * pw / (barometricPressure - pw);
	}
	/**
 * @brief Calculates Relative Humidity from Wet Bulb Temperature
 * 
 * @param dryBulbTemp double, temperature of inputted air in °F
 * @param wetBulbTemp double, wet bulb temperature in °F
 * @param cpGas double, BTU/lb-degF
 */
	double calculateRelativeHumidityFromWetBulb(const double dryBulbTemp, const double wetBulbTemp,
												const double cpGas) const
	{
		double const nMol = 0.62198;
		double const local_pIn = pbo + (pso / 13.608703);
		//double const pAtm = 29.9213 / pbo, nMol = 18.02 / (g * 28.98);
		double const psatDb = calculateSaturationPressure(dryBulbTemp);
		//	double const wSat = nMol * psatDb / (pAtm - psatDb);
		double const psatWb = calculateSaturationPressure(wetBulbTemp);
		double const wStar = nMol * psatWb / (local_pIn - psatWb);
		double const w = ((1093 - (1 - 0.444) * wetBulbTemp) * wStar - cpGas * (dryBulbTemp - wetBulbTemp)) / (1093 + (0.444 * dryBulbTemp) - wetBulbTemp);

		double const pV = local_pIn * w / (nMol + w);

		/*
		std::ofstream fout;
    	fout.open("debug.txt", std::ios::app);
		fout << "calculateRelativeHumidityFromWetBulb" << std::endl;
		fout << "psatDb: " << psatDb << std::endl;
		fout << "psatWb: " << psatWb << std::endl;
		fout << "wStar:  " << wStar << std::endl;
		fout << "w:      " << w << std::endl;
		fout << "------------------------------" << std::endl << std::endl;
		fout.close();
		*/

		return pV / psatDb;
	}
	/**
 * @brief Calculates Relative Humidity Ratio from Wet Bulb Temperature
 * 
 * @param dryBulbTemp double, temperature of inputted air in °F
 * @param wetBulbTemp double, wet bulb temperature in °F
 * @param cpGas double, BTU/lb-degF
 * @return humidityRatio double, Humidity Ratio, unitless
 */
	double calculateHumidityRatioFromWetBulb(const double dryBulbTemp, const double wetBulbTemp, const double cpGas) const
	{
		double const nMol = 0.62198;
		double const local_pIn = pbo + (pso / 13.608703);

		double const psatWb = calculateSaturationPressure(wetBulbTemp);
		double const wStar = nMol * psatWb / (local_pIn - psatWb);
		double const w = ((1093 - (1 - 0.444) * wetBulbTemp) * wStar - cpGas * (dryBulbTemp - wetBulbTemp)) / (1093 + (0.444 * dryBulbTemp) - wetBulbTemp);

		return w;
	}
	/**
 * @brief Calculates numerous fan attributes. Note: This function assumes that the member variables pbo, pso, saturationPressure, and relativeHumidity already 
 		  have valid values.
 * 
 * @param inputType const, type of input, unitless
 * @param relativeHumidityOrDewPoint double, const, relative humidity in % or Dewpoint in °F
 */
	void calculateFanAttributes(InputType const inputType, double const relativeHumidityOrDewPoint = -1)
	{
		double const nMol = 0.62198;

		absolutePressure = pbo + (pso / 13.608703);
		saturatedHumidity = nMol * saturationPressure / (absolutePressure - saturationPressure);
		saturationDegree = relativeHumidity / (1 + (1 - relativeHumidity) * saturatedHumidity / nMol);
		humidityRatio = saturationDegree * saturatedHumidity;
		specificVolume = (10.731557 * (tdo + 459.67) * (1 + 1.6078 * humidityRatio)) / (28.9645 * absolutePressure * 0.4911541);
		gasDensity = (1 / specificVolume) * (1 + humidityRatio);
		enthalpy = (0.247 * tdo) + (humidityRatio * (1061 + 0.444 * tdo));

		if (inputType != InputType::DewPoint)
		{
			double const alpha = std::log(absolutePressure * 0.4911541 * humidityRatio / (nMol + humidityRatio));

			if (tdo < 32)
			{
				dewPoint = 90.12 + 26.412 * alpha + 0.8927 * alpha * alpha;
			}
			else
			{
				dewPoint = 100.45 + 33.193 * alpha + 2.319 * alpha * alpha + 0.17074 * alpha * alpha * alpha + 1.2063 * (std::pow((absolutePressure * 0.4911541 * humidityRatio / (0.62196 + humidityRatio)), 0.1984));
			}
		}
		else
		{
			dewPoint = relativeHumidityOrDewPoint;
		}

		if (inputType != InputType::WetBulbTemp) // If not given as an input, calculate wet bulb temperature
		{
			wetBulbTemp = calculateWetBulbTemperature(tdo, relativeHumidity, absolutePressure);
		}
	}

	// dry bulb temp, reference static pressure, reference barometric pressure, gas density respectively
	const double tdo, pso, pbo;

	// gasDensity, specificGravity
	/**
	 * @param gasDensity double, GasDensity, density of a gas in pounds per sqft, lb/scf
	 * @param g double, Specific Gravity, unitless
	 */
	double gasDensity, g;
	const GasType gasType;

	/**
	 * @param absolutePressure double, Absolute Pressure In in in Hg XXX
	 * @param saturatedHumidity double, Saturated Humidity Ratio, unitless
	 * @param saturationDegree double, Degree of Saturation, unitless
	 * @param humidityRatio double, Humidity Ratio, unitless
	 * @param specificVolume double, Specific Volume, ft^3/lb
	 * @param entropy double, Entropy, B/lb
	 * @param dewPoint double, Dewpoint, deg F
	 * @param relativeHumidity double, Relative Humidity, %
	 * @param saturationPressure double, Saturation Pressure, in Hg
	 * @param wetBulbTemp double, Wet Bulb Temperature, deg F
	 */
	double absolutePressure, saturatedHumidity, saturationDegree, humidityRatio, specificVolume, enthalpy, dewPoint, relativeHumidity, saturationPressure, wetBulbTemp;

	friend class PlaneData;
	friend class Fan203;
};

/**
 * Constructor for the Plane Data class
 * Calculates Plane Data
 * 
 */
class PlaneData
{
public:
	// used to access private stuff from the nan bindings
	/**
	 * @param density const, double, density in pounds per sqft, lb/scf
	 * @param velocity const, double, velocity in ft/min
	 * @param volumeFlowRate const, double,  ft3/min
	 * @param velocityPressure const, double, pressure in inches of water (in WC)
	 * @param totalPressure const, double, pressure in inches of water (in WC)
	 */
	struct NodeBinding
	{
		struct Data
		{
			Data(const double density, const double velocity, const double volumeFlowRate, const double velocityPressure,
				 const double totalPressure)
				: gasDensity(density), gasVelocity(velocity), gasVolumeFlowRate(volumeFlowRate),
				  gasVelocityPressure(velocityPressure), gasTotalPressure(totalPressure) {}

			const double gasDensity = 0, gasVelocity = 0, gasVolumeFlowRate = 0, gasVelocityPressure = 0, gasTotalPressure = 0;
		};
		/**
	 * @param density const, double, density in pounds per sqft, lb/scf
	 * @param velocity const, double, velocity in ft/min
	 * @param volumeFlowRate const, double, ft3/min
	 * @param velocityPressure const, double, pressure in inches of water (in WC)
	 * @param totalPressure const, double, pressure in inches of water (in WC)
	 * @staticPressure const, double, pressure in inches of water (in WC)
	 */
		struct DataFlange : Data
		{
			DataFlange(const double density, const double velocity, const double volumeFlowRate, const double velocityPressure,
					   const double totalPressure, const double staticPressure)
				: Data(density, velocity, volumeFlowRate, velocityPressure, totalPressure),
				  staticPressure(staticPressure) {}

			const double staticPressure = 0;
		};
		struct Output
		{
			explicit Output(PlaneData const &planeData)
				: fanInletFlange(getDataFlange(planeData.fanInletFlange)),
				  fanOrEvaseOutletFlange(getDataFlange(planeData.fanOrEvaseOutletFlange)),
				  flowTraverse(getData(planeData.flowTraverse)),
				  inletMstPlane(getData(planeData.inletMstPlane)),
				  outletMstPlane(getData(planeData.outletMstPlane)),
				  addlTravPlanes(getDataTrav(planeData.addlTravPlanes))
			{
			}
			DataFlange fanInletFlange, fanOrEvaseOutletFlange;
			Data flowTraverse, inletMstPlane, outletMstPlane;
			std::vector<Data> addlTravPlanes;
		};
		static Output calculate(PlaneData &planeData, BaseGasDensity const &baseGasDensity)
		{
			planeData.calculate(baseGasDensity);
			return Output(planeData);
		}

	private:
		static Data getData(Planar const &plane)
		{
			return {plane.gasDensity, plane.gasVelocity, plane.gasVolumeFlowRate, plane.gasVelocityPressure, plane.gasTotalPressure};
		}
		static DataFlange getDataFlange(Planar const &plane)
		{
			return {plane.gasDensity, plane.gasVelocity, plane.gasVolumeFlowRate, plane.gasVelocityPressure, plane.gasTotalPressure, plane.staticPressure};
		}
		static std::vector<Data> getDataTrav(std::vector<TraversePlane> const &addlPlanes)
		{
			std::vector<Data> data;
			for (auto const &plane : addlPlanes)
			{
				data.push_back({plane.gasDensity, plane.gasVelocity, plane.gasVolumeFlowRate, plane.gasVelocityPressure, plane.gasTotalPressure});
			}
			return data;
		}
	};

	PlaneData(FlangePlane fanInletFlange, FlangePlane fanOrEvaseOutletFlange,
			  TraversePlane flowTraverse, std::vector<TraversePlane> addlTravPlanes,
			  MstPlane inletMstPlane, MstPlane outletMstPlane,
			  const double totalPressureLossBtwnPlanes1and4, const double totalPressureLossBtwnPlanes2and5,
			  bool const plane5upstreamOfPlane2)
		: fanInletFlange(std::move(fanInletFlange)), fanOrEvaseOutletFlange(std::move(fanOrEvaseOutletFlange)), flowTraverse(std::move(flowTraverse)),
		  addlTravPlanes(std::move(addlTravPlanes)), inletMstPlane(std::move(inletMstPlane)), outletMstPlane(std::move(outletMstPlane)),
		  plane5upstreamOfPlane2(plane5upstreamOfPlane2),
		  totalPressureLossBtwnPlanes1and4(totalPressureLossBtwnPlanes1and4),
		  totalPressureLossBtwnPlanes2and5(totalPressureLossBtwnPlanes2and5)
	{
	}

private:
	void establishFanInletOrOutletDensity(Planar &plane,
										  std::function<double(Planar const &, const double)> const &calcDensity,
										  double const mTotal, double const assumedDensity)
	{
		double calculatedDensity = assumedDensity;
		for (auto i = 0; i < 50; i++)
		{
			plane.gasDensity = calculatedDensity;
			std::cout << "plane.gasDensity: " << plane.gasDensity << std::endl;
			plane.gasVolumeFlowRate = mTotal / plane.gasDensity;
			std::cout << "plane.gasVolumeFlowRate: " << plane.gasVolumeFlowRate << std::endl;
			plane.gasVelocity = plane.gasVolumeFlowRate / plane.area;
			std::cout << "plane.gasVelocity: " << plane.gasVelocity << std::endl;
			plane.gasVelocityPressure = plane.gasDensity * std::pow(plane.gasVelocity / 1096, 2);
			std::cout << "plane.gasVelocityPressure: " << plane.gasVelocityPressure << std::endl;
			std::cout << "plane.gasTotalPressure: " << plane.gasTotalPressure << std::endl;
			double fanInletOrOutletStaticPressure = plane.gasTotalPressure - plane.gasVelocityPressure;
			std::cout << "fanInletOrOutletStaticPressure: " << fanInletOrOutletStaticPressure << std::endl;
			double fanInletOrOutletGasDensity = calcDensity(plane, fanInletOrOutletStaticPressure);
			std::cout << "fanInletOrOutletGasDensity: " << fanInletOrOutletGasDensity << std::endl;

			calculatedDensity = fanInletOrOutletGasDensity;
			double testVal = fabs(fanInletOrOutletGasDensity - plane.gasDensity);
			std::cout << "while: " << testVal << std::endl;
			if (fabs(fanInletOrOutletGasDensity - plane.gasDensity) < 0.0001)
			{
				plane.gasDensity = fanInletOrOutletGasDensity;
				std::cout << "plane.gasDensity: " << plane.gasDensity << std::endl;
				plane.staticPressure = fanInletOrOutletStaticPressure;
				std::cout << "plane.staticPressure : " << plane.staticPressure << std::endl;
				return;
			}
		}
		throw std::runtime_error("In PlaneData::establishFanInletOrOutletDensity - density iteration did not converge");
	}

	void calculate(BaseGasDensity const &bgd)
	{
		/*
		If using custom density or the fields for dry bulb temperature, static pressure and barometric pressure are blank in "Base Gas Density", 
		then the desktop is sending the values entered for Plane 3a (the first traverse plane).
		 */
		auto const calcDensity = [&bgd](Planar const &plane, const double psx) {
			return bgd.gasDensity * (bgd.tdo + 460) * (psx + 13.63 * plane.barometricPressure) / ((plane.dryBulbTemperature + 460) * (bgd.pso + 13.63 * bgd.pbo));
		};

		flowTraverse.gasDensity = calcDensity(flowTraverse, flowTraverse.staticPressure);
		for (auto &p : addlTravPlanes)
		{
			p.gasDensity = calcDensity(p, p.staticPressure);
		}
		inletMstPlane.gasDensity = calcDensity(inletMstPlane, inletMstPlane.staticPressure);
		outletMstPlane.gasDensity = calcDensity(outletMstPlane, outletMstPlane.staticPressure);

		flowTraverse.gasVelocity = 1096 * std::sqrt(flowTraverse.pv3 / flowTraverse.gasDensity);
		flowTraverse.gasVolumeFlowRate = flowTraverse.gasVelocity * flowTraverse.area;
		//MARK ADDITION FOR issue 259
		flowTraverse.gasVelocityPressure = flowTraverse.gasDensity * std::pow((flowTraverse.gasVelocity / 1096), 2);
		flowTraverse.gasTotalPressure = flowTraverse.staticPressure + flowTraverse.gasVelocityPressure;

		double mTotal = flowTraverse.gasDensity * flowTraverse.gasVolumeFlowRate;
		for (auto &plane : addlTravPlanes)
		{
			plane.gasVelocity = 1096 * std::sqrt(plane.pv3 / plane.gasDensity);
			plane.gasVolumeFlowRate = plane.gasVelocity * plane.area;
			//MARK ADDITION FOR issue 259
			plane.gasVelocityPressure = plane.gasDensity * std::pow((plane.gasVelocity / 1096), 2);
			plane.gasTotalPressure = plane.staticPressure + plane.gasVelocityPressure;
			mTotal += plane.gasDensity * plane.gasVolumeFlowRate;
		}

		inletMstPlane.gasVolumeFlowRate = mTotal / inletMstPlane.gasDensity;
		inletMstPlane.gasVelocity = inletMstPlane.gasVolumeFlowRate / inletMstPlane.area;
		inletMstPlane.gasVelocityPressure = inletMstPlane.gasDensity * std::pow((inletMstPlane.gasVelocity / 1096), 2);
		inletMstPlane.gasTotalPressure = inletMstPlane.staticPressure + inletMstPlane.gasVelocityPressure;

		// step 7
		fanInletFlange.gasTotalPressure = inletMstPlane.gasTotalPressure - totalPressureLossBtwnPlanes1and4;

		// steps 8 - 13
		establishFanInletOrOutletDensity(fanInletFlange, calcDensity, mTotal, inletMstPlane.gasDensity);

		// calculating plane 2 inlet density and pressure
		outletMstPlane.gasVolumeFlowRate = mTotal / outletMstPlane.gasDensity;
		outletMstPlane.gasVelocity = outletMstPlane.gasVolumeFlowRate / outletMstPlane.area;
		outletMstPlane.gasVelocityPressure = outletMstPlane.gasDensity * std::pow(outletMstPlane.gasVelocity / 1096, 2);
		outletMstPlane.gasTotalPressure = outletMstPlane.staticPressure + outletMstPlane.gasVelocityPressure;

		// step 7
		fanOrEvaseOutletFlange.gasTotalPressure = outletMstPlane.gasTotalPressure;
		fanOrEvaseOutletFlange.gasTotalPressure +=
			(plane5upstreamOfPlane2) ? -totalPressureLossBtwnPlanes2and5 : totalPressureLossBtwnPlanes2and5;

		// step 8 - iteration
		establishFanInletOrOutletDensity(fanOrEvaseOutletFlange, calcDensity, mTotal, outletMstPlane.gasDensity);
	}

	FlangePlane fanInletFlange, fanOrEvaseOutletFlange;
	TraversePlane flowTraverse;
	std::vector<TraversePlane> addlTravPlanes;
	MstPlane inletMstPlane;
	MstPlane outletMstPlane;

	bool const plane5upstreamOfPlane2;
	const double totalPressureLossBtwnPlanes1and4, totalPressureLossBtwnPlanes2and5;

	friend class Fan203;
	friend struct NodeBinding;
};

/**
 * Constructor for Fan203
 * Calculates Fan203
 *
 */
class Fan203
{
public:
	Fan203(FanRatedInfo fanRatedInfo, PlaneData planeData, BaseGasDensity baseGasDensity, FanShaftPower fanShaftPower)
		: fanRatedInfo(fanRatedInfo), planeData(std::move(planeData)),
		  baseGasDensity(baseGasDensity), fanShaftPower(fanShaftPower)
	{
		this->planeData.calculate(this->baseGasDensity);
	};

	struct Results
	{
		Results(const double kpc, const double power, const double flow,
				const double pressureTotal, const double pressureStatic,
				const double staticPressureRise)
			: kpc(kpc), power(power),
			  flow(flow), pressureTotal(pressureTotal),
			  pressureStatic(pressureStatic), staticPressureRise(staticPressureRise)
		{
		}
		const double kpc, power, flow, pressureTotal;
		const double pressureStatic, staticPressureRise;
	};

	struct Output
	{
		Output(const double fanEfficiencyTotalPressure, const double fanEfficiencyStaticPressure,
			   const double fanEfficiencyStaticPressureRise, const Results asTested, const Results converted)
			: fanEfficiencyTotalPressure(fanEfficiencyTotalPressure), fanEfficiencyStaticPressure(fanEfficiencyStaticPressure),
			  fanEfficiencyStaticPressureRise(fanEfficiencyStaticPressureRise), asTested(asTested), converted(converted)
		{
		}

		const double fanEfficiencyTotalPressure, fanEfficiencyStaticPressure, fanEfficiencyStaticPressureRise;
		const Results asTested, converted;
	};

	Output calculate()
	{
		// TODO barometricPressure = barometric pressure, what to do if barometric pressure does vary between planes ? pg
		double const x = (planeData.fanOrEvaseOutletFlange.gasTotalPressure - planeData.fanInletFlange.gasTotalPressure) / (planeData.fanInletFlange.gasTotalPressure + 13.63 * planeData.fanInletFlange.barometricPressure);

		double isentropicExponent = 1.4; // TODO what value to use for GasTypes other than Air ?
		if (baseGasDensity.gasType == BaseGasDensity::GasType::AIR)
			isentropicExponent = 1.4;

		// TODO barometricPressure = barometric pressure, what to do if barometric pressure does vary between planes ? pg 61
		double const z = ((isentropicExponent - 1) / isentropicExponent) * ((6362 * fanShaftPower.getFanPowerInput() / planeData.fanInletFlange.gasVolumeFlowRate) / (planeData.fanInletFlange.gasTotalPressure + 13.63 * planeData.fanInletFlange.barometricPressure));

		double const kp = (std::log(1 + x) / x) * (z / (std::log(1 + z)));

		planeData.flowTraverse.gasTotalPressure =
			planeData.flowTraverse.staticPressure + planeData.flowTraverse.gasVelocityPressure;
		for (auto &p : planeData.addlTravPlanes)
		{
			p.gasTotalPressure = p.staticPressure + p.gasVelocityPressure;
		}

		double const fanTotalPressure = planeData.fanOrEvaseOutletFlange.gasTotalPressure - planeData.fanInletFlange.gasTotalPressure + fanShaftPower.getSEF();

		double const fanStaticPressure = planeData.fanOrEvaseOutletFlange.staticPressure - planeData.fanInletFlange.gasTotalPressure + fanShaftPower.getSEF();

		double const staticPressureRise =
			planeData.fanOrEvaseOutletFlange.staticPressure - planeData.fanInletFlange.staticPressure +
			fanShaftPower.getSEF();

		double const kpFactorRatio = calculateCompressibilityFactor(x, z, isentropicExponent);

		// corrected variables
		double const qc = planeData.fanInletFlange.gasVolumeFlowRate * (fanRatedInfo.fanSpeedCorrected / fanRatedInfo.fanSpeed) * kpFactorRatio;

		double const ptc = fanTotalPressure * kpFactorRatio * std::pow(fanRatedInfo.fanSpeedCorrected / fanRatedInfo.fanSpeed, 2) * (fanRatedInfo.densityCorrected / planeData.fanInletFlange.gasDensity);

		double const psc = fanStaticPressure * kpFactorRatio * std::pow(fanRatedInfo.fanSpeedCorrected / fanRatedInfo.fanSpeed, 2) * (fanRatedInfo.densityCorrected / planeData.fanInletFlange.gasDensity);

		double const sprc =
			staticPressureRise * kpFactorRatio * std::pow(fanRatedInfo.fanSpeedCorrected / fanRatedInfo.fanSpeed, 2) * (fanRatedInfo.densityCorrected / planeData.fanInletFlange.gasDensity);

		double const hc = fanShaftPower.getFanPowerInput() * kpFactorRatio * std::pow(fanRatedInfo.fanSpeedCorrected / fanRatedInfo.fanSpeed, 3) * (fanRatedInfo.densityCorrected / planeData.fanInletFlange.gasDensity);

		double const kpc = kp / kpFactorRatio;

		double const efficiency = planeData.fanInletFlange.gasVolumeFlowRate * kp / (6362 * fanShaftPower.getFanPowerInput());

		return {
			fanTotalPressure * efficiency * 100, fanStaticPressure * efficiency * 100, staticPressureRise * efficiency * 100, {kpFactorRatio, fanShaftPower.getFanPowerInput(), planeData.fanInletFlange.gasVolumeFlowRate, fanTotalPressure, fanStaticPressure, staticPressureRise}, {kpc, hc, qc, ptc, psc, sprc}};
	}

private:
	double calculateCompressibilityFactor(const double x, const double z, const double isentropic)
	{
		double assumedKpOverKpc = 1.0;
		auto const &p1 = planeData.fanInletFlange;
		for (auto i = 0; i < 50; i++)
		{
			double const pt1c = p1.gasTotalPressure * std::pow(fanRatedInfo.fanSpeedCorrected / fanRatedInfo.fanSpeed, 2) * (fanRatedInfo.densityCorrected / p1.gasDensity) * assumedKpOverKpc;

			// TODO how to get isentropic exponent for gas at converted conditions? section 9.4.1 step 2
			double const zOverZc = ((pt1c + 13.63 * fanRatedInfo.pressureBarometricCorrected) / (p1.gasTotalPressure + 13.63 * p1.barometricPressure)) * (p1.gasDensity / fanRatedInfo.densityCorrected) * std::pow(fanRatedInfo.fanSpeed / fanRatedInfo.fanSpeedCorrected, 2) * ((isentropic - 1) / isentropic) * (isentropic / (isentropic - 1));

			double const zc = z / zOverZc;

			double const ln1xc = std::log(1 + x) * ((std::log(1 + zc) / std::log(1 + z))) * ((isentropic - 1) / isentropic) * (isentropic / (isentropic - 1));

			double const xc = std::exp(ln1xc) - 1;

			double const kpOverKpc = (z / zc) * (xc / x) * (isentropic / (isentropic - 1)) * ((isentropic - 1) / isentropic);
			if (fabs(kpOverKpc - assumedKpOverKpc) < 0.0000001)
			{
				return kpOverKpc;
			}
			assumedKpOverKpc = kpOverKpc;
		}
		throw std::runtime_error("compressibility factor ratio iteration did not converge");
	}

	FanRatedInfo const fanRatedInfo;
	PlaneData planeData;
	BaseGasDensity const baseGasDensity;
	FanShaftPower const fanShaftPower;
};

#endif //AMO_TOOLS_SUITE_FAN_H
