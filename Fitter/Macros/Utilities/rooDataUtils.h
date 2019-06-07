#ifndef rooDataUtils_h
#define rooDataUtils_h

#include "TSystem.h"
#include "TFile.h"
#include "TObject.h"
#include "TIterator.h"
#include "TH1.h"

#include "RooFit.h"
#include "RooMsgService.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooAbsPdf.h"
#include "RooFitResult.h"
#include "RooStringVar.h"
#include "RooList.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>

#include "initClasses.h"
#include "../../../Utilities/dataUtils.h"


void getRange(std::vector<double>& range, const TH1D& hist, const int& nMaxBins)
{
  // 1) Find the bin with the maximum Y value
  const auto& binMaximum = hist.GetMaximumBin();
  // 2) Loop backward and find the first bin
  int binWithContent = -1;
  int firstBin = 1;
  for (int i = binMaximum; i > 0; i--) {
    if (hist.GetBinContent(i) > 0.0) {
      if ( (binWithContent > 0) && ((binWithContent-i) > nMaxBins) && (hist.GetBinContent(i) < 5.0) ) { firstBin = binWithContent; break; }
      else { binWithContent = i; }
    }
  }
  // 3) Loop forward and find the last bin
  binWithContent = -1;
  int lastBin = hist.GetNbinsX();
  for (int i = binMaximum; i < hist.GetNbinsX(); i++) {
    if (hist.GetBinContent(i) > 0.0) {
      if ( ( binWithContent > 0) && ((i - binWithContent) > nMaxBins) && (hist.GetBinContent(i) < 5.0) ) { lastBin = binWithContent+1; break; }
      else { binWithContent = i; }
    }
  }
  // 4) Build the set of bins
  const auto& startBin = ( (firstBin > 1) ? (firstBin - 1) : firstBin );
  const auto& nNewBins = lastBin - startBin + 1;
  double binning[nNewBins+2];
  binning[0] = hist.GetXaxis()->GetXmin();
  binning[nNewBins+1] = hist.GetXaxis()->GetXmax();
  for (int i = 1; i <= nNewBins; i++) {
    int iBin = startBin + i;
    binning[i] = hist.GetBinLowEdge(iBin);
  }
  // 5) Save the bin range
  range.push_back(binning[(firstBin>1)?1:0]);
  range.push_back(binning[nNewBins]);
  //
  return;
};


void addString(RooWorkspace& ws, const std::string& strName, const std::string& strVal, const bool& asObj=true)
{
  if (ws.obj(strName.c_str())) {
    const auto& strVar = dynamic_cast<RooStringVar*>(ws.obj(strName.c_str()));
    if (strVar) { strVar->setVal(strVal.c_str()); }
  }
  else if (ws.arg(strName.c_str())) {
    const auto& strVar = dynamic_cast<RooStringVar*>(ws.arg(strName.c_str()));
    if (strVar) { strVar->setVal(strVal.c_str()); }
  }
  else {
    RooStringVar strVar(strName.c_str(), strName.c_str(), strVal.c_str(), strVal.length()+2);
    if (asObj) { ws.import(*dynamic_cast<TObject*>(&strVar), strVar.GetTitle()); }
    else { ws.import(strVar); }
  }
};


std::string getString(const RooWorkspace& ws, const std::string& strName)
{
  std::string strVal = "";
  if (ws.obj(strName.c_str())) {
    const auto& strVar = dynamic_cast<RooStringVar*>(ws.obj(strName.c_str()));
    if (strVar) { strVal = strVar->getVal(); }
  }
  else if (ws.arg(strName.c_str())) {
    const auto& strVar = dynamic_cast<RooStringVar*>(ws.arg(strName.c_str()));
    if (strVar) { strVal = strVar->getVal(); }
  }
  return strVal;
};


bool setConstant(RooWorkspace& ws, const std::string& parName, const bool& CONST)
{
  if (ws.var(parName.c_str())) { 
    ws.var(parName.c_str())->setConstant(CONST);
    if (CONST) { std::cout << "[INFO] Setting parameter " << parName << " : " << ws.var(parName.c_str())->getVal() << " to constant!" << std::endl; }
  }
  else if (!ws.function(parName.c_str())) { 
    std::cout << "[ERROR] Parameter " << parName << " was not found!" << std::endl;
    return false;
  }
  return true;
};


void setFixedVarsToContantVars(RooWorkspace& ws)
{
  const auto& listVar = ws.allVars();
  auto parIt = std::unique_ptr<TIterator>(listVar.createIterator());
  for (auto itp = parIt->Next(); itp!=NULL; itp = parIt->Next() ) {
    const auto& it = dynamic_cast<RooRealVar*>(itp); if (!it) continue;
    if (it->getMin()==it->getMax() && !it->isConstant()) {
      setConstant(ws, it->GetName(), kTRUE);
    }
  }
};


void saveSnapshot(RooWorkspace& ws, const RooArgSet& set, const std::string& snapName)
{
  if (snapName=="") { std::cout << "[ERROR] Snapshot name is empty!" << std::endl; return; }
  // Register the snapshot in WS
  ws.defineSet(snapName.c_str(), set, kTRUE);
  addString(ws, snapName, snapName, false);
  ws.extendSet("snapshots", snapName.c_str());
  // Save snapshot
  ws.saveSnapshot(snapName.c_str(), set, kTRUE);
};


void saveSnapshot(RooWorkspace& ws, const std::string& snapName, const std::string& dsName="")
{
  if (snapName=="") { std::cout << "[ERROR] Snapshot name is empty!" << std::endl; return; }
  // Add the variables and categories
  RooArgSet set(ws.allVars(), snapName.c_str()); set.add(ws.allCats());
  // Remove those from the dataset
  if (dsName!="") { set.remove(*ws.data(dsName.c_str())->get(), false, true); }
  // Save the snapshot
  saveSnapshot(ws, set, snapName);
};


