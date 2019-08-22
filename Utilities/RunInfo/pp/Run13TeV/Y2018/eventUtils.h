#ifndef pp_Run13TeV_Y2018_eventUtils_H_
#define pp_Run13TeV_Y2018_eventUtils_H_

#include<map>

namespace pp {
    namespace R13TeV {
        namespace Y2018 {
            // Trigger
            enum TRIGGERBIT {
                HLT_FullTrack_Multiplicity85  = 0,
                HLT_FullTrack_Multiplicity100 = 1,
                HLT_FullTrack_Multiplicity130 = 2,
                HLT_FullTrack_Multiplicity155 = 3,
                HLT_L1MinimumBiasHF_OR = 4,
                HLT_L1DoubleMu0 = 5,
                HLT_INVALID = 6,
            };
            static const std::map< TRIGGERBIT , std::string > TRIGNAME =
            {
                { HLT_FullTrack_Multiplicity85  , "HLT_FullTrack_Multiplicity85"  },
                { HLT_FullTrack_Multiplicity100 , "HLT_FullTrack_Multiplicity100" },
                { HLT_FullTrack_Multiplicity130 , "HLT_FullTrack_Multiplicity130" },
                { HLT_FullTrack_Multiplicity155 , "HLT_FullTrack_Multiplicity155" },
                { HLT_L1MinimumBiasHF_OR        , "HLT_L1MinimumBiasHF_OR"        },
                { HLT_L1DoubleMu0               , "HLT_L1DoubleMu0"               },
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
                trigBits.at(HLT_L1DoubleMu0) = "DiMuon";
                return trigBits;
            };
            static const std::map< std::string , std::vector<uint> > PDTRIG =
            {
                { "DIMUON"    , { HLT_L1DoubleMu0 } },
                { "HIGHMULT"  , { HLT_FullTrack_Multiplicity100 } },
                { "HIGHMULT2" , { HLT_FullTrack_Multiplicity155 } },
                { "MINBIAS"   , { HLT_L1MinimumBiasHF_OR } },
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
                primaryVertexFilter = 1,
                NoScraping = 2,
                pileupVertexFilterCut = 3,
                pileupVertexFilterCutGplus = 4,
            };
            // Luminosity
            static const std::map< TRIGGERBIT , double > TRIGLUMI =
            {
                { HLT_FullTrack_Multiplicity85  , 1.41  },
                { HLT_FullTrack_Multiplicity100 , 1.41  },
                { HLT_FullTrack_Multiplicity130 , 0.31  },
                { HLT_FullTrack_Multiplicity155 , 10.22 },
                { HLT_L1MinimumBiasHF_OR        , 0.034 },
                { HLT_L1DoubleMu0               , 29.37 },
            };
            double LumiFromHLTBit(const TRIGGERBIT& bit)
            {
                if (TRIGLUMI.count(bit)) { return TRIGLUMI.at(bit); }
                assert(Form("[ERROR] HLT bit %d is invalid", bit));
                return 0.0;
            };
            static const std::map< std::string , double > PDLUMI =
            {
                { "DIMUON"    , TRIGLUMI.at(HLT_L1DoubleMu0) },
                { "HIGHMULT"  , TRIGLUMI.at(HLT_FullTrack_Multiplicity100) },
                { "HIGHMULT2" , TRIGLUMI.at(HLT_FullTrack_Multiplicity155) },
                { "MINBIAS"   , TRIGLUMI.at(HLT_L1MinimumBiasHF_OR) },
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


#endif /* pp_Run13TeV_Y2017_eventUtils_H_ */
