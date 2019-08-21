#include "/Users/syang/work/run2/dimuon/DiMuonAnalysis2019/Utilities/Ntuple/VertexCompositeTree.h"
#include <iostream>
  
void test()
{
  const auto& inputFile = "/Users/syang/work/run2/dimuon/DiMuonAnalysis2019/rootfiles/VertexCompositeTree_HIDoubleMuon_HIRun2018_04Apr2019_DiMuMassMin7.root";
  const auto& treeDir = "dimucontana"; // For MC use dimucontana_mc
  // Extract the tree
  VertexCompositeTree tree;
  if (!tree.GetTree(inputFile, treeDir)) { std::cout << "Invalid tree!" << std::endl; return; }
  // Loop over the tree
  for(Long64_t jentry=1; jentry<tree.GetEntries(); jentry++)
  {
    // Get the entry
    cout<<endl;
    cout<<"chain entry: "<<jentry<<endl;
    Int_t entryStatus = tree.GetEntry(jentry);
    cout<<"tree.GetEntry(jentry) status: "<<entryStatus<<endl;
    if (entryStatus<0) { std::cout << "Invalid entry!" << std::endl; return; }
    // Loop over the candidates
    for(uint iCand=0; iCand<tree.candSize(); iCand++)
    {
      // Print the candidate mass
      std::cout << "Candidate " << iCand << " has mass: " << tree.mass()[iCand] << std::endl;
    }
    // Stop after 10 events
    if (jentry>1) break;
  }
}