void clearWorkspace(RooWorkspace& inWS, const std::string& smp="All", const bool& delVar=true)
{
  //
  const auto& level = RooMsgService::instance().globalKillBelow();
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
  // Copy all category variables
  if (inWS.allCats().getSize()>0 && delVar) {
    const auto& listCat = inWS.allCats();
    auto catIt = std::unique_ptr<TIterator>(listCat.createIterator());
    for (auto it = catIt->Next(); it!=NULL; it = catIt->Next()) { inWS.RecursiveRemove(it); if(it) delete it; }
  }
  // Clear all category functions
  if (inWS.allCatFunctions().getSize()>0 && delVar) {
    const auto& listFnc = inWS.allCatFunctions();
    auto fncIt = std::unique_ptr<TIterator>(listFnc.createIterator());
    for (auto it = fncIt->Next(); it!=NULL; it = fncIt->Next()) { inWS.RecursiveRemove(it); if(it) delete it; }
  }
  // Clear all variables
  if (inWS.allVars().getSize()>0 && delVar) {
    const auto& listVar = inWS.allVars();
    auto parIt = std::unique_ptr<TIterator>(listVar.createIterator());
    for (auto it = parIt->Next(); it!=NULL; it = parIt->Next()) { inWS.RecursiveRemove(it); if(it) delete it; }
  }
  // Clear all functions
  if (inWS.allFunctions().getSize()>0 && delVar) {
    const auto& listFnc = inWS.allFunctions();
    auto fncIt = std::unique_ptr<TIterator>(listFnc.createIterator());
    for (auto it = fncIt->Next(); it!=NULL; it = fncIt->Next()) { inWS.RecursiveRemove(it); if(it) delete it; }
  }
  // Clear all PDFs
  if (inWS.allPdfs().getSize()>0) {
    const auto& listPdf = inWS.allPdfs();
    auto pdfIt = std::unique_ptr<TIterator>(listPdf.createIterator());
    for (auto it = pdfIt->Next(); it!=NULL; it = pdfIt->Next()) { inWS.RecursiveRemove(it); if(it) delete it; }
  }
  // Clear all Datasets
  if (inWS.allData().size()>0 && smp!="") {
    const auto& listData = inWS.allData();
    for (const auto& it : listData) { if (smp=="All" || std::string(it->GetName()).find(smp)!=std::string::npos) { inWS.RecursiveRemove(it); if(it) delete it; } }
  }
  // Clear all Embedded Datasets
  if (inWS.allEmbeddedData().size()>0 && smp!="") {
    const auto& listData = inWS.allEmbeddedData();
    for (const auto& it : listData) { if (smp=="All" || std::string(it->GetName()).find(smp)!=std::string::npos) { inWS.RecursiveRemove(it); if(it) delete it; } }
  }
  // Clear all Generic Objects
  if (inWS.allGenericObjects().size()>0) {
    const auto& listObj = inWS.allGenericObjects();
    for (const auto& it : listObj) { inWS.RecursiveRemove(it); if(it) delete it; }
  }
  // Clear RooStudyManager
  inWS.clearStudies();
  // Clear any remaining component
  if (inWS.components().getSize()>0 && delVar) {
    auto cmpIt = std::unique_ptr<TIterator>(inWS.componentIterator());
    for (auto it = cmpIt->Next(); it!=NULL; it = cmpIt->Next()) { inWS.RecursiveRemove(it); if(it) delete it; }
  }
  // Return the RooMessenger Level
  RooMsgService::instance().setGlobalKillBelow(level);
};


void copyWorkspace(RooWorkspace& outWS, const RooWorkspace& inWS, const std::string& smp="All", const bool& addVar=true, const bool& addSnap=true, const bool& addPDF=true, const bool& addObj=true)
{
  //
  const auto& level = RooMsgService::instance().globalKillBelow();
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
  // Copy all category variables
  if (inWS.allCats().getSize()>0 && addVar) {
    const auto& listCat = inWS.allCats();
    auto catIt = std::unique_ptr<TIterator>(listCat.createIterator());
    for (auto itp = catIt->Next(); itp!=NULL; itp = catIt->Next()) { const auto& it = dynamic_cast<RooCategory*>(itp); if (it && !outWS.cat(it->GetName())) { outWS.import(*it); } }
  }
  // Copy all category functions
  if (inWS.allCatFunctions().getSize()>0 && addVar) {
    const auto& listFnc = inWS.allCatFunctions();
    auto fncIt = std::unique_ptr<TIterator>(listFnc.createIterator());
    for (auto itp = fncIt->Next(); itp!=NULL; itp = fncIt->Next()) { const auto& it = dynamic_cast<RooCategory*>(itp); if (it && !outWS.catfunc(it->GetName())) { outWS.import(*it); } }
  }
  // Copy all variables
  if (inWS.allVars().getSize()>0 && addVar) {
    const auto& listVar = inWS.allVars();
    auto parIt = std::unique_ptr<TIterator>(listVar.createIterator());
    for (auto itp = parIt->Next(); itp!=NULL; itp = parIt->Next()) { const auto& it = dynamic_cast<RooRealVar*>(itp); if (it && !outWS.var(it->GetName())) { outWS.import(*it); } }
  }
  // Copy all functions
  if (inWS.allFunctions().getSize()>0 && addVar) {
    const auto& listFnc = inWS.allFunctions();
    auto fncIt = std::unique_ptr<TIterator>(listFnc.createIterator());
    for (auto itp = fncIt->Next(); itp!=NULL; itp = fncIt->Next()) { const auto& it = dynamic_cast<RooRealVar*>(itp); if (it && !outWS.function(it->GetName())) { outWS.import(*it); } }
  }
  // Copy all PDFs
  if (inWS.allPdfs().getSize()>0 && addPDF) {
    const auto& listPdf = inWS.allPdfs();
    auto pdfIt = std::unique_ptr<TIterator>(listPdf.createIterator());
    for (auto itp = pdfIt->Next(); itp!=NULL; itp = pdfIt->Next() ) { const auto& it = dynamic_cast<RooAbsPdf*>(itp); if (it && !outWS.pdf(it->GetName())) { outWS.import(*it, RooFit::RecycleConflictNodes()); } }
  }
  // Copy all Datasets
  if (inWS.allData().size()>0 && smp!="") {
    const auto& listData = inWS.allData();
    for (const auto& it : listData) { if (!outWS.data(it->GetName()) && (smp=="All" || std::string(it->GetName()).find(smp)!=std::string::npos)) { outWS.import(*it); } }
  }
  // Copy all Embedded Datasets
  if (inWS.allEmbeddedData().size()>0 && smp!="") {
    const auto& listData = inWS.allEmbeddedData();
    for (const auto& it : listData) { if (!outWS.embeddedData(it->GetName()) && (smp=="All" || std::string(it->GetName()).find(smp)!=std::string::npos)) { outWS.import(*it, RooFit::Embedded(1)); } }
  }
  // Copy all Generic Objects
  if (inWS.allGenericObjects().size()>0 && addObj) {
    const auto& listObj = inWS.allGenericObjects();
    for (const auto& it : listObj) { if (!outWS.genobj(it->GetTitle())) { outWS.import(*it, it->GetTitle()); } }
  }
  // Copy all snapshots variables
  if (const_cast<RooWorkspace*>(&inWS)->set("snapshots") && addSnap) {
    const auto& listSnap = const_cast<RooWorkspace*>(&inWS)->set("snapshots");
    auto snapIt = std::unique_ptr<TIterator>(listSnap->createIterator());
    for (auto its = snapIt->Next(); its!=NULL; its = snapIt->Next()) {
      const auto& it = dynamic_cast<RooStringVar*>(its);
      if (it && !outWS.set(it->GetName())) { saveSnapshot(outWS, *inWS.getSnapshot(it->GetName()), it->GetName()); }
    }
  }
  // Return the RooMessenger Level
  RooMsgService::instance().setGlobalKillBelow(level);
};


