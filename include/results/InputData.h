/**
 * @brief Contains InputData for both PSAT and Fan results
 *
 * @author Preston Shires (pshires)
 * @bug No known bugs.
 *
 */

#ifndef AMO_TOOLS_SUITE_INPUT_DATA_H
#define AMO_TOOLS_SUITE_INPUT_DATA_H

#include <cmath>
#include "calculator/util/Conversion.h"

struct Motor {
	enum class EfficiencyClass {
		STANDARD,
		ENERGY_EFFICIENT,
		PREMIUM,
		SPECIFIED,
	};

	enum class LineFrequency {
		FREQ60,
		FREQ50
	};

	enum class Drive {
		DIRECT_DRIVE,
		V_BELT_DRIVE,
		N_V_BELT_DRIVE,
		S_BELT_DRIVE,
		SPECIFIED
	};

	enum class LoadEstimationMethod {
		POWER,
		CURRENT
	};

	/**
	 * Constructor
	 * @param lineFrequency LineFrequency, main supply frequency at either 50Hz or 60Hz
	 * @param motorRatedPower double, rated power for the motor in hp
	 * @param motorRpm double, motor RPM
	 * @param efficiencyClass EfficiencyClass, classification of motor efficiency
	 * @param specifiedEfficiency double, specified % Efficiency of motor, unused unless efficiency class is SPECIFIED
	 * @param motorRatedVoltage double, motor nameplate design voltage in volts
	 * @param fullLoadAmps double, current at full load in amps
	 * @param sizeMargin double, size margin as defined in %
	 */
	Motor(const LineFrequency lineFrequency, const double motorRatedPower, const double motorRpm,
	      const EfficiencyClass efficiencyClass, const double specifiedEfficiency, const double motorRatedVoltage,
	      const double fullLoadAmps, const double sizeMargin = 1)
			: lineFrequency(lineFrequency), motorRatedPower(motorRatedPower), motorRpm(motorRpm),
			  efficiencyClass(efficiencyClass), specifiedEfficiency(specifiedEfficiency), 
			  motorRatedVoltage(motorRatedVoltage), fullLoadAmps(fullLoadAmps), sizeMargin(sizeMargin)
	{};

	const LineFrequency lineFrequency;
	const double motorRatedPower, motorRpm;
	const Motor::EfficiencyClass efficiencyClass;
	const double specifiedEfficiency, motorRatedVoltage, fullLoadAmps, sizeMargin;
};

namespace Fan {
	struct Input {
		Input(const double fanSpeed, const double airDensity, const Motor::Drive drive, double specifiedEfficiency)
				: fanSpeed(fanSpeed), airDensity(airDensity), drive(drive), specifiedEfficiency(specifiedEfficiency)
		{}

		double fanSpeed, airDensity;
		Motor::Drive drive;
		double specifiedEfficiency;
	};

	struct FieldDataModified {
		/**
		 * To be used for Modified and Optimal Fan results
		 * @param measuredVoltage
		 * @param measuredAmps
		 * @param flowRate
		 * @param inletPressure
		 * @param outletPressure
		 * @param compressibilityFactor
		 * @param velocityPressure
		 */
		FieldDataModified(const double measuredVoltage, const double measuredAmps, const double flowRate, const double inletPressure,
		                            const double outletPressure, const double compressibilityFactor, const double velocityPressure)
				: measuredVoltage(measuredVoltage), measuredAmps(measuredAmps), flowRate(flowRate),
				  inletPressure(inletPressure), outletPressure(outletPressure), compressibilityFactor(compressibilityFactor), velocityPressure(velocityPressure)
		{}

		double measuredVoltage, measuredAmps, flowRate, inletPressure, outletPressure, compressibilityFactor, velocityPressure;
	};

