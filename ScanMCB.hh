/*
 * ScanMCB.hh
 *
 *  Created on: Jul 12, 2016
 *      Author: Hovanes Egiyan
 */

#ifndef SCANMCB_HH_
#define SCANMCB_HH_

#include <glob.h>

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
#include <TDirectory.h>

#include "ClassName.hh"

#include "Container.hh"
#include "WrapperVirt.hh"
#include "Wrapper.hh"

#include "ScanPositionPlots.hh"

class ScanMCB: public TDirectory, public Histo::Container {
protected:

	static std::string smFolderName;

	std::string smDirName;
	std::string smFileNameBase;

	// Map of PositionPlot objects. The key is the ROOT filename
	std::map<std::string, ScanPositionPlots*> smPlotMap;

	// Protect default and copy constructor and the assignment operator
	ScanMCB() {}
	ScanMCB( const ScanMCB& scan ) {}
	ScanMCB& operator=( const ScanMCB& scan ) {return *this;}

public:
	ScanMCB(const std::string roodFileDir, const std::string rootFileNameBase);
	virtual ~ScanMCB();

	virtual void fillFileNames();
	static std::string findBaseName(std::string simTypeProbe,
			std::string rootFileName);
	static std::string findDirectoryName( std::string path );
	static std::string removeDirectoryName( std::string path );
	static bool otherTypesExist(std::string simTypeProbe,
			std::string rootFileName);

	virtual void processScanPositions();
	virtual void createHistograms();

	inline static std::string getFolderName() {
		return smFolderName;
	}
	inline static std::string setFolderName(const std::string name) {
		return (smFolderName = name);
	}

	inline virtual std::string getDirName() const {
		return smDirName;
	}
	inline virtual std::string getBaseName() const {
		return smFileNameBase;
	}

	inline virtual std::map<std::string, ScanPositionPlots*>& getPlotMap() {
		return smPlotMap;
	}

ClassDef(ScanMCB,0)
	;

};

#endif /* SCANMCB_HH_ */
