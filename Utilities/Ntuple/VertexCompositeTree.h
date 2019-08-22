#ifndef VertexCompositeTree_h
#define VertexCompositeTree_h

// Header file for ROOT classes
#include <TROOT.h>
#include <TChain.h>
#include <TInterpreter.h>
#include <TDirectory.h>
#include <TSystem.h>
#include <TFile.h>
#include <TVector3.h>

// Header file for c++ classes
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

// Header file for the classes stored in the TChain


typedef std::vector< std::vector<UChar_t> > UCharVecVec;
static const UInt_t NCAND = 1000;
static const UInt_t NGEN  = 10;


class VertexCompositeTree {

    public :

        VertexCompositeTree();
        virtual ~VertexCompositeTree();
        virtual Bool_t       GetTree         (const std::vector< std::string >&, const std::string& treeName="dimucontana");
        virtual Bool_t       GetTree         (const std::string&, const std::string& treeName="dimucontana");
        virtual Int_t        GetEntry        (Long64_t);
        virtual Long64_t     GetEntries      (void) const { return (fChain_ ? fChain_->GetEntries() : -1); }
        virtual Long64_t     GetTreeEntries  (void) const { return ((fChain_ && fChain_->GetTree()) ? fChain_->GetTree()->GetEntriesFast() : -1); }
        virtual Int_t        GetTreeNumber   (void) const { return fCurrent_; }
        virtual void         Clear           (void);

        // EVENT INFO GETTERS
        UInt_t    RunNb()                       { SetBranch("RunNb");                       return RunNb_;                      }
        UInt_t    LSNb()                        { SetBranch("LSNb");                        return LSNb_;                       }
        UInt_t    EventNb()                     { SetBranch("EventNb");                     return EventNb_;                    }
        Short_t   nPV()                         { SetBranch("nPV");                         return nPV_;                        }
        Float_t   bestvtxX()                    { SetBranch("bestvtxX");                    return bestvtxX_;                   }
        Float_t   bestvtxY()                    { SetBranch("bestvtxY");                    return bestvtxY_;                   }
        Float_t   bestvtxZ()                    { SetBranch("bestvtxZ");                    return bestvtxZ_;                   }
        Short_t   centrality()                  { SetBranch("centrality");                  return centrality_;                 }
        Int_t     Npixel()                      { SetBranch("Npixel");                      return Npixel_;                     }
        Float_t   HFsumETPlus()                 { SetBranch("HFsumETPlus");                 return HFsumETPlus_;                }
        Float_t   HFsumETMinus()                { SetBranch("HFsumETMinus");                return HFsumETMinus_;               }
        Float_t   ZDCPlus()                     { SetBranch("ZDCPlus");                     return ZDCPlus_;                    }
        Float_t   ZDCMinus()                    { SetBranch("ZDCMinus");                    return ZDCMinus_;                   }
        Int_t     Ntrkoffline()                 { SetBranch("Ntrkoffline");                 return Ntrkoffline_;                }
        Short_t*  trigPrescale()                { SetBranch("trigPrescale");                return trigPrescale_;               }
        Bool_t*   trigHLT()                     { SetBranch("trigHLT");                     return trigHLT_;                    }
        Bool_t*   evtSel()                      { SetBranch("evtSel");                      return evtSel_;                     }

        // EVENT PLANE GETTERS
        Float_t   ephfpSumW()                   { SetBranch("ephfpSumW");                   return ephfpSumW_;                  }
        Float_t*  ephfpAngle()                  { SetBranch("ephfpAngle");                  return ephfpAngle_;                 }
        Float_t*  ephfpQ()                      { SetBranch("ephfpQ");                      return ephfpQ_;                     }
        Float_t   ephfmSumW()                   { SetBranch("ephfmSumW");                   return ephfmSumW_;                  }
        Float_t*  ephfmAngle()                  { SetBranch("ephfmAngle");                  return ephfmAngle_;                 }
        Float_t*  ephfmQ()                      { SetBranch("ephfmQ");                      return ephfmQ_;                     }

        // CANDIDATE INFO GETTERS
        UInt_t    candSize()                    { SetBranch("candSize");                    return candSize_;                   }
        Float_t*  pT()                          { SetBranch("pT");                          return pT_;                         }
        Float_t*  eta()                         { SetBranch("eta");                         return eta_;                        }
        Float_t*  y()                           { SetBranch("y");                           return y_;                          }
        Float_t*  phi()                         { SetBranch("phi");                         return phi_;                        }
        Float_t*  mass()                        { SetBranch("mass");                        return mass_;                       }
        Float_t*  flavor()                      { SetBranch("flavor");                      return flavor_;                     }
        Float_t*  VtxProb()                     { SetBranch("VtxProb");                     return VtxProb_;                    }
        Float_t*  V3DCosPointingAngle()         { SetBranch("3DCosPointingAngle");          return V3DCosPointingAngle_;        }
        Float_t*  V3DPointingAngle()            { SetBranch("3DPointingAngle");             return V3DPointingAngle_;           }
        Float_t*  V2DCosPointingAngle()         { SetBranch("2DCosPointingAngle");          return V2DCosPointingAngle_;        }
        Float_t*  V2DPointingAngle()            { SetBranch("2DPointingAngle");             return V2DPointingAngle_;           }
        Float_t*  V3DDecayLengthSignificance()  { SetBranch("3DDecayLengthSignificance");   return V3DDecayLengthSignificance_; }
        Float_t*  V3DDecayLength()              { SetBranch("3DDecayLength");               return V3DDecayLength_;             }
        Float_t*  V3DDecayLengthError()         { SetBranch("3DDecayLengthError");          return V3DDecayLengthError_;        }
        Float_t*  V2DDecayLengthSignificance()  { SetBranch("2DDecayLengthSignificance");   return V2DDecayLengthSignificance_; }
        Float_t*  V2DDecayLength()              { SetBranch("2DDecayLength");               return V2DDecayLength_;             }
        Float_t*  zDCASignificanceDaugther1()   { SetBranch("zDCASignificanceDaugther1");   return zDCASignificanceDaugther1_;  }
        Float_t*  xyDCASignificanceDaugther1()  { SetBranch("xyDCASignificanceDaugther1");  return xyDCASignificanceDaugther1_; }
        Bool_t*   HighPurityDaugther1()         { SetBranch("HighPurityDaugther1");         return HighPurityDaugther1_;        }
        Float_t*  NHitD1()                      { SetBranch("NHitD1");                      return NHitD1_;                     }
        Float_t*  pTD1()                        { SetBranch("pTD1");                        return pTD1_;                       }
        Float_t*  pTerrD1()                     { SetBranch("pTerrD1");                     return pTerrD1_;                    }
        Float_t*  EtaD1()                       { SetBranch("EtaD1");                       return EtaD1_;                      }
        Float_t*  PhiD1()                       { SetBranch("PhiD1");                       return PhiD1_;                      }
        Short_t*  chargeD1()                    { SetBranch("chargeD1");                    return chargeD1_;                   }
        Float_t*  dedxHarmonic2D1()             { SetBranch("dedxHarmonic2D1");             return dedxHarmonic2D1_;            }
        Float_t*  zDCASignificanceDaugther2()   { SetBranch("zDCASignificanceDaugther2");   return zDCASignificanceDaugther2_;  }
        Float_t*  xyDCASignificanceDaugther2()  { SetBranch("xyDCASignificanceDaugther2");  return xyDCASignificanceDaugther2_; }
        Bool_t*   HighPurityDaugther2()         { SetBranch("HighPurityDaugther2");         return HighPurityDaugther2_;        }
        Float_t*  NHitD2()                      { SetBranch("NHitD2");                      return NHitD2_;                     }
        Float_t*  pTD2()                        { SetBranch("pTD2");                        return pTD2_;                       }
        Float_t*  pTerrD2()                     { SetBranch("pTerrD2");                     return pTerrD2_;                    }
        Float_t*  EtaD2()                       { SetBranch("EtaD2");                       return EtaD2_;                      }
        Float_t*  PhiD2()                       { SetBranch("PhiD2");                       return PhiD2_;                      }
        Short_t*  chargeD2()                    { SetBranch("chargeD2");                    return chargeD2_;                   }
        Float_t*  dedxHarmonic2D2()             { SetBranch("dedxHarmonic2D2");             return dedxHarmonic2D2_;            }
        Bool_t*   isSwap()                      { SetBranch("isSwap");                      return isSwap_;                     }
        Int_t*    idmom_reco()                  { SetBranch("idmom_reco");                  return idmom_reco_;                 }
        Bool_t*   matchGEN()                    { SetBranch("matchGEN");                    return matchGEN_;                   }
        Int_t*    PIDD1()                       { SetBranch("PIDD1");                       return PIDD1_;                      }
        Int_t*    PIDD2()                       { SetBranch("PIDD2");                       return PIDD2_;                      }

