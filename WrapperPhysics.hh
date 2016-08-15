/*
 * WrapperPhysics.hh
 *
 *  Created on: Aug 8, 2016
 *      Author: hovanes
 */

#ifndef WRAPPERPHYSICS_HH_
#define WRAPPERPHYSICS_HH_

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

namespace Histo {


// Generic histogram wrapper (RAW, SCALED, SCAN etc).
template<char* const TYPE, class HISTO>
struct WrapperPhysics : public WrapperType<TYPE, HISTO> {
    WrapperPhysics( const std::string aID = "", const std::string cID = "" ) :
             WrapperType<TYPE, HISTO>( aID, cID ) {
    }
    WrapperPhysics( const HISTO& hist, const std::string aID = "", const std::string cID = "" ) :
             WrapperType<TYPE, HISTO>( hist, aID, cID ) {
    }
    WrapperPhysics( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "", double scale = 1.0 ) :
             WrapperType<TYPE, HISTO>( wrapper, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        this->getHist()->Scale( scale );
    }
    WrapperPhysics( const WrapperVirt& sum, const WrapperVirt& dif, const std::string aID = "",
            const std::string cID = "" ) :
             WrapperType<TYPE, HISTO>( aID, cID ) {
    }
    WrapperPhysics( const WrapperVirt& perp, const WrapperVirt& para, const WrapperVirt& inc, const std::string aID = "",
            const std::string cID = "" ) :
             WrapperType<TYPE, HISTO>( aID, cID ) {
    }

