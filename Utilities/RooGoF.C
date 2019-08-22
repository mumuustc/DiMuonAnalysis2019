#include "RooGoF.h"
#include "TError.h"

#include <vector>

using RooStats::SamplingDistribution;

namespace RooFit {
    RooGoF::RooGoF(RooHist *hist, RooCurve *curve) : TObject() {
        _curve = curve;
        _dataB = hist;
        _dataU = NULL;
        _pdf = NULL;
        _cdf = NULL;
        _poi = NULL;
        _ndat = hist->GetN();
        _themin = hist->GetX()[0]-hist->GetEXlow()[0];
        _themax = hist->GetX()[_ndat-1]+hist->GetEXhigh()[_ndat-1];
        _min_binc = 0;
        _rebinObs = false;
        _NToys = 0;
        _doReFit = false;
        _sd_AD = NULL;
        _sd_KS = NULL;
    }

    RooGoF::RooGoF(RooDataSet *data, RooCurve *curve, const char* varname) : TObject() {
        _curve = curve;
        _dataB = NULL;
        _pdf = NULL;
        _cdf = NULL;
        _poi = NULL;
        _themin = -1e99;
        _themax = 1e99;
        _min_binc = 0;
        _rebinObs = false;
        _NToys = 0;
        _doReFit = false;
        _sd_AD = NULL;
        _sd_KS = NULL;

        // build the dataset
        _ndat = data->numEntries();
        _dataU = new double[_ndat];
        for (int i=0; i<_ndat; i++) {
            _dataU[i] = ((RooRealVar*) data->get(i)->find(varname))->getVal();
            // if (_dataU[i]<_themin) _themin = _dataU[i];
            // if (_dataU[i]>_themax) _themax = _dataU[i];
        }
    }

    RooGoF::RooGoF(RooDataSet *data, RooAbsPdf *pdf, RooRealVar *poi) : TObject() {
        _curve = NULL;
        _dataB = NULL;
        _pdf = pdf;
        _poi = poi;
        _themin = -1e99;
        _themax = 1e99;
        _min_binc = 0;
        _rebinObs = false;
        _NToys = 0;
        _doReFit = false;
        _sd_AD = NULL;
        _sd_KS = NULL;

        // build the dataset
        _ndat = data->numEntries();
        _dataU = new double[_ndat];
        for (int i=0; i<_ndat; i++) {
            _dataU[i] = ((RooRealVar*) data->get(i)->find(*poi))->getVal();
            // if (_dataU[i]<_themin) _themin = _dataU[i];
            // if (_dataU[i]>_themax) _themax = _dataU[i];
        }

        // create the cdf
        _cdf = _pdf->createCdf(*_poi);
    }

    double RooGoF::curve(double x) {
        if (!_curve) return 0;
        return _curve->Eval(x);
    }

    RooGoF::~RooGoF() {
        if (_dataU) delete[] _dataU;
        if (_cdf) delete _cdf;
    }

    double RooGoF::curve_cdf(double x) {
        if (!_cdf || !_poi) return 0;
        _poi->setVal(x);
        return _cdf->getVal();
    }

    void RooGoF::setRange(double xmin, double xmax) {
        _themin = xmin;
        _themax = xmax;
    }

    void RooGoF::setRebin(int min_bincontent, bool rebinObs) {
        _min_binc = min_bincontent;
        _rebinObs = rebinObs;
    }

    void RooGoF::setNtoys(int nToys, bool doReFit,
            const RooCmdArg &arg1, const RooCmdArg &arg2, const RooCmdArg &arg3, const RooCmdArg &arg4, 
            const RooCmdArg &arg5, const RooCmdArg &arg6, const RooCmdArg &arg7, const RooCmdArg &arg8) {
        _NToys = nToys;
        _doReFit = doReFit;
        _arg1 = arg1;
        _arg2 = arg2;
        _arg3 = arg3;
        _arg4 = arg4;
        _arg5 = arg5;
        _arg6 = arg6;
        _arg7 = arg7;
        _arg8 = arg8;
    }

    void RooGoF::setSamplingDist_AD(SamplingDistribution *sd) {_sd_AD = sd;}
    void RooGoF::setSamplingDist_KS(SamplingDistribution *sd) {_sd_KS = sd;}
    SamplingDistribution* RooGoF::getSamplingDist_AD() {return _sd_AD;}
    SamplingDistribution* RooGoF::getSamplingDist_KS() {return _sd_KS;}