        // MUON INFO GETTERS
        Bool_t*   OneStMuon1()                  { SetBranch("OneStMuon1");                  return OneStMuon1_;                 }
        Bool_t*   PFMuon1()                     { SetBranch("PFMuon1");                     return PFMuon1_;                    }
        Bool_t*   GlbMuon1()                    { SetBranch("GlbMuon1");                    return GlbMuon1_;                   }
        Bool_t*   trkMuon1()                    { SetBranch("trkMuon1");                    return trkMuon1_;                   }
        Bool_t*   tightMuon1()                  { SetBranch("tightMuon1");                  return tightMuon1_;                 }
        Bool_t*   softMuon1()                   { SetBranch("softMuon1");                   return softMuon1_;                  }
        Bool_t*   hybridMuon1()                 { SetBranch("hybridMuon1");                 return hybridMuon1_;                }
        Bool_t*   HPMuon1()                     { SetBranch("HPMuon1");                     return HPMuon1_;                    }
        UCharVecVec trigMuon1()                 { SetBranch("trigMuon1");                   return GET(trigMuon1_);             }
        Short_t*  nMatchedStationD1()           { SetBranch("nMatchedStationD1");           return nMatchedStationD1_;          }
        Short_t*  nTrackerLayerD1()             { SetBranch("nTrackerLayerD1");             return nTrackerLayerD1_;            }
        Short_t*  nPixelLayerD1()               { SetBranch("nPixelLayerD1");               return nPixelLayerD1_;              }
        Short_t*  nPixelHitD1()                 { SetBranch("nPixelHitD1");                 return nPixelHitD1_;                }
        Short_t*  nMuonHitD1()                  { SetBranch("nMuonHitD1");                  return nMuonHitD1_;                 }
        Float_t*  GlbTrkChiD1()                 { SetBranch("GlbTrkChiD1");                 return GlbTrkChiD1_;                }
        Float_t*  muondXYD1()                   { SetBranch("muondXYD1");                   return muondXYD1_;                  }
        Float_t*  muondZD1()                    { SetBranch("muondZD1");                    return muondZD1_;                   }
        Float_t*  dXYD1()                       { SetBranch("dXYD1");                       return dXYD1_;                      }
        Float_t*  dZD1()                        { SetBranch("dZD1");                        return dZD1_;                       }
        Short_t*  nMatchedChamberD1()           { SetBranch("nMatchedChamberD1");           return nMatchedChamberD1_;          }
        Float_t*  EnergyDepositionD1()          { SetBranch("EnergyDepositionD1");          return EnergyDepositionD1_;         }
        Float_t*  dx1_seg()                     { SetBranch("dx1_seg");                     return dx1_seg_;                    }
        Float_t*  dy1_seg()                     { SetBranch("dy1_seg");                     return dy1_seg_;                    }
        Float_t*  dxSig1_seg()                  { SetBranch("dxSig1_seg");                  return dxSig1_seg_;                 }
        Float_t*  dySig1_seg()                  { SetBranch("dySig1_seg");                  return dySig1_seg_;                 }
        Float_t*  ddxdz1_seg()                  { SetBranch("ddxdz1_seg");                  return ddxdz1_seg_;                 }
        Float_t*  ddydz1_seg()                  { SetBranch("ddydz1_seg");                  return ddydz1_seg_;                 }
        Float_t*  ddxdzSig1_seg()               { SetBranch("ddxdzSig1_seg");               return ddxdzSig1_seg_;              }
        Float_t*  ddydzSig1_seg()               { SetBranch("ddydzSig1_seg");               return ddydzSig1_seg_;              }
        Bool_t*   OneStMuon2()                  { SetBranch("OneStMuon2");                  return OneStMuon2_;                 }
        Bool_t*   PFMuon2()                     { SetBranch("PFMuon2");                     return PFMuon2_;                    }
        Bool_t*   GlbMuon2()                    { SetBranch("GlbMuon2");                    return GlbMuon2_;                   }
        Bool_t*   trkMuon2()                    { SetBranch("trkMuon2");                    return trkMuon2_;                   }
        Bool_t*   tightMuon2()                  { SetBranch("tightMuon2");                  return tightMuon2_;                 }
        Bool_t*   softMuon2()                   { SetBranch("softMuon2");                   return softMuon2_;                  }
        Bool_t*   hybridMuon2()                 { SetBranch("hybridMuon2");                 return hybridMuon2_;                }
        Bool_t*   HPMuon2()                     { SetBranch("HPMuon2");                     return HPMuon2_;                    }
        UCharVecVec trigMuon2()                 { SetBranch("trigMuon2");                   return GET(trigMuon2_);             }
        Short_t*  nMatchedStationD2()           { SetBranch("nMatchedStationD2");           return nMatchedStationD2_;          }
        Short_t*  nTrackerLayerD2()             { SetBranch("nTrackerLayerD2");             return nTrackerLayerD2_;            }
        Short_t*  nPixelLayerD2()               { SetBranch("nPixelLayerD2");               return nPixelLayerD2_;              }
        Short_t*  nPixelHitD2()                 { SetBranch("nPixelHitD2");                 return nPixelHitD2_;                }
        Short_t*  nMuonHitD2()                  { SetBranch("nMuonHitD2");                  return nMuonHitD2_;                 }
        Float_t*  GlbTrkChiD2()                 { SetBranch("GlbTrkChiD2");                 return GlbTrkChiD2_;                }
        Float_t*  muondXYD2()                   { SetBranch("muondXYD2");                   return muondXYD2_;                  }
        Float_t*  muondZD2()                    { SetBranch("muondZD2");                    return muondZD2_;                   }
        Float_t*  dXYD2()                       { SetBranch("dXYD2");                       return dXYD2_;                      }
        Float_t*  dZD2()                        { SetBranch("dZD2");                        return dZD2_;                       }
        Short_t*  nMatchedChamberD2()           { SetBranch("nMatchedChamberD2");           return nMatchedChamberD2_;          }
        Float_t*  EnergyDepositionD2()          { SetBranch("EnergyDepositionD2");          return EnergyDepositionD2_;         }
        Float_t*  dx2_seg()                     { SetBranch("dx2_seg");                     return dx2_seg_;                    }
        Float_t*  dy2_seg()                     { SetBranch("dy2_seg");                     return dy2_seg_;                    }
        Float_t*  dxSig2_seg()                  { SetBranch("dxSig2_seg");                  return dxSig2_seg_;                 }
        Float_t*  dySig2_seg()                  { SetBranch("dySig2_seg");                  return dySig2_seg_;                 }
        Float_t*  ddxdz2_seg()                  { SetBranch("ddxdz2_seg");                  return ddxdz2_seg_;                 }
        Float_t*  ddydz2_seg()                  { SetBranch("ddydz2_seg");                  return ddydz2_seg_;                 }
        Float_t*  ddxdzSig2_seg()               { SetBranch("ddxdzSig2_seg");               return ddxdzSig2_seg_;              }
        Float_t*  ddydzSig2_seg()               { SetBranch("ddydzSig2_seg");               return ddydzSig2_seg_;              }

        // GEN INFO GETTERS
        Float_t   weight_gen()                  { SetBranch("weight_gen");                  return weight_gen_;                 }
        UInt_t    candSize_gen()                { SetBranch("candSize_gen");                return candSize_gen_;               }
        Float_t*  pT_gen()                      { SetBranch("pT_gen");                      return pT_gen_;                     }
        Float_t*  eta_gen()                     { SetBranch("eta_gen");                     return eta_gen_;                    }
        Float_t*  y_gen()                       { SetBranch("y_gen");                       return y_gen_;                      }
        Short_t*  status_gen()                  { SetBranch("status_gen");                  return status_gen_;                 }
        Int_t*    PID_gen()                     { SetBranch("PID_gen");                     return PID_gen_;                    }
        Int_t*    MotherID_gen()                { SetBranch("MotherID_gen");                return MotherID_gen_;               }
        Short_t*  RecIdx_gen()                  { SetBranch("RecIdx_gen");                  return RecIdx_gen_;                 }
        Float_t*  V3DPointingAngle_gen()        { SetBranch("3DPointingAngle_gen");         return V3DPointingAngle_gen_;       }
        Float_t*  V2DPointingAngle_gen()        { SetBranch("2DPointingAngle_gen");         return V2DPointingAngle_gen_;       }
        Float_t*  V3DDecayLength_gen()          { SetBranch("3DDecayLength_gen");           return V3DDecayLength_gen_;         }
        Float_t*  V2DDecayLength_gen()          { SetBranch("2DDecayLength_gen");           return V2DDecayLength_gen_;         }
        Int_t*    PIDD1_gen()                   { SetBranch("PIDD1_gen");                   return PIDD1_gen_;                  }
        Short_t*  chargeD1_gen()                { SetBranch("chargeD1_gen");                return chargeD1_gen_;               }
        Float_t*  pTD1_gen()                    { SetBranch("pTD1_gen");                    return pTD1_gen_;                   }
        Float_t*  EtaD1_gen()                   { SetBranch("EtaD1_gen");                   return EtaD1_gen_;                  }
        Float_t*  PhiD1_gen()                   { SetBranch("PhiD1_gen");                   return PhiD1_gen_;                  }
        Int_t*    PIDD2_gen()                   { SetBranch("PIDD2_gen");                   return PIDD2_gen_;                  }
        Short_t*  chargeD2_gen()                { SetBranch("chargeD2_gen");                return chargeD2_gen_;               }
        Float_t*  pTD2_gen()                    { SetBranch("pTD2_gen");                    return pTD2_gen_;                   }
        Float_t*  EtaD2_gen()                   { SetBranch("EtaD2_gen");                   return EtaD2_gen_;                  }
        Float_t*  PhiD2_gen()                   { SetBranch("PhiD2_gen");                   return PhiD2_gen_;                  }

        // EXTRA GETTERS
        Bool_t    tightMuon1  (const UInt_t& iC, const std::string& type="");
        Bool_t    tightMuon2  (const UInt_t& iC, const std::string& type="");
        Bool_t    hybridMuon1 (const UInt_t& iC, const std::string& type="");
        Bool_t    hybridMuon2 (const UInt_t& iC, const std::string& type="");
        Bool_t    softMuon1   (const UInt_t& iC, const std::string& type="");
        Bool_t    softMuon2   (const UInt_t& iC, const std::string& type="");
        Bool_t    tightCand   (const UInt_t& iC, const std::string& type="") { return (tightMuon1(iC, type) && tightMuon2(iC, type));   }
        Bool_t    hybridCand  (const UInt_t& iC, const std::string& type="") { return (hybridMuon1(iC, type) && hybridMuon2(iC, type)); }
        Bool_t    softCand    (const UInt_t& iC, const std::string& type="") { return (softMuon1(iC, type) && softMuon2(iC, type));     }
        Bool_t    trigCand    (const UInt_t& iT, const UInt_t& iC, const bool& OR=false) { if (trigMuon1().size()<=iT) { std::cout << "[ERROR] Trigger index1: "<<iT<<">"<<trigMuon1().size() << std::endl; return false; }; return (OR ? (trigMuon1()[iT][iC] || trigMuon2()[iT][iC]) : (trigMuon1()[iT][iC] && trigMuon2()[iT][iC])); }
        Double_t  phiAsym     (const UInt_t& iC);
        Int_t     GenIdx      (const Short_t& iC) { for (uint iGen=0; iGen<candSize_gen(); iGen++) { if (iC==RecIdx_gen()[iGen]) { return iGen; } }; return -1; }


    private:

        virtual Long64_t  LoadTree        (Long64_t);
        virtual char      GetBranchStatus (const std::string&);
        virtual void      SetBranch       (const std::string&);
        virtual void      InitTree        (void);
        virtual Int_t     LoadEntry       (void) { return fChain_->GetEntry(entry_); }
        virtual void      GenerateDictionaries (void);

        template <typename T> T GET(T* x) { return ( (x) ? *x : T() ); }

        std::map<std::string, TChain*> fChainM_;
        TChain*   fChain_; // DONT USE SMART POINTERS
        Int_t     fCurrent_=-1;
        Long64_t  entry_;

