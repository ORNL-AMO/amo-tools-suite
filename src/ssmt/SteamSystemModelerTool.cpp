#include <ssmt/SteamSystemModelerTool.h>
#include <array>
#include <cmath>
#include <iostream>

// where t is temperature and p is pressure
SteamSystemModelerTool::SteamPropertiesOutput SteamSystemModelerTool::region1(const double t, const double p) {

	static const std::array<double, 34> n = {
			{
					0.14632971213167, -0.84548187169114, -0.37563603672040e1, 0.33855169168385e1, -0.95791963387872,
					0.15772038513228, -0.16616417199501e-1, 0.81214629983568e-3, 0.28319080123804e-3, -0.60706301565874e-3,
					-0.18990068218419e-1, -0.32529748770505e-1, -0.21841717175414e-1, -0.52838357969930e-4,
					-0.47184321073267e-3, -0.30001780793026e-3, 0.47661393906987e-4, -0.44141845330846e-5,
					-0.72694996297594e-15, -0.31679644845054e-4, -0.28270797985312e-5, -0.85205128120103e-9,
					-0.22425281908000e-5, -0.65171222895601e-6, -0.14341729937924e-12, -0.40516996860117e-6,
					-0.12734301741641e-8, -0.17424871230634e-9, -0.68762131295531e-18, 0.14478307828521e-19,
					0.26335781662795e-22, -0.11947622640071e-22, 0.18228094581404e-23, -0.93537087292458e-25
			}
	};

	static const std::array<int, 34> j = {
			{
					-2, -1, 0, 1, 2, 3, 4, 5, -9, -7, -1, 0, 1, 3, -3, 0, 1, 3, 17, -4, 0, 6, -5, -2,
					10, -8, -11, -6, -29, -31, -38, -39, -40, -41
			}
	};

	static const std::array<int, 34> i = {
			{
					0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
					3, 3, 3, 4, 4, 4, 5, 8, 8, 21, 23, 29, 30, 31, 32
			}
	};

	auto const reducedPressure = p / 16.53;
	auto const inversedReducedTemp = 1386.0 / t;

	double gibbs = 0;
    double gibbsPi = 0;
    double gibbsT = 0;

	for (std::size_t k = 0; k < n.size(); k++ ) {
		gibbs += n[k] * pow((7.1 - reducedPressure), i[k]) * pow((inversedReducedTemp - 1.222), j[k]);
		gibbsPi += -n[k] * i[k] * pow((7.1 - reducedPressure), i[k] - 1) * pow((inversedReducedTemp - 1.222), j[k]);
		gibbsT += n[k] * pow((7.1 - reducedPressure), i[k]) * j[k] * pow((inversedReducedTemp - 1.222), j[k] - 1);
	}

	auto const r = 0.461526;
	return {
			t, p, 0,
			reducedPressure * gibbsPi * t * r / p / 1000.0, 1 / (reducedPressure * gibbsPi * t * r / p / 1000.0),
			inversedReducedTemp * gibbsT * t * r, (inversedReducedTemp * gibbsT - gibbs) * r
	};
}