    void RooGoF::unbinnedTest(double &pvalue, double &testStat, TSmode mode) {
        pvalue=0;
        testStat=1e99;
        if (!_dataU) return;
        if (!(mode==AD || mode==KS)) return;

        // here there are many errors like:
        // Error in <GSLError>: Error 18 in qags.c at 548 : cannot reach tolerance because of roundoff error
        // ... suppress them
        int olderrors = gErrorIgnoreLevel;
        gErrorIgnoreLevel = kBreak;

        ROOT::Math::Functor1D *f = NULL;
        ROOT::Math::GoFTest* goftest = NULL;

        if (_curve) {
            f = new ROOT::Math::Functor1D(this, &RooGoF::curve);
            goftest = new ROOT::Math::GoFTest(_ndat, _dataU, *f,  ROOT::Math::GoFTest::kPDF, _themin, _themax);  // need to specify am interval
        } else if (_cdf) {
            f = new ROOT::Math::Functor1D(this, &RooGoF::curve_cdf);
            goftest = new ROOT::Math::GoFTest(_ndat, _dataU, *f,  ROOT::Math::GoFTest::kCDF, _themin, _themax);  // need to specify am interval
        } else { // if neither _curve nor _cdf was found
            gErrorIgnoreLevel = olderrors;
            return;
        }

        if (mode==AD) goftest->AndersonDarlingTest(pvalue, testStat);
        else if (mode==KS) goftest->KolmogorovSmirnovTest(pvalue, testStat);

        if (_NToys>0 && (!_sd_AD || !_sd_KS)) generateSamplingDist();
        if (mode==AD && _sd_AD) pvalue = 1.-_sd_AD->CDF(testStat);
        if (mode==KS && _sd_KS) pvalue = 1.-_sd_KS->CDF(testStat);

        gErrorIgnoreLevel = olderrors;

        if (goftest) delete goftest;
        if (f) delete f;
    }

    void RooGoF::binnedTest(double &pvalue, double &testStat, int &ndf, TSmode mode, int d_ndf) {
        pvalue=0;
        testStat=1e99;
        if (!_dataB) return;
        if (mode==AD || mode==KS) return;

        if (mode==RooFitChi2) {
            Int_t i ;
            Int_t nbin(0) ;
            for (i=0 ; i<_ndat ; i++) { if (_dataB->GetY()[i]>0) { nbin++; } } // internally, RooCurve::chiSquare() removes empty bins
            ndf = nbin-d_ndf;
            testStat = ndf*_curve->chiSquare(*_dataB,d_ndf);
            // cout << nbin << " " << d_ndf << " " << _curve->chiSquare(*_dataB,d_ndf) << endl;
            pvalue = TMath::Prob(testStat,ndf);
            return;
        }

        testStat=0;
        int nbin=0;

        // simple case: no rebinning
        if (_min_binc<=0) {
            for (int i=0; i<_ndat; i++) {
                double x = _dataB->GetX()[i];
                if (x<_themin || x>_themax) continue;

                double dd = _dataB->GetY()[i];
                if (dd<0 || (dd==0 && mode==NeymanChi2)) { // exclude empty bins for the Neyman chi2
                    coutW(Fitting) << "RooGoF::binnedTest: empty bin " << i << "! Consider rebinning." << endl; 
                    continue;
                }

                double binmin = x-_dataB->GetEXlow()[i];
                double binmax = x+_dataB->GetEXhigh()[i];
                double ff = _curve->average(binmin, binmax);
                if (ff<0 || ((mode==BCChi2 || mode==PearsonChi2) && ff==0)) { // exclude bins with 0 expectation for Pearson and B-C chi2
                    coutW(Fitting) << "RooGoF::binnedTest:  negative or null function in bin " << i << "!" << endl; 
                    continue;
                }

                if (mode==BCChi2) testStat += ff - dd + (dd>0.0 ? dd*log(dd/ff) : 0.0);
                else if (mode==PearsonChi2) testStat += pow(dd-ff,2)/ff;
                else if (mode==NeymanChi2) testStat += pow(dd-ff,2)/dd;

                nbin++; // exclude empty bins
            } // loop on bins
        } else {
            double dd=0, ff=0; // this will hold the data and function in the current (merged) bin
            double ddp=-1, ffp=-1; // data and function in the previous (merged) bin
            double dchi2=-1; // previous addition to the chi2

            for (int i=0; i<_ndat; i++) {
                double x = _dataB->GetX()[i];
                if (x<_themin || x>_themax) continue;

                dd += _dataB->GetY()[i];

                double binmin = x-_dataB->GetEXlow()[i];
                double binmax = x+_dataB->GetEXhigh()[i];
                ff += _curve->average(binmin, binmax);

                if (!_rebinObs && ff<_min_binc) continue; // loop until we reach the minimum number of expected events
                if (_rebinObs && dd<_min_binc) continue; // loop until we reach the minimum number of observed events

                if (dd<0 || (dd==0 && mode==NeymanChi2)) { // exclude empty bins for the Neyman chi2
                    coutW(Fitting) << "RooGoF::binnedTest: empty bin " << i << "! Consider rebinning more." << endl; 
                    continue;
                }
                if (ff<0 || ((mode==BCChi2 || mode==PearsonChi2) && ff==0)) { // exclude bins with 0 expectation for Pearson and B-C chi2
                    coutW(Fitting) << "RooGoF::binnedTest:  negative or null function in bin " << i << "!" << endl; 
                    continue;
                }

                if (ddp>=0 || ffp>=0) { // if either of ddp or ffp is positive, we are not in the first bin
                    if (mode==BCChi2) dchi2 = ffp - ddp + (ddp>0.0 ? ddp*log(ddp/ffp) : 0.0);
                    else if (mode==PearsonChi2) dchi2 = pow(ddp-ffp,2)/ffp;
                    else if (mode==NeymanChi2) dchi2 = pow(ddp-ffp,2)/ddp;

                    testStat += dchi2;
                    nbin++;
                }

                ddp=dd; ffp=ff;
                dd=0;   ff=0;
            }

            // take care of the last bin
            if (dd>0 || ff>0) { 
                // We filled a last bin but did not finish.
                // Merge it with the next to last bin, and undo the last chi2 iteration.
                nbin--;
                testStat -= dchi2;
                ddp+=dd; ffp+=ff;
            }

            if (mode==BCChi2 && ffp>0) dchi2 = ffp - ddp + (ddp>0.0 ? ddp*log(ddp/ffp) : 0.0);
            else if (mode==PearsonChi2 && ffp>0) dchi2 = pow(ddp-ffp,2)/ffp;
            else if (mode==NeymanChi2 && ddp>0) dchi2 = pow(ddp-ffp,2)/ddp;

            testStat += dchi2;
            nbin++;
        } // if (rebin)

        if (mode==BCChi2) testStat *= 2.;

        pvalue = TMath::Prob(testStat,nbin-d_ndf);
        // pvalue = TMath::Prob(testStat,_ndat-d_ndf);
        ndf = nbin-d_ndf;
    }