        std::unordered_map<std::string, bool> activeBranches_;

        static const UInt_t NEP   = 3;
        static const UInt_t NTRG  = 15;
        static const UInt_t NSEL  = 10;

        // EVENT INFO VARIABLES
        UInt_t            RunNb_=0;
        UInt_t            LSNb_=0;
        UInt_t            EventNb_=0;
        Short_t           nPV_=-1.;
        Float_t           bestvtxX_=-99.;
        Float_t           bestvtxY_=-99.;
        Float_t           bestvtxZ_=-99.;
        Short_t           centrality_=-1;
        Int_t             Npixel_=-1;
        Float_t           HFsumETPlus_=-1.;
        Float_t           HFsumETMinus_=-1.;
        Float_t           ZDCPlus_=-1.;
        Float_t           ZDCMinus_=-1.;
        Int_t             Ntrkoffline_=-1;
        Short_t           trigPrescale_[NTRG]={0};
        Bool_t            trigHLT_[NTRG]={0};
        Bool_t            evtSel_[NSEL]={0};

        // EVENT PLANE VARIABLES
        Float_t           ephfpSumW_=-99.;
        Float_t           ephfpAngle_[NEP]={0};
        Float_t           ephfpQ_[NEP]={0};
        Float_t           ephfmSumW_=-99.;
        Float_t           ephfmAngle_[NEP]={0};
        Float_t           ephfmQ_[NEP]={0};

        // CANDIDATE INFO VARIABLES
        UInt_t            candSize_=0;
        Float_t           pT_[NCAND]={0};   //[candSize]
        Float_t           eta_[NCAND]={0};   //[candSize]
        Float_t           y_[NCAND]={0};   //[candSize]
        Float_t           phi_[NCAND]={0};   //[candSize]
        Float_t           mass_[NCAND]={0};   //[candSize]
        Float_t           flavor_[NCAND]={0};   //[candSize]
        Float_t           VtxProb_[NCAND]={0};   //[candSize]
        Float_t           V3DCosPointingAngle_[NCAND]={0};   //[candSize]
        Float_t           V3DPointingAngle_[NCAND]={0};   //[candSize]
        Float_t           V2DCosPointingAngle_[NCAND]={0};   //[candSize]
        Float_t           V2DPointingAngle_[NCAND]={0};   //[candSize]
        Float_t           V3DDecayLengthSignificance_[NCAND]={0};   //[candSize]
        Float_t           V3DDecayLength_[NCAND]={0};   //[candSize]
        Float_t           V3DDecayLengthError_[NCAND]={0};   //[candSize]
        Float_t           V2DDecayLengthSignificance_[NCAND]={0};   //[candSize]
        Float_t           V2DDecayLength_[NCAND]={0};   //[candSize]
        Float_t           zDCASignificanceDaugther1_[NCAND]={0};   //[candSize]
        Float_t           xyDCASignificanceDaugther1_[NCAND]={0};   //[candSize]
        Bool_t            HighPurityDaugther1_[NCAND]={0};   //[candSize]
        Float_t           NHitD1_[NCAND]={0};   //[candSize]
        Float_t           pTD1_[NCAND]={0};   //[candSize]
        Float_t           pTerrD1_[NCAND]={0};   //[candSize]
        Float_t           EtaD1_[NCAND]={0};   //[candSize]
        Float_t           PhiD1_[NCAND]={0};   //[candSize]
        Short_t           chargeD1_[NCAND]={0};   //[candSize]
        Float_t           dedxHarmonic2D1_[NCAND]={0};   //[candSize]
        Float_t           zDCASignificanceDaugther2_[NCAND]={0};   //[candSize]
        Float_t           xyDCASignificanceDaugther2_[NCAND]={0};   //[candSize]
        Bool_t            HighPurityDaugther2_[NCAND]={0};   //[candSize]
        Float_t           NHitD2_[NCAND]={0};   //[candSize]
        Float_t           pTD2_[NCAND]={0};   //[candSize]
        Float_t           pTerrD2_[NCAND]={0};   //[candSize]
        Float_t           EtaD2_[NCAND]={0};   //[candSize]
        Float_t           PhiD2_[NCAND]={0};   //[candSize]
        Short_t           chargeD2_[NCAND]={0};   //[candSize]
        Float_t           dedxHarmonic2D2_[NCAND]={0};   //[candSize]
        Bool_t            isSwap_[NCAND]={0};   //[candSize]
        Int_t             idmom_reco_[NCAND]={0};   //[candSize]
        Bool_t            matchGEN_[NCAND]={0};   //[candSize]
        Int_t             PIDD1_[NCAND]={0};   //[candSize]
        Int_t             PIDD2_[NCAND]={0};   //[candSize]

        // MUON INFO VARIABLES
        Bool_t            OneStMuon1_[NCAND]={0};   //[candSize]
        Bool_t            PFMuon1_[NCAND]={0};   //[candSize]
        Bool_t            GlbMuon1_[NCAND]={0};   //[candSize]
        Bool_t            trkMuon1_[NCAND]={0};   //[candSize]
        Bool_t            tightMuon1_[NCAND]={0};   //[candSize]
        Bool_t            softMuon1_[NCAND]={0};   //[candSize]
        Bool_t            hybridMuon1_[NCAND]={0};   //[candSize]
        Bool_t            HPMuon1_[NCAND]={0};   //[candSize]
        UCharVecVec*      trigMuon1_=0;
        Short_t           nMatchedStationD1_[NCAND]={0};   //[candSize]
        Short_t           nTrackerLayerD1_[NCAND]={0};   //[candSize]
        Short_t           nPixelLayerD1_[NCAND]={0};   //[candSize]
        Short_t           nPixelHitD1_[NCAND]={0};   //[candSize]
        Short_t           nMuonHitD1_[NCAND]={0};   //[candSize]
        Float_t           GlbTrkChiD1_[NCAND]={0};   //[candSize]
        Float_t           muondXYD1_[NCAND]={0};   //[candSize]
        Float_t           muondZD1_[NCAND]={0};   //[candSize]
        Float_t           dXYD1_[NCAND]={0};   //[candSize]
        Float_t           dZD1_[NCAND]={0};   //[candSize]
        Short_t           nMatchedChamberD1_[NCAND]={0};   //[candSize]
        Float_t           EnergyDepositionD1_[NCAND]={0};   //[candSize]
        Float_t           dx1_seg_[NCAND]={0};   //[candSize]
        Float_t           dy1_seg_[NCAND]={0};   //[candSize]
        Float_t           dxSig1_seg_[NCAND]={0};   //[candSize]
        Float_t           dySig1_seg_[NCAND]={0};   //[candSize]
        Float_t           ddxdz1_seg_[NCAND]={0};   //[candSize]
        Float_t           ddydz1_seg_[NCAND]={0};   //[candSize]
        Float_t           ddxdzSig1_seg_[NCAND]={0};   //[candSize]
        Float_t           ddydzSig1_seg_[NCAND]={0};   //[candSize]
        Bool_t            OneStMuon2_[NCAND]={0};   //[candSize]
        Bool_t            PFMuon2_[NCAND]={0};   //[candSize]
        Bool_t            GlbMuon2_[NCAND]={0};   //[candSize]
        Bool_t            trkMuon2_[NCAND]={0};   //[candSize]
        Bool_t            tightMuon2_[NCAND]={0};   //[candSize]
        Bool_t            softMuon2_[NCAND]={0};   //[candSize]
        Bool_t            hybridMuon2_[NCAND]={0};   //[candSize]
        Bool_t            HPMuon2_[NCAND]={0};   //[candSize]
        UCharVecVec*      trigMuon2_=0;
        Short_t           nMatchedStationD2_[NCAND]={0};   //[candSize]
        Short_t           nTrackerLayerD2_[NCAND]={0};   //[candSize]
        Short_t           nPixelLayerD2_[NCAND]={0};   //[candSize]
        Short_t           nPixelHitD2_[NCAND]={0};   //[candSize]
        Short_t           nMuonHitD2_[NCAND]={0};   //[candSize]
        Float_t           GlbTrkChiD2_[NCAND]={0};   //[candSize]
        Float_t           muondXYD2_[NCAND]={0};   //[candSize]
        Float_t           muondZD2_[NCAND]={0};   //[candSize]
        Float_t           dXYD2_[NCAND]={0};   //[candSize]
        Float_t           dZD2_[NCAND]={0};   //[candSize]
        Short_t           nMatchedChamberD2_[NCAND]={0};   //[candSize]
        Float_t           EnergyDepositionD2_[NCAND]={0};   //[candSize]
        Float_t           dx2_seg_[NCAND]={0};   //[candSize]
        Float_t           dy2_seg_[NCAND]={0};   //[candSize]
        Float_t           dxSig2_seg_[NCAND]={0};   //[candSize]
        Float_t           dySig2_seg_[NCAND]={0};   //[candSize]
        Float_t           ddxdz2_seg_[NCAND]={0};   //[candSize]
        Float_t           ddydz2_seg_[NCAND]={0};   //[candSize]
        Float_t           ddxdzSig2_seg_[NCAND]={0};   //[candSize]
        Float_t           ddydzSig2_seg_[NCAND]={0};   //[candSize]

        // GEN INFO VARIABLES
        Float_t           weight_gen_=-99.;
        UInt_t            candSize_gen_=0;
        Float_t           pT_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           eta_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           y_gen_[NGEN]={0};   //[candSize_gen]
        Short_t           status_gen_[NGEN]={0};   //[candSize_gen]
        Int_t             PID_gen_[NGEN]={0};   //[candSize_gen]
        Int_t             MotherID_gen_[NGEN]={0};   //[candSize_gen]
        Short_t           RecIdx_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           V3DPointingAngle_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           V2DPointingAngle_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           V3DDecayLength_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           V2DDecayLength_gen_[NGEN]={0};   //[candSize_gen]
        Int_t             PIDD1_gen_[NGEN]={0};   //[candSize_gen]
        Short_t           chargeD1_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           pTD1_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           EtaD1_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           PhiD1_gen_[NGEN]={0};   //[candSize_gen]
        Int_t             PIDD2_gen_[NGEN]={0};   //[candSize_gen]
        Short_t           chargeD2_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           pTD2_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           EtaD2_gen_[NGEN]={0};   //[candSize_gen]
        Float_t           PhiD2_gen_[NGEN]={0};   //[candSize_gen]

        // BRANCHES
        std::map<std::string, TBranch*> b; //!
};

