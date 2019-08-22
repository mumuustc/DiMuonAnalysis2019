#ifndef pp_Run5TeV_Y2017_eventUtils_H_
#define pp_Run5TeV_Y2017_eventUtils_H_

#include<map>

namespace pp {
    namespace R5TeV {
        namespace Y2017 {
            // Trigger
            enum TRIGGERBIT {
                HLT_HIL1DoubleMu0 = 0,
                HLT_HIL3Mu12 = 1,
                HLT_INVALID = 2,
            };
            static const std::map< TRIGGERBIT , std::string > TRIGNAME =
            {
                { HLT_HIL1DoubleMu0 , "HLT_HIL1DoubleMu0" },
                { HLT_HIL3Mu12      , "HLT_HIL3Mu12"      },
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
                trigBits[HLT_HIL1DoubleMu0] = "DiMuon";
                trigBits[HLT_HIL3Mu12] = "Muon";
                return trigBits;
            };
            static const std::map< std::string , std::vector<uint> > PDTRIG =
            {
                { "MUON"       , { HLT_HIL3Mu12} },
                { "DIMUON"     , { HLT_HIL1DoubleMu0 } },
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
                { HLT_HIL1DoubleMu0 , 294.07 },
                { HLT_HIL3Mu12      , 294.75 },
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
                { "DIMUON"     , TRIGLUMI.at(HLT_HIL1DoubleMu0) },
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


#endif /* pp_Run5TeV_Y2017_eventUtils_H_ */
