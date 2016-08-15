/*
 * ScanPositionPlots.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: Hovanes Egiyan
 */

#include "ScanPositionPlots.hh"

using namespace std;
using namespace Histo;

ClassImp( ScanPositionPlots )

string ScanPositionPlots::sppTreeName = "mcb_tree";

std::map<std::string, mcbTreeSelector*> ScanPositionPlots::sppSelectorMapInit = { { "sum", 0 }, { "dif", 0 },
        { "inc", 0 }, { "amo", 0 } };

ScanPositionPlots::ScanPositionPlots( const std::string rootFileNameBase ) :
        TDirectoryFile( removeDirectoryName( rootFileNameBase ).c_str(), rootFileNameBase.c_str(), "", gDirectory ) {
    this->cd();
    // Loop over all selectors in the map. The selector keys are inserted in the
    // declaration and the pointers are set to zero there.
    for ( auto& pairPlot : sppSelectorMap ) {
        auto& typeName = pairPlot.first;
        auto& dataPtr = pairPlot.second;

        // Open the root file from the simulation
        string rootFileName = getRootFileName( rootFileNameBase, typeName );
        sppRootFile = new TFile( rootFileName.c_str() );
        if ( sppRootFile == 0 ) {
            stringstream errMsg;
            errMsg << "ScanPositionPlots : could not open ROOT file " << rootFileName;
            throw runtime_error( errMsg.str() );
        }

        // Get the  ROOT tree
        TTree* rootTree = dynamic_cast<TTree*>( sppRootFile->Get( sppTreeName.c_str() ) );
        if ( rootTree == 0 ) {
            stringstream errMsg;
            errMsg << "ScanPositionPlots : could not find object object " << sppTreeName << " in ROOT file "
                    << rootFileName << " for simulation type " << typeName;
            throw runtime_error( errMsg.str().c_str() );
        }

        this->cd();
        // Create the data object for this type of simulation file and assign
        // the pointer to the new object to the corresponding map element.
        dataPtr = new mcbTreeSelector( typeName, rootTree );
        if ( dataPtr == 0 ) {
            stringstream errMsg;
            errMsg << "ScanPositionPlots : could not create selector object for " << typeName;
            throw runtime_error( errMsg.str().c_str() );
        }
    }
}

ScanPositionPlots::~ScanPositionPlots() {
    // Clear the data map. This should destroy the objects
    // kept in the map as well.
    this->cd();
    sppSelectorMap.clear();

// Close the ROOT file with the data
    if ( sppRootFile != 0 ) {
        sppRootFile->Close();
        delete sppRootFile;
        sppRootFile = 0;
    }
}

// Process the selectors. This will call tree->Process  with pointer for each
// selector.
void ScanPositionPlots::processSelectors( string option, Long64_t nEntries, Long64_t firstEntry ) {
    this->cd();
    cout << "Processing plots for  ScanPositionPlots " << GetName() << endl;
    for ( auto& pairPlot : sppSelectorMap ) {
        this->cd();
        mcbTreeSelector* selector = pairPlot.second;
        selector->processTree( option, nEntries, firstEntry );
        cout << "Selector " << string(selector->GetName()) << " has been processed" << endl;
        this->cd();
        selector->createScaledHistograms();
    }
    this->createHistograms();
    return;
}

// Static method that returns the ROOT file name for a given simulation type
std::string ScanPositionPlots::getRootFileName( const string baseName, const std::string type ) {
    string fileName = baseName + "." + type + "." + "root";
    return fileName;

}

// Assume that path contains the directory followed by a slash and base file name.
inline string ScanPositionPlots::removeDirectoryName( string path ) {
    size_t lastSlashPosition = path.rfind( "/" );
    path.replace( path.begin(), path.begin() + lastSlashPosition + 1, "" );
    return path;
}

// Get all the keys in the selector map. The keys are the different types
// of MCB simulation files or types of histograms like para, perp, pol etc.
vector<string> ScanPositionPlots::getSelectorMapKeys() {
    vector<string> typeList;
    for ( auto& iterSelector : sppSelectorMapInit ) {
        typeList.push_back( iterSelector.first );
    }
    return typeList;
}

void ScanPositionPlots::createHistograms() {
    cout << "Creating histograms for" << GetName() << endl;
    this->cd();

    makePerpPara();
    makeDiamond();
//    makeEnhamcement();
//    makeCoherentPolzrization();
//    makePolarization();
    return;
}