VertexCompositeTree::VertexCompositeTree() : fChain_(0)
{
};

VertexCompositeTree::~VertexCompositeTree()
{
    if (fChain_) { const auto& f = fChain_->GetCurrentFile(); if (f) { f->Close(); delete f; }; fChain_->Reset(); }
    for (auto& c : fChainM_) { if (c.second) { c.second->Reset(); } }
};

Bool_t VertexCompositeTree::GetTree(const std::string& fileName, const std::string& treeName)
{
    const auto& fileNames = std::vector<std::string>({fileName});
    return GetTree(fileNames, treeName);
};

Bool_t VertexCompositeTree::GetTree(const std::vector< std::string >& inFileName, const std::string& treeName)
{
    // Check the File Names
    auto fileName = inFileName;
    for (auto& f : fileName) { if (f.rfind("/store/", 0)==0) { f = "root://cms-xrd-global.cern.ch/" + f; } }
    // Open the input files
    const auto& f = TFile::Open(fileName[0].c_str(), "READ");
    if (!f || !f->IsOpen() || f->IsZombie()) { std::cout << "[ERROR] Failed to open file: " << fileName[0] << std::endl; return false; }
    // Extract the input TChains
    std::cout << "[INFO] Extracting tree: " << treeName.c_str() << std::endl;
    fChainM_.clear();
    TDirectory * dir;
    if (fileName[0].rfind("root://", 0)==0) { dir = dynamic_cast<TDirectory*>(f->Get(treeName.c_str())); }
    else { dir = dynamic_cast<TDirectory*>(f->Get((fileName[0]+":/"+treeName).c_str())); }
    if (!dir) { std::cout << "[ERROR] Failed to open directory: " << treeName << std::endl; return false; }
    if (dir->GetListOfKeys()->Contains("VertexCompositeNtuple")) { fChainM_["VertexCompositeNtuple"] = new TChain((treeName+"/VertexCompositeNtuple").c_str() , "VertexCompositeNtuple"); }
    if (fChainM_.size()==0) { std::cout << "[ERROR] fChain VertexCompositeTree was not created, some input files are   missing" << std::endl; return false; }
    // Add the files in the TChain
    for (auto& c : fChainM_) {
        for (auto& f : fileName) { c.second->Add(Form("%s/%s/%s", f.c_str(), treeName.c_str(), c.first.c_str())); }; c.second->GetEntries();
    }
    for (const auto& c : fChainM_) { if (!c.second) { std::cout << "[ERROR] fChain " << c.first << " was not created, some input files are missing" << std::endl; return false; } }
    // Initialize the input TChains (set their branches)
    InitTree();
    // Add Friend TChains
    if (fChain_) { delete fChain_; }
    fChain_ = dynamic_cast<TChain*>(fChainM_.begin()->second->Clone(treeName.c_str()));
    for (auto& c : fChainM_) {
        if (c.second != fChain_) {
            c.second->SetMakeClass(1); // For the proper setup.
            fChain_->AddFriend(c.second, c.first.c_str(), kTRUE); // Add the Friend TChain
        }
    }
    if (!fChain_) return false;
    // Set All Branches to Status 0
    fChain_->SetBranchStatus("*",0);
    //
    return true;
};

Int_t VertexCompositeTree::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    entry_ = entry;
    if (LoadTree(entry_) < 0) return -1;
    //Clear();
    const auto& status = LoadEntry();
    // Check contents of entry
    if (candSize_ >= NCAND) { std::cout << "[ERROR] Reconstructed candidate size ("<<candSize_<<") is larger than "<<NCAND << std::endl; return -9; }
    if (candSize_gen_ >= NGEN) { std::cout << "[ERROR] Generated candidate size ("<<candSize_gen_<<") is larger than "<<NGEN << std::endl; return -9; }
    return status;
};

Long64_t VertexCompositeTree::LoadTree(Long64_t entry)
{
    // Set the environment to read one entry
    if (!fChain_) return -5;
    const auto& centry = fChain_->LoadTree(entry);
    if (fChain_->GetTreeNumber() != fCurrent_) { fCurrent_ = fChain_->GetTreeNumber(); }
    return centry;
};

char VertexCompositeTree::GetBranchStatus(const std::string& n)
{
    if (activeBranches_[n]) return 1;
    return fChain_->GetBranchStatus(n.c_str());
};

void VertexCompositeTree::SetBranch(const std::string& n)
{
    if (GetBranchStatus(n)==0) {
        fChain_->SetBranchStatus(n.c_str(), 1);
        GetEntry(entry_);
        activeBranches_.at(n) = true;
    }
};

