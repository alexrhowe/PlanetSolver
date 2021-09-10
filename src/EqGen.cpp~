/* 
 * wemEG: Equation of State Generator
 * Wesley Verne
 */

#include "stdafx.h"
#include "files.h"

using namespace cons;
using namespace std;

void printHelp(const char* progName);
bool isNum(const char* arg);
string eosName(int eosNum);

int main(int argc, const char* argv[])
{
   // default values
   bool useP = true;
   bool verbose = false;
   int num = 1;
   double step = 0.01;
   double startP = 1e6;
   double endP = 1e15;
   double startRho = 6220;
   double endRho = 1e5;
   double T = 0.0;
   string oFilename = "wemEG_out";
   EOS* eos = new EOS();
   eos->setNum(1);

   // accept user input
   if (argc == 1)
   {
      printHelp(argv[0]);
      return 0;
   }
   int i = 1;
   while (i < argc)
   {
      if (!strcmp("-h", argv[i]) || !strcmp("-help", argv[i]))
      {
	 printHelp(argv[0]);
	 return 0;
      }
      else if (!strcmp("-n", argv[i]) || !strcmp("-num", argv[i]))
      {
	 if ((i == argc-1) || (!isNum(argv[i+1])))
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 int eosNum = atoi(argv[i+1]);
	 eos->setNum(eosNum);
	 oFilename = "EOS_" + eosName(eosNum);
	 i += 2;
      }
      else if (!strcmp("-d", argv[i]) || !strcmp("-density", argv[i]))
      {
	 useP = false;
	 i++;
      }
      else if (!strcmp("-p", argv[i]) || !strcmp("-pressure", argv[i]))
      {
	 useP = true;
	 i++;
      }
      else if (!strcmp("-s", argv[i]) || !strcmp("-step", argv[i]))
      {
	 if ((i == argc-1) || (!isNum(argv[i+1])))
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 step = atof(argv[i+1]);
	 i += 2;
      }
      else if (!strcmp("-b", argv[i]) || !strcmp("-start", argv[i]))
      {
	 if ((i == argc-1) || (!isNum(argv[i+1])))
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 startP = startRho = atof(argv[i+1]);
	 i += 2;
      }
      else if (!strcmp("-e", argv[i]) || !strcmp("-end", argv[i]))
      {
	 if ((i == argc-1) || (!isNum(argv[i+1])))
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 endP = endRho = atof(argv[i+1]);
	 i += 2;
      }
      else if (!strcmp("-tn", argv[i]) || !strcmp("-tempnum", argv[i]))
      {
	 if ((i == argc-1) || (!isNum(argv[i+1])))
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 eos->setThermal(atoi(argv[i+1]));
	 i += 2;
      }
      else if (!strcmp("-t", argv[i]) || !strcmp("-temp", argv[i]))
      {
	 if ((i == argc-1) || (!isNum(argv[i+1])))
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 T = atof(argv[i+1]);
	 i += 2;
      }
      else if (!strcmp("-o", argv[i]) || !strcmp("-outfile", argv[i]))
      {
	 if (i == argc-1)
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 oFilename = argv[i+1];
	 i += 2;
      }
      else if (!strcmp("-m", argv[i]) || !strcmp("-mix", argv[i]))
      {
	 if ((i >= argc-3) || (!isNum(argv[i+1])) 
	     || (!isNum(argv[i+2])) || (!isNum(argv[i+3])))
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 int eos1 = atoi(argv[i+1]);
	 int eos2 = atoi(argv[i+2]);
	 double frac = atof(argv[i+3]);
	 if ((frac < 0.0) || (frac > 1.0))
	 {
	    printHelp(argv[0]);
	    return 0;
	 }
	 eos->setMix(eos1, eos2, frac);
	 char buf[100];
	 sprintf(buf, "EOS_%1.2f%s%1.2f%s", frac, eosName(eos1).c_str(),
		                        1 - frac, eosName(eos2).c_str());
	 oFilename = buf;
	 i += 4;
      }
      else if (!strcmp("-v", argv[i]) || !strcmp("-verbose", argv[i]))
      {
	 verbose = true;
	 i++;
      }
      else
      {
	 fprintf(stderr, "Unrecognized argument: %s\n", argv[i]);
	 i++;
      }
   }

   if (verbose)
      printf("Calculating EOS...\n");

   // calculate and output EOS table
   if (useP)
      eos->printEOS(step, startP, endP, T, oFilename);
   else
      eos->printEOSRho(step, startRho, endRho, T, oFilename);      

   delete eos;
   return 0;
}

void printHelp(const char* progName)
{
   printf("Usage: %s arguments\n", progName);
   printf("Arguments: -help/-h\n");
   printf("           -num/-n eos_num\n");
   printf("           -density/-d\n");
   printf("           -pressure/-p (default)\n");
   printf("           -step/-s step_size\n");
   printf("           -start/-b start_density_or_pressure\n");
   printf("           -end/-e end_density_or_pressure\n");
   printf("           -tempnum/-tn temp_num\n");
   printf("           -temp/-t temperature\n");
   printf("           -outfile/-o filename\n");
   printf("           -mix/-m eos_num1 eos_num2 ratio\n");
   printf("           -verbose/-v\n");
   printf("EOS Numbers: 1 Fe (epsilon) Vinet\n");
   printf("             2 MgSiO3 (pv) Vinet\n");
   printf("             3 MgO (periclase) BME3\n");
   printf("             4 SiC (ZB + RS) BME3\n");
   printf("             5 Diamond Vinet\n");
   printf("             6 Platinum Vinet (has bugs)\n");
   printf("             7 H2O Iced VII Vinet\n");
   printf("See readme for details, default values, and more eos numbers\n");
}

bool isNum(const char* arg)
{
   if (*arg == '-')
   {
      if (!isdigit(arg[1]) && !(arg[1] == '.'))
	 return false;
   }
   return true;
}

string eosName(int eosNum)
{
   switch (eosNum)
   {
      case  0:
	 return "FeBME";
	 break;
      case  1:
	 return "Fe";
	 break;
      case  2:
	 return "pv";
	 break;
      case  3:
	 return "MgO";
	 break;
      case  4:
	 return "SiC";
	 break;
      case  5:
	 return "Diamond";
	 break;
      case  6:
	 return "Platinum";
	 break;
      case  7:
	 return "H2OVII";
	 break;
      case -1:
	 return "whitedwarf";
	 break;
      case -2:
	 return "PlatinumBME";
	 break;
      case -3:
	 return "H2OVIIBME";
	 break;
      case -4:
	 return "FeVinet";
	 break;
      case -5:
	 return "pvBME";
	 break;
      case -6:
	 return "FeTFD";
	 break;
      case -7:
	 return "n1polytrope";
	 break;
      default:
	 throw "Invalid eosNum";
   }
}