// Create PARA histograms, require sum and and dif files.
// This will loop through all histos in the "sum" selector,
// find the SCALED histograms and only for them it will
// create the PARA histograms.
void ScanPositionPlots::makePerpPara() {
    this->cd();
    /// Fist get the maps of histograms from the sum and dif selectors
    mcbTreeSelector* sumSelector = sppSelectorMap["sum"];
    mcbTreeSelector* difSelector = sppSelectorMap["dif"];
    if( sumSelector == nullptr || difSelector == nullptr ) return;
    auto& sumHistos = sumSelector->getHistoMap();
    auto& difHistos = difSelector->getHistoMap();

    // Loop through sum histograms
    for ( auto& sumAxesIter : sumHistos ) {
        string axesID = sumAxesIter.first;
        auto& sumHistoMap = sumAxesIter.second;
        for ( auto& sumCutIter : sumHistoMap ) {
            string cutID = sumCutIter.first;
            auto sumHistPtr = sumCutIter.second;
            // Check that the sum is a scaled histogram
            if ( dynamic_cast<WrapperTypeI<Histo::scaledString>*>( sumHistPtr ) != nullptr ) {
                if ( difHistos.count( axesID ) == 0 || difHistos[axesID].count( cutID ) == 0 ) continue;
                auto difHistPtr = difHistos[axesID][cutID];
                if ( dynamic_cast<WrapperTypeI<Histo::scaledString>*>( difHistPtr ) == nullptr ) continue;

                // Create PERP wrapper with histogram
                string perpAxesID = replaceHistoSuffix( axesID, Histo::scaledString, Histo::perpString );
                auto perpClassName = replaceClassName( *sumHistPtr, scaledString, perpString );
                if ( WrapperVirt::getBinaryOperatorMap().count( perpClassName ) > 0 ) {
                    histoMap[perpAxesID][cutID] = WrapperVirt::getBinaryOperatorMap()[perpClassName]( *sumHistPtr,
                            *difHistPtr, perpAxesID, cutID );
                    if ( histoMap[perpAxesID][cutID] != nullptr ) {
                        histoMap[perpAxesID][cutID]->getHist()->SetTitle( histoMap[perpAxesID][cutID]->getHist()->GetName() );
                        histoMap[perpAxesID][cutID]->getHist()->Add( difHistPtr->getHist(), -1 );
                    }
                }
                // Create PARA wrapper with histogram
                string paraAxesID = replaceHistoSuffix( axesID, Histo::scaledString, Histo::paraString );
                auto paraClassName = replaceClassName( *sumHistPtr, scaledString, paraString );
                if ( WrapperVirt::getBinaryOperatorMap().count( paraClassName ) > 0 ) {
                    histoMap[paraAxesID][cutID] = WrapperVirt::getBinaryOperatorMap()[paraClassName]( *sumHistPtr,
                            *difHistPtr, paraAxesID, cutID );
                    if ( histoMap[paraAxesID][cutID] != nullptr ) {
                        histoMap[paraAxesID][cutID]->getHist()->SetTitle( histoMap[paraAxesID][cutID]->getHist()->GetName() );
                        histoMap[paraAxesID][cutID]->getHist()->Add( difHistPtr->getHist(), -1 );
                    }
                }
            }
        }
    }
}