bool saveWorkSpace(const RooWorkspace& ws, const std::string& outputDir, const std::string& fileName, const bool& saveDS=true)
{
  // Create the output file
  gSystem->mkdir(outputDir.c_str(), kTRUE);
  auto file = std::unique_ptr<TFile>(new TFile((outputDir+fileName).c_str(), "RECREATE"));
  if (!file || !file->IsOpen() || file->IsZombie()) {
    std::cout << "[ERROR] Output root file with fit results could not be created!" << std::endl; if (file) { file->Close(); }; return false;
  }
  file->cd();
  // Save the workspace
  if (saveDS) { ws.Write("workspace"); }
  else {
    RooWorkspace tmpWS;
    copyWorkspace(tmpWS, ws, (saveDS ? "All" : ""));
    tmpWS.Write("workspace");
  }
  // Save the snapshots for later checks (BUG FIX)
  if (const_cast<RooWorkspace*>(&ws)->set("snapshots")) {
    const auto& listSnap = const_cast<RooWorkspace*>(&ws)->set("snapshots");
    auto snapIt = std::unique_ptr<TIterator>(listSnap->createIterator());
    for (auto its = snapIt->Next(); its!=NULL; its = snapIt->Next()) {
      const auto& it = dynamic_cast<RooStringVar*>(its); if (!it) continue;
      const auto& snap = ws.getSnapshot(it->GetName());
      if (snap) { snap->Write(it->GetName()); }
    }
  }
  // Write and close output file
  file->Write(); file->Close();
  std::cout << "[INFO] RooWorkspace saved in: " << (outputDir+fileName) << std::endl;
  return true;
};


bool getWorkSpace(RooWorkspace& ws, const std::string& fileName, const std::string& getDS="All", const bool& onlySnap=false)
{
  // Check if input file exists
  if (gSystem->AccessPathName(fileName.c_str())) {
    std::cout << "[INFO] File: " << fileName << " not found!" << std::endl; return false;
  }
  // Open the input file
  auto file = std::unique_ptr<TFile>(TFile::Open(fileName.c_str()));
  if (!file || !file->IsOpen() || file->IsZombie()) {
    std::cout << "[ERROR] File: " << fileName << " is corrupted!" << std::endl; if(file) { file->Close(); }; return false;
  }
  file->cd();
  TH1::AddDirectory(kFALSE); // Avoid adding objects to memory
  if (onlySnap) {
    TIter keyIt(file->GetListOfKeys());
    for (auto itk = keyIt(); itk!=NULL; itk = keyIt() ) {
      const auto& key = dynamic_cast<TKey*>(itk); if (!key) continue;
      if (std::string(key->GetClassName())!="RooArgSet") continue;
      const auto& snap = dynamic_cast<RooArgSet*>(key->ReadObj());
      if (snap) { saveSnapshot(ws, *snap, key->GetName()); }
    }
  }
  else {
    const auto& inWS = dynamic_cast<RooWorkspace*>(file->Get("workspace"));
    if (!inWS) { std::cout << "[ERROR] Workspace in: " << fileName << " not found!" << std::endl; file->Close(); return false; }
    copyWorkspace(ws, *inWS, getDS);
  }
  // Close input file
  file->Close();
  return true;
};


