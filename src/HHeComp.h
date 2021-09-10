#include "stdafx.h"

#ifndef HHECOMP_H
#define HHECOMP_H

class HHeComp{

private:
  // private constants
  static const double STEP;
  static const double RT_PRECISION;
  static const double STEP_SIZE;
  static const int MAX_STEP;
  
  // private variables
  double t11H,t11L,t12H,t12L,t21H,t21L,t22H,t22L; // temperature boundary values
  double rho1H,rho1L,rho2H,rho2L;                 // density boundary values
  double yfrac;                                   // helium fraction
  int length;                                  // grid size
  double fs,fp,dpdr;                              // atmosphere properties
  int uselow;                                     // determines which grid to use
  double density,temperature;                     // atmosphere properties

  // storage for atmosphere grid
  vector<vector<double> > stabH;
  vector<vector<double> > ptabH;
  vector<vector<double> > stabL;
  vector<vector<double> > ptabL;

public:

  // --- public methods ---
  HHeComp(double metals);
  // read in the pressure and entropy files
  void newton(double press, double entropy);
  // Newton-Raphson method to compute temperature and density
  double getRho();
  double getT();
  double getdPdRho();

private:
  // --- private methods ---
  void look(double rho, double temp);
  // computes pressure and entropy from density and temperature
};

#endif // HHECOMP_H