// where t is temperature in K and p is pressure in MPa
SteamSystemModelerTool::SteamPropertiesOutput SteamSystemModelerTool::region2(const double t, const double p) {

	static const std::array<double, 9> n0 = {
			{
					-0.96927686500217E+01, 0.10086655968018E+02, -0.56087911283020E-02, 0.71452738081455E-01,
					-0.40710498223928E+00,  0.14240819171444E+01, -0.43839511319450E+01, -0.28408632460772E+00, 0.21268463753307E-01
			}
	};
	static const std::array<double, 9> j0 = {{
			0, 1, -5, -4, -3, -2, -1, 2, 3
	}};
	static const std::array<double, 43> n1 = {
			{
					-0.17731742473213E-02, -0.17834862292358E-01, -0.45996013696365E-01, -0.57581259083432E-01,
					-0.50325278727930E-01, -0.33032641670203E-04, -0.18948987516315E-03, -0.39392777243355E-02,
					-0.43797295650573E-01, -0.26674547914087E-04, 0.20481737692309E-07,  0.43870667284435E-06,
					-0.32277677238570E-04, -0.15033924542148E-02, -0.40668253562649E-01, -0.78847309559367E-09,
					0.12790717852285E-07,  0.48225372718507E-06, 0.22922076337661E-05, -0.16714766451061E-10,
					-0.21171472321355E-02, -0.23895741934104E+02, -0.59059564324270E-17, -0.12621808899101E-05,
					-0.38946842435739E-01, 0.11256211360459E-10, -0.82311340897998E+01, 0.19809712802088E-07,
					0.10406965210174E-18, -0.10234747095929E-12, -0.10018179379511E-08, -0.80882908646985E-10,
					0.10693031879409E+00, -0.33662250574171E+00, 0.89185845355421E-24, 0.30629316876232E-12,
					-0.42002467698208E-05, -0.59056029685639E-25, 0.37826947613457E-05, -0.12768608934681E-14,
					0.73087610595061E-28, 0.55414715350778E-16, -0.94369707241210E-06
			}
	};
	static const std::array<double, 43> j1 = {
			{
					0, 1, 2, 3, 6, 1, 2, 4, 7, 36, 0, 1, 3, 6, 35, 1, 2, 3, 7, 3, 16, 35, 0, 11,
					25, 8, 36, 13, 4, 10, 14, 29, 50, 57, 20, 35, 48, 21, 53, 39, 26, 40, 58
			}
	};
	static const std::array<double, 43> i1 = {
			{
					1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 6, 6, 6, 7, 7, 7,
					8, 8, 9, 10, 10, 10, 16, 16, 18, 20, 20, 20, 21, 22, 23, 24, 24, 24
			}
	};

	auto const reducedPressure = p;
	auto const inverseReducedTemp = 540 / t;

	double gibbs0 = log(reducedPressure);
	double gibbsPi0 = 1 / reducedPressure;
	double gibbsT0 = 0, gibbsTT0 = 0;

	for (std::size_t k = 0; k < n0.size(); k++) {
		gibbs0 += n0[k] * std::pow(inverseReducedTemp, j0[k]);
		gibbsT0 += n0[k] * j0[k] * std::pow(inverseReducedTemp, j0[k] - 1);
		gibbsTT0 += n0[k] * j0[k] * (j0[k] - 1) * std::pow(inverseReducedTemp, j0[k] - 2);
	}

	double gibbs1 = 0;
    double gibbsPi1 = 0;
    double gibbsT1 = 0;

	for (std::size_t k = 0; k < n1.size(); k++) {
		gibbs1 += n1[k] * std::pow(reducedPressure, i1[k]) * std::pow((inverseReducedTemp - 0.5), j1[k]);
		gibbsPi1 += n1[k] * i1[k] * std::pow(reducedPressure, i1[k] - 1) * std::pow((inverseReducedTemp - 0.5), j1[k]);
		gibbsT1 += n1[k] * std::pow(reducedPressure, i1[k]) * j1[k] * std::pow((inverseReducedTemp - 0.5), j1[k] - 1);
	}
	auto const r = 0.461526;
	return {
			t, p, 1,
			reducedPressure * (gibbsPi0 + gibbsPi1) * t * r / p / 1000.0,
			1 / (reducedPressure * (gibbsPi0 + gibbsPi1) * t * r / p / 1000.0),
			inverseReducedTemp * (gibbsT0 + gibbsT1) * t * r,
			((inverseReducedTemp * (gibbsT0 + gibbsT1)) - (gibbs0 + gibbs1)) * r,
			inverseReducedTemp * (gibbsT0 + gibbsT1) - reducedPressure * (gibbsPi0 + gibbsPi1) * t * r
	};
}

SteamSystemModelerTool::SteamPropertiesOutput SteamSystemModelerTool::region3(const double t, const double p) {
	auto boundary13Properties = region1(TEMPERATURE_Tp, p);
	auto densityA = boundary13Properties.density;
	auto region3propNew = region3Density( densityA, t);
	auto testPressureA = region3propNew.pressure;


	auto boundary23Properties = region2(boundaryByPressureRegion3to2(p), p);
	auto densityB = boundary23Properties.density;
	region3propNew = region3Density(densityB, t);
	auto testPressureB = region3propNew.pressure;

	double pressureNew = 0;
	for (std::size_t i = 0; i < 4; i++) {
		auto const densityNew = (densityA + densityB) / 2.0;
		region3propNew = region3Density(densityNew, t);
		pressureNew = region3propNew.pressure;
		if ( p > pressureNew ) {
			densityB = densityNew;
			testPressureB = pressureNew;
		} else {
			densityA = densityNew;
			testPressureA = pressureNew;
		}
	}


	// Uses Linear Interpolation
	std::size_t counter = 0;
	while ((std::abs(pressureNew - p) > 1e-10) && (counter++ < 50) && (testPressureA != testPressureB)) {
		auto const densityNew = p * (densityA - densityB) / (testPressureA - testPressureB) + densityA - testPressureA * (densityA - densityB) / (testPressureA - testPressureB);
		region3propNew = region3Density(densityNew, t);
		pressureNew = region3propNew.pressure;
		densityB = densityA;
		densityA = densityNew;
		testPressureB = testPressureA;
		testPressureA = pressureNew;
	}

	return region3propNew;

}

