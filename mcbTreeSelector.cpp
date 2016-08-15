/*
 * mcbTreeSelector.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: Hovanes Egiyan
 */

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("mcbTreeSelector.C")
// root> T->Process("mcbTreeSelector.C","some options")
// root> T->Process("mcbTreeSelector.C+")
//
#define MCBTREESELECTOR_CPP_

#include "mcbTreeSelector.hh"

using namespace std;
using namespace Histo;

ClassImp( mcbTreeSelector )

//string mcbTreeSelector::mcbio_lib_file = "/home/hovanes/GlueX/MCB/linux/obj/libTUserMCBIO.so";
//volatile int dummyInt = mcbTreeSelector::LoadSharedLibraries();
//
//
//int mcbTreeSelector::LoadSharedLibraries() {
//	// First make sure that the MCBIO so-file is loaded to be able to read
//	// the user data associated with the tree.
//	cout << "Trying to load libraries ... " << endl;
//	if ( string( gSystem->GetLibraries(mcbio_lib_file.c_str()) ) == string( "" ) ) {
//		cout << "Loading library " << mcbio_lib_file << endl;
//		if ( gSystem->Load(mcbio_lib_file.c_str()) != 0)
//			throw runtime_error(
//					string("Could not load ") + mcbio_lib_file
//							+ string("library"));
//	}
//	return 0;
//}

void mcbTreeSelector::Init( TTree *tree ) {
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the reader is initialized.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    cout << "Setting fChain to " << hex << showbase << tree << dec << endl;
    fChain = tree;

    // Get user info about input and output of the simulation from the tree
    GetUserMCBInput();
    GetUserMCBOutput();

    fReader.SetTree( tree );
    cout << "Initialized the tree reader" << endl;

    cout << "Number of electrons is " << mcbOut->no_Electrons << " Norm Volume is " << mcbOut->NormVolume << endl;
    this->createHistograms();
}

Bool_t mcbTreeSelector::Notify() {
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    cout << "Notification is sent about a new file" << endl;
    return kTRUE;
}

void mcbTreeSelector::Begin( TTree * /*tree*/) {
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
    return;
}

void mcbTreeSelector::SlaveBegin( TTree * /*tree*/) {
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
    return;
}

Bool_t mcbTreeSelector::Process( Long64_t entry ) {
    // The Process() function is called for each entry in the tree (or possibly
    // keyed object in the case of PROOF) to be processed. The entry argument
    // specifies which entry in the currently loaded tree is to be processed.
    // When processing keyed objects with PROOF, the object is already loaded
    // and is available via the fObject pointer.
    //
    // This function should contain the \"body\" of the analysis. It can contain
    // simple or elaborate selection criteria, run algorithms on the data
    // of the event and typically fill histograms.
    //
    // The processing can be stopped by calling Abort().
    //
    // Use fStatus to set the return value of TTree::Process().
    //
    // The return value is currently not used.

    fReader.SetEntry( entry );
    if ( entry % 100000 == 0 and entry > 0 ) cout << "Processing Entry " << entry << endl;

    Float_t intensity = **dynamic_cast<TTreeReaderValue<Float_t>*>( treeValueMap["intensity"] );
    Double_t intensitySign = TMath::Sign( 1.0, intensity );

    // Collimator distance is given in meters in the input info object
    Double_t dCol = mcbInput->coldist * 1000.0;
//	Double_t collimatorPosition = mcbInput->coloff_x * 1000.0;
    Double_t Eg = **dynamic_cast<TTreeReaderValue<Float_t>*>( treeValueMap["Eg"] );
    Double_t thetaGamma = **dynamic_cast<TTreeReaderValue<Float_t>*>( treeValueMap["thg"] );
    Double_t phiGamma = **dynamic_cast<TTreeReaderValue<Float_t>*>( treeValueMap["phg"] );
    Double_t xGamma = dCol * sin( thetaGamma ) * cos( phiGamma );
    Double_t yGamma = dCol * sin( thetaGamma ) * sin( phiGamma );

    const Double_t crackSizeY = 0.1;

    if ( histoMap.count( "YvsX" ) > 0 && histoMap["YvsX"].count( "is" ) > 0 )
        ( (Wrapper<Histo::rawString, TH2D>*) histoMap["YvsX"]["is"] )->getHistoPtr()->Fill( xGamma, yGamma,
                intensitySign );
    if ( histoMap.count( "EvsYvsX" ) > 0 && histoMap["EvsYvsX"].count( "is" ) > 0 )
        ( (Wrapper<Histo::rawString, TH3D>*) histoMap["EvsYvsX"]["is"] )->getHistoPtr()->Fill( xGamma, yGamma, Eg,
                intensitySign );

//	 Check if the photon passed the collimator
    if ( *( *dynamic_cast<TTreeReaderValue<Char_t>*>( treeValueMap["col"] ) ) == 1 ) {
        if ( histoMap.count( "Eg" ) > 0 && histoMap["Eg"].count( "is&col" ) > 0 )
            ( (Wrapper<Histo::rawString, TH1D>*) histoMap["Eg"]["is&col"] )->getHistoPtr()->Fill( Eg, intensitySign );
    }
	// Check if the photon passed the collimator or went through the horizontal crack
	if( ( *( *dynamic_cast<TTreeReaderValue<Char_t>*>(treeValueMap["col"]) ) == 1 ) ||
			fabs( yGamma ) < crackSizeY ) {
		if( histoMap.count( "Eg" ) > 0 && histoMap["Eg"].count("is&crack") > 0 )
		    ((Wrapper<Histo::rawString,TH1D>*)histoMap["Eg"]["is&crack"])->getHistoPtr()->Fill( Eg, intensitySign );
	}
    return kTRUE;
}

