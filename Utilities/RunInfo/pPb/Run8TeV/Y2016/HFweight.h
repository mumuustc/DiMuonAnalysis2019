#ifndef HFweight_h
#define HFweight_h

#include "TFile.h"
#include "TH1.h"

#include <iostream>

using namespace std;

class HFweight {
    public:
        enum HFside {
            both,
            plus,
            minus,
            track
        };
        HFweight(const char* weightfile = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/HFweight.root");
        ~HFweight();
        double weight(double hiHF, HFside side=both, bool isPV1=false);

    private:
        const char* weightfile = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/HFweight.root";
        TFile *f;
        TH1F* h_hiHF_ratio;
        TH1F* h_hiHFplus_ratio;
        TH1F* h_hiHFminus_ratio;
        TH1F* h_hiNtracks_ratio;
        TH1F* h_hiHF_PV1_ratio;
        TH1F* h_hiHFplus_PV1_ratio;
        TH1F* h_hiHFminus_PV1_ratio;
        TH1F* h_hiNtracks_PV1_ratio;
};

HFweight::HFweight(const char* weightfile) {
    f = new TFile(weightfile);
    if (!f || !f->IsOpen()) {
        cout << "Error! could not find HF weight file " << weightfile << endl;
        return;
    } else {
        cout << "Opened " << weightfile << " for HF weights." << endl;
    }

    h_hiHF_ratio = (TH1F*) f->Get("h_hiHF_ratio");
    h_hiHFplus_ratio = (TH1F*) f->Get("h_hiHFplus_ratio");
    h_hiHFminus_ratio = (TH1F*) f->Get("h_hiHFminus_ratio");
    h_hiNtracks_ratio = (TH1F*) f->Get("h_hiNtracks_ratio");
    h_hiHF_PV1_ratio = (TH1F*) f->Get("h_hiHF_PV1_ratio");
    h_hiHFplus_PV1_ratio = (TH1F*) f->Get("h_hiHFplus_PV1_ratio");
    h_hiHFminus_PV1_ratio = (TH1F*) f->Get("h_hiHFminus_PV1_ratio");
    h_hiNtracks_PV1_ratio = (TH1F*) f->Get("h_hiNtracks_PV1_ratio");
}

HFweight::~HFweight() {
    f->Close();
    if (f) {
        delete f;
        f=0;
    }
}

double HFweight::weight(double hiHF, HFweight::HFside side, bool isPV1) {
    TH1F *hist;
    if (side==both) hist = isPV1 ? h_hiHF_PV1_ratio : h_hiHF_ratio;
    else if (side==plus) hist = isPV1 ? h_hiHFplus_PV1_ratio : h_hiHFplus_ratio;
    else if (side==minus) hist = isPV1 ? h_hiHFminus_PV1_ratio : h_hiHFminus_ratio;
    else if (side==track) hist = isPV1 ? h_hiNtracks_PV1_ratio : h_hiNtracks_ratio;
    else {
        cout << "Error, requesting unknown HF configuration!" << endl;
        return 1;
    }

    if (!hist) {
        cout << "Error, missing weight histo" << endl;
        return 1;
    }

    return std::min(7.0 , hist->GetBinContent(hist->FindBin(hiHF)));
}

#endif // #ifndef HFweight_h
