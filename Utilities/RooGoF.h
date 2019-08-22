#include "RooHist.h"
#include "RooCurve.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooCmdArg.h"

#include "Math/GoFTest.h"
#include "Math/Functor.h"
#include "Math/Integrator.h"
#include "Math/IntegratorMultiDim.h"
#include "Math/AllIntegrationTypes.h"
#include "Math/GaussIntegrator.h"
#include "RooStats/SamplingDistribution.h"

using RooStats::SamplingDistribution;

namespace RooFit {
    class RooGoF : public TObject {
        public:
            RooGoF(RooHist *hist, RooCurve *curve);
            RooGoF(RooDataSet *data, RooCurve *curve, const char* varname);
            RooGoF(RooDataSet *data, RooAbsPdf *pdf, RooRealVar *poi);
            ~RooGoF();

            double curve(double x);
            double curve_cdf(double x);

            // set the range for the GoF test
            void setRange(double xmin, double xmax);

            // rebinning mode (default = none)
            void setRebin(int min_bincontent = 5, bool rebinObs = false); // if rebinObs = true, the minimum bin content is applied to observed counts instead of expected

            // toys
            void setNtoys(int nToys, bool doReFit=true,
                    const RooCmdArg &arg1=RooCmdArg::none(), const RooCmdArg &arg2=RooCmdArg::none(), 
                    const RooCmdArg &arg3=RooCmdArg::none(), const RooCmdArg &arg4=RooCmdArg::none(), 
                    const RooCmdArg &arg5=RooCmdArg::none(), const RooCmdArg &arg6=RooCmdArg::none(), 
                    const RooCmdArg &arg7=RooCmdArg::none(), const RooCmdArg &arg8=RooCmdArg::none());
            void setSamplingDist_AD(SamplingDistribution *sd);
            void setSamplingDist_KS(SamplingDistribution *sd);
            SamplingDistribution* getSamplingDist_AD();
            SamplingDistribution* getSamplingDist_KS();

            // GoF test functions
            void KSTest(double &pvalue, double &testStat);
            void ADTest(double &pvalue, double &testStat);
            void BCChi2Test(double &pvalue, double &testStat, int &ndf, int d_ndf=0);
            void PearsonChi2Test(double &pvalue, double &testStat, int &ndf, int d_ndf=0);
            void NeymanChi2Test(double &pvalue, double &testStat, int &ndf, int d_ndf=0);
            void RooFitChi2Test(double &pvalue, double &testStat, int &ndf, int d_ndf=0);

            // enum
            enum TSmode {KS, AD, BCChi2, PearsonChi2, NeymanChi2, RooFitChi2};

        private:
            double     *_dataU;
            RooHist    *_dataB;
            int        _ndat;
            RooCurve   *_curve;
            RooAbsPdf  *_pdf;
            RooAbsReal *_cdf;
            RooRealVar *_poi;
            double     _themin;
            double     _themax;
            int        _min_binc;
            bool       _rebinObs;
            bool       _doReFit;
            int        _NToys;
            RooCmdArg  _arg1;
            RooCmdArg  _arg2;
            RooCmdArg  _arg3;
            RooCmdArg  _arg4;
            RooCmdArg  _arg5;
            RooCmdArg  _arg6;
            RooCmdArg  _arg7;
            RooCmdArg  _arg8;
            SamplingDistribution *_sd_AD;
            SamplingDistribution *_sd_KS;

            // private functions
            void unbinnedTest(double &pvalue, double &testStat, TSmode mode);
            void binnedTest(double &pvalue, double &testStat, int &ndf, TSmode mode, int d_ndf);
            void generateSamplingDist();
    };
}
