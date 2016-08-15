/*
 * MCBHisto.hh
 *
 *  Created on: Jul 18, 2016
 *      Author: Hovanes Egiyan
 */

#ifndef MCBHISTO_HH_
#define MCBHISTO_HH_

#include <math.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>
#include <regex>
#include <iterator>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TObject.h"

#include "mcbTreeSelector.hh"

class ScanPositionPlots;

#include "MCBHistoFactory.hh"

namespace MCBHistStringTypes {
char mtString[] 		= "";
char rawString[] 		= "raw";
char scaledString[]     = "scaled";
char sumString[]        = "sum";
char difString[]        = "dif";
char paraString[] 		= "para";
char perpString[]       = "perp";
char scanString[]       = "scan";

};
using namespace MCBHistStringTypes;


// Abstract class from which the templated classes will inherit
class MCBHistoVirt  {
protected:
    std::string mhvAxesID;
    std::string mhvCutID;
    MCBHistoVirt() {
    }
public:
    MCBHistoVirt( std::string axesID, std::string cutID ) :
            mhvAxesID( axesID ), mhvCutID( cutID )  {
    }
    MCBHistoVirt( const MCBHistoVirt& hist ) :
             mhvAxesID( hist.mhvAxesID ), mhvCutID( hist.mhvCutID ) {
    }
    virtual ~MCBHistoVirt() {
    }
    inline MCBHistoVirt& operator=( const MCBHistoVirt& hist ) {
        if ( this != &hist ) {
            mhvAxesID = hist.mhvAxesID;
            mhvCutID = hist.mhvCutID;
        }
        return *this;
    }

    virtual inline std::string getAxesID() const {return mhvAxesID;}
    virtual inline std::string getCutID() const {return mhvCutID;}

    virtual inline std::string& setAxisID( const std::string id ) {
        return (mhvAxesID=id);
    }
    virtual inline std::string& setCutID( const std::string id ) {
        return (mhvCutID=id);
    }

    // Purely virtual methods
    virtual MCBHistoVirt* cloneAndScale(MCBHistoVirt& hist, ScanPositionData& data) const = 0;
    virtual std::string  constructHistoName() const = 0;
    virtual std::string  getType() const = 0;
    virtual std::string getHistoClassName() const = 0;

    virtual MCBHistoVirt* clone( const std::string& name ) const = 0;

    virtual TH1* getHisto() = 0;

};

// Abstract templated class from which the templated class will inherit.
// HISTO is the class of the the histogram such as TH1D, TH2D etc.
template<class HISTO>
class MCBHistoDim : public virtual MCBHistoVirt, virtual public HISTO {
protected:
	MCBHistoDim(){}
public:
	// main constructor
	MCBHistoDim( std::string axesID, std::string cutID ) : MCBHistoVirt( axesID, cutID ),  HISTO()  {
		setName( constructHistoName().c_str() );
		setTitle(constructHistoName().c_str() );
	}
	//     Copy constructor
    MCBHistoDim( const MCBHistoDim& hist ) :
            MCBHistoVirt( hist ), HISTO() {
        if ( dynamic_cast<TNamed*>( this ) != nullptr ) {
            MCBHistoDim& histRef = const_cast<MCBHistoDim&>( hist );
            std::string newName = std::string( "clone_" )
                + histRef.getHisto()->GetName();
            HISTO* tempHisto = dynamic_cast<HISTO*>( HISTO::Clone( newName.c_str() ) );
            if ( tempHisto != nullptr ) {
                dynamic_cast<HISTO&>( *this ) = *tempHisto;
                delete tempHisto;
            }
        } else {
            std::stringstream errMessage;
            errMessage << "MCBHistoDim: Trying to use template parameter that does not inherit from TNamed ";
            std::runtime_error( errMessage.str() );
        }
    }

    // Copy constructor with a new name
    MCBHistoDim( const MCBHistoDim& hist, std::string newName ) :
            MCBHistoVirt(hist), HISTO() {
        if ( dynamic_cast<TNamed*>( this ) != 0 ) {
            HISTO* tempHist = dynamic_cast<HISTO*>( HISTO::Clone( newName.c_str() ) );
            if ( tempHist != nullptr ) {
                dynamic_cast<HISTO&>( *this ) = *tempHist;
                delete tempHist;
            }
        } else {
            std::stringstream errMessage;
            errMessage << "MCBHisto: Trying to use template parameter that does not inherit from TNamed ";
            std::runtime_error( errMessage.str() );
        }
    }

    virtual ~MCBHistoDim() {}

