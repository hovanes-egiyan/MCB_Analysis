/*
 * Wrapper.hh
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#ifndef WRAPPER_HH_
#define WRAPPER_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <regex>
#include <iterator>

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

#include "WrapperVirt.hh"
#include "WrapperDim.hh"
#include "WrapperType.hh"
#include "WrapperPhysics.hh"

namespace Histo {

// Class Wrapper inheriting from the WrapperJoin and which will actually be used in the software.
// This class will have the partial specializations for HISTO types.
template<char* const TYPE, class HISTO>
struct Wrapper : public WrapperPhysics<TYPE, HISTO> {
    Wrapper( const std::string aID = "", const std::string cID = "" ) :
            WrapperPhysics<TYPE, HISTO>( aID, cID ) {
    }
    Wrapper( const std::string& aID, const std::string& cID, int nX, double xMin, double xMax ) :
            WrapperPhysics<TYPE, HISTO>( aID, cID ) {
    }
    Wrapper( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "", double scale =
            1.0 ) :
            WrapperPhysics<TYPE, HISTO>( wrapper, aID, cID, scale ) {
    }
    Wrapper( const WrapperVirt& sum, const WrapperVirt& dif, const std::string aID = "",
            const std::string cID = "" ) :
            WrapperPhysics<TYPE, HISTO>( sum, dif, aID, cID ) {
    }
    Wrapper( const WrapperVirt& perp, const WrapperVirt& para, const WrapperVirt& inc,
            const std::string aID = "", const std::string cID = "" ) :
            WrapperPhysics<TYPE, HISTO>( perp, para, inc, aID, cID ) {
    }
    virtual ~Wrapper() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
};

// Specialized class for 1D histograms
template<char* const TYPE>
struct Wrapper<TYPE, TH1D> : public WrapperPhysics<TYPE, TH1D> {
    Wrapper( const std::string aID = "", const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH1D>( aID, cID ) {
    }
    Wrapper( const std::string& aID, const std::string& cID, int nX, double xMin, double xMax ) :
            WrapperPhysics<TYPE, TH1D>( TH1D( "", "", nX, xMin, xMax ), aID, cID ) {
    }
    Wrapper( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "",
            double scale = 1.0 ) :
            WrapperPhysics<TYPE, TH1D>( wrapper, aID, cID, scale ) {
    }
    Wrapper( const WrapperVirt& sum, const WrapperVirt& dif, const std::string aID = "",
            const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH1D>( sum, dif, aID, cID ) {
    }
    Wrapper( const WrapperVirt& perp, const WrapperVirt& para, const WrapperVirt& inc,
            const std::string aID = "", const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH1D>( perp, para, inc, aID, cID ) {
    }
    virtual ~Wrapper() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
};

// Specialized class for 2D histograms
template<char* const TYPE>
struct Wrapper<TYPE, TH2D> : public WrapperPhysics<TYPE, TH2D> {
    Wrapper( const std::string aID = "", const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH2D>( aID, cID ) {
    }
    Wrapper( const std::string& aID, const std::string& cID, int nX, double xMin, double xMax, int nY, double yMin,
            double yMax ) :
            WrapperPhysics<TYPE, TH2D>( TH2D( "", "", nX, xMin, xMax, nY, yMin, yMax ), aID, cID ) {
    }
    Wrapper( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "",
            double scale = 1.0 ) :
            WrapperPhysics<TYPE, TH2D>( wrapper, aID, cID, scale ) {
    }
    Wrapper( const WrapperVirt& sum, const WrapperVirt& dif, const std::string aID = "",
            const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH2D>( sum, dif, aID, cID ) {
    }
    Wrapper( const WrapperVirt& perp, const WrapperVirt& para, const WrapperVirt& inc,
            const std::string aID = "", const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH2D>( perp, para, inc, aID, cID ) {
    }
    virtual ~Wrapper() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
};

// Specialized class for 3D histograms
template<char* const TYPE>
struct Wrapper<TYPE, TH3D> : public WrapperPhysics<TYPE, TH3D> {
    Wrapper( const std::string aID = "", const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH3D>( aID, cID ) {
    }
    Wrapper( const std::string& aID, const std::string& cID, int nX, double xMin, double xMax, int nY, double yMin,
            double yMax, int nZ, double zMin, double zMax ) :

            WrapperPhysics<TYPE, TH3D>( TH3D( "", "", nX, xMin, xMax, nY, yMin, yMax, nZ, zMin, zMax ), aID, cID ) {
    }
    Wrapper( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "",
            double scale = 1.0 ) :
            WrapperPhysics<TYPE, TH3D>( wrapper, aID, cID, scale ) {
    }
    Wrapper( const WrapperVirt& sum, const WrapperVirt& dif, const std::string aID = "",
            const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH3D>( sum, dif, aID, cID ) {
    }
    Wrapper( const WrapperVirt& perp, const WrapperVirt& para, const WrapperVirt& inc,
            const std::string aID = "", const std::string cID = "" ) :
            WrapperPhysics<TYPE, TH3D>( perp, para, inc, aID, cID ) {
    }
    virtual ~Wrapper() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
};

} /* namespace Histo */

#endif /* WRAPPER_HH_ */