bool isCompatibleDataset(const RooDataSet& ds, const RooDataSet& ref, const bool& checkRange=true)
{
  // Check that the DataSets have the same number of events
  if (ds.numEntries()!=ref.numEntries()){ std::cout << "[ERROR] DataSet disagreement in number of events : "
                                                    << ds.GetName() << " (" << ds.numEntries() << ") and "
                                                    << ref.GetName() << " (" << ref.numEntries() << ") !" << std::endl; return false; }
  if (ds.sumEntries()!=ref.sumEntries()){ std::cout << "[ERROR] DataSet disagreement in sum of weights : "
                                                    << ds.GetName() << " (" << ds.sumEntries() << ") and "
                                                    << ref.GetName() << " (" << ref.sumEntries() << ") !" << std::endl; return false; }
  // Check that the input DataSet have the same variables and distributions as the reference
  const auto& listVar = ref.get();
  auto parIt = std::unique_ptr<TIterator>(listVar->createIterator());
  for (auto itp = parIt->Next(); itp!=NULL; itp = parIt->Next()) {
    const auto& it = dynamic_cast<RooRealVar*>(itp); if (!it) continue;
    if ( ds.get()->find(it->GetName()) == NULL ) { std::cout << "[ERROR] DataSet " << ds.GetName() << " does not contain the variable " << it->GetName() << " !" << std::endl; return false; }
    if (checkRange) {
      const auto& var = static_cast<RooRealVar*>(ds.get()->find(it->GetName()));
      if ( it->getMin() != var->getMin() ) { std::cout << "[ERROR] " << it->GetName() << " Min Range disagreement : "
						       << ds.GetName() << " ( " << var->getMin() << " ) " << " and "
						       << ref.GetName() << " ( " << it->getMin() << " ) ! " << std::endl; return false; }
      if ( it->getMax() != var->getMax() ) { std::cout << "[ERROR] " << it->GetName() << " Max Range disagreement : "
						       << ds.GetName() << " ( " << var->getMax() << " ) " << " and "
						       << ref.GetName() << " ( " << it->getMax() << " ) ! " << std::endl; return false; }
    }
    for (uint i = 1; i < 5; i++) {
      const auto& rMom = ref.moment(*it, i);
      const auto& dMom = ds.moment(*it, i);
      if (rMom!=dMom) { std::cout << "[ERROR] " << it->GetName() << " " << i << " Moment Value disagreement : "
				  << ds.GetName() << " ( " << dMom << " ) " << " and "
				  << ref.GetName() << " ( " << rMom << " ) " << " ! " << std::endl; return false; }
    }
  }
  // DataSets are compatible if they passed all tests
  std::cout << "[INFO] DataSets " << ds.GetName() << " and " << ref.GetName() << " are compatible!" << std::endl; return true;
};


bool compareSnapshots(const RooArgSet& pars1, const RooArgSet& pars2)
{
  auto parIt = std::unique_ptr<TIterator>(pars1.createIterator());
  for (auto itp = parIt->Next(); itp!=NULL; itp = parIt->Next() ) {
    const auto& it = dynamic_cast<RooRealVar*>(itp); if (!it) continue;
    const auto& val = pars2.getRealValue(it->GetName(),-1e99);
    if (val==-1e99) return false;           // the parameter was not found!
    if (val != it->getVal()) return false;  // the parameter was found, but with a different value!
    if ( ((RooRealVar&)pars2[it->GetName()]).getMin() != it->getMin() ) return false;  // the parameter has different lower limit
    if ( ((RooRealVar&)pars2[it->GetName()]).getMax() != it->getMax() ) return false;  // the parameter has different upper limit
  }
  return true;
};


bool isFitAlreadyFound(const RooArgSet& newpars, const std::string& fileName, const std::string& snapName="initialParameters")
{
  RooWorkspace ws; if (!getWorkSpace(ws, fileName, "", true)) { return false; }
  const RooArgSet* params = (ws.set(snapName.c_str()) ? ws.getSnapshot(snapName.c_str()) : NULL);
  if (!params) { std::cout << "[INFO] Snapshot " << snapName << " was not found!" << std::endl; return false; }
  return compareSnapshots(newpars, *params);
};


RooRealVar getVar(const RooArgSet& set, const std::string& varName)
{
  if (set.find(varName.c_str())) {
    const auto& it = dynamic_cast<RooRealVar*>(set.find(varName.c_str()));
    if (it) { return *it; }
  }
  std::cout << "[WARNING] Variable " << varName << " was not found in the set, return empty RooRealVar" << std::endl;
  return RooRealVar();
};


void updateParameterRange(RooWorkspace& ws, GlobalInfo&  info, const std::string& chg, const std::string& DSTAG, const std::string& var="Cand_Mass", const double& maxRange=-1.0)
{
  // Check if maxRange is the same as current range, otherwise return
  if (maxRange==ws.var(var.c_str())->getMax()) { return; }
  //
  const std::string& dsName = ( "d" + chg + "_" + DSTAG );
  //
  double varMin , varMax;
  varMin = ws.var(var.c_str())->getMin(); varMax = ws.var(var.c_str())->getMax();
  if (maxRange > 0.0) { varMax = maxRange; }
  else {
    ws.data(dsName.c_str())->getRange(*ws.var(var.c_str()), varMin, varMax);
  }
  const std::string& varFitRange = Form("(%g <= %s && %s < %g)", varMin, var.c_str(), var.c_str(), varMax);
  //
  if (ws.data(dsName.c_str())->reduce(varFitRange.c_str())->numEntries() <= ws.data(dsName.c_str())->numEntries()) {
    const auto& nBins = getNBins(varMin, varMax, ws.var(var.c_str())->getBinWidth(0));
    ws.var(var.c_str())->setRange("FitWindow", varMin, varMax);
    ws.var(var.c_str())->setBins(nBins, "FitWindow");
    //
    auto dataToFit = std::unique_ptr<RooDataSet>(dynamic_cast<RooDataSet*>(ws.data(dsName.c_str())->reduce(varFitRange.c_str())->Clone((dsName+"_FIT").c_str())));
    ws.import(*dataToFit);
    info.Var.at("numEntries").at(chg) = ws.data((dsName+"_FIT").c_str())->sumEntries();
    ws.factory(Form("numEntries_%s_FIT[%.0f]", dsName.c_str(), info.Var.at("numEntries").at(chg)));
    //
    std::cout << Form("[INFO] %s range was updated to : %g <= %s < %g", var.c_str(), varMin, var.c_str(), varMax) << std::endl;
  }
  //
  return;
};


bool loadParameterRange(GlobalInfo&  info, const std::string& var, const std::string& fileName, const std::string& snap="fittedParameters")
{
  RooWorkspace ws; if (!getWorkSpace(ws, fileName, "", true)) { return false; }
  ws.loadSnapshot(snap.c_str());
  if (ws.var(var.c_str())) {
    info.Var.at(var).at("Min") = ws.var(var.c_str())->getMin();
    info.Var.at(var).at("Max") = ws.var(var.c_str())->getMax();
  }
  else { std::cout << "[ERROR] Variable " << var << " was not found!" << std::endl; return false; }
  return true;
};


