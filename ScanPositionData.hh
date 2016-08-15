/*
 * ScanPositionData.hh
 *
 *  Created on: Jun 27, 2016
 *      Author: Hovanes Egiyan
 */

#ifndef SCANPOSITIONDATA_HH_
#define SCANPOSITIONDATA_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TSelector.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TObject.h"

#include "Wrapper.hh"
#include "Container.hh"

#include "TUserMCBInp.h"
#include "TUserMCBOut.h"

class ScanPositionData : public Histo::Container {
protected:
    TDirectoryFile* directoryPtr = nullptr; // It's own directory

	// Type of the position data, like sum, dif, inc or amo.
	std::string spdName;
	// pointer to the tree object
	TTree* spdTree = 0;

	// weight factor for this three
	double spdWeight = 0;

	// Offset of the collimator nominal position
	double spdCollimatorMotorOffset = -10.24;

	ScanPositionData(const ScanPositionData& data) {
	}
	ScanPositionData( ScanPositionData&& data) = delete;

	ScanPositionData& operator=(const ScanPositionData& data) {
		return *this;
	}


public:
	ScanPositionData(const std::string type, TTree* tree);
	virtual ~ScanPositionData();

	// This method needs to be implemented by the daughter classes
	virtual void processTree(std::string option = "", Long64_t nentries =
			TVirtualTreePlayer::kMaxEntries, Long64_t firstentry = 0) = 0;

	// Getters and Setters
	virtual inline double getWeight() const {
		return spdWeight;
	}
	virtual inline double getCollimatorMotorOffset() const {
		return spdCollimatorMotorOffset;
	}
	virtual inline double setWeight(const double weight) {
		return (spdWeight = weight);
	}
	virtual std::string getName() const {
		return spdName;
	}

	virtual TDirectory* getDirectory() { return directoryPtr ;}

	virtual inline double setCollimatorMotorOffset(const double offset) {
		return (spdCollimatorMotorOffset = offset);
	}
    ClassDef(ScanPositionData,0)

};

#endif /* SCANPOSITIONDATA_HH_ */