    virtual ~WrapperPhysics() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
    virtual void* clone( const std::string aID = "", const std::string cID = "" ) const override {
        return new WrapperPhysics( *this, aID, cID );
    }
};


// Partial specialization for PARA types
template<class HISTO>
struct WrapperPhysics<paraString, HISTO> : public WrapperType<paraString, HISTO> {
    WrapperPhysics( const std::string aID = "", const std::string cID = "" ) :
             WrapperType<paraString, HISTO>( aID, cID ) {
    }
    WrapperPhysics( const HISTO& hist, const std::string aID, const std::string cID ) :
             WrapperType<paraString, HISTO>( hist, aID, cID ) {
    }
//    WrapperPhysics( const WrapperPhysics* wrapper, const std::string aID, const std::string cID ) :
//             WrapperType<paraString, HISTO>( *wrapper, aID, cID  ) {
//    }
    WrapperPhysics( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "",
            double scale = 1.0 ) :
             WrapperType<paraString, HISTO>( wrapper, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        this->getHist()->Scale( scale );
    }
    WrapperPhysics( const WrapperVirt& sum, const WrapperVirt& dif, const std::string aID = "",
            const std::string cID = "" ) :
             WrapperType<paraString, HISTO>( sum, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        this->getHistoPtr()->Add( const_cast<WrapperVirt&>( dif ).getHist(), -1.0 );
    }
    virtual ~WrapperPhysics() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
    virtual void* clone( const std::string aID = "", const std::string cID = "" ) const override {
        return new WrapperPhysics<paraString,HISTO>( *this, aID, cID ) ;
    }
};

// Partial specialization for PERP types
template<class HISTO>
struct WrapperPhysics<perpString, HISTO> : public WrapperType<perpString, HISTO> {
    WrapperPhysics( const std::string aID = "", const std::string cID = "" ) :
             WrapperType<perpString, HISTO>( aID, cID ) {
    }
    WrapperPhysics( const HISTO& hist, const std::string aID = "", const std::string cID= "" ) :
             WrapperType<perpString, HISTO>( hist, aID, cID ) {
    }
    WrapperPhysics( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "",
            double scale = 1.0 ) :
            WrapperType<perpString, HISTO>( wrapper, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        this->getHist()->Scale( scale );
    }
    WrapperPhysics( const WrapperVirt& sum, const WrapperVirt& dif, const std::string aID = "",
            const std::string cID = "" ) :
             WrapperType<perpString, HISTO>( sum, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        this->getHistoPtr()->Add( const_cast<WrapperVirt&>( dif ).getHist(), +1.0 );
    }
    virtual ~WrapperPhysics() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
    virtual void* clone( const std::string aID = "", const std::string cID = "" ) const override {
        return new WrapperPhysics( *this, aID, cID );
    }
};


// Partial specialization for COHERENT POLARIZATION types
template<class HISTO>
struct WrapperPhysics<coherentPolarizationString, HISTO> : public WrapperType<coherentPolarizationString, HISTO> {
    WrapperPhysics( const std::string aID = "", const std::string cID = "" ) :
             WrapperType<coherentPolarizationString, HISTO>( aID, cID ) {
    }
    WrapperPhysics( const HISTO& hist, const std::string aID, const std::string cID ) :
             WrapperType<coherentPolarizationString, HISTO>( aID, cID, hist ) {
    }
    WrapperPhysics( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "",
            double scale = 1.0 ) :
            WrapperType<coherentPolarizationString, HISTO>( wrapper, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        this->getHist()->Scale( scale );
    }
    WrapperPhysics( const WrapperVirt& perp, const WrapperVirt& para, const std::string aID = "",
            const std::string cID = "" ) :
             WrapperType<coherentPolarizationString, HISTO>( perp, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        WrapperVirt& perpRef = const_cast<WrapperVirt&>( perp );
        WrapperVirt& paraRef = const_cast<WrapperVirt&>( para );
        HISTO* numerator = dynamic_cast<HISTO*>( perpRef.getHist()->Clone( "numerator" ) );
        HISTO* denominator = dynamic_cast<HISTO*>( perpRef.getHist()->Clone( "denominator" ) );
        if ( numerator != nullptr && denominator != nullptr ) {
            numerator->Add( paraRef.getHist(), -1.0 );
            denominator->Add( paraRef.getHist(), +1.0 );
            this->getHist()->Divide( numerator, denominator );
            delete numerator;
            delete denominator;
        }
    }
    virtual ~WrapperPhysics() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
    virtual void* clone( const std::string aID = "", const std::string cID = "" ) const override {
        return new WrapperPhysics( *this, aID, cID );
    }
};


// Partial specialization for  POLARIZATION types
template<class HISTO>
struct WrapperPhysics<polarizationString, HISTO> : public WrapperType<polarizationString, HISTO> {
    WrapperPhysics( const std::string aID = "", const std::string cID = "" ) :
             WrapperType<polarizationString, HISTO>( aID, cID ) {
    }
    WrapperPhysics( const HISTO& hist, const std::string aID, const std::string cID ) :
             WrapperType<polarizationString, HISTO>( aID, cID, hist ) {
    }
    WrapperPhysics( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "",
            double scale = 1.0 ) :
            WrapperType<polarizationString, HISTO>( wrapper, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        this->getHist()->Scale( scale );
    }
    WrapperPhysics( const WrapperVirt& perp, const WrapperVirt& para, const WrapperVirt& inc, const std::string aID = "",
            const std::string cID = "" ) :
             WrapperType<polarizationString, HISTO>( perp, aID, cID ) {
        if( this->getHist()->GetSumw2N() == 0 )
            this->getHist()->Sumw2();
        WrapperVirt& perpRef = const_cast<WrapperVirt&>( perp );
        WrapperVirt& paraRef = const_cast<WrapperVirt&>( para );
        WrapperVirt& incRef = const_cast<WrapperVirt&>( inc );
        HISTO* numerator = dynamic_cast<HISTO*>( perpRef.getHist()->Clone( "numerator" ) );
        HISTO* denominator = dynamic_cast<HISTO*>( perpRef.getHist()->Clone( "denominator" ) );
        if ( numerator != nullptr && denominator != nullptr ) {
            numerator->Add( paraRef.getHist(), -1.0 );
            denominator->Add( paraRef.getHist(), +1.0 );
            denominator->Add( incRef.getHist(), +1.0 );
            this->getHist()->Divide( numerator, denominator );
            delete numerator;
            delete denominator;
        }
    }
    virtual ~WrapperPhysics() {
    }
    virtual WrapperVirt* getClassPointer()  override {
        return this;
    }
    virtual void* clone( const std::string aID = "", const std::string cID = "" ) const override {
        return new WrapperPhysics( *this, aID, cID );
    }
};


} /* namespace Histo */

#endif /* WRAPPERPHYSICS_HH_ */
