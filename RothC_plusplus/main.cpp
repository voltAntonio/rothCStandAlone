#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
double EPSILON = 0.00000001;


using namespace std;
#include <vector>
#include <cmath>

// Calculates the plant retainment modifying factor (RMF_PC)
double RMF_PC(bool PC) {
    double RM_PC;
    if (!PC) {
        RM_PC = 1.0;
    } else {
        RM_PC = 0.6;
    }
    return RM_PC;
}

// Calculates the rate modifying factor for moisture (RMF_Moist)
double RMF_Moist(double RAIN, double PEVAP, double clay, double depth, bool PC, double &SWC) {
    const double RMFMax = 1.0;
    const double RMFMin = 0.2;

    //calc soil water functions properties
    double SMDMax = -(20 + 1.3 * clay - 0.01 * (clay * clay));
    double SMDMaxAdj = SMDMax * depth / 23.0;
    double SMD1bar = 0.444 * SMDMaxAdj;
    double SMDBare = 0.556 * SMDMaxAdj;

    double DF = RAIN - 0.75 * PEVAP;

    double minSWCDF = std::min(0.0, SWC + DF);
    double minSMDBareSWC = std::min(SMDBare, SWC);
    if (PC) {
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

double RMF_Moist(double monthlyBIC, double clay, double depth, bool PC, double &SWC) {
    const double RMFMax = 1.0;
    const double RMFMin = 0.2;

    //calc soil water functions properties
    double SMDMax = -(20 + 1.3 * clay - 0.01 * (clay * clay));
    double SMDMaxAdj = SMDMax * depth / 23.0;
    double SMD1bar = 0.444 * SMDMaxAdj;
    double SMDBare = 0.556 * SMDMaxAdj;

    double DF = monthlyBIC;

    double minSWCDF = std::min(0.0, SWC + DF);
    double minSMDBareSWC = std::min(SMDBare, SWC);
    if (PC) {
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

void decomp(int timeFact, double &DPM, double &RPM, double &BIO, double &HUM, double &IOM, double &LAB, double &REC, double &SOC, double &co2, double &DPM_Rage, double &RPM_Rage, double &BIO_Rage, double &HUM_Rage, double &IOM_Rage, double &Total_Rage, double &modernC, double &RateM, double clay, double C_Inp, double FYM_Inp, double BIOCHAR_Inp, double &DPM_RPM)
{
    const double DPM_k = 10.0;
    const double RPM_k = 0.3;
    const double BIO_k = 0.66;
    const double HUM_k = 0.02;
    const double REC_k = 0.08/RateM;
    const double LAB_k = 2.55/RateM;

    const double conr = 0.0001244876401867718; // equivalent to std::log(2.0)/5568.0;
    double tstep = 1.0/timeFact; //monthly 1/12 or daily 1/365
    double exc = std::exp(-conr*tstep);

    //priming effect factor NOTA
    double PE;
    if (LAB > EPSILON || REC > EPSILON)
        PE = - std::log(0.16);
    else
        PE = 1;

    //decomposition
    double DPM1 = DPM*std::exp(-RateM*DPM_k*tstep);
    double RPM1 = RPM*std::exp(-RateM*RPM_k*tstep);
    double BIO1 = BIO*std::exp(-RateM*BIO_k*tstep);
    double HUM1 = HUM*std::exp(-RateM*HUM_k*tstep);
    double LAB1 = LAB*std::exp(-LAB_k*tstep); //NOTA
    double REC1 = REC*std::exp(-REC_k*tstep); //NOTA

    double DPM_d = DPM - DPM1;
    double RPM_d = RPM - RPM1;
    double BIO_d = BIO - BIO1;
    double HUM_d = HUM - HUM1;
    double LAB_d = LAB - LAB1;
    double REC_d = REC - REC1;

    double x = 1.67*(1.85+1.60*std::exp(-0.0786*clay));
    double xPlusPlus = x + 1;
    double ratioFactor[3];
    ratioFactor[0] = x / xPlusPlus;
    ratioFactor[1] = 0.46 / xPlusPlus;
    ratioFactor[2] = 0.54 / xPlusPlus;
    //proportion C from each pool into CO2, BIO and HUM
    double DPM_co2 = DPM_d * ratioFactor[0];
    double DPM_BIO = DPM_d * ratioFactor[1];
    double DPM_HUM = DPM_d * ratioFactor[2];

    double RPM_co2 = RPM_d * ratioFactor[0];
    double RPM_BIO = RPM_d * ratioFactor[1];
    double RPM_HUM = RPM_d * ratioFactor[2];

    double BIO_co2 = BIO_d * ratioFactor[0];
    double BIO_BIO = BIO_d * ratioFactor[1];
    double BIO_HUM = BIO_d * ratioFactor[2];

    double HUM_co2 = HUM_d * ratioFactor[0];
    double HUM_BIO = HUM_d * ratioFactor[1];
    double HUM_HUM = HUM_d * ratioFactor[2];

    double LAB_co2 = LAB_d;
    double REC_co2 = REC_d;

    //update C pools
    DPM = DPM1;
    RPM = RPM1;
    BIO = BIO1 + DPM_BIO + RPM_BIO + BIO_BIO + HUM_BIO;
    HUM = HUM1 + DPM_HUM + RPM_HUM + BIO_HUM + HUM_HUM;

    LAB = LAB1;
    REC = REC1;

    co2 = DPM_co2 + RPM_co2 + BIO_co2 + HUM_co2 + LAB_co2 + REC_co2;

    //split plant C to DPM and RPM
    double PI_C_DPM = DPM_RPM / (DPM_RPM + 1.0) * C_Inp;
    double PI_C_RPM = 1.0 / (DPM_RPM + 1.0) * C_Inp;

    //split FYM C to DPM, RPM and HUM
    double FYM_C_DPM = 0.49*FYM_Inp;
    double FYM_C_RPM = 0.49*FYM_Inp;
    double FYM_C_HUM = 0.02*FYM_Inp;

    double BC_Inp_C_LAB = 0.4*BIOCHAR_Inp;
    double BC_Inp_C_REC = 0.96*BIOCHAR_Inp;

    //add plant C and FYM_C to DPM, RPM and HUM
    DPM = DPM + PI_C_DPM + FYM_C_DPM;
    RPM = RPM + PI_C_RPM + FYM_C_RPM;
    HUM = HUM + FYM_C_HUM;

    LAB = LAB + BC_Inp_C_LAB;
    REC = REC + BC_Inp_C_REC;

    //calc new ract of each pool
    double DPM_Ract = DPM1 * std::exp(-conr*DPM_Rage);
    double RPM_Ract = RPM1 * std::exp(-conr*RPM_Rage);

    double BIO_Ract = BIO1 * std::exp(-conr*BIO_Rage);
    double DPM_BIO_Ract = DPM_BIO * std::exp(-conr*DPM_Rage);
    double RPM_BIO_Ract = RPM_BIO * std::exp(-conr*RPM_Rage);
    double BIO_BIO_Ract = BIO_BIO * std::exp(-conr*BIO_Rage);
    double HUM_BIO_Ract = HUM_BIO * std::exp(-conr*HUM_Rage);

    double HUM_Ract = HUM1 *std::exp(-conr*HUM_Rage);
    double DPM_HUM_Ract = DPM_HUM * std::exp(-conr*DPM_Rage);
    double RPM_HUM_Ract = RPM_HUM * std::exp(-conr*RPM_Rage);
    double BIO_HUM_Ract = BIO_HUM * std::exp(-conr*BIO_Rage);
    double HUM_HUM_Ract = HUM_HUM * std::exp(-conr*HUM_Rage);

    double IOM_Ract = IOM * std::exp(-conr*IOM_Rage);

    //assign new C from plant and FYM the correct age
    double PI_DPM_Ract = modernC * PI_C_DPM;
    double PI_RPM_Ract = modernC * PI_C_RPM;

    double FYM_DPM_Ract = modernC * FYM_C_DPM;
    double FYM_RPM_Ract = modernC * FYM_C_RPM;
    double FYM_HUM_Ract = modernC * FYM_C_HUM;

    // update ract for each pool
    double DPM_Ract_new = FYM_DPM_Ract + PI_DPM_Ract + DPM_Ract*exc;
    double RPM_Ract_new = FYM_RPM_Ract + PI_RPM_Ract + RPM_Ract*exc;

    double BIO_Ract_new = (BIO_Ract + DPM_BIO_Ract + RPM_BIO_Ract + BIO_BIO_Ract + HUM_BIO_Ract )*exc;

    double HUM_Ract_new = FYM_HUM_Ract + (HUM_Ract + DPM_HUM_Ract + RPM_HUM_Ract + BIO_HUM_Ract + HUM_HUM_Ract)*exc;

    SOC = DPM + RPM + BIO + HUM + IOM + LAB + REC;
    double Total_Ract = DPM_Ract_new + RPM_Ract_new + BIO_Ract_new + HUM_Ract_new + IOM_Ract;

    //calculate rage of each pool
    if (DPM <= EPSILON)
        DPM_Rage = 0;
    else
        DPM_Rage = (std::log(DPM/DPM_Ract_new) ) / conr;


    if(RPM <= EPSILON)
        RPM_Rage = 0;
    else
        RPM_Rage = (std::log(RPM/RPM_Ract_new) ) / conr;

    if(BIO <= EPSILON)
        BIO_Rage = 0;
    else
        BIO_Rage = ( std::log(BIO/BIO_Ract_new) ) / conr;


    if(HUM <= EPSILON)
        HUM_Rage = 0;
    else
        HUM_Rage = ( std::log(HUM/HUM_Ract_new) ) / conr;


    if(SOC <= EPSILON)
        Total_Rage = 0;
    else
        Total_Rage = ( std::log(SOC/Total_Ract) ) / conr;

    return;
}

// The Rothamsted Carbon Model: RothC
void RothC(int timeFact, double &DPM, double &RPM, double &BIO, double &HUM, double &IOM, double &LAB, double &REC, double &SOC, double &co2, double &DPM_Rage, double &RPM_Rage, double &BIO_Rage, double &HUM_Rage, double &IOM_Rage, double &Total_Rage, double &modernC, double &clay, double &depth, double &TEMP, double &RAIN, double &WATERLOSS,bool isET0, bool &PC, double &DPM_RPM, double C_Inp, double FYM_Inp, double BIOCHAR_Inp, double &SWC) {
    // Calculate RMFs
    double RM_TMP = RMF_Tmp(TEMP);
    double RM_Moist;
    if (isET0)
    {
        double monthlyBIC = RAIN - WATERLOSS;
        RM_Moist = RMF_Moist(monthlyBIC, clay, depth, PC, SWC);
    }
    else
    {
        RM_Moist = RMF_Moist(RAIN, WATERLOSS, clay, depth, PC, SWC);
    }

    double RM_PC = RMF_PC(PC);

    // Combine RMF's into one.
    double RateM = RM_TMP * RM_Moist * RM_PC;

    decomp(timeFact, DPM, RPM, BIO, HUM, IOM, LAB, REC, SOC, co2, DPM_Rage, RPM_Rage, BIO_Rage, HUM_Rage, IOM_Rage, Total_Rage, modernC, RateM, clay, C_Inp, FYM_Inp, BIOCHAR_Inp, DPM_RPM);

    return;
}



std::vector<std::vector<double>> leggi_csv(const std::string& nome_file) {
    std::vector<std::vector<double>> dati;
    std::ifstream file(nome_file);

    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file: " << nome_file << std::endl;
        return dati;
    }

    std::string linea;
    std::getline(file, linea);

    while (std::getline(file, linea)) {
        std::vector<double> riga;
        std::istringstream ss(linea);
        std::string token;

        for (int i = 0; i < 11; ++i) {
            std::getline(ss, token, ',');
            try {
                riga.push_back(std::stod(token));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Errore nella conversione di un valore in double: " << token << std::endl;
            }
        }
        if (!riga.empty())
            dati.push_back(riga);
    }
    file.close();
    return dati;
}

void scrivi_csv(const std::string& nome_file, const std::vector<std::vector<double>>& dati) {
    std::ofstream file(nome_file);

    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file: " << nome_file << std::endl;
        return;
    }

    file << "patate,Year,Month,DPM_t_C_ha,RPM_t_C_ha,BIO_t_C_ha,HUM_t_C_ha,IOM_t_C_ha,BC_LAB_t_C_ha,BC_REC_t_C_ha,SOC_t_C_ha,deltaC,co2" << std::endl;

    for (const auto& riga : dati) {
        std::stringstream ss;
        for (double valore : riga) {
            ss << valore << ",";
        }
        ss.seekp(-1, std::ios::end);
        ss << std::endl;
        file << ss.str();
    }

    file.close();
}


int main()
{
    //set initial pool values
    double DPM = 0;
    double RPM = 0;
    double BIO = 0;
    double HUM = 0;
    double SOC = 0;

    double DPM_Rage = 0.0;
    double RPM_Rage = 0.0;
    double BIO_Rage = 0.0;
    double HUM_Rage = 0.0;
    double IOM_Rage = 50000.0;

    //set initial soil water content (deficit)
    double SWC = 0;
    double TOC1 = 0;

    //TODO: read in RothC input data file
    double clay = 13.0;     //[%]
    double depth = 25.0;    //[cm]
    double IOM = 3.0041;    //[t C/ha]
    int nsteps = 840;       //[-]

    //std::vector<std::vector<double>> data = createDataMatrix();
    std::vector<std::vector<double>> data = leggi_csv("C:/Github/rothCStandAlone/C_data_input.csv");

    int k = -1;
    int j = -1;

    SOC = DPM + RPM + BIO + HUM + IOM;

    std::cout << j << "," << DPM << ","<< RPM << ","<< BIO << ","<< HUM << ","<< IOM << ","<< SOC << "\n";

    int timeFact = 12;
    double Total_Rage;

    double TEMP;
    double RAIN;
    double PEVAP;
    bool isET0 = false;
    bool PC;
    double DPM_RPM;
    double C_Inp;
    double FYM_Inp;
    double modernC;
    double BIOCHAR_Inp;
    double co2 = 0;
    double LAB = 0;
    double REC = 0;

    double test = 100;
    while (test > 0.000001)
    {
        k = k+1;
        j = j+1;

        if (k == timeFact) k = 0;

        TEMP = data[k][3];
        RAIN = data[k][4];
        PEVAP = data[k][5];
        PC = bool(data[k][9]);
        DPM_RPM = data[k][10];
        C_Inp = data[k][6];
        FYM_Inp = data[k][7];
        BIOCHAR_Inp = data[k][8];
        modernC = data[k][2]/100;

        Total_Rage = 0;

        RothC(timeFact, DPM, RPM, BIO, HUM, IOM, LAB, REC, SOC, co2, DPM_Rage, RPM_Rage, BIO_Rage, HUM_Rage, IOM_Rage, Total_Rage, modernC, clay, depth, TEMP, RAIN, PEVAP, isET0, PC, DPM_RPM, C_Inp, FYM_Inp, BIOCHAR_Inp, SWC);

        if (((k+1)%timeFact) == 0)
        {
            double TOC0 = TOC1;
            TOC1 = DPM + RPM + BIO + HUM;
            test = fabs(TOC1-TOC0);
        }
    }

    double totalDelta = (std::exp(-Total_Rage/8035.0) - 1) * 1000;

    std::cout << j << "," << DPM << "," << RPM << "," << BIO << "," << HUM << "," << SOC << "," << totalDelta << "\n";

    std::vector<std::vector<double>> yearList;
//    std::vector<std::vector<double>> yearList = {{double(1), double(j+1), DPM, RPM, BIO, HUM, IOM, SOC, totalDelta}};


    std::vector<std::vector<double>> monthList;
    int timeFactIndex;

    for (int i = timeFact; i < nsteps; i++)
    {
        TEMP = data[i][3];
        RAIN = data[i][4];
        PEVAP = data[i][5];
        PC = bool(data[i][9]);
        DPM_RPM = data[i][10];
        C_Inp = data[i][6];
        FYM_Inp = data[i][7];
        BIOCHAR_Inp = data[i][8];
        modernC = data[i][2]/100;

        RothC(timeFact, DPM, RPM, BIO, HUM, IOM, LAB, REC, SOC, co2, DPM_Rage, RPM_Rage, BIO_Rage, HUM_Rage, IOM_Rage, Total_Rage, modernC, clay, depth, TEMP, RAIN, PEVAP, isET0, PC, DPM_RPM, C_Inp, FYM_Inp, BIOCHAR_Inp, SWC);

        totalDelta = (std::exp(-Total_Rage/8035.0) - 1.0) * 1000;

        //std::cout << C_Inp << "," << FYM_Inp << "," << TEMP << "," << RAIN << "," << PEVAP << "," << SWC << ","
                  //<< PC << "," << DPM <<"," << RPM <<"," << BIO <<"," << HUM <<"," << IOM <<"," << SOC << "\n";

        monthList.push_back({double(i-timeFact), double(data[i][0]), double(data[i][1]), DPM, RPM, BIO, HUM, IOM, LAB, REC, SOC, totalDelta, co2});

        if (int(data[i][1]) == timeFact)
        {
            timeFactIndex = int(i/timeFact);
            yearList.push_back({double(timeFactIndex), data[i][0], data[i][1], DPM, RPM, BIO, HUM, IOM, LAB, REC, SOC, totalDelta, co2});
            //std::cout << i << "," << DPM << "," << RPM << "," << BIO << "," << HUM << "," << IOM << "," << SOC << "," << totalDelta << "\n";
        }
    }

    scrivi_csv("C:/Github/rothCStandAlone/CMonthResults.csv", monthList);
    scrivi_csv("C:/Github/rothCStandAlone/CYearResults.csv", yearList);

    return 0;


}





