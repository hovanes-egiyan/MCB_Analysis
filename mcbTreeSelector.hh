/*
 * mcbTreeSelector.hh
 *
 *  Created on: June 8, 2016
 *      Author: Hovanes Egiyan
 */

#ifndef MCBTREESELECTOR_HH_
#define MCBTREESELECTOR_HH_

#include <math.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH2.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TObject.h"
#include "TDirectory.h"

#include "TUserMCBInp.h"
#include "TUserMCBOut.h"

#include "ClassName.hh"
#include "Wrapper.hh"
#include "Container.hh"
#include "ScanPositionData.hh"
// Headers needed by this particular selector

class mcbTreeSelector: public TSelector, public ScanPositionData {
protected:
	TTreeReader fReader;  //!the tree reader
	TTree *fChain = 0;   //!pointer to the analyzed TTree or TChain

	TUserMCBInp* mcbInput = 0;	// MCB input object
	TUserMCBOut* mcbOut = 0;	// MCB output object

	bool SelectionCut();		//! event cut function
public:
	// Readers to access the data (delete the ones you do not need).
	std::map<std::string, ROOT::Internal::TTreeReaderValueBase*> treeValueMap =
	{
			{ "ih"			, new TTreeReaderValue<Char_t>( fReader, "ih" 			) } ,
			{ "ik"			, new TTreeReaderValue<Char_t>( fReader, "ik" 			) } ,
			{ "il"			, new TTreeReaderValue<Char_t>( fReader, "il" 			) } ,
			{ "col"			, new TTreeReaderValue<Char_t>( fReader, "col" 			) } ,
			{ "Eg"			, new TTreeReaderValue<Float_t>( fReader, "Eg" 			) } ,
			{ "the"			, new TTreeReaderValue<Float_t>( fReader, "the" 		) } ,
			{ "phe"			, new TTreeReaderValue<Float_t>( fReader, "phe" 		) } ,
			{ "thg"			, new TTreeReaderValue<Float_t>( fReader, "thg" 		) } ,
			{ "phg"			, new TTreeReaderValue<Float_t>( fReader, "phg" 		) } ,
			{ "xe"			, new TTreeReaderValue<Float_t>( fReader, "xe" 			) } ,
			{ "ye"			, new TTreeReaderValue<Float_t>( fReader, "ye" 			) } ,
			{ "ze"			, new TTreeReaderValue<Float_t>( fReader, "ze" 			) } ,
			{ "intensity"	, new TTreeReaderValue<Float_t>( fReader, "intensity" 	) } ,
			{ "sigms"		, new TTreeReaderValue<Float_t>( fReader, "sigms" 		) } ,
			{ "th"			, new TTreeReaderValue<Float_t>( fReader, "th" 			) } ,
			{ "al"			, new TTreeReaderValue<Float_t>( fReader, "al" 			) } ,
			{ "q"			, new TTreeReaderValue<Double_t>( fReader, "q" 			) } ,
			{ "ql2"			, new TTreeReaderValue<Double_t>( fReader, "ql2" 		) } ,
			{ "qt2"			, new TTreeReaderValue<Double_t>( fReader, "qt2" 		) }
	};


	mcbTreeSelector( const std::string type, TTree* tree ) :
		TSelector(), ScanPositionData( type, tree )  {
	}
	virtual ~mcbTreeSelector() {
	}
	virtual Int_t Version() const {
		return 2;
	}
	virtual void Begin(TTree *tree);
	virtual void SlaveBegin(TTree *tree);
	virtual void Init(TTree *tree);
	virtual Bool_t Notify();
	virtual Bool_t Process(Long64_t entry);
	virtual Int_t GetEntry(Long64_t entry, Int_t getall = 0) {
		return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
	}

	// This method will call the Process method for the tree which will call
	// the Process method of the same object.
	virtual void processTree( std::string option = "", Long64_t nentries =
			TVirtualTreePlayer::kMaxEntries, Long64_t firstentry = 0);

    virtual void createHistograms();
    virtual void createScaledHistograms();

	virtual inline double getWeight() const {
		return mcbOut->no_Electrons > 0 ?
				(mcbOut->NormVolume * 1000.0 / mcbOut->no_Electrons) : 0.0;
	}
	virtual inline std::string getDataName() const { return ScanPositionData::getName(); }
	virtual inline double getCollimatorOffset() const {
		return mcbInput->coloff_x * 1000.0;
	}
	virtual inline double getCollimatorMotorPosition() const {
		return getCollimatorOffset() + getCollimatorMotorOffset();
	}
	virtual void SetOption(const char *option) {
		fOption = option;
	}
	virtual void SetObject(TObject *obj) {
		fObject = obj;
	}
	virtual void SetInputList(TList *input) {
		fInput = input;
	}
	virtual TList *GetOutputList() const {
		return fOutput;
	}
	virtual void SlaveTerminate();
	virtual void Terminate();

	virtual TUserMCBInp* GetUserMCBInput();
	virtual TUserMCBOut* GetUserMCBOutput();



ClassDef(mcbTreeSelector,0)
	;

};

#endif /* MCBTREESELECTOR_HH_ */