bool loadParameters(RooWorkspace& ws, const std::string& fileName, const std::string& varName="*", const std::string& col="", 
		    const std::string& dsName="", const std::string& snap="fittedParameters")
{
  std::cout << "[INFO] Loading model parameters from snapshot " << snap << " in " << fileName << std::endl;
  RooWorkspace inWS; if (!getWorkSpace(inWS, fileName, dsName, (dsName==""))) { return false; }
  const auto& inDS = dynamic_cast<RooDataSet*>(inWS.data(dsName.c_str()));
  const auto& ds = dynamic_cast<RooDataSet*>(ws.data(dsName.c_str()));
  if (dsName!="" && (!inDS || !ds)) {
    std::cout << "[INFO] loadParameters: RooDataset " << dsName << " was not found!" << std::endl; return false;
  }
  else if (dsName!="" && !isCompatibleDataset(*ds, *inDS)) {
    std::cout << "[INFO] loadParameters: RooDataset " << dsName << " is different!" << std::endl; return false;
  }
  const auto& params = inWS.getSnapshot(snap.c_str());
  if (!params) { std::cout << "[INFO] Snapshot " << snap << " was not found!" << std::endl; return false; }
  auto parIt = std::unique_ptr<TIterator>(params->createIterator());
  for (auto itp = parIt->Next(); itp!=NULL; itp = parIt->Next() ) {
    const auto& it = dynamic_cast<RooRealVar*>(itp); if (!it) continue;
    const std::string& name = it->GetName();
    if ((varName!="*" && name.rfind(varName,0)!=0) || (col!="" && name.rfind(col)==std::string::npos)) continue;
    if (ws.var(name.c_str())) {
      ws.var(name.c_str())->setVal(it->getVal());
      ws.var(name.c_str())->setError(it->getError());
      ws.var(name.c_str())->setMin(it->getMin());
      ws.var(name.c_str())->setMax(it->getMax());
    }
    else {
      ws.factory((name+Form("[ %.2f, %.2f, %.2f ]", it->getVal(), it->getMin(), it->getMax())).c_str());
    }
    std::cout << "[INFO] Parameter " << name << " set to " << it->getVal() << "+-" << it->getError() << std::endl;
  }
  return true;
};


bool loadSPlotDS(RooWorkspace& myws, const std::string& fileName, const std::string& dsName)
{
  RooWorkspace ws; if (!getWorkSpace(ws, fileName)) { return false; }
  if (ws.data(dsName.c_str())) {
    myws.import(*ws.data(dsName.c_str()), RooFit::Rename((dsName+"_INPUT").c_str()));
    if (myws.data((dsName+"_INPUT").c_str())) { std::cout << "[INFO] RooDataset " << (dsName+"_INPUT") << " was imported!" << std::endl; }
    else { std::cout << "[ERROR] Importing RooDataset " << (dsName+"_INPUT") << " failed!" << std::endl; }
  }
  else { std::cout << "[ERROR] RooDataset " << dsName << " was not found!" << std::endl; return false; }
  return true;
};


bool createBinnedDataset(RooWorkspace& ws, const std::string& var="Cand_Mass")
{
  //
  const std::string& DSTAG = getString(ws, "DSTAG");
  const std::string& chg   = getString(ws, "fitCharge");
  const std::string& dsName = ( "d" + chg + "_" + DSTAG );
  //
  if (ws.data(dsName.c_str())==NULL) { std::cout << "[WARNING] DataSet " << dsName << " was not found!" << std::endl; return false; }
  if (ws.data(dsName.c_str())->numEntries()<=10.0) { std::cout << "[WARNING] DataSet " << dsName << " has too few events!" << std::endl; return false; }
  if (ws.var(var.c_str())==NULL) { std::cout << "[WARNING] Variable " << var << " was not found!" << std::endl; return false; }
  //
  const auto& min  = ws.var(var.c_str())->getMin();
  const auto& max  = ws.var(var.c_str())->getMax();
  const uint& nBin = ws.var(var.c_str())->getBins();
  //
  // Create the histogram
  auto histName = dsName + "_" + var;
  histName.replace(histName.find("d"), std::string("d").length(), "h");
  std::unique_ptr<TH1D> hist = std::unique_ptr<TH1D>(dynamic_cast<TH1D*>(ws.data(dsName.c_str())->createHistogram(histName.c_str(), *ws.var(var.c_str()), RooFit::Binning(nBin, min, max))));
  if (hist==NULL) { std::cout << "[WARNING] Histogram " << histName << " is NULL!" << std::endl; return false; }
  // Cleaning the input histogram
  // 1) Remove the Under and Overflow bins
  hist->ClearUnderflowAndOverflow();
  // 2) Set negative bin content to zero
  for (int i=0; i<=hist->GetNbinsX(); i++) { if (hist->GetBinContent(i)<0.0) { hist->SetBinContent(i, 0.0); } }
  // 2) Reduce the range of histogram and rebin it
  //hist.reset(dynamic_cast<TH1D*>(rebinhist(*hist, range[1], range[2])));
  if (hist==NULL) { std::cout << "[WARNING] Cleaned Histogram of " << histName << " is NULL!" << std::endl; return false; }
  const auto& dataName = (dsName +"_"+var+"_FIT");
  std::unique_ptr<RooDataHist> dataHist = std::unique_ptr<RooDataHist>(new RooDataHist(dataName.c_str(), "", *ws.var(var.c_str()), hist.get()));
  if (dataHist==NULL) { std::cout << "[WARNING] DataHist used to create " << dsName << " failed!" << std::endl; return false; }
  if (dataHist->sumEntries()==0) { std::cout << "[WARNING] DataHist used to create " << dsName << " is empty!" << std::endl; return false; }
  if (std::abs(dataHist->sumEntries() - hist->GetSumOfWeights())>0.001) { std::cout << "[ERROR] DataHist used to create " << dsName << "  " << " is invalid!  " << std::endl; return false; }
  ws.import(*dataHist);
  ws.var(var.c_str())->setBins(nBin); // Bug Fix
  return true;
};

//
//---------------------------------------------------------------------------------------------
//