SteamSystemModelerTool::SteamPropertiesOutput SteamSystemModelerTool::region3Density(const double d, const double t) {

	static const std::array<double, 40> n = {
			{
					0.10658070028513E+01, -0.15732845290239E+02, 0.20944396974307E+02, -0.76867707878716E+01,
					0.26185947787954E+01, -0.28080781148620E+01, 0.12053369696517E+01, -0.84566812812502E-02,
					-0.12654315477714E+01, -0.11524407806681E+01, 0.88521043984318E+00, -0.64207765181607E+00,
					0.38493460186671E+00, -0.85214708824206E+00, 0.48972281541877E+01, -0.30502617256965E+01,
					0.39420536879154E-01, 0.12558408424308E+00, -0.27999329698710E+00, 0.13899799569460E+01,
					-0.20189915023570E+01, -0.82147637173963E-02, -0.47596035734923E+00, 0.43984074473500E-01,
					-0.44476435428739E+00, 0.90572070719733E+00, 0.70522450087967E+00, 0.10770512626332E+00,
					-0.32913623258954E+00, -0.50871062041158E+00, -0.22175400873096E-01, 0.94260751665092E-01,
					0.16436278447961E+00, -0.13503372241348E-01, -0.14834345352472E-01, 0.57922953628084E-03,
					0.32308904703711E-02, 0.80964802996215E-04, -0.16557679795037E-03, -0.44923899061815E-04
			}
	};
	static const std::array<int, 40> j = {
			{
					0, 0, 1, 2, 7, 10, 12, 23, 2, 6, 15, 17, 0, 2, 6, 7, 22, 26, 0, 2,
					4, 16, 26, 0, 2, 4, 26, 1, 3, 26, 0, 2, 26, 2, 26, 2, 26, 0, 1, 26
			}
	};
	static const std::array<int, 40> i = {
			{
					0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3,
					3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 8, 9, 9, 10, 10, 11
			}
	};

	auto const reducedDensity = d / 322.0;
	auto const inverseReducedTemp = 647.096 / t;
	auto helmholtz = n[0] * std::log(reducedDensity);
	auto helmholtzS = n[0] / reducedDensity;
	double helmholtzT = 0;

	for (std::size_t k = 1; k < n.size(); k++) {
		helmholtz += n[k] * std::pow(reducedDensity, i[k]) * std::pow(inverseReducedTemp, j[k]);
		helmholtzS += n[k] * i[k] * std::pow(reducedDensity, i[k] - 1.0) * std::pow(inverseReducedTemp, j[k]);
		helmholtzT += n[k] * std::pow(reducedDensity, i[k]) * j[k] * std::pow(inverseReducedTemp, j[k] - 1);
	}

	auto const r = 0.461526;

	// TODO determine what quality should be in this region - Quality question
	return {
			t, reducedDensity * helmholtzS * d * t * r / 1000.0, 1, 1 / d, d,
			(inverseReducedTemp * helmholtzT + reducedDensity * helmholtzS) * t * r,
			(inverseReducedTemp * helmholtzT - helmholtz) * r,
			inverseReducedTemp * helmholtzT * t * r
	};
}

double SteamSystemModelerTool::backwardRegion3Exact(const double pressure, const double X, SteamSystemModelerTool::Key key) {
    double temperature = SteamSystemModelerTool::TEMPERATURE_Tp;
    Point pointA = SteamSystemModelerTool::generatePoint(1, key, pressure, temperature);
    Point pointB = SteamSystemModelerTool::generatePoint(2, key, pressure, SteamSystemModelerTool::boundaryByPressureRegion3to2(pressure));
    double temperatureB = SteamSystemModelerTool::linearTestPoint(X, pointA, pointB);
    int counter = 0;

    while((std::abs(temperature - temperatureB) > 1e-6) && (counter++ < 15)) {
        pointA = pointB;
        pointB = SteamSystemModelerTool::generatePoint(3, key, pressure, temperatureB);
        temperature = temperatureB;
        temperatureB = SteamSystemModelerTool::linearTestPoint(X, pointA, pointB);
    }

    return temperatureB;
}


double SteamSystemModelerTool::region4(const double t) {
	auto const v = t - 0.23855557567849E+00 / (t - 0.65017534844798E+03);
	auto const a = v * v + 0.11670521452767E+04 * v - 0.72421316703206E+06;
	auto const b = -0.17073846940092E+02 * v * v + 0.12020824702470E+05 * v - 0.32325550322333E+07;
	auto const c = 0.14915108613530E+02 * v * v - 0.48232657361591E+04 * v + 0.40511340542057E+06;

	return std::pow(2 * c / (-b + sqrt(std::pow(b, 2) - 4 * a * c)), 4);
}

/**
 * @param p Pressure in MPa.
 * @param t Temperature in K.
 */