void VertexCompositeTree::InitTree(void)
{
    // Generate the dictionary's needed
    GenerateDictionaries();

    // Initialize pointers
    trigMuon1_ = 0;
    trigMuon2_ = 0;

    if (fChainM_.count("VertexCompositeNtuple")>0) {
        auto& fChain = fChainM_.at("VertexCompositeNtuple");

        // SET EVENT INFO BRANCHES
        if (fChain->GetBranch("RunNb"))                       fChain->SetBranchAddress("RunNb",                     &RunNb_,                      &(b["RunNb"])                     );
        if (fChain->GetBranch("LSNb"))                        fChain->SetBranchAddress("LSNb",                      &LSNb_,                       &(b["LSNb"])                      );
        if (fChain->GetBranch("EventNb"))                     fChain->SetBranchAddress("EventNb",                   &EventNb_,                    &(b["EventNb"])                   );
        if (fChain->GetBranch("nPV"))                         fChain->SetBranchAddress("nPV",                       &nPV_,                        &(b["nPV"])                       );
        if (fChain->GetBranch("bestvtxX"))                    fChain->SetBranchAddress("bestvtxX",                  &bestvtxX_,                   &(b["bestvtxX"])                  );
        if (fChain->GetBranch("bestvtxY"))                    fChain->SetBranchAddress("bestvtxY",                  &bestvtxY_,                   &(b["bestvtxY"])                  );
        if (fChain->GetBranch("bestvtxZ"))                    fChain->SetBranchAddress("bestvtxZ",                  &bestvtxZ_,                   &(b["bestvtxZ"])                  );
        if (fChain->GetBranch("centrality"))                  fChain->SetBranchAddress("centrality",                &centrality_,                 &(b["centrality"])                );
        if (fChain->GetBranch("Npixel"))                      fChain->SetBranchAddress("Npixel",                    &Npixel_,                     &(b["Npixel"])                    );
        if (fChain->GetBranch("HFsumETPlus"))                 fChain->SetBranchAddress("HFsumETPlus",               &HFsumETPlus_,                &(b["HFsumETPlus"])               );
        if (fChain->GetBranch("HFsumETMinus"))                fChain->SetBranchAddress("HFsumETMinus",              &HFsumETMinus_,               &(b["HFsumETMinus"])              );
        if (fChain->GetBranch("ZDCPlus"))                     fChain->SetBranchAddress("ZDCPlus",                   &ZDCPlus_,                    &(b["ZDCPlus"])                   );
        if (fChain->GetBranch("ZDCMinus"))                    fChain->SetBranchAddress("ZDCMinus",                  &ZDCMinus_,                   &(b["ZDCMinus"])                  );
        if (fChain->GetBranch("Ntrkoffline"))                 fChain->SetBranchAddress("Ntrkoffline",               &Ntrkoffline_,                &(b["Ntrkoffline"])               );
        if (fChain->GetBranch("trigPrescale"))                fChain->SetBranchAddress("trigPrescale",               trigPrescale_,               &(b["trigPrescale"])              );
        if (fChain->GetBranch("trigHLT"))                     fChain->SetBranchAddress("trigHLT",                    trigHLT_,                    &(b["trigHLT"])                   );
        if (fChain->GetBranch("evtSel"))                      fChain->SetBranchAddress("evtSel",                     evtSel_,                     &(b["evtSel"])                    );

        // SET EVENT PLANE BRANCHES
        if (fChain->GetBranch("ephfpSumW"))                   fChain->SetBranchAddress("ephfpSumW",                 &ephfpSumW_,                  &(b["ephfpSumW"])                 );
        if (fChain->GetBranch("ephfpAngle"))                  fChain->SetBranchAddress("ephfpAngle",                 ephfpAngle_,                 &(b["ephfpAngle"])                );
        if (fChain->GetBranch("ephfpQ"))                      fChain->SetBranchAddress("ephfpQ",                     ephfpQ_,                     &(b["ephfpQ"])                    );
        if (fChain->GetBranch("ephfmSumW"))                   fChain->SetBranchAddress("ephfmSumW",                 &ephfmSumW_,                  &(b["ephfmSumW"])                 );
        if (fChain->GetBranch("ephfmAngle"))                  fChain->SetBranchAddress("ephfmAngle",                 ephfmAngle_,                 &(b["ephfmAngle"])                );
        if (fChain->GetBranch("ephfmQ"))                      fChain->SetBranchAddress("ephfmQ",                     ephfmQ_,                     &(b["ephfmQ"])                    );

        // SET CANDIDATE INFO BRANCHES
        if (fChain->GetBranch("candSize"))                    fChain->SetBranchAddress("candSize",                  &candSize_,                   &(b["candSize"])                  );
        if (fChain->GetBranch("pT"))                          fChain->SetBranchAddress("pT",                         pT_,                         &(b["pT"])                        );
        if (fChain->GetBranch("eta"))                         fChain->SetBranchAddress("eta",                        eta_,                        &(b["eta"])                       );
        if (fChain->GetBranch("y"))                           fChain->SetBranchAddress("y",                          y_,                          &(b["y"])                         );
        if (fChain->GetBranch("phi"))                         fChain->SetBranchAddress("phi",                        phi_,                        &(b["phi"])                       );
        if (fChain->GetBranch("mass"))                        fChain->SetBranchAddress("mass",                       mass_,                       &(b["mass"])                      );
        if (fChain->GetBranch("flavor"))                      fChain->SetBranchAddress("flavor",                     flavor_,                     &(b["flavor"])                    );
        if (fChain->GetBranch("VtxProb"))                     fChain->SetBranchAddress("VtxProb",                    VtxProb_,                    &(b["VtxProb"])                   );
        if (fChain->GetBranch("3DCosPointingAngle"))          fChain->SetBranchAddress("3DCosPointingAngle",         V3DCosPointingAngle_,        &(b["3DCosPointingAngle"])        );
        if (fChain->GetBranch("3DPointingAngle"))             fChain->SetBranchAddress("3DPointingAngle",            V3DPointingAngle_,           &(b["3DPointingAngle"])           );
        if (fChain->GetBranch("2DCosPointingAngle"))          fChain->SetBranchAddress("2DCosPointingAngle",         V2DCosPointingAngle_,        &(b["2DCosPointingAngle"])        );
        if (fChain->GetBranch("2DPointingAngle"))             fChain->SetBranchAddress("2DPointingAngle",            V2DPointingAngle_,           &(b["2DPointingAngle"])           );
        if (fChain->GetBranch("3DDecayLengthSignificance"))   fChain->SetBranchAddress("3DDecayLengthSignificance",  V3DDecayLengthSignificance_, &(b["3DDecayLengthSignificance"]) );
        if (fChain->GetBranch("3DDecayLength"))               fChain->SetBranchAddress("3DDecayLength",              V3DDecayLength_,             &(b["3DDecayLength"])             );
        if (fChain->GetBranch("3DDecayLengthError"))          fChain->SetBranchAddress("3DDecayLengthError",         V3DDecayLengthError_,        &(b["3DDecayLengthError"])        );
        if (fChain->GetBranch("2DDecayLengthSignificance"))   fChain->SetBranchAddress("2DDecayLengthSignificance",  V2DDecayLengthSignificance_, &(b["2DDecayLengthSignificance"]) );
        if (fChain->GetBranch("2DDecayLength"))               fChain->SetBranchAddress("2DDecayLength",              V2DDecayLength_,             &(b["2DDecayLength"])             );
        if (fChain->GetBranch("zDCASignificanceDaugther1"))   fChain->SetBranchAddress("zDCASignificanceDaugther1",  zDCASignificanceDaugther1_,  &(b["zDCASignificanceDaugther1"]) );
        if (fChain->GetBranch("xyDCASignificanceDaugther1"))  fChain->SetBranchAddress("xyDCASignificanceDaugther1", xyDCASignificanceDaugther1_, &(b["xyDCASignificanceDaugther1"]));
        if (fChain->GetBranch("HighPurityDaugther1"))         fChain->SetBranchAddress("HighPurityDaugther1",        HighPurityDaugther1_,        &(b["HighPurityDaugther1"])       );
        if (fChain->GetBranch("NHitD1"))                      fChain->SetBranchAddress("NHitD1",                     NHitD1_,                     &(b["NHitD1"])                    );
        if (fChain->GetBranch("pTD1"))                        fChain->SetBranchAddress("pTD1",                       pTD1_,                       &(b["pTD1"])                      );
        if (fChain->GetBranch("pTerrD1"))                     fChain->SetBranchAddress("pTerrD1",                    pTerrD1_,                    &(b["pTerrD1"])                   );
        if (fChain->GetBranch("EtaD1"))                       fChain->SetBranchAddress("EtaD1",                      EtaD1_,                      &(b["EtaD1"])                     );
        if (fChain->GetBranch("PhiD1"))                       fChain->SetBranchAddress("PhiD1",                      PhiD1_,                      &(b["PhiD1"])                     );
        if (fChain->GetBranch("chargeD1"))                    fChain->SetBranchAddress("chargeD1",                   chargeD1_,                   &(b["chargeD1"])                  );
        if (fChain->GetBranch("dedxHarmonic2D1"))             fChain->SetBranchAddress("dedxHarmonic2D1",            dedxHarmonic2D1_,            &(b["dedxHarmonic2D1"])           );
        if (fChain->GetBranch("zDCASignificanceDaugther2"))   fChain->SetBranchAddress("zDCASignificanceDaugther2",  zDCASignificanceDaugther2_,  &(b["zDCASignificanceDaugther2"]) );
        if (fChain->GetBranch("xyDCASignificanceDaugther2"))  fChain->SetBranchAddress("xyDCASignificanceDaugther2", xyDCASignificanceDaugther2_, &(b["xyDCASignificanceDaugther2"]));
        if (fChain->GetBranch("HighPurityDaugther2"))         fChain->SetBranchAddress("HighPurityDaugther2",        HighPurityDaugther2_,        &(b["HighPurityDaugther2"])       );
        if (fChain->GetBranch("NHitD2"))                      fChain->SetBranchAddress("NHitD2",                     NHitD2_,                     &(b["NHitD2"])                    );
        if (fChain->GetBranch("pTD2"))                        fChain->SetBranchAddress("pTD2",                       pTD2_,                       &(b["pTD2"])                      );
        if (fChain->GetBranch("pTerrD2"))                     fChain->SetBranchAddress("pTerrD2",                    pTerrD2_,                    &(b["pTerrD2"])                   );
        if (fChain->GetBranch("EtaD2"))                       fChain->SetBranchAddress("EtaD2",                      EtaD2_,                      &(b["EtaD2"])                     );
        if (fChain->GetBranch("PhiD2"))                       fChain->SetBranchAddress("PhiD2",                      PhiD2_,                      &(b["PhiD2"])                     );
        if (fChain->GetBranch("chargeD2"))                    fChain->SetBranchAddress("chargeD2",                   chargeD2_,                   &(b["chargeD2"])                  );
        if (fChain->GetBranch("dedxHarmonic2D2"))             fChain->SetBranchAddress("dedxHarmonic2D2",            dedxHarmonic2D2_,            &(b["dedxHarmonic2D2"])           );
        if (fChain->GetBranch("isSwap"))                      fChain->SetBranchAddress("isSwap",                     isSwap_,                     &(b["isSwap"])                    );
        if (fChain->GetBranch("idmom_reco"))                  fChain->SetBranchAddress("idmom_reco",                 idmom_reco_,                 &(b["idmom_reco"])                );
        if (fChain->GetBranch("matchGEN"))                    fChain->SetBranchAddress("matchGEN",                   matchGEN_,                   &(b["matchGEN"])                  );
        if (fChain->GetBranch("PIDD1"))                       fChain->SetBranchAddress("PIDD1",                      PIDD1_,                      &(b["PIDD1"])                     );
        if (fChain->GetBranch("PIDD2"))                       fChain->SetBranchAddress("PIDD2",                      PIDD2_,                      &(b["PIDD2"])                     );

        // SET MUON INFO BRANCHES
        if (fChain->GetBranch("OneStMuon1"))                  fChain->SetBranchAddress("OneStMuon1",                 OneStMuon1_,                 &(b["OneStMuon1"])                );
        if (fChain->GetBranch("PFMuon1"))                     fChain->SetBranchAddress("PFMuon1",                    PFMuon1_,                    &(b["PFMuon1"])                   );
        if (fChain->GetBranch("GlbMuon1"))                    fChain->SetBranchAddress("GlbMuon1",                   GlbMuon1_,                   &(b["GlbMuon1"])                  );
        if (fChain->GetBranch("trkMuon1"))                    fChain->SetBranchAddress("trkMuon1",                   trkMuon1_,                   &(b["trkMuon1"])                  );
        if (fChain->GetBranch("tightMuon1"))                  fChain->SetBranchAddress("tightMuon1",                 tightMuon1_,                 &(b["tightMuon1"])                );
        if (fChain->GetBranch("softMuon1"))                   fChain->SetBranchAddress("softMuon1",                  softMuon1_,                  &(b["softMuon1"])                 );
        if (fChain->GetBranch("hybridMuon1"))                 fChain->SetBranchAddress("hybridMuon1",                hybridMuon1_,                &(b["hybridMuon1"])               );
        if (fChain->GetBranch("HPMuon1"))                     fChain->SetBranchAddress("HPMuon1",                    HPMuon1_,                    &(b["HPMuon1"])                   );
        if (fChain->GetBranch("trigMuon1"))                   fChain->SetBranchAddress("trigMuon1",                 &trigMuon1_,                  &(b["trigMuon1"])                 );
        if (fChain->GetBranch("nMatchedStationD1"))           fChain->SetBranchAddress("nMatchedStationD1",          nMatchedStationD1_,          &(b["nMatchedStationD1"])         );
        if (fChain->GetBranch("nTrackerLayerD1"))             fChain->SetBranchAddress("nTrackerLayerD1",            nTrackerLayerD1_,            &(b["nTrackerLayerD1"])           );
        if (fChain->GetBranch("nPixelLayerD1"))               fChain->SetBranchAddress("nPixelLayerD1",              nPixelLayerD1_,              &(b["nPixelLayerD1"])             );
        if (fChain->GetBranch("nPixelHitD1"))                 fChain->SetBranchAddress("nPixelHitD1",                nPixelHitD1_,                &(b["nPixelHitD1"])               );
        if (fChain->GetBranch("nMuonHitD1"))                  fChain->SetBranchAddress("nMuonHitD1",                 nMuonHitD1_,                 &(b["nMuonHitD1"])                );
        if (fChain->GetBranch("GlbTrkChiD1"))                 fChain->SetBranchAddress("GlbTrkChiD1",                GlbTrkChiD1_,                &(b["GlbTrkChiD1"])               );
        if (fChain->GetBranch("muondZD1"))                    fChain->SetBranchAddress("muondZD1",                   muondZD1_,                   &(b["muondZD1"])                  );
        if (fChain->GetBranch("muondXYD1"))                   fChain->SetBranchAddress("muondXYD1",                  muondXYD1_,                  &(b["muondXYD1"])                 );
        if (fChain->GetBranch("dZD1"))                        fChain->SetBranchAddress("dZD1",                       dZD1_,                       &(b["dZD1"])                      );
        if (fChain->GetBranch("dXYD1"))                       fChain->SetBranchAddress("dXYD1",                      dXYD1_,                      &(b["dXYD1"])                     );
        if (fChain->GetBranch("OneStMuon2"))                  fChain->SetBranchAddress("OneStMuon2",                 OneStMuon2_,                 &(b["OneStMuon2"])                );
        if (fChain->GetBranch("PFMuon2"))                     fChain->SetBranchAddress("PFMuon2",                    PFMuon2_,                    &(b["PFMuon2"])                   );
        if (fChain->GetBranch("GlbMuon2"))                    fChain->SetBranchAddress("GlbMuon2",                   GlbMuon2_,                   &(b["GlbMuon2"])                  );
        if (fChain->GetBranch("trkMuon2"))                    fChain->SetBranchAddress("trkMuon2",                   trkMuon2_,                   &(b["trkMuon2"])                  );
        if (fChain->GetBranch("tightMuon2"))                  fChain->SetBranchAddress("tightMuon2",                 tightMuon2_,                 &(b["tightMuon2"])                );
        if (fChain->GetBranch("softMuon2"))                   fChain->SetBranchAddress("softMuon2",                  softMuon2_,                  &(b["softMuon2"])                 );
        if (fChain->GetBranch("hybridMuon2"))                 fChain->SetBranchAddress("hybridMuon2",                hybridMuon2_,                &(b["hybridMuon2"])               );
        if (fChain->GetBranch("HPMuon2"))                     fChain->SetBranchAddress("HPMuon2",                    HPMuon2_,                    &(b["HPMuon2"])                   );
        if (fChain->GetBranch("trigMuon2"))                   fChain->SetBranchAddress("trigMuon2",                 &trigMuon2_,                  &(b["trigMuon2"])                 );
        if (fChain->GetBranch("nMatchedStationD2"))           fChain->SetBranchAddress("nMatchedStationD2",          nMatchedStationD2_,          &(b["nMatchedStationD2"])         );
        if (fChain->GetBranch("nTrackerLayerD2"))             fChain->SetBranchAddress("nTrackerLayerD2",            nTrackerLayerD2_,            &(b["nTrackerLayerD2"])           );
        if (fChain->GetBranch("nPixelLayerD2"))               fChain->SetBranchAddress("nPixelLayerD2",              nPixelLayerD2_,              &(b["nPixelLayerD2"])             );
        if (fChain->GetBranch("nPixelHitD2"))                 fChain->SetBranchAddress("nPixelHitD2",                nPixelHitD2_,                &(b["nPixelHitD2"])               );
        if (fChain->GetBranch("nMuonHitD2"))                  fChain->SetBranchAddress("nMuonHitD2",                 nMuonHitD2_,                 &(b["nMuonHitD2"])                );
        if (fChain->GetBranch("GlbTrkChiD2"))                 fChain->SetBranchAddress("GlbTrkChiD2",                GlbTrkChiD2_,                &(b["GlbTrkChiD2"])               );
        if (fChain->GetBranch("muondZD2"))                    fChain->SetBranchAddress("muondZD2",                   muondZD2_,                   &(b["muondZD2"])                  );
        if (fChain->GetBranch("muondXYD2"))                   fChain->SetBranchAddress("muondXYD2",                  muondXYD2_,                  &(b["muondXYD2"])                 );
        if (fChain->GetBranch("dZD2"))                        fChain->SetBranchAddress("dZD2",                       dZD2_,                       &(b["dZD2"])                      );
        if (fChain->GetBranch("dXYD2"))                       fChain->SetBranchAddress("dXYD2",                      dXYD2_,                      &(b["dXYD2"])                     );

        // SET GEN INFO BRANCHES
        if (fChain->GetBranch("weight_gen"))                  fChain->SetBranchAddress("weight_gen",                &weight_gen_,                 &(b["weight_gen"])                );
        if (fChain->GetBranch("candSize_gen"))                fChain->SetBranchAddress("candSize_gen",              &candSize_gen_,               &(b["candSize_gen"])              );
        if (fChain->GetBranch("pT_gen"))                      fChain->SetBranchAddress("pT_gen",                     pT_gen_,                     &(b["pT_gen"])                    );
        if (fChain->GetBranch("eta_gen"))                     fChain->SetBranchAddress("eta_gen",                    eta_gen_,                    &(b["eta_gen"])                   );
        if (fChain->GetBranch("y_gen"))                       fChain->SetBranchAddress("y_gen",                      y_gen_,                      &(b["y_gen"])                     );
        if (fChain->GetBranch("status_gen"))                  fChain->SetBranchAddress("status_gen",                 status_gen_,                 &(b["status_gen"])                );
        if (fChain->GetBranch("PID_gen"))                     fChain->SetBranchAddress("PID_gen",                    PID_gen_,                    &(b["PID_gen"])                   );
        if (fChain->GetBranch("MotherID_gen"))                fChain->SetBranchAddress("MotherID_gen",               MotherID_gen_,               &(b["MotherID_gen"])              );
        if (fChain->GetBranch("RecIdx_gen"))                  fChain->SetBranchAddress("RecIdx_gen",                 RecIdx_gen_,                 &(b["RecIdx_gen"])                );
        if (fChain->GetBranch("3DPointingAngle_gen"))         fChain->SetBranchAddress("3DPointingAngle_gen",        V3DPointingAngle_gen_,       &(b["3DPointingAngle_gen"])       );
        if (fChain->GetBranch("2DPointingAngle_gen"))         fChain->SetBranchAddress("2DPointingAngle_gen",        V2DPointingAngle_gen_,       &(b["2DPointingAngle_gen"])       );
        if (fChain->GetBranch("3DDecayLength_gen"))           fChain->SetBranchAddress("3DDecayLength_gen",          V3DDecayLength_gen_,         &(b["3DDecayLength_gen"])         );
        if (fChain->GetBranch("2DDecayLength_gen"))           fChain->SetBranchAddress("2DDecayLength_gen",          V2DDecayLength_gen_,         &(b["2DDecayLength_gen"])         );
        if (fChain->GetBranch("PIDD1_gen"))                   fChain->SetBranchAddress("PIDD1_gen",                  PIDD1_gen_,                  &(b["PIDD1_gen"])                 );
        if (fChain->GetBranch("chargeD1_gen"))                fChain->SetBranchAddress("chargeD1_gen",               chargeD1_gen_,               &(b["chargeD1_gen"])              );
        if (fChain->GetBranch("pTD1_gen"))                    fChain->SetBranchAddress("pTD1_gen",                   pTD1_gen_,                   &(b["pTD1_gen"])                  );
        if (fChain->GetBranch("EtaD1_gen"))                   fChain->SetBranchAddress("EtaD1_gen",                  EtaD1_gen_,                  &(b["EtaD1_gen"])                 );
        if (fChain->GetBranch("PhiD1_gen"))                   fChain->SetBranchAddress("PhiD1_gen",                  PhiD1_gen_,                  &(b["PhiD1_gen"])                 );
        if (fChain->GetBranch("PIDD2_gen"))                   fChain->SetBranchAddress("PIDD2_gen",                  PIDD2_gen_,                  &(b["PIDD2_gen"])                 );
        if (fChain->GetBranch("chargeD2_gen"))                fChain->SetBranchAddress("chargeD2_gen",               chargeD2_gen_,               &(b["chargeD2_gen"])              );
        if (fChain->GetBranch("pTD2_gen"))                    fChain->SetBranchAddress("pTD2_gen",                   pTD2_gen_,                   &(b["pTD2_gen"])                  );
        if (fChain->GetBranch("EtaD2_gen"))                   fChain->SetBranchAddress("EtaD2_gen",                  EtaD2_gen_,                  &(b["EtaD2_gen"])                 );
        if (fChain->GetBranch("PhiD2_gen"))                   fChain->SetBranchAddress("PhiD2_gen",                  PhiD2_gen_,                  &(b["PhiD2_gen"])                 );
    }
};