std::string findLabel(const std::string& par, const std::string& obj, const std::string& chg,
		      const std::string& col, const std::string& cha, const GlobalInfo& info)
{
  std::string tryLabel="";
  const StringVector_t tryChannel = { cha , "" };
  StringVector_t trySystem  = { col , "" };
  if (info.Flag.at("doPA8Y16")) { trySystem.push_back("PA8Y16"); }
  const StringVector_t tryCharge  = { chg , "" };
  for (const auto& tryCha : tryChannel) {
    bool trySuccess = false;
    for (const auto& tryCol : trySystem) {
      for (const auto& tryChg : tryCharge) {
	tryLabel = obj + tryCha + tryChg + (tryCol!="" ? "_"+tryCol : "");
	if (contain(info.Par, par+"_"+tryLabel)) { trySuccess = true; break; }
      }
      if (trySuccess) break;
    }
    if (trySuccess) break;
  }
  return tryLabel;
};


bool setModel(StringDiMap_t& model, GlobalInfo&  info, const std::string& type="Cand_Mass")
{
  const auto& cha = info.Par.at("channel");
  for (const auto& col : info.StrS.at("fitSystem")) {
    for (const auto& obj : info.StrS.at("fitObject")) {
      for (const auto& chg : info.StrS.at("fitCharge")) {
        const std::string& label = Form("Model_%s_%s", (obj+cha+chg).c_str(), col.c_str());
        info.StrS["tags"].insert(obj+cha+chg+"_"+col);
	const std::string inputLabel = "Model_"+findLabel("Model", obj, chg, col, cha, info);
        if (contain(info.Par, inputLabel)) {
          const auto& value = info.Par.at(inputLabel);
          info.Par[label] = value;
          StringVector_t k;
          if (value.find("+")!=std::string::npos) { splitString(k, value, "+"); }
          else { k.push_back(value); }
          for (auto& kk : k) {
            std::string modelName = kk;
            StringVector_t p;
            if (kk.find("[")!=std::string::npos) { 
              modelName = kk.substr(0, kk.find("["));
              kk.erase(0, kk.find("[")+std::string("[").length());
              if (kk.rfind("]")==std::string::npos) { std::cout << "[ERROR] Missing ']' in model: " << value << std::endl; return false; }
              kk = kk.substr(0, kk.rfind("]"));
              if (kk.find(";")!=std::string::npos) { splitString(p, kk, ";"); }
              else if (kk.find(",")!=std::string::npos) { splitString(p, kk, ","); }
              else { p.push_back(kk); }
            }
            else { p.push_back(obj); }
            for (const auto& ll : p) {
              std::string objectName  = Form("%s", (ll+cha+chg).c_str());;
              if (info.Flag.at("fitMC") && ll!=obj && modelName=="TEMP") continue;
              if (modelName=="TEMP") { modelName = "Template";    }
              if (modelName=="MJET") { modelName = "MultiJetBkg"; }
              if (!contain(ModelDictionary, modelName) || ModelDictionary.at(modelName)==0) {
                std::cout << "[ERROR] The " << (ll+cha+chg) << " " << type << " model: " << modelName << " is invalid" << std::endl; return false;
              }
              model[label][ll] = modelName;
              if (modelName == "Template") {
                std::string dsTag = ( "MC_" + ll + "_" + info.Par.at("channelDS") + "_" + col );
                info.StrS[Form("TEMPDS_%s_%s", (obj+cha+chg).c_str(), col.c_str())].insert(dsTag);
              }
              info.StrS["addObjectModel_"+(obj+cha+chg)+"_"+col].insert(ll);
            }
          }
        } else {
          std::cout << "[ERROR] " << (obj+cha+chg) << " " << type << " model for " << col << " was not found in the initial parameters!" << std::endl; return false;
        }
      }
    }
  }
  return true;
};


void setDSParamaterRange(RooWorkspace& ws, const std::string& dsName, const GlobalInfo& info)
{
  const auto& ds = dynamic_cast<RooDataSet*>(ws.data(dsName.c_str()));
  if (!ds) { std::cout << "[ERROR] Dataset " << dsName << " was not found!" << std::endl; return; }
  const auto& row = ds->get();
  ws.defineSet(("SET_"+dsName).c_str(), *row);
  for (const auto& var : info.Var) {
    const bool& isAbs = (var.first.find("Abs")!=std::string::npos);
    auto varN = var.first; if (isAbs) { varN.erase(varN.find("Abs"), 3); }
    if (row->find(varN.c_str()) && (contain(var.second, "Min"))) {
      const auto& v = dynamic_cast<RooRealVar*>(row->find(varN.c_str()));
      v->setMin(isAbs ? -var.second.at("Max") : var.second.at("Min"));
      v->setMax(var.second.at("Max"));
    }
  }
};


void setDefaultRange(RooWorkspace& ws, const GlobalInfo& info)
{
  for (const auto& v : info.Var) {
    if (!ws.var(v.first.c_str()) || !contain(v.second, "Default_Min")) continue;
    ws.var(v.first.c_str())->setRange("DEFAULT", v.second.at("Default_Min"), v.second.at("Default_Max"));
  }
};


bool setFitParameterRange(RooWorkspace& ws, const GlobalInfo& info)
{
  // Store the default range
  setDefaultRange(ws, info);
  // Set the fit parameter range
  for (const auto& var : info.StrS.at("fitVariable")) {
    if (!ws.var(var.c_str())) { std::cout << "[ERROR] Parameter " << var << " does not exist, failed to set fit parameter range!" << std::endl; return false; }
    ws.var(var.c_str())->setRange("FitWindow", info.Var.at(var).at("Min"), info.Var.at(var).at("Max"));
    const auto& nBins = getNBins(info.Var.at(var).at("Min"), info.Var.at(var).at("Max"), info.Var.at(var).at("binWidth"));
    ws.var(var.c_str())->setBins(nBins, "FitWindow");
    ws.var(var.c_str())->setBins(nBins);
  }
  return true;
};


