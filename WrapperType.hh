/*
 * WrapperType.hh
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#ifndef WRAPPERTYPE_HH_
#define WRAPPERTYPE_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <regex>
#include <iterator>
#include <functional>

#include "WrapperVirt.hh"
#include "WrapperDim.hh"


namespace Histo {

extern char rawString[];
extern char scaledString[];
extern char paraString[];
extern char perpString[];
extern char diamondString[];
extern char enancementString[];
extern char polarizationString[];
extern char coherentPolarizationString[];
extern char scanString[];

template<char* const TYPE>
struct WrapperTypeI {
    WrapperTypeI() {
    }
    virtual ~WrapperTypeI() {
    }
};


template<char* const TYPE, class HISTO>
struct WrapperType : public WrapperTypeI<TYPE>, public WrapperDim<HISTO> {
    WrapperType( const std::string aID = "", const std::string cID = "" ) :
             WrapperDim<HISTO>( aID, cID ) {
    }
    WrapperType( const HISTO& hist, const std::string aID = "", const std::string cID = "" ) :
             WrapperDim<HISTO>( hist, aID, cID ) {
    }
    WrapperType( const WrapperVirt& wrapper, const std::string aID = "", const std::string cID = "" ) :
             WrapperDim<HISTO>( wrapper, aID, cID ) {
    }
    virtual ~WrapperType() {
    }
    virtual std::string getType() const override {
        return getClassType();
    }

    virtual void* clone( const std::string aID = "", const std::string cID = "" ) const override  {
        return new WrapperType( *this, aID, cID );
    }
    virtual std::string constructHistoName() const {
//        return this->getType() + "_" + this->getAxesID() + "_" + this->getCutID();
        return WrapperType<TYPE,HISTO>::constructHistoName( this->getAxesID(), this->getCutID() );

    }
    static std::string getClassType() {
        return std::string( TYPE ) ;
    }
    static std::string constructHistoName( const std::string& aID, const std::string& cID ) {
        return getClassType() + "_" + WrapperDim<HISTO>::constructHistoName( aID, cID );
    }

};

} /* namespace Histo */

#endif /* WRAPPERTYPE_HH_ */
