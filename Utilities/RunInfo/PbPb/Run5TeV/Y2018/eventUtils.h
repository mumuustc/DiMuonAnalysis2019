#ifndef PbPb_Run5TeV_Y2018_eventUtils_H_
#define PbPb_Run5TeV_Y2018_eventUtils_H_

#include<map>

namespace PbPb {
    namespace R5TeV {
        namespace Y2018 {
            // Trigger
            enum TRIGGERBIT {
                HLT_HIL1DoubleMuOpen_OS_Centrality_40_100 = 0,
                HLT_HIL1DoubleMuOpen_Centrality_50_100 = 1,
                HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf = 2,
                HLT_HIL1DoubleMu10 = 3,
                HLT_HIUPC_DoubleMu0_NotMBHF2AND = 4,
                HLT_HIL1MuOpen_Centrality_80_100 = 5,
                HLT_HIL3Mu12 = 6,
                HLT_HIUPC_SingleMuOpen_NotMBHF2AND = 7,
                HLT_INVALID = 8,
            };
            static const std::map< TRIGGERBIT , std::string > TRIGNAME =
            {
                { HLT_HIL1DoubleMuOpen_OS_Centrality_40_100 , "HLT_HIL1DoubleMuOpen_OS_Centrality_40_100" },
                { HLT_HIL1DoubleMuOpen_Centrality_50_100    , "HLT_HIL1DoubleMuOpen_Centrality_50_100"    },
                { HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf        , "HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf"        },
                { HLT_HIL1DoubleMu10                        , "HLT_HIL1DoubleMu10"                        },
                { HLT_HIUPC_DoubleMu0_NotMBHF2AND           , "HLT_HIUPC_DoubleMu0_NotMBHF2AND"           },
                { HLT_HIL1MuOpen_Centrality_80_100          , "HLT_HIL1MuOpen_Centrality_80_100"          },
                { HLT_HIL3Mu12                              , "HLT_HIL3Mu12"                              },
                { HLT_HIUPC_SingleMuOpen_NotMBHF2AND        , "HLT_HIUPC_SingleMuOpen_NotMBHF2AND"        },
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
                for (const auto& t : TRIGNAME) { trigBits[t.first] = "DiMuon"; }
                trigBits.at(HLT_HIL1MuOpen_Centrality_80_100) = "Muon";
                trigBits.at(HLT_HIL3Mu12) = "Muon";
                trigBits.at(HLT_HIUPC_SingleMuOpen_NotMBHF2AND) = "Muon";
                return trigBits;
            };
            static const std::map< std::string , std::vector<uint> > PDTRIG =
            {
                { "MUON"       , { HLT_HIL3Mu12 } },
                { "DIMUON"     , { HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf } },
                { "DIMUONPERI" , { HLT_HIL1DoubleMuOpen_OS_Centrality_40_100 } },
                { "UPC"        , { HLT_HIUPC_SingleMuOpen_NotMBHF2AND } },
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
                hfCoincFilter2Th4 = 1,
                primaryVertexFilter = 2,
                clusterCompatibilityFilter = 3,
            };
            // Luminosity
            static const std::map< TRIGGERBIT , double > TRIGLUMI =
            {
                { HLT_HIL1DoubleMuOpen_OS_Centrality_40_100 , 1531.80 },
                { HLT_HIL1DoubleMuOpen_Centrality_50_100    , 1545.91 },
                { HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf        , 1450.55 },
                { HLT_HIL1DoubleMu10                        , 1550.64 },
                { HLT_HIUPC_DoubleMu0_NotMBHF2AND           , 1583.79 },
                { HLT_HIL1MuOpen_Centrality_80_100          , 1550.64 },
                { HLT_HIL3Mu12                              , 1606.05 },
                { HLT_HIUPC_SingleMuOpen_NotMBHF2AND        , 1586.30 },
            };
            double LumiFromHLTBit(const TRIGGERBIT& bit)
            {
                if (TRIGLUMI.count(bit)) { return TRIGLUMI.at(bit); }
                assert(Form("[ERROR] HLT bit %d is invalid", bit));
                return 0.0;
            };
            static const std::map< std::string , double > PDLUMI =
            {
                { "MUON"       , TRIGLUMI.at(HLT_HIL3Mu12) },
                { "DIMUON"     , TRIGLUMI.at(HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf) },
                { "DIMUONPERI" , TRIGLUMI.at(HLT_HIL1DoubleMuOpen_OS_Centrality_40_100) },
                { "UPC"        , TRIGLUMI.at(HLT_HIUPC_SingleMuOpen_NotMBHF2AND) },
            };
            double LumiFromPD(const std::string& PD)
            {
                if (PDLUMI.count(PD)) { return PDLUMI.at(PD); }
                assert(Form("[ERROR] PD %s is invalid", PD.c_str()));
                return 0.0;
            };
        };
    };  
};


#endif /* PbPb_Run5TeV_Y2018_eventUtils_H_ */