int importDataset(RooWorkspace& myws, GlobalInfo& info, const RooWorkspaceMap_t& inputWS, const std::string& chg)
{
  // Check info container
  if (!contain(info.StrS, "dsList")) { std::cout << "[ERROR] DSList was not found while importing dataset!" << std::endl; return -1; }
  for (const auto& inWS : inputWS) { if (inWS.second.allData().empty()) { std::cout << "[ERROR] Input workspace " << inWS.first << " is empty!" << std::endl; return -1; } }
  //
  // Define the selection string
  std::string cutDS = "";
  info.StrS["cutPars"].clear();
  for (const auto& it : inputWS.at(*info.StrS.at("dsList").begin()).allData()) {
    const std::string& inDS = it->GetName();
    if (inDS.rfind("d"+chg+"_",0)!=0) continue;
    const auto& dsVars = it->get();
    for (const auto& p : info.Par) {
      if (dsVars->find(p.first.c_str()) && p.second!="") { cutDS += Form("(%s == %s::%s) && ", p.first.c_str(), p.first.c_str(), p.second.c_str()); info.StrS.at("cutPars").insert(p.first); }
    }
    for (const auto& v : info.Var) {
      const bool& isAbs = (v.first.find("Abs")!=std::string::npos);
      auto varN = v.first; if (isAbs) { varN.erase(varN.find("Abs"), 3); }
      if (!dsVars->find(varN.c_str())) continue;
      if (v.second.at("Min")==v.second.at("Default_Min") && v.second.at("Max")==v.second.at("Default_Max")) continue;
      if (v.first=="Centrality" && (inDS.rfind("PbPb")==std::string::npos || info.Par.at("PD")=="UPC")) continue;
      if (isAbs) { varN = "abs("+varN+")"; }
      if (v.second.at("Min")==v.second.at("Max")) { cutDS += Form("(%s == %g) && ", varN.c_str(), v.second.at("Max")); }
      else if (v.second.at("Min")==v.second.at("Default_Min")) { cutDS += Form("(%s < %g) && ", varN.c_str(), v.second.at("Max")); }
      else if (v.second.at("Max")==v.second.at("Default_Max")) { cutDS += Form("(%g <= %s) && ", v.second.at("Min"), varN.c_str()); }
      else { cutDS += Form("(%g <= %s && %s < %g) && ", v.second.at("Min"), varN.c_str(), varN.c_str(), v.second.at("Max")); }
      info.StrS.at("cutPars").insert(v.first);
    }
    break;
  }
  cutDS = cutDS.substr(0, cutDS.rfind(" && "));
  addString(myws, "cutDS", cutDS); // Save the cut expression for bookkeeping
  std::cout << "[INFO] Importing local RooDataSets with cuts: " << cutDS << std::endl;
  //
  // Reduce and import the datasets
  for (const auto& labelT : info.StrS.at("dsList")) {
    auto label = labelT;
    const bool& isSwapDS = (labelT.find("Swap_")!=std::string::npos);
    if (isSwapDS) { stringReplace(label, "Swap", ""); }
    // Extract the RooDatasets
    std::string dsType = "";
    for (auto type = info.StrV.at("dsType").rbegin(); type != info.StrV.at("dsType").rend(); ++type) {
      if (inputWS.at(label).data(Form("d%s_%s_%s", chg.c_str(), type->c_str(), label.c_str()))) { dsType = *type; break; }
    }
    if (dsType=="") { std::cout << "[ERROR] Sample " << label << " was not found!" << std::endl; return -1; }
    const auto& extLabel = dsType + "_" + label;
    std::cout << "[INFO] Importing local RooDataSet " << extLabel << std::endl;
    //
    if (chg!="") {
      const auto& dsName = "d"+chg+"_"+labelT;
      const auto& dsExtName = "d"+chg+"_"+extLabel;
      if (!myws.data(dsName.c_str())) {
        if (!contain(inputWS, label) || !inputWS.at(label).data(dsExtName.c_str())){ 
          std::cout << "[ERROR] The dataset " <<  dsExtName << " was not found!" << std::endl; return -1;
        }
        const auto& cutDST = cutDS + (isSwapDS ? "&&(Cand_IsSwap==Cand_IsSwap::Yes)" : "");
        auto data = std::unique_ptr<RooDataSet>(dynamic_cast<RooDataSet*>(inputWS.at(label).data(dsExtName.c_str())->reduce(RooFit::Cut(cutDST.c_str()), RooFit::Name(dsName.c_str()), RooFit::Title(dsName.c_str()))));
        if (!data || data->sumEntries()==0){
          if (extLabel.rfind("MC_",0)==0 || chg=="SS") {
            std::cout << "[WARNING] No events from dataset " <<  dsExtName << " passed the kinematic cuts!" << std::endl;
          }
          else { std::cout << "[ERROR] No events from dataset " <<  dsExtName << " passed the kinematic cuts!" << std::endl; return -1; }
        }
        else {
          myws.import(*data);
	  if (!myws.data(dsName.c_str())) { std::cout << "[ERROR] Importing RooDataSet " <<  dsName << " failed!" << std::endl; return -1; }
	  copyWorkspace(myws, inputWS.at(label), "", false, false, true); // Copy the generic objects
        }
        std::cout << "[INFO] " << data->numEntries() << " entries imported from local RooDataSet " << dsName << std::endl;
        // Set the range of each global parameter in the local roodataset
	if (myws.data(dsName.c_str())) { setDSParamaterRange(myws, dsName, info); }
      }
      //
      const auto& dsSPLOTInputName = (dsName+"_SPLOT_INPUT");
      if (myws.data(dsSPLOTInputName.c_str())) {
        auto data = std::unique_ptr<RooDataSet>(dynamic_cast<RooDataSet*>(myws.data(dsSPLOTInputName.c_str())->reduce(cutDS.c_str())));
        // Set the range of each global parameter in the local roodataset
	setDSParamaterRange(myws, dsSPLOTInputName, info);
        if (data->sumEntries()==0){ std::cout << "[ERROR] No events from dataset " <<  dsSPLOTInputName << " passed the kinematic cuts!" << std::endl; }
        else if (!isCompatibleDataset(*data, *dynamic_cast<RooDataSet*>(myws.data(dsName.c_str())))) { cout << "[ERROR] sPlot and Original Datasets are inconsistent!" << std::endl; return -1; }
        else {
          const auto& dsSPLOTName = (dsName+"_SPLOT");
          data->SetName(dsSPLOTName.c_str());
          myws.import(*data, RooFit::Rename(dsSPLOTName.c_str()));
          if (myws.data(dsSPLOTName.c_str())) { std::cout << "[INFO] RooDataSet " << dsSPLOTName << " was imported!" << std::endl; }
          else { std::cout << "[ERROR] Importing RooDataSet " << dsSPLOTName << " failed!" << std::endl; return -1; }
          std::cout << "[INFO] SPlotDS Events: " << data->sumEntries() << " , origDS Events: " << myws.data(Form("dOS_%s", label.c_str()))->sumEntries() << std::endl;
        }
      }
    }
  }
  // Check if the user wants to use the Center-of-Mass frame
  for (auto& v : info.Var) {
    if (contain(info.Flag, "use"+v.first+"CM") && info.Flag.at("use"+v.first+"CM")) {
      myws.factory(Form("use%sCM[1.0]", v.first.c_str()));
    }
  }
  // Set the range of each global parameter in the local workspace
  for (const auto& var : info.Var) {
    const bool& isAbs = (var.first.find("Abs")!=std::string::npos);
    auto varN = var.first; if (isAbs) { varN.erase(varN.find("Abs"), 3); }
    if (myws.var(varN.c_str()) && contain(var.second, "Min")) {
      myws.var(varN.c_str())->setMin(isAbs ? -var.second.at("Max") : var.second.at("Min"));
      myws.var(varN.c_str())->setMax(var.second.at("Max"));
    }
    else if (!myws.var(var.first.c_str()) && contain(var.second, "Val")) {
      myws.factory(Form("%s[%.10f]", var.first.c_str(), var.second.at("Val")));
    }
    if (isAbs && !myws.var(var.first.c_str()) && contain(var.second, "Min")) {
      myws.factory(Form("%s[%.10f,%.10f,%.10f]", var.first.c_str(), 0.0, var.second.at("Min"), var.second.at("Max")));
    }
  }
  // Print bin information
  std::string binInfo = "[INFO] Analyzing bin:";
  for (const auto& var : info.StrS.at("cutPars")) {
    // Ignore if it is the fitting variable
    if (contain(info.StrS.at("fitVariable"), var)) continue;
    // Check if is an absolute value variable
    const bool& isAbs = (var.find("Abs")!=std::string::npos);
    auto varN = var; if (isAbs) { varN.erase(varN.find("Abs"), 3); }
    // Print the binning
    if (myws.var(varN.c_str())) {
      const auto& varMin = (isAbs ? info.Var.at(var).at("Min") : myws.var(varN.c_str())->getMin());
      const auto& varMax = myws.var(varN.c_str())->getMax();
      if (contain(info.Flag, "use"+var+"CM")  && info.Flag.at("use"+var+"CM")) {
	const bool& ispPb = (info.Flag.at("fitpPb8Y16") || info.Flag.at("fitPA8Y16"));
	binInfo += Form(" %g <= %s < %g ,", pPb::EtaLABtoCM(varMin, ispPb), (var+"CM").c_str(), pPb::EtaLABtoCM(varMax, ispPb));
      }
      else {
	binInfo += Form(" %g <= %s < %g ,", varMin, var.c_str(), varMax);
      }
    }
    else if (myws.cat(var.c_str())) {
      const auto& catLbl = myws.cat(var.c_str())->getLabel();
      binInfo += Form(" %s == %s ,", var.c_str(), catLbl);
    }
  }
  binInfo = binInfo.substr(0, binInfo.rfind(" ,"));
  std::cout << binInfo << std::endl;
  return 1;
};