int SteamSystemModelerTool::regionSelect(const double p, const double t) {
    const std::string methodName = std::string("SteamSystemModelerTool::") + std::string(__func__) + ": ";

    // std::cout << methodName << "pressure in MPa=" << p << ", temp in K=" << t << std::endl;

	const double boundaryPressure = (t >= TEMPERATURE_Tp) ? boundaryByTemperatureRegion3to2(t) : region4(t);
	// std::cout << methodName << "boundaryPressure=" << boundaryPressure << std::endl;

	if (t >= TEMPERATURE_MIN && t <= TEMPERATURE_Tp) {
		if (p <= PRESSURE_MAX && p >= boundaryPressure) return 1;
		if (p > 0 && p <= boundaryPressure) return 2;
	}

	if (t >= TEMPERATURE_Tp && t <= TEMPERATURE_REGION3_MAX) {
		if (p > 0 && p <= boundaryPressure) return 2;
		if (p <= PRESSURE_MAX && p > boundaryPressure) return 3;
	}

	if (t > TEMPERATURE_REGION3_MAX && t <= TEMPERATURE_MAX)  return 2;

	auto message =
	        "regionSelect failed for combination of values: temp in K=" + std::to_string(t) +
	        ", pressure in MPa=" + std::to_string(p) +
	        ", boundaryPressure=" + std::to_string(boundaryPressure) +
	        "; valid temp range=" + std::to_string(TEMPERATURE_MIN) + " - " + std::to_string(TEMPERATURE_MAX) +
	        "; max pressure=" + std::to_string(PRESSURE_MAX);
	// std::cout << methodName << message << std::endl;
	throw std::runtime_error(message);
}

double SteamSystemModelerTool::backwardPressureEnthalpyRegion1(const double pressure, const double enthalpy) {
	static const std::array<int, 20> I = {{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 5, 6}};

	static const std::array<int, 20> J = {{0, 1, 2, 6, 22, 32, 0, 1, 2, 3, 4, 10, 32, 10, 32, 10, 32, 32, 32, 32}};

	static const std::array<double, 20> n = {
			{
					-0.23872489924521E+3, 0.40421188637945E+3, 0.11349746881718E+3, -0.58457616048039E+1, -0.15285482413140E-3,
					-0.10866707695377E-5, -0.13391744872602E+2, 0.43211039183559E+2, -0.54010067170506E+2, 0.30535892203916E+2,
					-0.65964749423638E+1, 0.93965400878363E-2, 0.11573647505340E-6, -0.25858641282073E-4, -0.40644363084799E-8,
					0.66456186191635E-7, 0.80670734103027E-10, -0.93477771213947E-12, 0.58265442020601E-14, -0.15020185953503E-16
			}
	};

	auto const nu = enthalpy / 2500.0;
	double temp = 0.0;

	for (std::size_t i = 0; i < n.size(); i++) {
		temp += n[i] * std::pow(pressure, I[i]) * std::pow((nu + 1), J[i]);
	}
	return temp;
}

double SteamSystemModelerTool::backwardPressureEnthalpyRegion2A(const double pressure, const double enthalpy) {
	static const std::array<int, 34> array0 = {
		    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7}
    };
	static const std::array<int, 34> array1 = {
		    {0, 1, 2, 3, 7, 20, 0, 1, 2, 3, 7, 9, 11, 18, 44, 0, 2, 7, 36, 38, 40, 42, 44, 24, 44, 12, 32, 44, 32, 36, 42, 34, 44, 28}
    };
	static const std::array<double, 34> array2 = {
		    {
				    0.10898952318288E+4, 0.84951654495535E+3, -0.10781748091826E+3, 0.33153654801263E+2, -0.74232016790248E+1,
				    0.11765048724356E+2, 0.18445749355790E+1, -0.41792700549624E+1, 0.62478196935812E+1, -0.17344563108114E+2,
				    -0.20058176862096E+3, 0.27196065473796E+3, -0.45511318285818E+3, 0.30919688604755E+4, 0.25226640357872E+6,
				    -0.61707422868339E-2, -0.31078046629583, 0.11670873077107E+2, 0.12812798404046E+9, -0.98554909623276E+9,
				    0.28224546973002E+10, -0.35948971410703E+10, 0.17227349913197E+10, -0.13551334240775E+5, 0.12848734664650E+8,
				    0.13865724283226E+1, 0.23598832556514E+6, -0.13105236545054E+8, 0.73999835474766E+4, -0.55196697030060E+6,
				    0.37154085996233E+7, 0.19127729239660E+5, -0.41535164835634E+6, -0.62459855192507E+2
		    }
    };

    double temperature = 0.0;
    auto const nu = enthalpy / 2000.0;

    for(std::size_t i = 0; i < array0.size(); i++) {
        temperature += array2[i] * std::pow(pressure, array0[i]) * std::pow((nu - 2.1), array1[i]);
    }
    return temperature;
}

