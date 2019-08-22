#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <memory>


TTree* skimTree(TTree* told, std::vector<int>& evtV);


void skimVertexCompositeTree(const std::string& filein, const std::string& fileout)
{
    auto fin = std::unique_ptr<TFile>(TFile::Open(filein.c_str(), "READ"));
    auto fout = std::unique_ptr<TFile>(TFile::Open(fileout.c_str(),"RECREATE"));
    if (!fin || !fin->IsOpen() || fin->IsZombie()) return;
    if (!fout || !fout->IsOpen() || fout->IsZombie()) return;

    fout->cd();
    const auto& tdir = fout->mkdir("dimucontana");
    tdir->cd();
    std::vector<int> evtVec;
    const auto& tree = skimTree(dynamic_cast<TTree*>(fin->Get("dimucontana/VertexCompositeNtuple")), evtVec);

    fout->cd();
    const auto& tdir_ws = fout->mkdir("dimucontana_wrongsign");
    tdir_ws->cd();
    const auto& tree_ws = skimTree(dynamic_cast<TTree*>(fin->Get("dimucontana_wrongsign/VertexCompositeNtuple")), evtVec);

    fout->Write();
    fout->Close();
    fin->Close();
};


TTree* skimTree(TTree* told, std::vector<int>& evtV)
{
    const auto& tnew = told->CloneTree(0);
    tnew->SetAutoSave(0);
    tnew->SetAutoFlush(0);
    uint candSize;
    float mass[500];
    bool evtSel[10], trigHLT[20], softMuon1[500], softMuon2[500];

    told->SetBranchAddress("candSize",&candSize);
    told->SetBranchAddress("mass",&mass);
    told->SetBranchAddress("evtSel",&evtSel);
    told->SetBranchAddress("softMuon1",&softMuon1);
    told->SetBranchAddress("softMuon2",&softMuon2);

    const auto& nentries = told->GetEntriesFast();
    const bool& fillV = (int(evtV.size())<nentries);
    if (fillV) {
        evtV.clear(); evtV.reserve(nentries);
        for (int i=0; i<nentries; i++) {
            told->GetEntry(i);
            bool hasGoodCandidate = false;
            for (uint j=0; j<candSize; j++) {
                if ((mass[j]>2.1 && mass[j]<4.8) && (softMuon1[j] && softMuon2[j])) { hasGoodCandidate = true; break; }
            }
            const bool& keepEvt = (hasGoodCandidate && evtSel[0]);
            evtV.push_back(keepEvt);
            if (keepEvt) tnew->Fill();
        }
    }
    else {
        for (int i=0; i<nentries; i++) { if (evtV[i]) { told->GetEntry(i); tnew->Fill(); } }
    }
    return tnew;
};