void setFileName(std::string& fileName, std::string& outputDir, const StringSet_t& fitV, const std::string& DSTAG, const std::string& plotLabel, const GlobalInfo& info)
{
  const auto& dsTag  = DSTAG.substr(0, DSTAG.rfind("_DIMUON"));
  const auto& colTag = DSTAG.substr(DSTAG.find_last_of("_")+1);
  const auto& objTag = *info.StrS.at("fitObject").begin();
  std::string fitVar = "";
  for (const auto& v : fitV) { auto s = v; stringReplace(s, "_", ""); fitVar += s+"_"; }
  fitVar = fitVar.substr(0, fitVar.rfind("_"));
  outputDir = Form("%s%s/%s/%s/%s/", outputDir.c_str(), fitVar.c_str(), dsTag.c_str(), objTag.c_str(), colTag.c_str());
  std::string varLbl = "";
  for (const auto& var : info.StrS.at("cutPars")) {
    bool incVar = true;
    for (const auto& v : fitV) { if (var==v) { incVar = false; break; } }
    if (!incVar || !contain(info.Var, var) || !contain(info.Var.at(var), "Min")) continue;
    if (info.Var.at(var).at("Min")==info.Var.at(var).at("Default_Min") && info.Var.at(var).at("Max")==info.Var.at(var).at("Default_Max")) continue;
    if (var=="Centrality" && (DSTAG.rfind("PbPb")==std::string::npos || info.Par.at("PD")=="UPC")) continue;
    auto varN = var; stringReplace(varN, "_", "");
    auto varMin = info.Var.at(var).at("Min");
    auto varMax = info.Var.at(var).at("Max");
    if (contain(info.Flag, "use"+var+"CM") && info.Flag.at("use"+var+"CM")) {
      varN += "CM";
      const bool& ispPb = (info.Flag.at("fitpPb8Y16") || info.Flag.at("fitPA8Y16"));
      varMin = pPb::EtaLABtoCM(varMin, ispPb);
      varMax = pPb::EtaLABtoCM(varMax, ispPb);
    }
    if (varMin==varMax) { varLbl += Form("%s_%.0f_", varN.c_str(), varMax*100.); }
    else { varLbl += Form("%s_%.0f_%.0f_", varN.c_str(), varMin*100., varMax*100.); }
  }
  varLbl = varLbl.substr(0, varLbl.rfind("_"));
  fileName = Form("%s_%s_%s%s", fitVar.c_str(), dsTag.c_str(), plotLabel.c_str(), varLbl.c_str());
};
 

#endif // #ifndef rooDataUtils_h
