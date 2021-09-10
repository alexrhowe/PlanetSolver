// inside constants.h
#ifndef WEM_CONSTANTS_H
#define WEM_CONSTANTS_H

// all in SI units
namespace cons
{
   // --- basic constants ---
   extern const double G;     // gravitational constant
   extern const double PI;    // pi
   extern const double HBAR;  // planck's constant
   extern const double E;     // electron charge
   extern const double MASS_E;   // electron mass
   extern const double U;     // atomic mass unit
   extern const double R;     // gas constant
   extern const double N_A;   // avogadro constant

   extern const double M_FE;  // molar mass of iron
   extern const double M_PV;  // molar mass of MgSiO3
   extern const double M_MGO; // molar mass of MgO
   extern const double M_C;   // molar mass of carbon

   extern const double M_EARTH; // mass of Earth
   extern const double R_EARTH; // radius of Earth
   
   extern const double MYR;
   extern const double AU;

   // --- derived constants ---
   extern const double K_B;  // boltzmann constant
   extern const double K_WD; // white dwarf K (polytropic)

   // --- functions ---
   // finds index i of ordered array vals for which vals[i - 1] <= find
   //<= vals[i]
   int findIndex(double find, vector<double> vals, int numVals); 
   // finds y for given x using spline interpolation
   double splineInterp(double x, double x1, double x2, double y1, 
                       double y2, double dydx1, double dydx2); 
   // finds y for given x using linear interpolation
   double linInterp(double x, double x1, double x2, double y1, 
                    double y2); 
}

#endif
