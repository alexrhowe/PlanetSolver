#include "stdafx.h"
#include "EOS.h"

#ifndef PLANET_H
#define PLANET_H

class Planet
{
 private:
  static const double MAX_RAD;
  static const int MAX_STEP;
  
  double h;
  double minP;
  EOS *eos;
  double T;

  vector<double> rVals;
  vector<double> rhoVals;
  vector<double> mVals;
  vector<double> pVals;
  vector<double> iVals;

  typedef pair<EOS*, double> EOSBoundary;
  struct EOSBoundaryCompare{
    bool operator()(const EOSBoundary& p1, const EOSBoundary& p2) const;
  };
  priority_queue<EOSBoundary, vector<EOSBoundary>, EOSBoundaryCompare> boundaries;

 public:
  double iceCV;
  double tSurf;
  double tCore;
  double pCore;
  double Tdm;
  
  Planet(double numH, double pCentral, double setP, EOS* EOSc);
  void addEOS(double newM, EOS* newEOS);
  void setT(double newT);
  void integrate();
  void step();
  void checkBoundary();
  double getT();
  double getR();
  double getMTotal();
  double getTcore();
  double getTsurf();
  double getPcore();
  double getTdm();
  double getIceCV();
    
  void findVals(int interval, double entropy, double metals, double mcore, double mrock);
  void printRecord(string outFile, int interval,double entropy, double metals);

  double f(double rn, double rhon, double mn);
  double g(double rn, double rhon);
  double kRho1();
  double kM1();
  double kRho2(double kRho, double kM);
  double kM2(double kRho, double kM);
  double kRho3(double kRho, double kM);
  double kM3(double kRho, double kM);
  double kRho4(double kRho, double kM);
  double kM4(double kRho, double kM);
};

#endif
