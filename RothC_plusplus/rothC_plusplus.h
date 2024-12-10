#ifndef ROTHC_PLUSPLUS_H
#define ROTHC_PLUSPLUS_H

double RMF_PC(bool PC);
double RMF_Moist(double RAIN, double PEVAP, double clay, double depth, bool PC, double &SWC);
double RMF_Moist(double monthlyBIC, double clay, double depth, bool PC, double &SWC);
double RMF_Tmp(double TEMP);
void decomp(int timeFact, double &DPM, double &RPM, double &BIO, double &HUM, double &IOM,
            double &SOC, double &DPM_Rage, double &RPM_Rage, double &BIO_Rage, double &HUM_Rage,
            double &IOM_Rage, double &Total_Rage, double &modernC, double &RateM, double &clay,
            double &C_Inp, double &FYM_Inp, double &DPM_RPM);
void RothC(int timeFact, double &DPM, double &RPM, double &BIO, double &HUM, double &IOM, double &SOC,
           double &DPM_Rage, double &RPM_Rage, double &BIO_Rage, double &HUM_Rage, double &IOM_Rage,
           double &Total_Rage, double &modernC, double &clay, double &depth, double &TEMP, double &RAIN,
           double &WATERLOSS, bool isET0, bool &PC, double &DPM_RPM, double C_Inp, double FYM_Inp, double &SWC);

#endif // ROTHC_PLUSPLUS_H