    MCBHistoDim& operator=( const MCBHistoDim& hist ) {
        if ( this != &hist ) {
            *dynamic_cast<MCBHistoVirt*>( this ) = dynamic_cast< const MCBHistoVirt&>( hist );
            *dynamic_cast<HISTO*>( this ) = dynamic_cast<const HISTO&>( hist );
        }
        return *this;
    }

    virtual inline TH1* getHisto() {
        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
            return dynamic_cast<TH1*>(new HISTO());
        return nullptr;
    }

//    virtual inline MCBHistoVirt& add( const MCBHistoVirt& histo, double scale = 1 ) {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
//            HISTO::Add( *dynamic_cast<HISTO*>(this), scale );
//        return *this;
//    }
//
//    virtual inline MCBHistoVirt& scale(  double scale = 1 ) {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
//            HISTO::Scale( scale );
//        return *this;
//    }

	// return class name of this histogram
    virtual inline std::string getHistoClassName() const  {
        std::string name = "";
        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
            name = HISTO::ClassName() ;
        return name;
    }

//    virtual inline std::string getName() const {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
//            return HISTO::GetName();
//        return "";
//    }
//
//    virtual inline std::string getTitle() const {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
//            return HISTO::GetTitle();
//        return "";
//    }
//
//    virtual inline std::string getMaximum() const {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
//            return HISTO::GetMaximum();
//        return 0;
//    }
//    virtual inline TAxis* getXAxis() {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
//            return HISTO::GetXaxis();
//        return nullptr;
//    }
//
//    virtual inline TAxis* getYAxis() {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
//            return HISTO::GetYaxis();
//        return nullptr;
//    }

//	virtual inline std::string setName( const std::string& name ) {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  ) {
//            HISTO::SetName( name.c_str() ) ;
//            return HISTO::GetName();
//        }
//        return "";
//	}
//	virtual inline std::string setTitle( const std::string& title ) {
//	    if( dynamic_cast<TH1*>(new HISTO()) != nullptr  ) {
//	        HISTO::SetTitle( title.c_str() ) ;
//	        return HISTO::GetTitle();
//	    }
//	    return "";
//	}
//
//    virtual inline std::string setXTitle( const std::string& title ) {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  ) {
//            HISTO::SetXTitle( title.c_str() ) ;
//            return HISTO::GetXaxis()->GetTitle();
//        }
//        return "";
//    }
//    virtual inline std::string setYTitle( const std::string& title ) {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  ) {
//            HISTO::SetXTitle( title.c_str() ) ;
//            return HISTO::GetYaxis()->GetTitle();
//        }
//        return "";
//    }
//
//	virtual void sumw2() {
//        if( dynamic_cast<TH1*>(new HISTO()) != nullptr  )
//            HISTO::Sumw2();
//	}
};

template <char* const TYPE>
class MCBHistoType : virtual public MCBHistoVirt {
protected:
    MCBHistoType() {}
public:
    // Return the TYPE template parameter as a string
    virtual inline std::string getType() const {
        return std::string(TYPE);
    }
    // method to calculate the appropriate histo name
    virtual inline std::string constructHistoName() const {
        return constructHistoName( getAxesID(), getCutID() );
    }
    static inline std::string constructHistoName( const std::string& axisID, const std::string& cutID ) {
        return std::string(TYPE) + "_" + axisID + "_" + cutID;
    }
};


// TYPE shows what the final histogram will be presenting (PARA, PERP, polarization, enhancement etc.
// Basically, TYPE defines what operations are expected to be performed on this histogram.
// DIM parameter represents the dimension of the histogram.
template<char* const TYPE, class HISTO>
class MCBHisto : public MCBHistoDim<HISTO>, public MCBHistoType<TYPE> {
protected:

    // Protect default constructor
    MCBHisto() {}
public:
    MCBHisto( const std::string axisIDd, const std::string cutID ) : MCBHistoVirt( axisIDd, cutID ), HISTO(),
    MCBHistoDim<HISTO>(), MCBHistoType<TYPE>() {}

	MCBHisto(ScanPositionPlots& plots, mcbTreeSelector& sel, std::string sourceName ) {
		std::cout << "Bad non-specialized call for TYPE " << TYPE << std::endl;
	}
	// Copy constructor
	MCBHisto( const MCBHisto<TYPE, HISTO>& hist ) : HISTO(), MCBHistoDim<HISTO>(hist),
	        MCBHistoType<TYPE>(hist), MCBHistoVirt(hist) {
	}
    // Copy constructor with a new name
    MCBHisto( const MCBHisto<TYPE, HISTO>& hist, std::string newName ) : MCBHistoVirt(hist),
            HISTO(), MCBHistoDim<HISTO>( hist ), MCBHistoType<TYPE>(hist) {
    }
    virtual ~MCBHisto(){}

