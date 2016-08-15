/*
 * ScanMCB.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: Hovanes Egiyan
 */

#include "ScanMCB.hh"
#include "Wrapper.hh"

using namespace std;
using namespace Histo;

ClassImp(ScanMCB)

string ScanMCB::smFolderName = "MCB";

// Main constructor
ScanMCB::ScanMCB(const std::string rootFileDir, const string rootFileNameBase) :
        TFile( (removeDirectoryName(rootFileNameBase)+string("_ana.root")).c_str(), "recreate" ),
//		TDirectory(removeDirectoryName(rootFileNameBase).c_str(), smFolderName.c_str(), "",
//				gDirectory),
        smDirName(rootFileDir),
				smFileNameBase(
				rootFileNameBase), smPlotMap() {
	fillFileNames();
	return;
}

// Destructor
ScanMCB::~ScanMCB() {
    this->cd();
    TFile::Write();
    TFile::Close();
	// Delete all entries in the plot map and erase all map entries
	smPlotMap.clear();
	return;
}

// A method to create keys for map of the plots. The keys of this map
// are the ROOT file names.
void ScanMCB::fillFileNames() {
	cout << "Filling file names " << endl;
	vector<string> typeVec = ScanPositionPlots::getSelectorMapKeys() ;
	if( typeVec.size() < 1 ) return;
	// Pick the first simulation type and use it to find all ROOT file
	// matching the pattern.
	string simTypeProbe = typeVec[0];
	// Find the list of ROOT files in the directory defined below
	string globPattern = smDirName + "/" + smFileNameBase + "*." + simTypeProbe	+ ".root";
	cout << "Pattern is " << globPattern << endl ;
	glob_t glob_result;
	// Fins all files matching the pattern.
	glob(globPattern.c_str(), GLOB_TILDE, NULL, &glob_result);
	// Loop over all files for that first type and check that all other types exist
	for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
		string rootFileName(string(glob_result.gl_pathv[i]));
		cout << "Checking file " << rootFileName << " starting with " << simTypeProbe << endl;
		// if ROOT files for all other types exist then add this base filename to the list
		if( otherTypesExist( simTypeProbe, rootFileName ) ) {
			cout << "File " << rootFileName << " is good " << endl;
			// Determine the base file name without the simulation type part in the file name
			string baseName = findBaseName( simTypeProbe, rootFileName );
			// Create a new ScanPositionPlot object and put it into the map using the
			// base name as the key in the map of the ScanPositionPlot objects.
			this->cd();
			smPlotMap[baseName] = new ScanPositionPlots( baseName );
			cout << "Created ScanPositionPlots with base-name " << baseName << endl;
		}
	}
	globfree(&glob_result);

	return;
}

// Find where simTypeProbe string is and remove everything from one position to
// left where "." is.
inline string ScanMCB::findBaseName( string simTypeProbe, string rootFileName ) {
	size_t typePosition = rootFileName.find( simTypeProbe );
	// -1 is to take care of the "." character, it needs to be gone too.
	rootFileName.replace( rootFileName.begin() + typePosition - 1, rootFileName.end(), ""  );
	return rootFileName;
}

// Assume that path contains the directory followed by a slash and base file name.
inline string ScanMCB::findDirectoryName( string path ) {
	size_t lastSlashPosition = path.rfind( "/" );
	path.replace( path.begin() + lastSlashPosition, path.end(), "" );
	return path;
}

// Assume that path contains the directory followed by a slash and base file name.
inline string ScanMCB::removeDirectoryName( string path ) {
	size_t lastSlashPosition = path.rfind( "/" );
	path.replace( path.begin(), path.begin() + lastSlashPosition + 1, "" );
	return path;
}


// Check if the files for all simulation types exist
bool ScanMCB::otherTypesExist(std::string simTypeProbe,	std::string rootFileName) {
	cout << "Cecking other types for file name " <<  rootFileName << " starting with " << simTypeProbe << endl;
	bool allFilesExist = true; // Return flag set to true
	vector<string> typeVec = ScanPositionPlots::getSelectorMapKeys() ;
	if( typeVec.size() < 1 ) return false;
	size_t typePosition = rootFileName.find( simTypeProbe );

	// Loop over all simulation types and check the presence of the file.
	for( auto& simType : typeVec ) {
		cout << "Trying type " << simType << endl;
		string fileName = rootFileName;
		// replace the type provided in the argument by the current simulation type.
		// Here we use the fact that all simulation types are represented by string with same length (3).
		cout << "Will replace from " << typePosition << simTypeProbe.length() << " characters " << endl;
		fileName.replace( typePosition , simTypeProbe.length(), simType );
		cout << "Replacement result is " << fileName << endl;
		// check if the file exists by opening input stream and checking if it was successful
		ifstream testFile(fileName);
		// If stream was not create successfully set the return flag to false and break out of the loop
		if ( ! testFile.good() ) {
	    	allFilesExist = false;
	    	break;
	    } else {
	    	cout << "File " << fileName << " is OK" << endl;
	    	testFile.close();
	    }
	}
	return allFilesExist;
}

void ScanMCB::processScanPositions() {
    cout << "Processing scan " << GetName() << endl;
    this->cd();
	for( auto& scanPositionIter : smPlotMap ) {
		string fileName = scanPositionIter.first;
		ScanPositionPlots* positionPlot = scanPositionIter.second;
		positionPlot->processSelectors();
		cout << "ScanPositionPlots called " << positionPlot->GetName() << " complete" << endl;
	}
	this->createHistograms();
	return;
}