void VertexCompositeTree::Clear(void)
{
    if (fChainM_.size()==0) return;

    // CLEAR EVENT INFO VARIABLES
    if (GetBranchStatus("RunNb")==1)        RunNb_        = 0;
    if (GetBranchStatus("LSNb")==1)         LSNb_         = 0;
    if (GetBranchStatus("EventNb")==1)      EventNb_      = 0;
    if (GetBranchStatus("nPV")==1)          nPV_          = -1;
    if (GetBranchStatus("bestvtxX")==1)     bestvtxX_     = -99.;
    if (GetBranchStatus("bestvtxY")==1)     bestvtxY_     = -99.;
    if (GetBranchStatus("bestvtxZ")==1)     bestvtxZ_     = -99.;
    if (GetBranchStatus("centrality")==1)   centrality_   = -1;
    if (GetBranchStatus("Npixel")==1)       Npixel_       = -1;
    if (GetBranchStatus("HFsumETPlus")==1)  HFsumETPlus_  = -1.;
    if (GetBranchStatus("HFsumETMinus")==1) HFsumETMinus_ = -1.;
    if (GetBranchStatus("ZDCPlus")==1)      ZDCPlus_      = -1.;
    if (GetBranchStatus("ZDCMinus")==1)     ZDCMinus_     = -1.;
    if (GetBranchStatus("Ntrkoffline")==1)  Ntrkoffline_  = -1;
    if (GetBranchStatus("trigPrescale")==1) std::fill_n(trigPrescale_, NTRG, -9);
    if (GetBranchStatus("trigHLT")==1)      std::fill_n(trigHLT_, NTRG, 0);
    if (GetBranchStatus("evtSel")==1)       std::fill_n(evtSel_, NSEL, 0);

    // CLEAR EVENT PLANE VARIABLES
    if (GetBranchStatus("ephfpSumW")==1)  ephfpSumW_ = -99.;
    if (GetBranchStatus("ephfmSumW")==1)  ephfmSumW_ = -99.;
    if (GetBranchStatus("ephfpAngle")==1) std::fill_n(ephfpAngle_, NEP, -99.);
    if (GetBranchStatus("ephfpQ")==1)     std::fill_n(ephfpQ_, NEP, -99.);
    if (GetBranchStatus("ephfmAngle")==1) std::fill_n(ephfmAngle_, NEP, -99.);
    if (GetBranchStatus("ephfmQ")==1)     std::fill_n(ephfmQ_, NEP, -99.);

    // CLEAR CANDIDATE INFO VARIABLES
    const auto& nCand = (candSize_>0 ? candSize_ : NCAND);
    if (GetBranchStatus("candSize")==1)                   candSize_ = 0;
    if (GetBranchStatus("pT")==1)                         std::fill_n(pT_, nCand, -1.);
    if (GetBranchStatus("eta")==1)                        std::fill_n(eta_, nCand, -9.);
    if (GetBranchStatus("y")==1)                          std::fill_n(y_, nCand, -9.);
    if (GetBranchStatus("phi")==1)                        std::fill_n(phi_, nCand, -9.);
    if (GetBranchStatus("mass")==1)                       std::fill_n(mass_, nCand, -1.);
    if (GetBranchStatus("flavor")==1)                     std::fill_n(flavor_, nCand, 0.);
    if (GetBranchStatus("VtxProb")==1)                    std::fill_n(VtxProb_, nCand, -1.);
    if (GetBranchStatus("3DCosPointingAngle")==1)         std::fill_n(V3DCosPointingAngle_, nCand, -9.);
    if (GetBranchStatus("3DPointingAngle")==1)            std::fill_n(V3DPointingAngle_, nCand, -9.);
    if (GetBranchStatus("2DCosPointingAngle")==1)         std::fill_n(V2DCosPointingAngle_, nCand, -9.);
    if (GetBranchStatus("2DPointingAngle")==1)            std::fill_n(V2DPointingAngle_, nCand, -9.);
    if (GetBranchStatus("3DDecayLengthSignificance")==1)  std::fill_n(V3DDecayLengthSignificance_, nCand, -1.);
    if (GetBranchStatus("3DDecayLength")==1)              std::fill_n(V3DDecayLength_, nCand, -1.);
    if (GetBranchStatus("3DDecayLengthError")==1)         std::fill_n(V3DDecayLengthError_, nCand, -1.);
    if (GetBranchStatus("2DDecayLengthSignificance")==1)  std::fill_n(V2DDecayLengthSignificance_, nCand, -1.);
    if (GetBranchStatus("2DDecayLength")==1)              std::fill_n(V2DDecayLength_, nCand, -1.);
    if (GetBranchStatus("zDCASignificanceDaugther1")==1)  std::fill_n(zDCASignificanceDaugther1_, nCand, -1.);
    if (GetBranchStatus("xyDCASignificanceDaugther1")==1) std::fill_n(xyDCASignificanceDaugther1_, nCand, -1.);
    if (GetBranchStatus("HighPurityDaugther1")==1)        std::fill_n(HighPurityDaugther1_, nCand, 0);
    if (GetBranchStatus("NHitD1")==1)                     std::fill_n(NHitD1_, nCand, -1.);
    if (GetBranchStatus("pTD1")==1)                       std::fill_n(pTD1_, nCand, -1.);
    if (GetBranchStatus("EtaD1")==1)                      std::fill_n(EtaD1_, nCand, -9.);
    if (GetBranchStatus("PhiD1")==1)                      std::fill_n(PhiD1_, nCand, -9.);
    if (GetBranchStatus("chargeD1")==1)                   std::fill_n(chargeD1_, nCand, -9);
    if (GetBranchStatus("dedxHarmonic2D1")==1)            std::fill_n(dedxHarmonic2D1_, nCand, -1.);
    if (GetBranchStatus("zDCASignificanceDaugther2")==1)  std::fill_n(zDCASignificanceDaugther2_, nCand, -1.);
    if (GetBranchStatus("xyDCASignificanceDaugther2")==1) std::fill_n(xyDCASignificanceDaugther2_, nCand, -1.);
    if (GetBranchStatus("HighPurityDaugther2")==1)        std::fill_n(HighPurityDaugther2_, nCand, 0);
    if (GetBranchStatus("NHitD2")==1)                     std::fill_n(NHitD2_, nCand, -1.);
    if (GetBranchStatus("pTD2")==1)                       std::fill_n(pTD2_, nCand, -1.);
    if (GetBranchStatus("EtaD2")==1)                      std::fill_n(EtaD2_, nCand, -9.);
    if (GetBranchStatus("PhiD2")==1)                      std::fill_n(PhiD2_, nCand, -9.);
    if (GetBranchStatus("chargeD2")==1)                   std::fill_n(chargeD2_, nCand, -9);
    if (GetBranchStatus("dedxHarmonic2D2")==1)            std::fill_n(dedxHarmonic2D2_, nCand, -1.);
    if (GetBranchStatus("isSwap")==1)                     std::fill_n(isSwap_, nCand, 0);
    if (GetBranchStatus("idmom_reco")==1)                 std::fill_n(idmom_reco_, nCand, -999);
    if (GetBranchStatus("matchGEN")==1)                   std::fill_n(matchGEN_, nCand, 0);
    if (GetBranchStatus("PIDD1")==1)                      std::fill_n(PIDD1_, nCand, -999);
    if (GetBranchStatus("PIDD2")==1)                      std::fill_n(PIDD2_, nCand, -999);

    // CLEAR MUON INFO VARIABLES
    if (GetBranchStatus("OneStMuon1")==1)         std::fill_n(OneStMuon1_, nCand, 0);
    if (GetBranchStatus("PFMuon1")==1)            std::fill_n(PFMuon1_, nCand, 0);
    if (GetBranchStatus("GlbMuon1")==1)           std::fill_n(GlbMuon1_, nCand, 0);
    if (GetBranchStatus("trkMuon1")==1)           std::fill_n(trkMuon1_, nCand, 0);
    if (GetBranchStatus("tightMuon1")==1)         std::fill_n(tightMuon1_, nCand, 0);
    if (GetBranchStatus("softMuon1")==1)          std::fill_n(softMuon1_, nCand, 0);
    if (GetBranchStatus("hybridMuon1")==1)        std::fill_n(hybridMuon1_, nCand, 0);
    if (GetBranchStatus("HPMuon1")==1)            std::fill_n(HPMuon1_, nCand, 0);
    if (GetBranchStatus("trigMuon1")==1 && trigMuon1_) trigMuon1_->clear();
    if (GetBranchStatus("nMatchedStationD1")==1)  std::fill_n(nMatchedStationD1_, nCand, -1);
    if (GetBranchStatus("nTrackerLayerD1")==1)    std::fill_n(nTrackerLayerD1_, nCand, -1);
    if (GetBranchStatus("nPixelLayerD1")==1)      std::fill_n(nPixelLayerD1_, nCand, -1);
    if (GetBranchStatus("nPixelHitD1")==1)        std::fill_n(nPixelHitD1_, nCand, -1);
    if (GetBranchStatus("nMuonHitD1")==1)         std::fill_n(nMuonHitD1_, nCand, -1);
    if (GetBranchStatus("GlbTrkChiD1")==1)        std::fill_n(GlbTrkChiD1_, nCand, 99.);
    if (GetBranchStatus("muondZD1")==1)           std::fill_n(muondZD1_, nCand, -99.);
    if (GetBranchStatus("muondXYD1")==1)          std::fill_n(muondXYD1_, nCand, -99.);
    if (GetBranchStatus("dZD1")==1)               std::fill_n(dZD1_, nCand, -99.);
    if (GetBranchStatus("dXYD1")==1)              std::fill_n(dXYD1_, nCand, -99.);
    if (GetBranchStatus("nMatchedChamberD1")==1)  std::fill_n(nMatchedChamberD1_, nCand, -1);
    if (GetBranchStatus("EnergyDepositionD1")==1) std::fill_n(EnergyDepositionD1_, nCand, -1.);
    if (GetBranchStatus("dx1_seg")==1)            std::fill_n(dx1_seg_, nCand, -99.);
    if (GetBranchStatus("dy1_seg")==1)            std::fill_n(dy1_seg_, nCand, -99.);
    if (GetBranchStatus("dxSig1_seg")==1)         std::fill_n(dxSig1_seg_, nCand, -99.);
    if (GetBranchStatus("dySig1_seg")==1)         std::fill_n(dySig1_seg_, nCand, -99.);
    if (GetBranchStatus("ddxdz1_seg")==1)         std::fill_n(ddxdz1_seg_, nCand, -99.);
    if (GetBranchStatus("ddydz1_seg")==1)         std::fill_n(ddydz1_seg_, nCand, -99.);
    if (GetBranchStatus("ddxdzSig1_seg")==1)      std::fill_n(ddxdzSig1_seg_, nCand, -99.);
    if (GetBranchStatus("ddydzSig1_seg")==1)      std::fill_n(ddydzSig1_seg_, nCand, -99.);
    if (GetBranchStatus("OneStMuon2")==1)         std::fill_n(OneStMuon2_, nCand, 0);
    if (GetBranchStatus("PFMuon2")==1)            std::fill_n(PFMuon2_, nCand, 0);
    if (GetBranchStatus("GlbMuon2")==1)           std::fill_n(GlbMuon2_, nCand, 0);
    if (GetBranchStatus("trkMuon2")==1)           std::fill_n(trkMuon2_, nCand, 0);
    if (GetBranchStatus("tightMuon2")==1)         std::fill_n(tightMuon2_, nCand, 0);
    if (GetBranchStatus("softMuon2")==1)          std::fill_n(softMuon2_, nCand, 0);
    if (GetBranchStatus("hybridMuon2")==1)        std::fill_n(hybridMuon2_, nCand, 0);
    if (GetBranchStatus("HPMuon2")==1)            std::fill_n(HPMuon2_, nCand, 0);
    if (GetBranchStatus("trigMuon2")==1 && trigMuon2_) trigMuon2_->clear();
    if (GetBranchStatus("nMatchedStationD2")==1)  std::fill_n(nMatchedStationD2_, nCand, -1);
    if (GetBranchStatus("nTrackerLayerD2")==1)    std::fill_n(nTrackerLayerD2_, nCand, -1);
    if (GetBranchStatus("nPixelLayerD2")==1)      std::fill_n(nPixelLayerD2_, nCand, -1);
    if (GetBranchStatus("nPixelHitD2")==1)        std::fill_n(nPixelHitD2_, nCand, -1);
    if (GetBranchStatus("nMuonHitD2")==1)         std::fill_n(nMuonHitD2_, nCand, -1);
    if (GetBranchStatus("GlbTrkChiD2")==1)        std::fill_n(GlbTrkChiD2_, nCand, 99.);
    if (GetBranchStatus("muondZD2")==1)           std::fill_n(muondZD2_, nCand, -99.);
    if (GetBranchStatus("muondXYD2")==1)          std::fill_n(muondXYD2_, nCand, -99.);
    if (GetBranchStatus("dZD2")==1)               std::fill_n(dZD2_, nCand, -99.);
    if (GetBranchStatus("dXYD2")==1)              std::fill_n(dXYD2_, nCand, -99.);
    if (GetBranchStatus("nMatchedChamberD2")==1)  std::fill_n(nMatchedChamberD2_, nCand, -1);
    if (GetBranchStatus("EnergyDepositionD2")==1) std::fill_n(EnergyDepositionD2_, nCand, -1.);
    if (GetBranchStatus("dx2_seg")==1)            std::fill_n(dx2_seg_, nCand, -99.);
    if (GetBranchStatus("dy2_seg")==1)            std::fill_n(dy2_seg_, nCand, -99.);
    if (GetBranchStatus("dxSig2_seg")==1)         std::fill_n(dxSig2_seg_, nCand, -99.);
    if (GetBranchStatus("dySig2_seg")==1)         std::fill_n(dySig2_seg_, nCand, -99.);
    if (GetBranchStatus("ddxdz2_seg")==1)         std::fill_n(ddxdz2_seg_, nCand, -99.);
    if (GetBranchStatus("ddydz2_seg")==1)         std::fill_n(ddydz2_seg_, nCand, -99.);
    if (GetBranchStatus("ddxdzSig2_seg")==1)      std::fill_n(ddxdzSig2_seg_, nCand, -99.);
    if (GetBranchStatus("ddydzSig2_seg")==1)      std::fill_n(ddydzSig2_seg_, nCand, -99.);

    // CLEAR GEN INFO VARIABLES
    const auto& nGen = (candSize_gen_>0 ? candSize_gen_ : NGEN);
    if (GetBranchStatus("weight_gen")==1)   weight_gen_ = -99.;
    if (GetBranchStatus("candSize_gen")==1) candSize_gen_ = 0;
    if (GetBranchStatus("pT_gen")==1)       std::fill_n(pT_gen_, nGen, -1.);
    if (GetBranchStatus("eta_gen")==1)      std::fill_n(eta_gen_, nGen, -9.);
    if (GetBranchStatus("y_gen")==1)        std::fill_n(y_gen_, nGen, -9.);
    if (GetBranchStatus("status_gen")==1)   std::fill_n(status_gen_, nGen, -9);
    if (GetBranchStatus("PID_gen")==1)      std::fill_n(PID_gen_, nGen, -999);
    if (GetBranchStatus("MotherID_gen")==1) std::fill_n(MotherID_gen_, nGen, -999);
    if (GetBranchStatus("RecIdx_gen")==1)   std::fill_n(RecIdx_gen_, nGen, -1);
    if (GetBranchStatus("3DPointingAngle_gen")==1) std::fill_n(V3DPointingAngle_gen_, nGen, -1);
    if (GetBranchStatus("2DPointingAngle_gen")==1) std::fill_n(V2DPointingAngle_gen_, nGen, -1);
    if (GetBranchStatus("3DDecayLength_gen")==1) std::fill_n(V3DDecayLength_gen_, nGen, -1);
    if (GetBranchStatus("2DDecayLength_gen")==1) std::fill_n(V2DDecayLength_gen_, nGen, -1);
    if (GetBranchStatus("PIDD1_gen")==1)    std::fill_n(PIDD1_gen_, nGen, -999);
    if (GetBranchStatus("chargeD1_gen")==1) std::fill_n(chargeD1_gen_, nGen, -9);
    if (GetBranchStatus("pTD1_gen")==1)     std::fill_n(pTD1_gen_, nGen, -1.);
    if (GetBranchStatus("EtaD1_gen")==1)    std::fill_n(EtaD1_gen_, nGen, -9.);
    if (GetBranchStatus("PhiD1_gen")==1)    std::fill_n(PhiD1_gen_, nGen, -9.);
    if (GetBranchStatus("PIDD2_gen")==1)    std::fill_n(PIDD2_gen_, nGen, -999);
    if (GetBranchStatus("chargeD2_gen")==1) std::fill_n(chargeD2_gen_, nGen, -9);
    if (GetBranchStatus("pTD2_gen")==1)     std::fill_n(pTD2_gen_, nGen, -1.);
    if (GetBranchStatus("EtaD2_gen")==1)    std::fill_n(EtaD2_gen_, nGen, -9.);
    if (GetBranchStatus("PhiD2_gen")==1)    std::fill_n(PhiD2_gen_, nGen, -9.);
};

