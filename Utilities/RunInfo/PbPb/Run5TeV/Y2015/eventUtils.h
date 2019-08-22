#ifndef PbPb_Run5TeV_Y2015_eventUtils_H_
#define PbPb_Run5TeV_Y2015_eventUtils_H_

#include<map>

namespace PbPb {
    namespace R5TeV {
        namespace Y2015 {
            // Trigger
            enum TRIGGERBIT {
                HLT_HIL1DoubleMu0 = 0,
                HLT_HIL1DoubleMu0_part = 1,
                HLT_HIL1DoubleMu0_2HF = 2,
                HLT_HIL1DoubleMu0_2HF0 = 3,
                HLT_HIL1DoubleMu0_2HF_Cent30100 = 4,
                HLT_HIL1DoubleMu0_2HF0_Cent30100 = 5,
                HLT_HIL1DoubleMu10 = 6,
                HLT_HIUPCL1DoubleMuOpenNotZDCAND = 7,
                HLT_HIUPCL1DoubleMuOpenNotHF2 = 8,
                HLT_HIL3Mu15 = 9,
                HLT_HIUPCL1MuOpenNotZDCAND = 10,
                HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack = 11,
                HLT_INVALID = 12,
            };
            static const std::map< TRIGGERBIT , std::string > TRIGNAME =
            {
                { HLT_HIL1DoubleMu0                        , "HLT_HIL1DoubleMu0"                        },
                { HLT_HIL1DoubleMu0_part                   , "HLT_HIL1DoubleMu0_part"                   },
                { HLT_HIL1DoubleMu0_2HF                    , "HLT_HIL1DoubleMu0_2HF"                    },
                { HLT_HIL1DoubleMu0_2HF0                   , "HLT_HIL1DoubleMu0_2HF0"                   },
                { HLT_HIL1DoubleMu0_2HF_Cent30100          , "HLT_HIL1DoubleMu0_2HF_Cent30100"          },
                { HLT_HIL1DoubleMu0_2HF0_Cent30100         , "HLT_HIL1DoubleMu0_2HF0_Cent30100"         },
                { HLT_HIL1DoubleMu10                       , "HLT_HIL1DoubleMu10"                       },
                { HLT_HIUPCL1DoubleMuOpenNotZDCAND         , "HLT_HIUPCL1DoubleMuOpenNotZDCAND"         },
                { HLT_HIUPCL1DoubleMuOpenNotHF2            , "HLT_HIUPCL1DoubleMuOpenNotHF2"            },
                { HLT_HIL3Mu15                             , "HLT_HIL3Mu15"                             },
                { HLT_HIUPCL1MuOpenNotZDCAND               , "HLT_HIUPCL1MuOpenNotZDCAND"               },
                { HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack , "HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack" },
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
                trigBits.at(HLT_HIL3Mu15) = "Muon";
                trigBits.at(HLT_HIUPCL1MuOpenNotZDCAND) = "Muon";
                trigBits.at(HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack) = "Muon";
                return trigBits;
            }; 
            static const std::map< std::string , std::vector<uint> > PDTRIG =
            {
                { "MUON"       , { HLT_HIL3Mu15 } },
                { "DIMUON"     , { HLT_HIL1DoubleMu0 , HLT_HIL1DoubleMu0_part } },
                { "DIMUONPERI" , { HLT_HIL1DoubleMu0_2HF_Cent30100 , HLT_HIL1DoubleMu0_2HF0_Cent30100 } },
                { "UPC"        , { HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack } },
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
                hfCoincFilter3 = 1,
                primaryVertexFilter = 2,
                clusterCompatibilityFilter = 3,
            };
            // Luminosity
            static const std::map< TRIGGERBIT , double > TRIGLUMI =
            {
                { HLT_HIL1DoubleMu0                        , 133.25 },
                { HLT_HIL1DoubleMu0_part                   , 215.01 },
                { HLT_HIL1DoubleMu0_2HF                    , 47.48  },
                { HLT_HIL1DoubleMu0_2HF0                   , 47.48  },
                { HLT_HIL1DoubleMu0_2HF_Cent30100          , 466.64 },
                { HLT_HIL1DoubleMu0_2HF0_Cent30100         , 466.64 },
                { HLT_HIL1DoubleMu10                       , 466.58 },
                { HLT_HIUPCL1DoubleMuOpenNotZDCAND         , 0.00   },
                { HLT_HIUPCL1DoubleMuOpenNotHF2            , 465.90 },
                { HLT_HIL3Mu15                             , 463.48 },
                { HLT_HIUPCL1MuOpenNotZDCAND               , 0.00   },
                { HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack , 456.27 },
            };
            double LumiFromHLTBit(const TRIGGERBIT& bit)
            {
                if (TRIGLUMI.count(bit)) { return TRIGLUMI.at(bit); }
                assert(Form("[ERROR] HLT bit %d is invalid", bit));
                return 0.0;
            };
            static const std::map< std::string , double > PDLUMI =
            {
                { "MUON"       , TRIGLUMI.at(HLT_HIL3Mu15) },
                { "DIMUON"     , (TRIGLUMI.at(HLT_HIL1DoubleMu0) + TRIGLUMI.at(HLT_HIL1DoubleMu0_part)) },
                { "DIMUONPERI" , TRIGLUMI.at(HLT_HIL1DoubleMu0_2HF_Cent30100) },
                { "UPC"        , TRIGLUMI.at(HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack) },
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


#endif /* PbPb_Run5TeV_Y2015_eventUtils_H_ */