void mcbTreeSelector::SlaveTerminate() {
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.

}

void mcbTreeSelector::Terminate() {
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.
//    this->createScaledHistograms();
    return;
}

// Get the input user info about the tree, assign it to the corresponding member
// and return the pointer to it.
TUserMCBInp* mcbTreeSelector::GetUserMCBInput() {
    cout << "Tree pointer in GetUserMCBInput is " << fChain << endl;
    TList* treeList = fChain->GetUserInfo();
    if ( treeList != 0 ) {
        // Input saved at element 0 of the list by the simulation code
        mcbInput = dynamic_cast<TUserMCBInp*>( treeList->At( 0 ) );
    } else {
        cerr << "Treelist pointer is zero " << endl;
    }
    return mcbInput;
}

// Get the output user info about the tree, assign it to the corresponding member
// and return the pointer to it.
TUserMCBOut* mcbTreeSelector::GetUserMCBOutput() {
    cout << "Tree pointer in GetUserMCBOutput is " << fChain << endl;
    TList* treeList = fChain->GetUserInfo();
    if ( treeList != 0 ) {
        // Output is saved at element 1 of the list by the simulation code
        mcbOut = dynamic_cast<TUserMCBOut*>( treeList->At( 1 ) );
    } else {
        cerr << "Treelist pointer is zero " << endl;
    }
    return mcbOut;
}

// This method will call the Process method for the tree which will call
// the Process method of the same object inside the event loop of the tree.
void mcbTreeSelector::processTree( string option, Long64_t nEntries, Long64_t firstEntry ) {
    if ( spdTree != 0 ) {
        spdTree->Process( this, option.c_str(), nEntries, firstEntry );
        spdWeight = this->getWeight();
    }
}

void mcbTreeSelector::createHistograms() {
    cout << "creating histograms for " << GetName() << endl;
    string histName, axesName, cutName;
    axesName = "Eg";
    cutName = "is&col";
    histoMap[axesName][cutName] = new Wrapper<Histo::rawString, TH1D>( axesName, cutName, 200, 0., 12000. );
    histoMap[axesName][cutName]->getHist()->SetXTitle( "E_{#gamma} (MeV)" );

    axesName = "Eg";
    cutName = "is&crack";
    histoMap[axesName][cutName] = new Wrapper<Histo::rawString, TH1D>( axesName, cutName, 200, 0, 12000 );
    histoMap[axesName][cutName]->getHist()->Sumw2();
    histoMap[axesName][cutName]->getHist()->SetXTitle( "E_{#gamma} (Mev)" );

    axesName = "EvsYvsX";
    cutName = "is";
    histoMap[axesName][cutName] = new Wrapper<Histo::rawString, TH3D>( axesName, cutName, 50, -8, +8, 50, -8, +8, 50, 0,
            12000 );
    histoMap[axesName][cutName]->getHist()->Sumw2();
    histoMap[axesName][cutName]->getHist()->SetXTitle( "X (mm)" );
    histoMap[axesName][cutName]->getHist()->SetYTitle( "Y (mm)" );

    return;
}

void mcbTreeSelector::createScaledHistograms() {
    cout << "Creating scaled histograms for " << getName() << endl;
    cout << "In directory " << gDirectory->GetPath() << endl;
    // Cloning various dependences with appropriate weights.
    // Now get all the histograms in the scans. Loop over the axesID and
    // over the cut ID.
    auto tempCopyofHistoMap = this->getHistoMap();
    for ( auto& axesIter : tempCopyofHistoMap ) {
        string axesID = axesIter.first;
        // Now for the scaled histogram the axes will also contain "::scaled" at the end
        // to show that this is the rate of the events.
        string scaledAxesID = addHistoSuffix( axesID, Histo::scaledString );
        auto& cutMap = axesIter.second;
        for ( auto& cutIter : cutMap ) {
            string cutID = cutIter.first;
            cout << "Looking at cut ID " << cutID << endl;
            auto rawHistPtr = cutIter.second;
            if ( rawHistPtr != nullptr && rawHistPtr->getType() == rawString ) {
                auto scaledClassName = replaceClassName( *rawHistPtr, Histo::rawString,
                        Histo::scaledString );
                if ( WrapperVirt::getCopierMap().count( scaledClassName ) > 0 ) {
                    histoMap[scaledAxesID][cutID] = WrapperVirt::getCopierMap()[scaledClassName]( *rawHistPtr,
                            scaledAxesID, cutID );
                    if ( histoMap[scaledAxesID][cutID] != nullptr ) {
                        histoMap[scaledAxesID][cutID]->getHist()->SetTitle(
                                histoMap[scaledAxesID][cutID]->getHist()->GetName() );
                    }
                }
            }
        }
    }
    cout << "Scaled histograms have been created" << endl;
    return;
}