void VertexCompositeTree::GenerateDictionaries(void)
{
    const std::string& dic = "vector<vector<UChar_t>>;vector<UChar_t>";
    const std::string& CWD = getcwd(NULL, 0);
    const std::string& incP = gInterpreter->GetIncludePath();
    if (incP.rfind(CWD+"/cpp")!=std::string::npos)  return;
    gSystem->mkdir((CWD+"/cpp").c_str());
    gSystem->ChangeDirectory((CWD+"/cpp").c_str());
    gInterpreter->AddIncludePath((CWD+"/cpp").c_str()); // Needed to find the new dictionaries
    gInterpreter->GenerateDictionary(dic.c_str(), "vector");
    gSystem->ChangeDirectory(CWD.c_str());
};

Bool_t VertexCompositeTree::tightMuon1(const UInt_t& iC, const std::string& type)
{
    if      (type==""   ) { return tightMuon1()[iC]; } 
    else if (type=="Y15") {
        return ( GlbMuon1()[iC] && (GlbTrkChiD1()[iC] < 10.) &&
                (nMuonHitD1()[iC] > 0) && (nMatchedStationD1()[iC] > 1) &&
                (nPixelHitD1()[iC] > 0) && (nTrackerLayerD1()[iC] > 5) &&
                (fabs(muondXYD1()[iC]) < 0.2) && (fabs(muondZD1()[iC]) < 0.5) );
    }
    else if (type=="POG") {
        return ( GlbMuon1()[iC] && PFMuon1()[iC] && (GlbTrkChiD1()[iC] < 10.) &&
                (nMuonHitD1()[iC] > 0) && (nMatchedStationD1()[iC] > 1) &&
                (nPixelHitD1()[iC] > 0) && (nTrackerLayerD1()[iC] > 5) &&
                (fabs(muondXYD1()[iC]) < 0.2) && (fabs(muondZD1()[iC]) < 0.5) );
    }
    else { std::cout << "[ERROR] Tight MuonID is not defined for " << type << std::endl; }
    return false;
};