    MCBHisto<TYPE, HISTO>& operator=( const MCBHisto& hist ) {
        if ( this != &hist ) {
            *dynamic_cast<MCBHistoDim<HISTO>*>( this ) = dynamic_cast<MCBHistoDim<HISTO>&>( hist );
            *dynamic_cast<MCBHistoType<TYPE>*>( this ) = dynamic_cast<MCBHistoType<TYPE>&>( hist );
        }
        return *this;
    }

	// Clone this histogram with a new name and same TYPE
	virtual inline MCBHistoVirt* clone( const std::string& newName ) const {
		return ( new MCBHisto<TYPE, HISTO>( *this, newName ) );
	}


	virtual MCBHistoVirt* cloneAndScale( MCBHistoVirt& hist, ScanPositionData& data) const;

	// Clones given histogram and scales it.
	template <char* const NEWTYPE>
	MCBHisto<NEWTYPE,HISTO>* cloneAndScale( ScanPositionData& data ) const ;

	ClassDef(MCBHisto,0)
};


using namespace std;

// Method to create a histogram of the same TYPE and HISTO as hist, but to have the histogram
// content from this object. Then scale the histogram using the weight from the selector sel which
// is done by cloning this object with a scale factor.
template<char* const TYPE, class HISTO>
MCBHistoVirt* MCBHisto<TYPE, HISTO>::cloneAndScale( MCBHistoVirt& hist, ScanPositionData& data ) const {
    MCBHistoVirt* newHisto = nullptr;
    MCBHisto<TYPE, HISTO>* oldHisto = this->cloneAndScale<TYPE>( data );
    if ( oldHisto != nullptr ) {
        std::string newName = hist.constructHistoName();
        newHisto = hist.clone( newName );
        if ( newHisto != nullptr ) {
            *dynamic_cast<MCBHistoDim<HISTO>*>( newHisto ) = *dynamic_cast<const MCBHistoDim<HISTO>*>( this );
        }
        delete oldHisto;
    }
    return newHisto;
}

// Creates a new object of NEWTYPE, then assigns the MCBHistoDim properties of this object
// to the new object.
template<char* const TYPE, class HISTO>
template<char* const NEWTYPE>
MCBHisto<NEWTYPE, HISTO>* MCBHisto<TYPE, HISTO>::cloneAndScale( ScanPositionData& data ) const {
    MCBHisto<NEWTYPE, HISTO>* newHisto = new MCBHisto<NEWTYPE, HISTO>();
    if ( newHisto != nullptr ) {
        newHisto->SetName( newHisto->constructHistoName().c_str() );
        newHisto->Sumw2();
        *dynamic_cast<MCBHistoDim<HISTO>*>( newHisto ) = *dynamic_cast<const MCBHistoDim<HISTO>*>( this );
        newHisto->Scale( data.getWeight() );
        return newHisto;
    }
    return newHisto;
}



// Partial specialization for 1D histograms
template<char* const TYPE>
class MCBHisto<TYPE, TH1D> : public MCBHistoDim<TH1D>, public MCBHistoType<TYPE> {
public:
    MCBHisto( std::string axesID, std::string cutID ) :
            MCBHistoVirt( axesID, cutID ), TH1D(), MCBHistoDim<TH1D>(), MCBHistoType<TYPE>() {
    }
    MCBHisto( std::string axesID, std::string cutID, Int_t nX, Double_t xMin, Double_t xMax ) :
            MCBHistoVirt( axesID, cutID ),
                    TH1D( MCBHisto<TYPE, TH1I>( axesID, cutID ).constructHistoName().c_str(),
                            MCBHisto<TYPE, TH1I>( axesID, cutID ).constructHistoName().c_str(), nX, xMin, xMax ),
                    MCBHistoDim<TH1D>(), MCBHistoType<TYPE>() {
    }
    virtual inline std::string constructHistoName() const {
        using histType = TH1D;
        return MCBHisto<TYPE,histType>::constructHistoName();
    }
    virtual inline MCBHistoVirt* clone( const std::string& name ) const {
        using histType = TH1D;
        return MCBHisto<TYPE,histType>::clone( name );
    }
    virtual inline MCBHistoVirt* cloneAndScale(MCBHistoVirt& hist, ScanPositionData& data ) const {
        using histType = TH1D;
        return MCBHisto<TYPE,histType>::cloneAndScale( hist, data );
    }
};