double SteamSystemModelerTool::backwardPressureEnthalpyRegion2B(const double pressure, const double enthalpy) {
    static const std::array<int, 38> I = {
		    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 6, 7, 7, 9, 9}
    };

    static const std::array<int, 38> J = {
		    {0, 1, 2, 12, 18, 24, 28, 40, 0, 2, 6, 12, 18, 24, 28, 40, 2, 8, 18, 40, 1, 2, 12, 24, 2, 12, 18, 24, 28, 40, 18, 24, 40, 28, 2, 28, 1, 40}
    };

    static const std::array<double, 38> n = {
		    {
				    0.14895041079516E+4, 0.74307798314034E+3, -0.97708318797837E+2, 0.24742464705674E+1, -0.63281320016026,
				    0.11385952129658E+1, -0.47811863648625, 0.85208123431544E-2, 0.93747147377932, 0.33593118604916E+1,
				    0.33809355601454E+1, 0.16844539671904, 0.73875745236695, -0.47128737436186, 0.15020273139707,
				    -0.21764114219750E-2, -0.21810755324761E-1, -0.10829784403677, -0.46333324635812E-1, 0.71280351959551E-4,
				    0.11032831789999E-3, 0.18955248387902E-3, 0.30891541160537E-2, 0.13555504554949E-2, 0.28640237477456E-6,
				    -0.10779857357512E-4, -0.76462712454814E-4, 0.14052392818316E-4, -0.31083814331434E-4, -0.10302738212103E-5,
				    0.28217281635040E-6, 0.12704902271945E-5, 0.73803353468292E-7, -0.11030139238909E-7, -0.81456365207833E-13,
				    -0.25180545682962E-10, -0.17565233969407E-17, 0.86934156344163E-14
		    }
    };

    double temperature = 0.0;
    auto const nu = enthalpy / 2000.0;

    for (std::size_t i = 0; i < n.size(); i++) {
        temperature += n[i] * std::pow((pressure - 2), I[i]) * std::pow((nu - 2.6), J[i]);
    }
    return temperature;
}

double SteamSystemModelerTool::backwardPressureEnthalpyRegion2C(const double pressure, const double enthalpy) {
    static const std::array<int, 23> I = {{-7, -7, -6, -6, -5, -5, -2, -2, -1, -1, 0, 0, 1, 1, 2, 6, 6, 6, 6, 6, 6, 6, 6}};

    static const std::array<int, 23> J = {{0, 4, 0, 2, 0, 2, 0, 1, 0, 2, 0, 1, 4, 8, 4, 0, 1, 4, 10, 12, 16, 20, 22}};

    static const std::array<double, 23> n = {
		    {
				    -0.32368398555242E+13, 0.73263350902181E+13, 0.35825089945447E+12, -0.58340131851590E+12, -0.10783068217470E+11,
				    0.20825544563171E+11, 0.61074783564516E+6, 0.85977722535580E+6, -0.25745723604170E+5, 0.31081088422714E+5, 0.12082315865936E+4,
				    0.48219755109255E+3, 0.37966001272486E+1, -0.10842984880077E+2, -0.45364172676660E-1, 0.14559115658698E-12, 0.11261597407230E-11,
				    -0.17804982240686E-10, 0.12324579690832E-6, -0.11606921130984E-5, 0.27846367088554E-4, -0.59270038474176E-3, 0.12918582991878E-2
		    }
    };

    double temperature = 0.0;
    auto const nu = enthalpy / 2000.0;

    for (std::size_t i = 0; i < n.size(); i++) {
        temperature += n[i] * std::pow((pressure + 25), I[i]) * std::pow((nu - 1.8), J[i]);
    }
    return temperature;
}

double SteamSystemModelerTool::backwardPressureEntropyRegion2A(const double pressure, const double entropy) {
    static const std::array<double, 46> array0 = {
		    {
				    -1.5, -1.5, -1.5, -1.5, -1.5, -1.5,-1.25, -1.25, -1.25, -1, -1, -1, -1, -1, -1, -0.75, -0.75,
				    -0.5, -0.5, -0.5, -0.5, -0.25, -0.25, -0.25, -0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.5, 0.5, 0.5,
				    0.5, 0.5, 0.5, 0.75, 0.75, 0.75, 0.75, 1, 1, 1.25, 1.25, 1.5, 1.5
		    }
    };

    static const std::array<int, 46> array1 = {
		    {
				    -24, -23, -19, -13, -11, -10, -19, -15, -6, -26, -21, -17, -16, -9, -8, -15, -14, -26,
				    -13, -9, -7, -27, -25, -11, -6, 1, 4, 8, 11, 0, 1, 5, 6, 10, 14, 16, 0, 4, 9, 17, 7, 18,
				    3, 15, 5, 18
		    }
    };

    static const std::array<double, 46> array2 = {
		    {
				    -0.39235983861984E+6, 0.51526573827270E+6, 0.40482443161048E+5, -0.32193790923902E+3, 0.96961424218694E+2,
				    -0.22867846371773E+2, -0.44942914124357E+6, -0.50118336020166E+4, 0.35684463560015, 0.44235335848190E+5,
				    -0.13673388811708E+5, 0.42163260207864E+6, 0.22516925837475E+5, 0.47442144865646E+3, -0.14931130797647E+3,
				    -0.19781126320452E+6, -0.23554399470760E+5, -0.19070616302076E+5, 0.55375669883164E+5, 0.38293691437363E+4,
				    -0.60391860580567E+3, 0.19363102620331E+4, 0.42660643698610E+4, -0.59780638872718E+4, -0.70401463926862E+3,
				    0.33836784107553E+3, 0.20862786635187E+2, 0.33834172656196E-1, -0.43124428414893E-4, 0.16653791356412E+3,
				    -0.13986292055898E+3, -0.78849547999872, 0.72132411753872E-1, -0.59754839398283E-2, -0.12141358953904E-4,
				    0.23227096733871E-6, -0.10538463566194E+2, 0.20718925496502E+1, -0.72193155260427E-1, 0.20749887081120E-6,
				    -0.18340657911379E-1, 0.29036272348696E-6, 0.21037527893619, 0.25681239729999E-3, -0.12799002933781E-1,
				    -0.82198102652018E-5
		    }
    };

    double temperature = 0.0;

    for (std::size_t i = 0; i < array2.size(); i++) {
        temperature += array2[i] * std::pow(pressure, array0[i]) * std::pow((entropy/2 - 2), array1[i]);
    }
    return temperature;
}