Bool_t VertexCompositeTree::tightMuon2(const UInt_t& iC, const std::string& type)
{
    if      (type==""   ) { return tightMuon2()[iC]; }
    else if (type=="Y15") {
        return ( GlbMuon2()[iC] && (GlbTrkChiD2()[iC] < 10.) &&
                (nMuonHitD2()[iC] > 0) && (nMatchedStationD2()[iC] > 1) &&
                (nPixelHitD2()[iC] > 0) && (nTrackerLayerD2()[iC] > 5) &&
                (fabs(muondXYD2()[iC]) < 0.2) && (fabs(muondZD2()[iC]) < 0.5) );
    }
    else if (type=="POG") {
        return ( GlbMuon2()[iC] && PFMuon2()[iC] && (GlbTrkChiD2()[iC] < 10.) &&
                (nMuonHitD2()[iC] > 0) && (nMatchedStationD2()[iC] > 1) &&
                (nPixelHitD2()[iC] > 0) && (nTrackerLayerD2()[iC] > 5) &&
                (fabs(muondXYD2()[iC]) < 0.2) && (fabs(muondZD2()[iC]) < 0.5) );
    }
    else { std::cout << "[ERROR] Tight MuonID is not defined for " << type << std::endl; }
    return false;
};

Bool_t VertexCompositeTree::hybridMuon1(const UInt_t& iC, const std::string& type)
{
    if      (type==""   ) { return (hybridMuon1()[iC] && trkMuon1()[iC]); }
    else if (type=="Y15") {
        return ( GlbMuon1()[iC] && OneStMuon1()[iC] &&
                (nPixelLayerD1()[iC] > 0) && (nTrackerLayerD1()[iC] > 5) &&
                (fabs(dXYD1()[iC]) < 0.3) && (fabs(dZD1()[iC]) < 20.) );
    }
    else if (type=="Y18") {
        return ( GlbMuon1()[iC] && trkMuon1()[iC] &&
                (nPixelLayerD1()[iC] > 0) && (nTrackerLayerD1()[iC] > 5) &&
                (fabs(dXYD1()[iC]) < 0.3) && (fabs(dZD1()[iC]) < 20.) );
    }
    else { std::cout << "[ERROR] Hybrid MuonID is not defined for " << type << std::endl; }
    return false;
};

Bool_t VertexCompositeTree::hybridMuon2(const UInt_t& iC, const std::string& type)
{
    if      (type==""   ) { return (hybridMuon2()[iC] && trkMuon2()[iC]); }
    else if (type=="Y15") {
        return ( GlbMuon2()[iC] && OneStMuon2()[iC] &&
                (nPixelLayerD2()[iC] > 0) && (nTrackerLayerD2()[iC] > 5) &&
                (fabs(dXYD2()[iC]) < 0.3) && (fabs(dZD2()[iC]) < 20.) );
    }
    else if (type=="Y18") {
        return ( GlbMuon2()[iC] && trkMuon2()[iC] &&
                (nPixelLayerD2()[iC] > 0) && (nTrackerLayerD2()[iC] > 5) &&
                (fabs(dXYD2()[iC]) < 0.3) && (fabs(dZD2()[iC]) < 20.) );
    }
    else { std::cout << "[ERROR] Hybrid MuonID is not defined for " << type << std::endl; }
    return false;
};

Bool_t VertexCompositeTree::softMuon1(const UInt_t& iC, const std::string& type)
{
    if      (type==""   ) { return softMuon1()[iC]; }
    else if (type=="POG") {
        return ( OneStMuon1()[iC] && HPMuon1()[iC] &&
                (nPixelLayerD1()[iC] > 0) && (nTrackerLayerD1()[iC] > 5) &&
                (fabs(dXYD1()[iC]) < 0.3) && (fabs(dZD1()[iC]) < 20.) );
    }
    else { std::cout << "[ERROR] Soft MuonID is not defined for " << type << std::endl; }
    return false;
};

Bool_t VertexCompositeTree::softMuon2(const UInt_t& iC, const std::string& type)
{
    if      (type==""   ) { return softMuon2()[iC]; }
    else if (type=="POG") {
        return ( OneStMuon2()[iC] && HPMuon2()[iC] &&
                (nPixelLayerD2()[iC] > 0) && (nTrackerLayerD2()[iC] > 5) &&
                (fabs(dXYD2()[iC]) < 0.3) && (fabs(dZD2()[iC]) < 20.) );
    }
    else { std::cout << "[ERROR] Soft MuonID is not defined for " << type << std::endl; }
    return false;
};

Double_t VertexCompositeTree::phiAsym(const UInt_t& iC)
{
    const auto& pT1 = pTD1()[iC];
    const auto& phi1 = PhiD1()[iC];
    const auto& pT2 = pTD2()[iC];
    const auto& phi2 = PhiD2()[iC];
    TVector3 pTV1; pTV1.SetPtEtaPhi(pT1, 0.0, phi1);
    TVector3 pTV2; pTV2.SetPtEtaPhi(pT2, 0.0, phi2);
    const auto& pTVDif = 0.5*(pTV1 - pTV2);
    const auto& pTVSum = (pTV1 + pTV2);
    return pTVSum.Angle(pTVDif);
};

#endif
