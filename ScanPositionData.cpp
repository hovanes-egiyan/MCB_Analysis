/*
 * ScanPositionData.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: Hovanes Egiyan
 */

#include "ScanPositionData.hh"



using namespace std;
using namespace Histo;


ClassImp(ScanPositionData)


ScanPositionData::ScanPositionData( const std::string type, TTree* tree ) : Container(),
    directoryPtr( new TDirectoryFile( type.c_str(), type.c_str(), "", gDirectory) ),
		spdName(type), spdTree(tree), spdWeight() {
    directoryPtr->cd();
	return;
}


ScanPositionData::~ScanPositionData() {
//	// Delete all histograms in the map and erase the map elements
//	for (auto& iterAxes : spdHistoMap) {
//		string axesName = iterAxes.first;
//		map<string, MCBHistoVirt*>& hMap = iterAxes.second;
//		for (auto& iterCuts : hMap) {
//			string cutName = iterCuts.first;
//			MCBHistoVirt* histPtr = iterCuts.second;
//			if (histPtr != 0) {
//				string histName = histPtr->constructHistoName();
//				// Check to make sure that the histo is in this directory
//				// (cannot be otherwise if it exists)
//				if( spdDirPointer->FindObject(histName.c_str()) != 0 ) {
//					delete histPtr;
//				}
//			}
//			hMap.erase(axesName);
//		}
//		spdHistoMap.erase(axesName);
//	}
//	return;
}



