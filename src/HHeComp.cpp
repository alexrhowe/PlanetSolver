#include "stdafx.h"
#include "files.h"

using namespace cons;

const double HHeComp::STEP = 0.01;          // Sets the delta log(rho) and log(T) for computing derivatives.
const double HHeComp::RT_PRECISION = 0.001; // Required precision for log(rho) and log(T).
const double HHeComp::STEP_SIZE = 0.1;      // Maximum step size for log(rho) and log(T).
const int HHeComp::MAX_STEP = 10000;        // Maximum number of steps used by the Newton solver.

HHeComp::HHeComp(double metals){
  std::ifstream stabnew("EOS_Tables/stabnew.dat");
  std::ifstream ptabnew("EOS_Tables/ptabnew.dat");
  std::ifstream stablow("EOS_Tables/stablow.dat");
  std::ifstream ptablow("EOS_Tables/ptablow.dat");

  //std::ofstream HHe("HHe.dat"); // I think this was for testing, records entropy-pressure curves.
    
  stabnew >> yfrac >> length >> rho1H >> rho2H >> t11H >> t12H >> t21H >> t22H;
  ptabnew >> yfrac >> length >> rho1H >> rho2H >> t11H >> t12H >> t21H >> t22H;
  stablow >> yfrac >> length >> rho1L >> rho2L >> t11L >> t12L >> t21L >> t22L;
  ptablow >> yfrac >> length >> rho1L >> rho2L >> t11L >> t12L >> t21L >> t22L;

  stabH = vector<vector<double> >(length,vector<double>(100,0));
  ptabH = vector<vector<double> >(length,vector<double>(100,0));
  stabL = vector<vector<double> >(length,vector<double>(100,0));
  ptabL = vector<vector<double> >(length,vector<double>(100,0));

  double temp;
  metals *= 0.00276;
  double mmix = metals*16.93;
  double mrat = 2.247/(2.247+mmix);
  double tt, tt1, tt2, rho;

  for(int i=0; i<length; i++){
    for(int j=0; j<100; j++){
      rho = rho1H + (rho2H-rho1H)*i/((double)length-1.);
      tt1 = t11H + (t21H-t11H)*i/((double)length-1.);
      tt2 = t12H + (t22H-t12H)*i/((double)length-1.);
      tt = tt1 + (tt2-tt1)*j/99.;
      rho = pow(10,rho);
      tt = pow(10,tt);
      stabnew >> temp;
      stabH[i][j] = log10((1-metals)*pow(10,temp) + 1.77e-4*metals/0.00276*log(pow(tt,3./2.)/rho));
      
      rho = rho1L + (rho2L-rho1L)*i/((double)length-1.);
      tt1 = t11L + (t21L-t11L)*i/((double)length-1.);
      tt2 = t12L + (t22L-t12L)*i/((double)length-1.);
      tt = tt1 + (tt2-tt1)*j/99.;
      rho = pow(10,rho);
      tt = pow(10,tt);	  
      stablow >> temp;
      stabL[i][j] = log10((1-metals)*pow(10,temp) + 1.77e-4*metals/0.00276*log(pow(tt,3./2.)/rho));
      
      ptabnew >> temp;
      ptabH[i][j] = temp + log10(mrat);
      ptablow >> temp;
      ptabL[i][j] = temp + log10(mrat);
  
      // HHe << i << " " << j << " " << stabH[i][j] << " " << stabL[i][j] << " " << ptabH[i][j] << " " << stabL[i][j] << "\n";
    }
  }
  newton(1.e7,6.0);
}

double HHeComp::getRho(){
  return density;
}

double HHeComp::getT(){
  return temperature;
}

double HHeComp::getdPdRho(){
  return dpdr/1.e12;
}

