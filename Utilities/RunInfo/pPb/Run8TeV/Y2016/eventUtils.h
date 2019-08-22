#ifndef pPb_Run8TeV_Y2016_eventUtils_H_
#define pPb_Run8TeV_Y2016_eventUtils_H_

#include<map>

namespace pPb {
    namespace R8TeV {
        namespace Y2016 {
            // Trigger
            enum TRIGGERBIT {
                HLT_PAL1DoubleMuOpen = 0,
                HLT_PAL3Mu12 = 1,
                HLT_PAFullTracks_Multiplicity120 = 2,
                HLT_PAFullTracks_Multiplicity150 = 3,
                HLT_PAFullTracks_Multiplicity185 = 4,
                HLT_PAFullTracks_Multiplicity250 = 5,
                HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack = 6,
                HLT_INVALID = 7,
            };
            static const std::map< TRIGGERBIT , std::string > TRIGNAME =
            {
                { HLT_PAL1DoubleMuOpen                      , "HLT_PAL1DoubleMuOpen"                      },
                { HLT_PAL3Mu12                              , "HLT_PAL3Mu12"                              },
                { HLT_PAFullTracks_Multiplicity120          , "HLT_PAFullTracks_Multiplicity120"          },
                { HLT_PAFullTracks_Multiplicity150          , "HLT_PAFullTracks_Multiplicity150"          },
                { HLT_PAFullTracks_Multiplicity185          , "HLT_PAFullTracks_Multiplicity185"          },
                { HLT_PAFullTracks_Multiplicity250          , "HLT_PAFullTracks_Multiplicity250"          },
                { HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack , "HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack" },
            };
            std::string HLTPath(const TRIGGERBIT& bit)
            {
                if (TRIGNAME.count(bit)) { return TRIGNAME.at(bit); }
                assert(Form("[ERROR] HLT bit %d is invalid", bit));
                return "HLT_INVALID";
            }; 
            TRIGGERBIT HLTBit(const std::string& path)
            {
                for (const auto& trg : TRIGNAME) { if (trg.second==path) { return trg.first; } }
                assert(Form("[ERROR] HLT path %s is invalid", path.c_str()));
                return HLT_INVALID;
            };
            std::map<uint, std::string> HLTBits()
            {
                std::map<uint, std::string> trigBits;
                for (const auto& t : TRIGNAME) { trigBits[t.first] = ""; }
                trigBits.at(HLT_PAL1DoubleMuOpen) = "DiMuon";
                trigBits.at(HLT_PAL3Mu12) = "Muon";
                return trigBits;
            };
            static const std::map< std::string , std::vector<uint> > PDTRIG =
            {
                { "MUON"      , { HLT_PAL3Mu12 } },
                { "DIMUON"    , { HLT_PAL1DoubleMuOpen } },
                { "HIGHMULT"  , { HLT_PAFullTracks_Multiplicity185 } },
                { "HIGHMULT2" , { HLT_PAFullTracks_Multiplicity250 } },
                { "MINBIAS"   , { HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack } },
            };
            std::vector<uint> HLTBitsFromPD(const std::string& PD)
            {
                if (PDTRIG.count(PD)) { return PDTRIG.at(PD); }
                assert(Form("[ERROR] PD name %s is invalid", PD.c_str()));
                return {};
            };
            // Event Selection
            enum FILTERBIT {
                colEvtSel = 0,
                hfCoincFilter = 1,
                primaryVertexFilter = 2,
                NoScraping = 3,
                pileupVertexFilterCut = 4,
                pileupVertexFilterCutGplus =5,
            };
            // Luminosity
            static const std::map< TRIGGERBIT , std::vector<double> > TRIGLUMI =
            {
                { HLT_PAL1DoubleMuOpen                      , { 173.42*0.64 , 173.42*0.36 , 173.42 } },
                { HLT_PAL3Mu12                              , { 173.20*0.64 , 173.20*0.36 , 173.20 } },
                { HLT_PAFullTracks_Multiplicity120          , {   2.48*0.64 ,   2.48*0.36 ,   2.48 } },
                { HLT_PAFullTracks_Multiplicity150          , {   5.64*0.64 ,   5.64*0.36 ,   5.64 } },
                { HLT_PAFullTracks_Multiplicity185          , {  93.85*0.64 ,  93.85*0.36 ,  93.85 } },
                { HLT_PAFullTracks_Multiplicity250          , { 172.12*0.64 , 172.12*0.36 , 172.12 } },
                { HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack , {   4.04*0.64 ,   4.04*0.36 ,   4.04 } },
            };
            double LumiFromHLTBit(const TRIGGERBIT& bit, const std::string& col)
            {
                if (TRIGLUMI.count(bit)) {
                    if      (col=="pPb8Y16") { return TRIGLUMI.at(bit)[0]; }
                    else if (col=="Pbp8Y16") { return TRIGLUMI.at(bit)[1]; }
                    else if (col=="PA8Y16" ) { return TRIGLUMI.at(bit)[2]; }
                    assert(Form("[ERROR] System %s is invalid", col.c_str()));
                }
                assert(Form("[ERROR] HLT bit %d is invalid", bit));
                return 0.0;
            };
            static const std::map< std::string , std::vector<double> > PDLUMI =
            {
                { "MUON"      , TRIGLUMI.at(HLT_PAL3Mu12) },
                { "DIMUON"    , TRIGLUMI.at(HLT_PAL1DoubleMuOpen) },
                { "HIGHMULT"  , TRIGLUMI.at(HLT_PAFullTracks_Multiplicity185) },
                { "HIGHMULT2" , TRIGLUMI.at(HLT_PAFullTracks_Multiplicity250) },
                { "MINBIAS"   , TRIGLUMI.at(HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack) },
            };
            double LumiFromPD(const std::string& PD, const std::string& col)
            {
                if (PDLUMI.count(PD)) {
                    if      (col=="pPb8Y16") { return PDLUMI.at(PD)[0]; }
                    else if (col=="Pbp8Y16") { return PDLUMI.at(PD)[1]; }
                    else if (col=="PA8Y16" ) { return PDLUMI.at(PD)[2]; }
                    assert(Form("[ERROR] System %s is invalid", col.c_str()));
                }
                assert(Form("[ERROR] PD %s is invalid", PD.c_str()));
                return 0.0;
            };
        };
    };
};


#endif /* pPb_Run8TeV_Y2016_eventUtils_H_ */