	struct FieldDataBaseline: public FieldDataModified {
		/**
		 * To be used for Baseline Fan results
		 * @param measuredPower
		 * @param measuredVoltage
		 * @param measuredAmps
		 * @param flowRate
		 * @param inletPressure
		 * @param outletPressure
		 * @param compressibilityFactor
		 * @param loadEstimationMethod
		 * @param velocityPressure
		 */
		FieldDataBaseline(const double measuredPower, const double measuredVoltage, const double measuredAmps,
		                  const double flowRate, const double inletPressure, const double outletPressure,
		                  const double compressibilityFactor, Motor::LoadEstimationMethod loadEstimationMethod, const double velocityPressure)
				: FieldDataModified(measuredVoltage, measuredAmps, flowRate, inletPressure, outletPressure, compressibilityFactor, velocityPressure),
				  measuredPower(measuredPower), loadEstimationMethod(loadEstimationMethod)
		{}

		double measuredPower;
		Motor::LoadEstimationMethod loadEstimationMethod;
	};
}

namespace Pump {
	enum class SpecificSpeed {
		FIXED_SPEED,
		NOT_FIXED_SPEED
	};

	enum class Style {
		END_SUCTION_SLURRY,
		END_SUCTION_SEWAGE,
		END_SUCTION_STOCK,
		END_SUCTION_SUBMERSIBLE_SEWAGE,
		API_DOUBLE_SUCTION,
		MULTISTAGE_BOILER_FEED,
		END_SUCTION_ANSI_API,
		AXIAL_FLOW,
		DOUBLE_SUCTION,
		VERTICAL_TURBINE,
		LARGE_END_SUCTION,
		SPECIFIED_OPTIMAL_EFFICIENCY
	};

	struct FieldData {
		/**
		 * Constructor
		 * @param flowRate double, rate of flow. Units are gpm
		 * @param head double, pump head measured in feet
		 * @param loadEstimationMethod LoadEstimationMethod, classification of load estimation method
		 * @param motorPower double, power output of the pump's motor in hp.
		 * @param motorAmps double, current measured from the pump's motor in amps
		 * @param voltage double, the measured bus voltage in volts
		 */
		FieldData(const double flowRate, const double head, const Motor::LoadEstimationMethod loadEstimationMethod, const double motorPower,
		          const double motorAmps, const double voltage)
				: flowRate(flowRate), head(head), loadEstimationMethod(loadEstimationMethod),  motorPower(motorPower),
				  motorAmps(motorAmps), voltage(voltage)
		{}

		const double flowRate, head;
		const Motor::LoadEstimationMethod loadEstimationMethod;
		const double motorPower, motorAmps, voltage;
	};

	struct Input {
		/**
		 * Constructor
		 * @param style Style, classification of style of pump being used.
		 * @param pumpEfficiency double, pump % efficiency at the specified operating conditions
		 * @param rpm double, pump RPM to define its operating speed
		 * @param drive Drive, type of drive the pump uses from either direct or belt drive.
		 * @param kviscosity double, kinematic viscosity of the fluid being pumped in centistokes.
		 * @param specificGravity double, specific gravity- unitless
		 * @param stageCount int, the number of pump stages
		 * @param speed Speed, type of pump speed from either fixed or not fixed.
		 */
		Input(const Style style, double pumpEfficiency, const double rpm, const Motor::Drive drive,
		     const double kviscosity,
		     const double specificGravity, const int stageCount, const SpecificSpeed speed, double specifiedEfficiency)
				: style(style), pumpEfficiency(pumpEfficiency), rpm(rpm), drive(drive),  
				  kviscosity(kviscosity),
				  specificGravity(specificGravity), stageCount(stageCount), speed(speed), specifiedEfficiency(specifiedEfficiency) {
					  /**
					   * Convert percent values to fractions for proper calculation  
					   */
					//   this->specifiedEfficiency = Conversion(specifiedEfficiency).percentToFraction();
					//   this->pumpEfficiency = Conversion(pumpEfficiency).percentToFraction();
					//   this->specifiedEfficiency = specifiedEfficiency / 100.0;
					//   this->pumpEfficiency = pumpEfficiency / 100.0;
				  };

		const Style style;
		double pumpEfficiency, rpm;
		const Motor::Drive drive;
		const double kviscosity, specificGravity;
		const int stageCount;
		const SpecificSpeed speed;
		double specifiedEfficiency;
	
	};
}

#endif //AMO_TOOLS_SUITE_INPUT_DATA_H
