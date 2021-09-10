#include "stdafx.h"
#include "files.h"

using namespace cons;

// --- basic constants ---
const double cons::G     = 6.67428e-11;
const double cons::PI    = 3.1415926535897932384626433832795028841971693993751;
const double cons::HBAR  = 1.0545717e-34;
const double cons::E     = 2.7182818284590452353602874713526624977572470937;
const double cons::MASS_E   = 9.1093829e-31; // electron mass
const double cons::U     = 1.6605388e-27; // atomic mass unit
const double cons::R     = 8.3144621;     // gas constant
const double cons::N_A   = 6.02214129e23;

const double cons::M_FE  = 0.05585;
const double cons::M_PV  = 0.10039;
const double cons::M_MGO = 0.04030;
const double cons::M_C   = 0.01201;

const double cons::M_EARTH = 5.9736e24;     // Earth mass
const double cons::R_EARTH = 6.371e6;       // Earth radius

const double cons::MYR = 3.1556952e13;  // seconds to MYR
const double cons::AU = 1.495978707e11; // m to AU

// --- derived constants ---
const double cons::K_B  = R / N_A;
const double cons::K_WD = (HBAR * HBAR / (15 * MASS_E * PI * PI)) * pow((3 * PI * PI) / (U * 0.5), 5.0/3.0);

// --- functions ---
int cons::findIndex(double find, vector<double> vals, int numVals)
{
	if (find <= vals[0])
		return -1;
	if (find > vals[numVals - 1])
		return numVals;
	
	int index;
	int lo = 0;
	int hi = numVals - 1;
	while (lo < hi)
	{
		index = (lo + hi) / 2;
		if (find > vals[index])
			lo = index + 1;
		else
			hi = index;
	}
	index = lo;
	if ((vals[index] < find) || (vals[index - 1] > find))
		throw "Search fail";
	return index;
}

double cons::splineInterp(double x, double x1, double x2, double y1, 
			  double y2, double dydx1, double dydx2)
{
	double t = (x - x1) / (x2 - x1);
	double a = (dydx1 * (x2 - x1)) - (y2 - y1);
	double b = -(dydx2 * (x2 - x1)) + (y2 - y1);

	return ((1 - t) * y1) + (t * y2) + (t * (1 - t) * ((a * (1 - t)) + (b * t)));
}

double cons::linInterp(double x, double x1, double x2, double y1, 
		       double y2)
{
	double t = (x - x1) / (x2 - x1);
	return ((1 - t) * y1) + (t * y2);
}
