/**
 * @brief Contains the definition of functions of EstimateFLA class.
 *      calculate(): Calculates the estimated FLA and 25% interval values of Current.
 *
 * @author Subhankar Mishra (mishras)
 * @author Gina Accawi (accawigk)
 * @author Preston Shires (pshires) (Refactoring mostly)
 * @bug No known bugs.
 *
 */

#include <cmath>
#include <array>
#include "calculator/motor/EstimateFLA.h"
#include "calculator/motor/MotorEfficiency.h"
#include "calculator/motor/Poles.h"

/**
 * Estimating FLA methodology
 * - PSAT has four different methods of estimating FLA, with the four methods corresponding to the four efficiency choices.
 *  - For Standard Efficiency selection, it selects the FLA for the motor size and speed class that has been selected.  For example, the SE FLA for a 200 hp 4-pole motor is 228.6 amps.
 *  - For Energy Efficient selection, it selects the FLA for the motor size and speed class that has been selected.  For example, the EE FLA for a 200 hp 4-pole motor is 225.8 amps.
 *  - For Average Efficiency selection, it averages the FLA for Energy Efficient and Standard Efficiency choices.  The average of the two values listed above is 227.2 amps, which is what PSAT returns.
 *  - For a case where the efficiency has been specified, there is a multiple step process:
 *      1. The absolute value of the differences between the specified efficiency and the efficiency values for EE and SE motors are calculated
 *      2. Pick the smaller of the two absolute values, and use the nominal efficiency of that selection going forward
 *      3. Divide the selected (EE or SE) efficiency by the specified efficiency and then multiply that by the FLA for the corresponding selection.  See the example below.
 *
 * 	EE	SE	Specified		Choose:
 * Full load efficiency	95.63	93.92	95.0		EE
 * Full load amps (FLA)	225.8	228.6	227.3
 *
 * In this case, the specified efficiency is closer to the EE value, so the EE FLA and EE efficiency values are used.
 *
 * Equation for the calculation is:
 * Estimated FLA (specified efficiency) =EE FLA * (EE full load efficiency / Specified efficiency)
 * or, if the specified efficiency is closer to SE,
 * Estimated FLA (specified efficiency) =SE FLA * (SE full load efficiency / Specified efficiency)
 *
 * Nameplate voltage adjustment
 * If the motor nameplate is other than 460 V, the Estimate FLA process includes one more adjustment
 * Use an example of a motor known to have a 575 volt rating, but the FLA is not readable and the user wants to Estimate the FLA
 * The process is the same as above, except that the estimated FLA is adjusted in inverse proportion to the rated voltage.
 *
 * Estimated FLA (575) = Estimated FLA (460) * (460/575)
 * Estimated FLA (575)=	181.8
 *
 * @return std::array<double, 6>, 25% interval values of current.
 */

const std::array<std::array<double, 6>, 5> EstimateFLA::calculatePartialLoadCoefficients(const int pole) const
{
        if (pole == 0)
        {
                return {
                    {{{0.2385152, 0.31999999, 0.51999998, 0.75400001, 1, 1.253}},
                     {{0.04319932, 0.03559835, 0.02311537, 0.00837315, 0, 0}},
                     {{0.01191111, 0.00243387, 0.00018377, 0.00013998, 0, 0}},
                     {{0.176818, 0.1282783, 0.08376596, 0.03318606, 0, 0}},
                     {{0.1847319, 0.1831789, 0.16219489, 0.1855139, 0, 0}}}};
        }
        else if (pole == 1)
        {
                return {
                    {{{0.26803651, 0.3535319, 0.54000002, 0.76428652, 1, 1.251}},
                     {{0.08, 0.06387088, 0.02691548, 0.0081415, 0, 0}},
                     {{0.01172604, 0.01068725, 0.00297117, 0.00686711, 0, 0}},
                     {{0.2029126, 0.1705882, 0.09793611, 0.0471422, 0, 0}},
                     {{0.15413959, 0.1315202, 0.09374535, 0.1191377, 0, 0}}}};
        }
        else if (pole == 2)
        {
                return {
                    {{{0.28999999, 0.39519981, 0.55500001, 0.76973772, 1, 1.254}},
                     {{0.07532283, 0.06387088, 0.03, 0.02362101, 0, 0}},
                     {{0.00291026, 0.01068725, 0.00380051, 0.08841167, 0, 0}},
                     {{0.2405144, 0.1705882, 0.13686571, 0.02321446, 0, 0}},
                     {{0.09842682, 0.1315202, 0.15484551, 0.08854395, 0, 0}}}};
        }
        else if (pole == 3)
        {
                return {
                    {{{0.33000001, 0.43000001, 0.56062472, 0.78429687, 1, 1.237}},
                     {{0.06, 0.1060414, 0.04872012, 0.00763685, 0, 0}},
                     {{0.00125301, 0.031788, 0.00041673, 0.0023581, 0, 0}},
                     {{0.1939563, 0.115, 0.0841061, 0.00756603, 0, 0}},
                     {{0.05575958, 0.15824869, 0.05538078, 0.00250876, 0, 0}}}};
        }
        else if (pole == 4)
        {
                return {
                    {{{0.37, 0.44, 0.57857579, 0.78581947, 1, 1.233}},
                     {{0.04507853, 0.04205693, 0.06270286, 0.01749944, 0, 0}},
                     {{0.00098519, 0.00121622, 0.0005819, 0.00068763, 0, 0}},
                     {{0.20683549, 0.1764105, 0.08895831, 0.01809594, 0, 0}},
                     {{0.0551472, 0.05516704, 0.05197735, 0.03981217, 0, 0}}}};
        }
        // pole == 5 here
        return {
            {{{0.41999999, 0.5, 0.61000001, 0.80000001, 1, 1.215}},
             {{0.04507853, 0.04205693, 0.06270286, 0.01749944, 0, 0}},
             {{0.00098519, 0.00121622, 0.0005819, 0.00068763, 0, 0}},
             {{0.20683549, 0.1764105, 0.08895831, 0.01809594, 0, 0}},
             {{0.0551472, 0.05516704, 0.05197735, 0.03981217, 0, 0}}}};
}