// Sum of the coherent sum and incoherent contributions
void ScanPositionPlots::makeDiamond() {
    this->cd();
    /// Fist get the maps of histograms from the sum and inc selectors
    mcbTreeSelector* sumSelector = sppSelectorMap["sum"];
    mcbTreeSelector* incSelector = sppSelectorMap["inc"];
    if( sumSelector == nullptr || incSelector == nullptr ) return;
    auto& sumHistos = sumSelector->getHistoMap();
    auto& incHistos = incSelector->getHistoMap();

    // Loop through sum histograms, only considering TH1D histos for DIAMOND
    for ( auto& sumAxesIter : sumHistos ) {
        string axesID = sumAxesIter.first;
        auto& sumHistoMap = sumAxesIter.second;
        for ( auto& sumCutIter : sumHistoMap ) {
            string cutID = sumCutIter.first;
            auto sumHistPtr = sumCutIter.second;
            if( dynamic_cast<TH1D*>(sumHistPtr->getHist()) == nullptr ) continue;
            // Check that the sum is a scaled histogram
            if ( dynamic_cast<WrapperTypeI<scaledString>*>( sumHistPtr ) != nullptr ) {
                cout << "Got the right type sum for " <<  sumHistPtr->getHist()->GetName() << endl;
                if (incHistos.count( axesID ) == 0 || incHistos[axesID].count( cutID ) == 0 ) continue;
                auto incHistPtr = incHistos[axesID][cutID];
                if ( dynamic_cast<WrapperTypeI<scaledString>*>( incHistPtr ) == nullptr ) continue;
                cout << "Got the right type inc for " <<  incHistPtr->getHist()->GetName() << endl;

                sumHistPtr->getHist()->Print();
                incHistPtr->getHist()->Print();

                // Create DIAM wrapper with histogram
                string diamAxesID = replaceHistoSuffix( axesID, scaledString, diamondString );
                auto diamClassName = replaceClassName( *sumHistPtr, scaledString, diamondString );
                if ( WrapperVirt::getBinaryOperatorMap().count( diamClassName ) > 0 ) {
                    histoMap[diamAxesID][cutID] = WrapperVirt::getBinaryOperatorMap()[diamClassName]( *sumHistPtr,
                            *incHistPtr, diamAxesID, cutID );
                    if ( histoMap[diamAxesID][cutID] != nullptr ) {
                        histoMap[diamAxesID][cutID]->getHist()->SetTitle( histoMap[diamAxesID][cutID]->getHist()->GetName() );
                        histoMap[diamAxesID][cutID]->getHist()->Add( incHistPtr->getHist(), +1 );
                    }
                } else {
                    cout << "Could no find binary operator for class name " <<  diamClassName << endl;
                }
            }
        }
    }
}