// From pressure and entropy, computes density and temperature.
void HHeComp::newton(double press, double ent){
  density = -3.1;
  temperature = 3.95;
  int n=0;
  double dr=0, dt=0;
  double r,t,p0,p1,p2,s0,s1,s2,dpdt,dsdt,dsdr,detJ;

  press = log10(press);
  ent = log10(ent);
  uselow = 0;

  while(1){
    n++;
    look(density,temperature);
    p0 = fp;
    s0 = fs;
    look(density+STEP,temperature);
    p1 = fp;
    s1 = fs;
    look(density,temperature+STEP);
    p2 = fp;
    s2 = fs;

    dpdr = (p1-p0) / (density*STEP);
    dpdt = (p2-p0) / (temperature*STEP);

    dsdr = (s1-s0) / (density*STEP);
    dsdt = (s2-s0) / (temperature*STEP);

    detJ = dpdr*dsdt - dsdr*dpdt;
    
    if(detJ==0.) detJ=-0.001;

    //Determinant is zero when temperature goes off edge of table.

    dr = 1./detJ * ( dsdt*(p0-press) - dpdt*(s0-ent)) / density;
    dt = 1./detJ * (-dsdr*(p0-press) + dpdr*(s0-ent)) / temperature;
    
    if(fabs(dr)>STEP_SIZE) dr *= STEP_SIZE/fabs(dr);
    if(fabs(dt)>STEP_SIZE) dt *= STEP_SIZE/fabs(dt);

    density -= dr;
    temperature -= dt;

    if(temperature<0.5) break;
    
    if(fabs(dr)<RT_PRECISION && fabs(dt)<RT_PRECISION) break;
    if(n>MAX_STEP){
      printf("Envelope iteration limit exceeded in temperature solution.\n");
      std::exit(0);
      break;
    }
  }
  dpdr = (pow(10,p1)-pow(10,p0)) / (pow(10,density+STEP)-pow(10,density));
  // needs to be in the correct form when it's over
}

void HHeComp::look(double rho, double temp){
  double alpha = t11H + (rho-rho1H)/(rho2H-rho1H)*(t21H-t11H);
  double beta = t12H - t11H + ((t22H-t21H)-(t12H-t11H))*(rho-rho1H)/(rho2H-rho1H);
  double ql = (temp-alpha)/beta;

  if(uselow==0 && ql>0 && rho>rho1H){
    double delta = (rho-rho1H)/(rho2H-rho1H)*(double)length;
    double intpart;
    int jr = 1+(int)floor(delta);
    int jq = 1+(int)floor(100*ql);
    double p = modf(delta,&intpart);
    double q = modf(100.*ql,&intpart);

    fs = 0.5*q*(q-1.)*stabH[jr][jq-1]      + 0.5*p*(p-1.)     *stabH[jr-1][jq]
      + (1.+p*q-p*p-q*q)*stabH[jr][jq]    + 0.5*p*(p-2.*q+1.)*stabH[jr+1][jq]
      + 0.5*q*(q-2.*p+1.)*stabH[jr][jq+1] +     p*q          *stabH[jr+1][jq+1];

    fp = 0.5*q*(q-1.)*ptabH[jr][jq-1]      + 0.5*p*(p-1.)     *ptabH[jr-1][jq]
      + (1.+p*q-p*p-q*q)*ptabH[jr][jq]    + 0.5*p*(p-2.*q+1.)*ptabH[jr+1][jq]
      + 0.5*q*(q-2.*p+1.)*ptabH[jr][jq+1] +     p*q          *ptabH[jr+1][jq+1];
  }
  
  else{
    uselow=1;
    alpha = t11L + (rho-rho1L)/(rho2L-rho1L)*(t21L-t11L);
    beta = t12L - t11L + ((t22L-t21L)-(t12L-t11L))*(rho-rho1L)/(rho2L-rho1L);
    ql = (temp-alpha)/beta;
    double delta = (rho-rho1L)/(rho2L-rho1L)*(double)length;
    double intpart;
    int jr = 1+(int)floor(delta);
    int jq = 1+(int)floor(100.*ql);    
    double p = modf(delta,&intpart);
    double q = modf(100.*ql,&intpart);

    if(jq<1){
      jq=1;
      q=0.;
    }

    fs =      0.5*q*(q-1.)*stabL[jr][jq-1] + 0.5*p*(p-1.)     *stabL[jr-1][jq]
      +  (1.+p*q-p*p-q*q)*stabL[jr][jq]   + 0.5*p*(p-2.*q+1.)*stabL[jr+1][jq]
      + 0.5*q*(q-2.*p+1.)*stabL[jr][jq+1] +     p*q          *stabL[jr+1][jq+1];
    
    fp =      0.5*q*(q-1.)*ptabL[jr][jq-1] + 0.5*p*(p-1.)     *ptabL[jr-1][jq]
      +  (1.+p*q-p*p-q*q)*ptabL[jr][jq]   + 0.5*p*(p-2.*q+1.)*ptabL[jr+1][jq]
      + 0.5*q*(q-2.*p+1.)*ptabL[jr][jq+1] +     p*q          *ptabL[jr+1][jq+1];
  }
}
