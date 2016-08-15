/*
 * ScanPositionPlots.hh
 *
 *  Created on: Jun 23, 2016
 *      Author: Hovanes Egiyan
 */

#ifndef SCANPOSITIONPLOTS_HH_
#define SCANPOSITIONPLOTS_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TSelector.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TObject.h"

#include "TUserMCBInp.h"
#include "TUserMCBOut.h"
#include "mcbTreeSelector.hh"

#include "ClassName.hh"

#include "WrapperType.hh"
#include "Wrapper.hh"
#include "WrapperVirt.hh"
#include "Container.hh"
#include "ScanPositionData.hh"


class ScanPositionPlots : public TDirectory, public Histo::Container {
protected:

	// Value to which the selector maps are initialized to
	static std::map<std::string, mcbTreeSelector*> sppSelectorMapInit ;

	// ROOT file pointer
	TFile* sppRootFile = 0;

	// Map of the plots for each MCB simulation type. The key is the type name.
	// Initialize the map for four types but with zero pointers for data objects.
	std::map<std::string, mcbTreeSelector*> sppSelectorMap = sppSelectorMapInit;

	// name of the MCB tree
	static std::string sppTreeName;


	// Protect assignment operator and copy constructor
	ScanPositionPlots( const ScanPositionPlots& plots ) {};
	ScanPositionPlots& operator=( const ScanPositionPlots& plots ){return *this;}


public:
	ScanPositionPlots( const std::string rootFileNameBase );
	virtual ~ScanPositionPlots();

	virtual void processSelectors(std::string option = "", Long64_t nentries =
			TVirtualTreePlayer::kMaxEntries, Long64_t firstentry = 0);

	virtual void createHistograms() ;
    virtual void makePerpPara() ;
//    virtual void makeDiamond();
//    virtual void makeEnhamcement();
//    virtual void makeCoherentPolzrization();
//    virtual void makePolarization();

	static std::string removeDirectoryName( std::string path );
	static std::string getRootFileName( const std::string baseName, const std::string type );

	// Getters and setters
	// Return vector of types of MCB simulations
	static std::vector<std::string> getSelectorMapKeys() ;

	inline static std::string getTreeName() {return sppTreeName;}


	inline virtual std::map<std::string, mcbTreeSelector*>& getSelectorMap() const {
		return const_cast<std::map<std::string, mcbTreeSelector*>&>( sppSelectorMap );
	}

	inline virtual mcbTreeSelector* getSelector( const std::string type ) {
		if( sppSelectorMap.count( type ) > 0 ) return sppSelectorMap[type];
		return 0;
	}
	ClassDef(ScanPositionPlots,0);
};

#endif /* SCANPOSITIONPLOTS_HH_ */