/*

 void ScanPositionPlots::createHistograms() {
 cout << "Creating histograms for" << GetName() << endl;
 this->cd();
 // Cloning the Eg dependences with appropriate weights
 for (auto& scanType : ScanPositionPlots::getSelectorMapKeys()) {
 string histType = scanType + "Eg";
 auto histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << " using " << scanType << endl;
 sppHistoMap[histType] =
 dynamic_cast<TH1*>(sppSelectorMap[scanType]->getHisto("Eg")->Clone(
 histName.c_str()));
 if (sppHistoMap[histType] != 0) {
 sppHistoMap[histType]->Sumw2();
 cout << "Histo " << histName << " has been cloned" << endl;
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Scale(sppSelectorMap[scanType]->getWeight());
 cout << "Histo " << histName << " has been scaled by "
 << sppSelectorMap[scanType]->getWeight() << endl;
 }
 }

 for (auto& scanType : ScanPositionPlots::getSelectorMapKeys()) {
 string histType = scanType + "EgCrack";
 auto histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << " using " << scanType << endl;
 sppHistoMap[histType] =
 dynamic_cast<TH1*>(sppSelectorMap[scanType]->getHisto("EgCrack")->Clone(
 histName.c_str()));
 if (sppHistoMap[histType] != 0) {
 sppHistoMap[histType]->Sumw2();
 cout << "Histo " << histName << " has been cloned" << endl;
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Scale(sppSelectorMap[scanType]->getWeight());
 cout << "Histo " << histName << " has been scaled by "
 << sppSelectorMap[scanType]->getWeight() << endl;
 }
 }

 // Cloning the EvsYvsX dependences with appropriate weights
 for (auto& scanType : ScanPositionPlots::getSelectorMapKeys()) {
 string histType = scanType + "EvsYvsX";
 auto histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << " using " << scanType << endl;
 sppHistoMap[histType] =
 dynamic_cast<TH1*>(sppSelectorMap[scanType]->getHisto("EvsYvsX")->Clone(
 histName.c_str()));
 if (sppHistoMap[histType] != 0) {
 sppHistoMap[histType]->Sumw2();
 cout << "Histo " << histName << " has been cloned" << endl;
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Scale(sppSelectorMap[scanType]->getWeight());
 cout << "Histo " << histName << " has been scaled by "
 << sppSelectorMap[scanType]->getWeight() << endl;
 }
 }


 // PARA for Eg
 if (sppHistoMap.count("sumEg") > 0 && sppHistoMap.count("difEg") > 0) {
 string histType = string("para") + "Eg";
 string histName = string(this->GetName()) + "_" + histType;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["sumEg"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 cout << "Histo " << histName << " has been cloned" << endl;
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Add(sppHistoMap["difEg"], -1.0);
 cout << "Histo" << histName << " has been filled" << endl;
 }

 // PERP for Eg
 if (sppHistoMap.count("sumEg") > 0 && sppHistoMap.count("difEg") > 0) {
 string histType = string("perp") + "Eg";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["sumEg"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Add(sppHistoMap["difEg"], +1.0);
 }

 // Sum of the incoherent and sum (full diamond rate)
 if ( sppHistoMap.count("sumEg") > 0 &&  sppHistoMap.count("incEg")> 0 ) {
 string histType = string("diam") + "Eg";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["sumEg"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Add(sppHistoMap["incEg"], +1.0);
 }

 // Sum of the incoherent and sum (full diamond rate) with cracked collimator
 if ( sppHistoMap.count("sumEgCrack") > 0 &&  sppHistoMap.count("incEgCrack")> 0 ) {
 string histType = string("diam") + "EgCrack";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["sumEgCrack"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Add(sppHistoMap["incEgCrack"], +1.0);
 }

 // Sum of the incoherent and sum (full diamond rate) vs Eg vs Y and X
 if ( sppHistoMap.count("sumEvsYvsX") > 0 &&  sppHistoMap.count("incEvsYvsX")> 0 ) {
 string histType = string("diam") + "EvsYvsX";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["sumEvsYvsX"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Add(sppHistoMap["incEvsYvsX"], +1.0);
 }


 // Enhancement plot for Eg
 if ( sppHistoMap.count("amoEg") > 0 && sppHistoMap.count("diamEg") ) {
 string histType = string("rat") + "Eg";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["diamEg"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Divide(sppHistoMap["diamEg"], sppHistoMap["amoEg"],
 1.0
 / sppHistoMap["diamEg"]->Integral(1,
 sppHistoMap["diamEg"]->GetNbinsX()),
 1.0
 / sppHistoMap["amoEg"]->Integral(1,
 sppHistoMap["amoEg"]->GetNbinsX()));
 }
 // Enhancement plot for Eg with crack
 if ( sppHistoMap.count("amoEgCrack") > 0 && sppHistoMap.count("diamEgCrack") ) {
 string histType = string("rat") + "EgCrack";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["diamEgCrack"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Divide(sppHistoMap["diamEgCrack"], sppHistoMap["amoEgCrack"],
 1.0
 / sppHistoMap["diamEgCrack"]->Integral(1,
 sppHistoMap["diamEgCrack"]->GetNbinsX()),
 1.0
 / sppHistoMap["amoEgCrack"]->Integral(1,
 sppHistoMap["amoEgCrack"]->GetNbinsX()));
 }

 // Polarization vs Eg
 if ( sppHistoMap.count("diamEg") > 0 && sppHistoMap.count("difEg") > 0 ) {
 string histType = string("pol") + "Eg";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["difEg"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Divide(sppHistoMap["diamEg"]);
 }
 // Polarization vs Eg with Crack
 if ( sppHistoMap.count("diamEgCrack") > 0 && sppHistoMap.count("difEgCrack") > 0 ) {
 string histType = string("pol") + "EgCrack";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["difEgCrack"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Divide(sppHistoMap["diamEgCrack"]);
 }

 // Polarization Eg vs Y vs X
 if ( sppHistoMap.count("diamEvsYvsX") > 0 && sppHistoMap.count("difEvsYvsX") > 0 ) {
 string histType = string("pol") + "EvsYvsX";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["difEvsYvsX"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Divide(sppHistoMap["diamEvsYvsX"]);
 }

 // Polarization of coherent part vs Eg
 if ( sppHistoMap.count("sumEg") > 0 && sppHistoMap.count("difEg") > 0 ) {
 string histType = string("coh_pol") + "Eg";
 string histName = string(this->GetName()) + "_" + histType;
 cout << "Creating histo " << histName << endl;
 sppHistoMap[histType] = dynamic_cast<TH1*>(sppHistoMap["difEg"]->Clone(
 histName.c_str()));
 sppHistoMap[histType]->Sumw2();
 sppHistoMap[histType]->SetTitle(histName.c_str());
 sppHistoMap[histType]->Divide(sppHistoMap["sumEg"]);
 }


 return;
 }

 */