    void RooGoF::KSTest(double &pvalue, double &testStat) {unbinnedTest(pvalue,testStat,TSmode::KS);}
    void RooGoF::ADTest(double &pvalue, double &testStat) {unbinnedTest(pvalue,testStat,TSmode::AD);}
    void RooGoF::BCChi2Test(double &pvalue, double &testStat, int &ndf, int d_ndf) {binnedTest(pvalue,testStat,ndf,TSmode::BCChi2,d_ndf);}
    void RooGoF::PearsonChi2Test(double &pvalue, double &testStat, int &ndf, int d_ndf) {binnedTest(pvalue,testStat,ndf,TSmode::PearsonChi2,d_ndf);}
    void RooGoF::NeymanChi2Test(double &pvalue, double &testStat, int &ndf, int d_ndf) {binnedTest(pvalue,testStat,ndf,TSmode::NeymanChi2,d_ndf);}
    void RooGoF::RooFitChi2Test(double &pvalue, double &testStat, int &ndf, int d_ndf) {binnedTest(pvalue,testStat,ndf,TSmode::RooFitChi2,d_ndf);}

    void RooGoF::generateSamplingDist() {
        if (!_cdf) return;

        // save best fit parameters
        RooArgSet* params = _pdf->getParameters(*_poi) ;
        RooArgSet* bestFitParams = (RooArgSet*) params->snapshot() ;

        // silence RooFit output during toys
        coutI(Fitting) << "RooGoF::generateSamplingDist(): generating " << _NToys << " toys..." << endl;
        RooFit::MsgLevel oldLevel = RooMsgService::instance().globalKillBelow() ;
        RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL) ;
        RooMsgService::instance().setSilentMode(true) ;

        RooAbsReal *cdfold = _cdf;

        vector<double> v_AD, v_KS;
        for (int i=0; i<_NToys; i++) {
            // go back to initial parameters
            *params = *bestFitParams;

            // generate pseudo-dataset
            RooDataSet *datatoy = _pdf->generate(*_poi,_ndat) ;
            double *toy_i = new double[_ndat];
            for (int j=0; j<_ndat; j++) {
                toy_i[j] = ((RooRealVar*) datatoy->get(j)->find(*_poi))->getVal();
            }

            // do the fit
            if (_doReFit) _pdf->fitTo(*datatoy,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8) ;

            _cdf = _pdf->createCdf(*_poi);
            ROOT::Math::Functor1D *f = new ROOT::Math::Functor1D(this, &RooGoF::curve_cdf);
            ROOT::Math::GoFTest *goftest = new ROOT::Math::GoFTest(_ndat, toy_i, *f,  ROOT::Math::GoFTest::kCDF, _themin, _themax);  // need to specify am interval
            double ts,tmp;
            goftest->AndersonDarlingTest(tmp, ts);
            v_AD.push_back(ts);
            goftest->KolmogorovSmirnovTest(tmp,ts);
            v_KS.push_back(ts);

            // clean up
            delete f; f=NULL;
            delete goftest; goftest=NULL;
            delete datatoy;
            delete _cdf;
            delete[] toy_i;
        }

        if (!_sd_AD) _sd_AD = new SamplingDistribution("sd_AD","",v_AD);
        if (!_sd_KS) _sd_KS = new SamplingDistribution("sd_KS","",v_KS);

        RooMsgService::instance().setGlobalKillBelow(oldLevel) ;
        *params = *bestFitParams;
        _cdf = cdfold;
    }
}