// Partial specialization for 2D histograms
template<char* const TYPE>
class MCBHisto<TYPE, TH2D> : public MCBHistoDim<TH2D>, public MCBHistoType<TYPE> {
public:
    MCBHisto( std::string axesID, std::string cutID ) :
            MCBHistoVirt( axesID, cutID ), TH2D(), MCBHistoDim<TH2D>(), MCBHistoType<TYPE>() {
    }
    MCBHisto( std::string axesID, std::string cutID, Int_t nX, Double_t xMin, Double_t xMax, Int_t nY, Double_t yMin,
            Double_t yMax ) :
            MCBHistoVirt( axesID, cutID ),
                    TH2D( MCBHisto<TYPE, TH2D>( axesID, cutID ).constructHistoName().c_str(),
                            MCBHisto<TYPE, TH2D>( axesID, cutID ).constructHistoName().c_str(), nX, xMin, xMax, nY,
                            yMin, yMax ), MCBHistoDim<TH2D>(), MCBHistoType<TYPE>() {
    }
    virtual inline std::string constructHistoName() const {
        using histType = TH2D;
        return MCBHisto<TYPE,histType>::constructHistoName();
    }
    virtual inline MCBHistoVirt* clone( const std::string& name ) const {
        using histType = TH2D;
        return MCBHisto<TYPE,histType>::clone( name );
    }
    virtual inline MCBHistoVirt* cloneAndScale(MCBHistoVirt& hist, ScanPositionData& data ) const {
        using histType = TH2D;
        return MCBHisto<TYPE,histType>::cloneAndScale( hist, data );
    }
};

// Partial specialization for 3D histograms
template<char* const TYPE>
class MCBHisto<TYPE, TH3D> : public MCBHistoDim<TH3D>, public MCBHistoType<TYPE>  {
public:
    MCBHisto( std::string axesID, std::string cutID ) :
            MCBHistoVirt( axesID, cutID ), TH3D(), MCBHistoDim<TH3D>(), MCBHistoType<TYPE>() {
    }
    MCBHisto( std::string axesID, std::string cutID, Int_t nX, Double_t xMin, Double_t xMax, Int_t nY, Double_t yMin,
            Double_t yMax, Int_t nZ, Double_t zMin, Double_t zMax ) :
            MCBHistoVirt( axesID, cutID ),
                    TH3D( MCBHisto<TYPE, TH3I>( axesID, cutID ).constructHistoName().c_str(),
                            MCBHisto<TYPE, TH3I>( axesID, cutID ).constructHistoName().c_str(), nX, xMin, xMax, nY,
                            yMin, yMax, nZ, zMin, zMax ), MCBHistoDim<TH3D>(), MCBHistoType<TYPE>() {
    }
    virtual inline std::string constructHistoName() const {
        using histType = TH3D;
        return MCBHisto<TYPE,histType>::constructHistoName();
    }
    virtual inline MCBHistoVirt* clone( const std::string& name ) const {
        using histType = TH3D;
        return MCBHisto<TYPE,histType>::clone( name );
    }
    virtual inline MCBHistoVirt* cloneAndScale(MCBHistoVirt& hist, ScanPositionData& data ) const {
        using histType = TH3D;
        return MCBHisto<TYPE,histType>::cloneAndScale( hist, data );
    }
};


//// Partial specialization for PARA
//template <class HISTO>
//class MCBHisto<paraString,HISTO>  {
//public:
//	static MCBHisto<paraString,HISTO>* createHisto(ScanPositionPlots& plots, mcbTreeSelector& sel, string sourceName ) {
//		std::cout << "Will create PARA histo" << std::endl;
//		MCBHisto<paraString,HISTO>** newHist = 0;
//		string scanType = sel.GetName();
//		string histType = scanType + string(paraString);
//
//		string histType = string(paraString) + para;
//		string histName = string(sel.GetName()) + "_" + histType;
//		newHist = dynamic_cast<TH1*>(sel.getHisto(sourceName)->Clone(histName.c_str()));
//		newHist->Sumw2();
//		cout << "Histo " << histName << " has been cloned" << endl;
//		newHist->SetTitle(histName.c_str());
//		newHist->Add(sel.getHisto( "difEg" ), -1.0);
//		cout << "Histo" << histName << " has been filled" << endl;
//
//		return 0;
//	}
//};
//

#endif /* MCBHISTO_HH_ */



