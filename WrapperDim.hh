/*
 * WrapperDim.hh
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#ifndef WRAPPERDIM_HH_
#define WRAPPERDIM_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <regex>
#include <iterator>
#include <type_traits>

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
#include "WrapperAxesCuts.hh"

namespace Histo {

template<class HISTO>
class WrapperDim :  public WrapperVirt,  public WrapperAxesCuts {
    static_assert(std::is_base_of<TH1, HISTO>::value, "Template argument to class WrapperDim<HISTO> is not subclass of TH1");
protected:
    HISTO* histoPtr = 0;
public:
    WrapperDim( const std::string aID = "", const std::string cID = "" ) :
             WrapperAxesCuts( aID, cID ), histoPtr( new HISTO() ) {
        histoPtr->SetName( constructHistoName( aID, cID ).c_str() );
    }
    WrapperDim( const HISTO& hist, const std::string aID = "", const std::string cID = "" ) :
             WrapperAxesCuts( aID, cID ) {
//        std::cout << "WrapperDim( const HISTO& hist, const std::string aID = " ", const std::string cID = " "  )"
//                << std::endl;
//        std::cout << "The pointer to histogram is " << std::hex << std::showbase << &hist << std::dec << std::endl;
//        hist.Print();
//        std::cout << "Axis name is " << axesID << " cut name is " << cutID << std::endl;
//        std::cout << "Histogram name will be " << constructHistoName( aID, cID ) << std::endl;
        if ( dynamic_cast<const TH1*>( &hist ) != nullptr )
            histoPtr = reinterpret_cast<HISTO*>( hist.Clone( constructHistoName( aID, cID ).c_str() ) );
    }
    WrapperDim( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "" ) :
             WrapperAxesCuts( aID, cID ) {
        WrapperVirt& wrapperRef = const_cast<WrapperVirt&>( wrapper );
        HISTO* objHistPtr = dynamic_cast<HISTO*> ( wrapperRef.getHist() );
        if ( objHistPtr != nullptr && dynamic_cast<TH1*>( objHistPtr ) != nullptr ) {
            this->histoPtr = dynamic_cast<HISTO*>( objHistPtr->Clone(
                    constructHistoName( aID, cID ).c_str() ) );
        }
    }
    virtual ~WrapperDim() {
        if ( this->histoPtr != 0 ) delete histoPtr;
    }

    WrapperDim& operator=( const WrapperDim& wrapper ) {
        if ( this != &wrapper ) {
            WrapperDim& wrapperRef = const_cast<WrapperDim&>( wrapper );
            if ( dynamic_cast<TH1*>( wrapperRef.getHistoPtr() ) != nullptr ) {
                this->histoPtr = dynamic_cast<HISTO*>( wrapperRef.getHistoPtr()->Clone( wrapperRef.getHistoPtr()->GetName() ) );
            }
        }
        return *this;
    }

    virtual std::string getHistoClassName() const  {
        std::string name = "";
        HISTO* tempObject = new HISTO();
        if( dynamic_cast<TH1*>( tempObject ) != nullptr ) {
            name = tempObject->ClassName() ;
        }
        return name;
    }

    virtual WrapperVirt* getClassPointer() override {
        return this;
    }

    virtual TH1* getHist() override {
        return this->getHistoPtr();
    }

    virtual HISTO* getHistoPtr() {
        return histoPtr;
    }

    virtual HISTO* setHistoPtr( HISTO* histoPtr = nullptr ) {
        return ( this->histoPtr = histoPtr );
    }
};

} /* namespace Histo */

#endif /* WRAPPERDIM_HH_ */