double SteamSystemModelerTool::backwardPressureEntropyRegion2B(const double pressure, const double entropy) {
    static const std::array<int, 44> array0 = {
		    {
				    -6, -6, -5, -5, -4, -4, -4, -3, -3, -3, -3, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0,
				    0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5
		    }
    };

    static const std::array<int, 44> array1 = {
		    {
				    0, 11, 0, 11, 0, 1, 11, 0, 1, 11, 12, 0, 1, 6, 10, 0, 1, 5, 8, 9, 0, 1, 2, 4, 5, 6, 9, 0, 1, 2, 3,
				    7, 8, 0, 1, 5, 0, 1, 3, 0, 1, 0, 1, 2
		    }
    };

    static const std::array<double, 44> array2 = {
		    {
				    0.31687665083497E+6, 0.20864175881858E+2, -0.39859399803599E+6, -0.21816058518877E+2, 0.22369785194242E+6, -0.27841703445817E+4,
				    0.99207436071480E+1, -0.75197512299157E+5, 0.29708605951158E+4, -0.34406878548526E+1, 0.38815564249115, 0.17511295085750E+5,
				    -0.14237112854449E+4, 0.10943803364167E+1, 0.89971619308495, -0.33759740098958E+4, 0.47162885818355E+3, -0.19188241993679E+1,
				    0.41078580492196, -0.33465378172097, 0.13870034777505E+4, -0.40663326195838E+3, 0.41727347159610E+2, 0.21932549434532E+1,
				    -0.10320050009077E+1, 0.35882943516703, 0.52511453726066E-2, 0.12838916450705E+2, -0.28642437219381E+1, 0.56912683664855,
				    -0.99962954584931E-1, -0.32632037778459E-2, 0.23320922576723E-3, -0.15334809857450, 0.29072288239902E-1, 0.37534702741167E-3,
				    0.17296691702411E-2, -0.38556050844504E-3, -0.35017712292608E-4, -0.14566393631492E-4, 0.56420857267269E-5, 0.41286150074605E-7,
				    -0.20684671118824E-7, 0.16409393674725E-8
		    }
    };

    double temperature = 0.0;

    for (std::size_t i = 0; i < array2.size(); i++) {
        temperature += array2[i] * std::pow(pressure, array0[i]) * std::pow((10 - entropy/0.7853), array1[i]);
    }
    return temperature;
}

double SteamSystemModelerTool::backwardPressureEntropyRegion2C(const double pressure, const double entropy) {
    static const std::array<int, 30> array0 = {
		    {-2, -2, -1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 7, 7}
    };

    static const std::array<int, 30> array1 = {
		    {0, 1, 0, 0, 1, 2, 3, 0, 1, 3, 4, 0, 1, 2, 0, 1, 5, 0, 1, 4, 0, 1, 2, 0, 1, 0, 1, 3, 4, 5}
    };

    static const std::array<double, 30> array2 = {
		    {
				    0.90968501005365E+3, 0.24045667088420E+4, -0.59162326387130E+3, 0.54145404128074E+3, -0.27098308411192E+3, 0.97976525097926E+3,
				    -0.46966772959435E+3, 0.14399274604723E+2, -0.19104204230429E+2, 0.53299167111971E+1, -0.21252975375934E+2, -0.31147334413760,
				    0.60334840894623, -0.42764839702509E-1, 0.58185597255259E-2, -0.14597008284753E-1, 0.56631175631027E-2, -0.76155864584577E-4,
				    0.22440342919332E-3, -0.12561095013413E-4, 0.63323132660934E-6, -0.20541989675375E-5, 0.36405370390082E-7, -0.29759897789215E-8,
				    0.10136618529763E-7, 0.59925719692351E-11, -0.20677870105164E-10, -0.20874278181886E-10, 0.10162166825089E-9, -0.16429828281347E-9
		    }
    };

    double temperature = 0.0;

    for (std::size_t i = 0; i < array2.size(); i++) {
        temperature += array2[i] * std::pow(pressure, array0[i]) * std::pow((2 - entropy / 2.9251), array1[i]);
    }
    return temperature;
}

 double SteamSystemModelerTool::backwardPressureEntropyRegion1(const double pressure, const double entropy) {
    static const std::array<int, 20> array0 = {{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 4}};
    static const std::array<int, 20> array1 = {{0, 1, 2, 3, 11, 31, 0, 1, 2, 3, 12, 31, 0, 1, 2, 9, 31, 10, 32, 32}};
    static const std::array<double, 20> array2 = {
		    {
				    0.17478268058307E+3, 0.34806930892873E+2, 0.65292584978455E+1, 0.33039981775489, -0.19281382923196E-6,
				    -0.24909197244573E-22, -0.26107636489332, 0.22592965981586, -0.64256463395226E-1, 0.78876289270526E-2,
				    0.35672110607366E-9, 0.17332496994895E-23, 0.56608900654837E-3, -0.32635483139717E-3, 0.44778286690632E-4,
				    -0.51322156908507E-9, -0.42522657042207E-25, 0.26400441360689E-12, 0.78124600459723E-28, -0.30732199903668E-30
		    }
    };

    double temp = 0.0;

    for (std::size_t i = 0; i < array2.size(); i++) {
        temp += array2[i] * std::pow(pressure, array0[i]) * std::pow((entropy + 2), array1[i]);
    }
    return temp;
}