void ScanMCB::createHistograms() {
    cout << "Creating histograms for whole scan " << GetName() << endl;
    this->cd();
    // We are looking for this particular axes ID which also should mean that
    // the histogram will be 1D since we book histo with this axes ID to be 1D.
    string axesID = addHistoSuffix( "Eg", scaledString );

    // Template for the 2D histogram wrapper. Each position will have its own copy of this histo wrapper.
    Wrapper<scanString, TH2D> mtHisto( "temp", "temp", 200, 0, 12000, 63, -3.15, +3.15 );

    for ( auto& plotIter : smPlotMap ) {
        ScanPositionPlots* positionPlots = plotIter.second;
        auto selector = positionPlots->getSelector( "sum" );
        auto& selectorHistoMap = selector->getHistoMap();
        string newAxesID = replaceHistoSuffix( axesID, scaledString, scanString );
        if ( selectorHistoMap.count( axesID ) > 0 ) {
            if ( histoMap.count( axesID ) < 1 ) histoMap[newAxesID];
            for ( auto& cutIter : selectorHistoMap[axesID] ) {
                auto& cutID = cutIter.first;
                auto scaledHistPtr = cutIter.second;
                if( dynamic_cast<WrapperTypeI<Histo::scaledString>*>(scaledHistPtr) == nullptr ) continue;
                // Make sure that 2D histogram exists. If it does not create a new one copying the template above.
                if ( histoMap[newAxesID].count( cutID ) < 1 ) {
                    Wrapper<scanString, TH2D>* newHisto = new Wrapper<scanString, TH2D>( mtHisto, newAxesID, cutID );
                    newHisto->getHist()->SetTitle( newHisto->getHist()->GetName() );
                    histoMap[newAxesID][cutID] = newHisto;
                }
                // Clone the 1D histogram, change the name and scale it.
                string tempAxisID = axesID + "_temp";
                auto scanClassName = replaceClassName( *scaledHistPtr, scaledString, scanString );
                if ( WrapperVirt::getCopierMap().count( scanClassName ) > 0 ) {
                    WrapperVirt* tmpHisto = WrapperVirt::getCopierMap()[scanClassName]( *scaledHistPtr, newAxesID,
                            cutID );
                    if ( tmpHisto != nullptr ) {
                        tmpHisto->getHist()->Scale( 1. / tmpHisto->getHist()->GetMaximum() );
                        double collimatorOffset = selector->getCollimatorOffset();
                        cout << "Looking for the collimator bin for " << collimatorOffset << endl;
                        int iColBin = histoMap[newAxesID][cutID]->getHist()->GetYaxis()->FindFixBin( collimatorOffset );
                        cout << "Collimator bin is " << iColBin << endl;
                        // Put the content of the 1D histogram into the corresponding collimator position bin
                        // of the 2D histogram.
                        for ( int iEgBin = 1; iEgBin < histoMap[newAxesID][cutID]->getHist()->GetXaxis()->GetNbins();
                                iEgBin++ ) {
                            histoMap[newAxesID][cutID]->getHist()->SetBinContent( iEgBin, iColBin,
                                    tmpHisto->getHist()->GetBinContent( iEgBin ) );
                            histoMap[newAxesID][cutID]->getHist()->SetBinError( iEgBin, iColBin,
                                    tmpHisto->getHist()->GetBinError( iEgBin ) );
                        }
                        delete tmpHisto;
                    }
                } else {
                    cout << "Could not find copier for class name " << scanClassName << endl;
                }
            }
        }
    }
}

//void ScanMCB::createHistograms() {
//	cout << "Creating histograms for " << GetName() << endl;
//	this->cd();
//	string histType = "CollVsEg";
//	string histName = string(this->GetName()) + "_" + histType;
//	cout << "Creating histo " << histName << endl;
//	smHistoMap[histType] = new TH2D(histName.c_str(), histName.c_str(), 200, 0,
//			12000, 63, -3.15, +3.15 );
//	if (smHistoMap[histType] > 0) {
//		for (auto& plotIter : smPlotMap) {
//			ScanPositionPlots* positionPlots = plotIter.second;
//			auto& histoMap = positionPlots->getHistoMap();
//			// 2D plot of collimator position versus E-gamma enhancement
//			if (histoMap.count("ratEg") > 0) {
//				TH1D* tmpHisto = dynamic_cast<TH1D*>( histoMap["ratEg"]->Clone("temp_histo_rat") );
//				tmpHisto->Sumw2();
//				tmpHisto->Scale( 1. / tmpHisto->GetMaximum() );
//				mcbTreeSelector* selector =	positionPlots->getSelectorMap()[positionPlots->getSelectorMapKeys()[0]];
//				double collimatorOffset = selector->getCollimatorOffset();
//				cout << "Looking for the collimator bin for "
//						<< collimatorOffset << endl;
//				int iColBin = smHistoMap[histType]->GetYaxis()->FindFixBin(
//						collimatorOffset);
//				cout << "Collimator bin is " << iColBin << endl;
//				for (int iEgBin = 1; iEgBin < smHistoMap[histType]->GetNbinsX();
//						iEgBin++) {
//					smHistoMap[histType]->SetBinContent(iEgBin, iColBin,
//							tmpHisto->GetBinContent(iEgBin));
//					smHistoMap[histType]->SetBinError(iEgBin, iColBin,
//							tmpHisto->GetBinError(iEgBin));
//				}
//				delete tmpHisto;
//			}
//		}
//	}
//}
