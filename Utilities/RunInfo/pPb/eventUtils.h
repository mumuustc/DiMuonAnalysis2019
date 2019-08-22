#ifndef RunInfo_pPb_eventUtils_H_
#define RunInfo_pPb_eventUtils_H_


#include "Run8TeV/eventUtils.h"


namespace pPb {
    //
    void roundValue(double& value , const uint& nDecimals)
    {
        double tmp = value;
        tmp *= std::pow(10.0, nDecimals);
        tmp = std::round(tmp);
        tmp /= std::pow(10.0, nDecimals);
        value = tmp;
    };
    //
    // Centre of Mass Frames
    //
    double EtaLABtoCM(const double& etaLAB, const bool ispPb)
    {
        const double& shift = ( ispPb ? 0.465 : -0.465 );
        double etaCM = etaLAB - shift;
        roundValue(etaCM, 4);
        return etaCM;
    };
    //
    double EtaCMtoLAB(const double& etaCM, const bool ispPb)
    {
        const double& shift = ( ispPb ? 0.465 : -0.465 );
        double etaLAB = etaCM + shift;
        roundValue(etaLAB, 4);
        return etaLAB;
    };
    //
    double RapLABtoCM(const double& rapLAB, const bool ispPb) { return EtaLABtoCM(rapLAB, ispPb); };
    //
    double RapCMtoLAB(const double& rapCM , const bool ispPb) { return EtaCMtoLAB(rapCM, ispPb);  };
};


#endif /* RunInfo_pPb_eventUtils_H_ */