Point SteamSystemModelerTool::generatePoint(int region, SteamSystemModelerTool::Key key, double pressure, double temperature) {
    SteamSystemModelerTool::SteamPropertiesOutput result;

    switch (region) {
        case 1: {
	        result =  SteamSystemModelerTool::region1(temperature, pressure);
	        break;
        }
        case 2: {
            result =  SteamSystemModelerTool::region2(temperature, pressure);
	        break;
        }
        case 3: {
            result =  SteamSystemModelerTool::region3(temperature, pressure);
	        break;
        }
	    default:
		    throw std::range_error("Region must be 1, 2, or 3");
    }

	if (key == SteamSystemModelerTool::Key::ENTHALPY) return {result.specificEnthalpy, temperature};

	return {result.specificEntropy, temperature}; // else key must be ENTROPY
}

double SteamSystemModelerTool::linearTestPoint(const double X, Point const point1, Point const point2) {
    double slope = 0.0;
    if ((point1.getX() - point2.getX()) != 0.0) {
        slope = (point1.getY() - point2.getY()) / (point1.getX() - point2.getX());
    }
    const double yIntercept = point1.getY() - slope * point1.getX();
    return X * slope + yIntercept;
}

double SteamSystemModelerTool::backwardPressureEnthalpyRegion3(const double pressure, const double enthalpy) {
    return SteamSystemModelerTool::backwardRegion3Exact(pressure, enthalpy, SteamSystemModelerTool::Key::ENTHALPY);
}

double SteamSystemModelerTool::backwardPressureEntropyRegion3(const double pressure, const double entropy) {
    return SteamSystemModelerTool::backwardRegion3Exact(pressure, entropy, SteamSystemModelerTool::Key::ENTROPY);
}

double SteamSystemModelerTool::backwardExact(int region, SteamSystemModelerTool::Key key,
                                             SteamSystemModelerTool::Region regionFunction, const double pressure,
                                             const double entropyOrEnthalpy) {
    Point pointA = Point(0.0, 0.0);
    Point pointB = Point(0.0, 0.0);

    if (key == SteamSystemModelerTool::Key::ENTHALPY) {
        if(regionFunction == SteamSystemModelerTool::Region::REGION1) {
            pointA = generatePoint(region, key, pressure, backwardPressureEnthalpyRegion1(pressure, entropyOrEnthalpy));
            pointB = generatePoint(region, key, pressure, backwardPressureEnthalpyRegion1(pressure, pointA.getX()));
        }
        if(regionFunction == SteamSystemModelerTool::Region::REGION2A) {
            pointA = generatePoint(region, key, pressure, backwardPressureEnthalpyRegion2A(pressure, entropyOrEnthalpy));
            pointB = generatePoint(region, key, pressure, backwardPressureEnthalpyRegion2A(pressure, pointA.getX()));
        }
        if(regionFunction == SteamSystemModelerTool::Region::REGION2B) {
            pointA = generatePoint(region, key, pressure, backwardPressureEnthalpyRegion2B(pressure, entropyOrEnthalpy));
            pointB = generatePoint(region, key, pressure, backwardPressureEnthalpyRegion2B(pressure, pointA.getX()));
        }
        if(regionFunction == SteamSystemModelerTool::Region::REGION2C) {
            pointA = generatePoint(region, key, pressure, backwardPressureEnthalpyRegion2C(pressure, entropyOrEnthalpy));
            pointB = generatePoint(region, key, pressure, backwardPressureEnthalpyRegion2C(pressure, pointA.getX()));
        }
    } else if (key == SteamSystemModelerTool::Key::ENTROPY) {
        if(regionFunction == SteamSystemModelerTool::Region::REGION1) {
            pointA = generatePoint(region, key, pressure, backwardPressureEntropyRegion1(pressure, entropyOrEnthalpy));
            pointB = generatePoint(region, key, pressure, backwardPressureEntropyRegion1(pressure, pointA.getX()));
        }
        if(regionFunction == SteamSystemModelerTool::Region::REGION2A) {
            pointA = generatePoint(region, key, pressure, backwardPressureEntropyRegion2A(pressure, entropyOrEnthalpy));
            pointB = generatePoint(region, key, pressure, backwardPressureEntropyRegion2A(pressure, pointA.getX()));
        }
        if(regionFunction == SteamSystemModelerTool::Region::REGION2B) {
	        pointA = generatePoint(region, key, pressure, backwardPressureEntropyRegion2B(pressure, entropyOrEnthalpy));
	        pointB = generatePoint(region, key, pressure, backwardPressureEntropyRegion2B(pressure, pointA.getX()));
        }
        if(regionFunction == SteamSystemModelerTool::Region::REGION2C) {
            pointA = generatePoint(region, key, pressure, backwardPressureEntropyRegion2C(pressure, entropyOrEnthalpy));
            pointB = generatePoint(region, key, pressure, backwardPressureEntropyRegion2C(pressure, pointA.getX()));
        }
    }

    auto temperature = linearTestPoint(entropyOrEnthalpy, pointA, pointB);
    pointA = generatePoint(region, key, pressure, temperature);

    temperature = linearTestPoint(entropyOrEnthalpy, pointA, pointB);
    return temperature;
}