std::array<double, 6> EstimateFLA::calculate()
{
        /**
     * Calculate the number of poles based on the RPM and use it as an index
     */
        int const pole = (Poles(motorRPM, lineFrequency).calculate() / 2) - 1;

        /**
     * Calculate basic FLA value
     */
        double const basicFLAValue = motorRatedPower * (flaBasic[0][pole] + (flaBasic[1][pole] * std::exp(-flaBasic[2][pole] * motorRatedPower)) + (flaBasic[3][pole] * std::exp(-flaBasic[4][pole] * motorRatedPower)));
        /**
     * Calculate EE multiplier
     */
        double const eeMultiplier = eeFlaMultipliers[0][pole] + (eeFlaMultipliers[1][pole] * std::exp(-eeFlaMultipliers[2][pole] * motorRatedPower)) + (eeFlaMultipliers[3][pole] * std::exp(-eeFlaMultipliers[4][pole] * motorRatedPower));
        /**
     * Calculate SE multiplier
     */
        double const seMultiplier = seFlaMultipliers[0][pole] + (seFlaMultipliers[1][pole] * std::exp(-seFlaMultipliers[2][pole] * motorRatedPower)) + (seFlaMultipliers[3][pole] * std::exp(-seFlaMultipliers[4][pole] * motorRatedPower));

        /**
     * Calculate EE or SE values
     */
        double const eeFLAValue = eeMultiplier * basicFLAValue;
        double const seFLAValue = seMultiplier * basicFLAValue;

        /*
	 * Part load coefficients, 2004
     * Select partial load multipliers based on pole.
	 */
        const std::array<std::array<double, 6>, 5> partialLoadCoefficients = calculatePartialLoadCoefficients(pole);

        /**
	 * Calculating Multiplier for partial loads in 25% intervals via lambda function
	 */
        auto const plMult = [&partialLoadCoefficients, this](std::size_t i) {
                return partialLoadCoefficients[0][i] + (partialLoadCoefficients[1][i] * std::exp(-1 * partialLoadCoefficients[2][i] * motorRatedPower)) + (partialLoadCoefficients[3][i] * std::exp(-1 * partialLoadCoefficients[4][i] * motorRatedPower));
        };

        const std::array<double, 6> plMultiplier = {
            {plMult(0), plMult(1), plMult(2), plMult(3), plMult(4), plMult(5)}};

        // used to calculate FLA
        auto const adjustForVoltage = [this](double plVal, double effVal = 0) {
                auto const estimate = plVal * 460 / ratedVoltage;
                if (efficiencyClass == Motor::EfficiencyClass::SPECIFIED)
                {
                        return effVal * estimate * 100 / specifiedEfficiency;
                }
                return estimate;
        };

        if (efficiencyClass == Motor::EfficiencyClass::PREMIUM)
        {
                double const peMultiplier = peFlaMultipliers[0][pole] + (peFlaMultipliers[1][pole] * std::exp(-peFlaMultipliers[2][pole] * motorRatedPower)) + (peFlaMultipliers[3][pole] * std::exp(-peFlaMultipliers[4][pole] * motorRatedPower));

                double const peFLAValue = peMultiplier * basicFLAValue;
                estimatedFLA = adjustForVoltage(peFLAValue * plMultiplier[4]);
                return {
                    {peFLAValue * plMultiplier[0], peFLAValue * plMultiplier[1], peFLAValue * plMultiplier[2],
                     peFLAValue * plMultiplier[3], peFLAValue * plMultiplier[4], peFLAValue * plMultiplier[5]}};

                //        auto motorEfficiency = MotorEfficiency(lineFrequency, motorRPM, Motor::EfficiencyClass::ENERGY_EFFICIENT, motorRatedPower);
                //        const double effValEE = motorEfficiency.calculate(1);
                //        motorEfficiency.setEfficiencyClass(Motor::EfficiencyClass::PREMIUM);
                //        const double effValPE = motorEfficiency.calculate(1);
                //        const double val = eeFLAValue * effValPE / effValEE;
                //	    estimatedFLA = adjustForVoltage(val * plMultiplier[4]);
                //        return {
                //                {
                //                        val * plMultiplier[0], val * plMultiplier[1], val * plMultiplier[2],
                //                        val * plMultiplier[3], val * plMultiplier[4], val * plMultiplier[5]
                //                }
                //        };
        }

        if (efficiencyClass == Motor::EfficiencyClass::ENERGY_EFFICIENT)
        {
                estimatedFLA = adjustForVoltage(eeFLAValue * plMultiplier[4]);
                return {
                    {eeFLAValue * plMultiplier[0], eeFLAValue * plMultiplier[1], eeFLAValue * plMultiplier[2],
                     eeFLAValue * plMultiplier[3], eeFLAValue * plMultiplier[4], eeFLAValue * plMultiplier[5]}};
        }
        else if (efficiencyClass == Motor::EfficiencyClass::STANDARD)
        {
                estimatedFLA = adjustForVoltage(seFLAValue * plMultiplier[4]);
                return {
                    {seFLAValue * plMultiplier[0], seFLAValue * plMultiplier[1], seFLAValue * plMultiplier[2],
                     seFLAValue * plMultiplier[3], seFLAValue * plMultiplier[4], seFLAValue * plMultiplier[5]}};
        }
        else if (efficiencyClass == Motor::EfficiencyClass::SPECIFIED)
        {
                /**
         * For a case where the efficiency has been specified, there is a multiple step process:
         * 1. The absolute value of the differences between the specified efficiency and the efficiency values for EE and SE motors are calculated
         * 2. Pick the smaller of the two absolute values, and use the nominal efficiency of that selection going forward
         * 3. Divide the selected (EE or SE) efficiency by the specified efficiency and then multiply that by the FLA for the corresponding selection.
         */
                //        specifiedEfficiency = 95; // commented out due to this not making sense
                auto const seeffVal = MotorEfficiency(lineFrequency, motorRPM, Motor::EfficiencyClass::STANDARD,
                                                      motorRatedPower)
                                          .calculate(1, specifiedEfficiency);

                auto const eeeffVal = MotorEfficiency(lineFrequency, motorRPM, Motor::EfficiencyClass::ENERGY_EFFICIENT,
                                                      motorRatedPower)
                                          .calculate(1, specifiedEfficiency);

                if (std::fabs(seeffVal - specifiedEfficiency) < std::fabs(eeeffVal - specifiedEfficiency))
                {
                        // SE is the nominal efficiency
                        estimatedFLA = adjustForVoltage(seFLAValue * plMultiplier[4], seeffVal);
                        return {
                            {seFLAValue * plMultiplier[0], seFLAValue * plMultiplier[1], seFLAValue * plMultiplier[2],
                             seFLAValue * plMultiplier[3], seFLAValue * plMultiplier[4], seFLAValue * plMultiplier[5]}};
                }
                else
                {
                        /// EE is the nominal efficiency
                        estimatedFLA = adjustForVoltage(eeFLAValue * plMultiplier[4], eeeffVal);
                        return {
                            {eeFLAValue * plMultiplier[0], eeFLAValue * plMultiplier[1], eeFLAValue * plMultiplier[2],
                             eeFLAValue * plMultiplier[3], eeFLAValue * plMultiplier[4], eeFLAValue * plMultiplier[5]}};
                }
        }
        return {};
}
