#include <iostream>
#include <vector>
#include <cmath>

// Calculates the plant retainment modifying factor (RMF_PC)
double RMF_PC(double PC) {
    double RM_PC;
    if (PC == 0) {
        RM_PC = 1.0;
    } else {
        RM_PC = 0.6;
    }
    return RM_PC;
}

// Calculates the rate modifying factor for moisture (RMF_Moist)
double RMF_Moist(double RAIN, double PEVAP, double clay, double depth, double PC, double SWC) {
    double RMFMax = 1.0;
    double RMFMin = 0.2;
    double SMDMax = -(20 + 1.3 * clay - 0.01 * (clay * clay));
    double SMDMaxAdj = SMDMax * depth / 23.0;
    double SMD1bar = 0.444 * SMDMaxAdj;
    double SMDBare = 0.556 * SMDMaxAdj;
    double DF = RAIN - 0.75 * PEVAP;
    double minSWCDF = std::min(0.0, SWC + DF);
    double minSMDBareSWC = std::min(SMDBare, SWC);
    if (PC == 1) {
        SWC = std::max(SMDMaxAdj, minSWCDF);
    } else {
        SWC = std::max(minSMDBareSWC, minSWCDF);
    }
    double RM_Moist;
    if (SWC > SMD1bar) {
        RM_Moist = 1.0;
    } else {
        RM_Moist = RMFMin + (RMFMax - RMFMin) * (SMDMaxAdj - SWC) / (SMDMaxAdj - SMD1bar);
    }
    return RM_Moist;
}

// Calculates the rate modifying factor for temperature (RMF_Tmp)
double RMF_Tmp(double TEMP) {
    double RM_TMP;
    if (TEMP < -5.0) {
        RM_TMP = 0.0;
    } else {
        RM_TMP = 47.91 / (std::exp(106.06 / (TEMP + 18.27)) + 1.0);
    }
    return RM_TMP;
}

// The Rothamsted Carbon Model: RothC
void RothC(double timeFact, double DPM, double RPM, double BIO, double HUM, double IOM, double SOC, double DPM_Rage, double RPM_Rage, double BIO_Rage, double HUM_Rage, double Total_Rage, double modernC, double clay, double depth, double TEMP, double RAIN, double PEVAP, double PC, double DPM_RPM, double C_Inp, double FYM_Inp, double SWC) {
    // Calculate RMFs    
    double RM_TMP = RMF_Tmp(TEMP);
    double RM_Moist = RMF_Moist(RAIN, PEVAP, clay, depth, PC, SWC);
    double RM_PC = RMF_PC(PC);

    // Combine RMF's into one.     
    double RateM = RM_TMP * RM_Moist * RM_PC;

    decomp(timeFact, DPM, RPM, BIO, HUM, IOM, SOC, DPM_Rage, RPM_Rage, BIO_Rage, HUM_Rage, Total_Rage, modernC, RateM, clay, C_Inp, FYM_Inp, DPM_RPM);

    return;
}

int main() {
    // Example