double SteamSystemModelerTool::backwardPressureEnthalpyRegion1Exact(const double pressure, const double enthalpy) {
    return backwardExact(1, SteamSystemModelerTool::Key::ENTHALPY, SteamSystemModelerTool::Region::REGION1, pressure, enthalpy);
}

double SteamSystemModelerTool::backwardPressureEntropyRegion1Exact(const double pressure, const double entropy) {
    return backwardExact(1, SteamSystemModelerTool::Key::ENTROPY, SteamSystemModelerTool::Region::REGION1, pressure, entropy);
}

double SteamSystemModelerTool::backwardPressureEnthalpyRegion2AExact(const double pressure, const double enthalpy) {
    return backwardExact(2, SteamSystemModelerTool::Key::ENTHALPY, SteamSystemModelerTool::Region::REGION2A, pressure, enthalpy);
}

double SteamSystemModelerTool::backwardPressureEntropyRegion2AExact(const double pressure, const double entropy) {
    return backwardExact(2, SteamSystemModelerTool::Key::ENTROPY, SteamSystemModelerTool::Region::REGION2A, pressure, entropy);
}

double SteamSystemModelerTool::backwardPressureEnthalpyRegion2BExact(const double pressure, const double enthalpy) {
    return backwardExact(2, SteamSystemModelerTool::Key::ENTHALPY, SteamSystemModelerTool::Region::REGION2B, pressure, enthalpy);
}

double SteamSystemModelerTool::backwardPressureEntropyRegion2BExact(const double pressure, const double entropy) {
    return backwardExact(2, SteamSystemModelerTool::Key::ENTROPY, SteamSystemModelerTool::Region::REGION2B, pressure, entropy);
}

double SteamSystemModelerTool::backwardPressureEnthalpyRegion2CExact(const double pressure, const double enthalpy) {
    return backwardExact(2, SteamSystemModelerTool::Key::ENTHALPY, SteamSystemModelerTool::Region::REGION2C, pressure, enthalpy);
}

double SteamSystemModelerTool::backwardPressureEntropyRegion2CExact(const double pressure, const double entropy) {
    return backwardExact(2, SteamSystemModelerTool::Key::ENTROPY, SteamSystemModelerTool::Region::REGION2C, pressure, entropy);
}

std::ostream &operator<<(std::ostream &stream, const SteamSystemModelerTool::SteamPropertiesOutput &props) {
    stream << "SteamPropertiesOutput["
           << "temperature=" << props.temperature
           << ", pressure=" << props.pressure
           << ", quality=" << props.quality
           << ", specificVolume=" << props.specificVolume
           << ", density=" << props.density
           << ", specificEnthalpy=" << props.specificEnthalpy
           << ", specificEntropy=" << props.specificEntropy
           << ", internalEnergy=" << props.internalEnergy << "]";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const SteamSystemModelerTool::FluidProperties &props) {
    stream << "FluidProperties["
           << "temperature=" << props.temperature
           << ", pressure=" << props.pressure
           << ", quality=" << props.quality
           << ", specificVolume=" << props.specificVolume
           << ", density=" << props.density
           << ", specificEnthalpy=" << props.specificEnthalpy
           << ", specificEntropy=" << props.specificEntropy
           << ", internalEnergy=" << props.internalEnergy
           << ", massFlow=" << props.massFlow
           << ", energyFlow=" << props.energyFlow << "]";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const std::shared_ptr<SteamSystemModelerTool::FluidProperties> &props) {
    if (props == nullptr) {
        stream << "FluidProperties[nullptr]";
    } else {
        stream << *props;
    }
    return stream;
}
