/*
 * WrapperVirt.hh
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#ifndef WRAPPERVIRT_HH_
#define WRAPPERVIRT_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <regex>
#include <iterator>
#include <functional>
#include <tuple>
#include <unordered_set>
#include <memory>

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TObject.h"

#include "MyFun.hh"

namespace Histo {

class WrapperVirt;

using creatorFunctionT = MyFunBase;
typedef WrapperVirt* (*copierFunctionT)( const WrapperVirt&, const std::string&, const std::string& );
typedef WrapperVirt* (*binaryOperatorT)( const WrapperVirt&, const WrapperVirt&, const std::string&,
        const std::string& );
typedef WrapperVirt* (*ternaryOperatorT)( const WrapperVirt&, const WrapperVirt&, const WrapperVirt&,
        const std::string&, const std::string& );

template<char* const TYPE, class HISTO>
class Wrapper;

class WrapperVirt {
//    template <typename... Ts>
//    static std::map<std::string, creatorFunctionT<Ts...>> creatorMap;
    static std::map<std::string, creatorFunctionT> creatorMap;
    static std::map<std::string, copierFunctionT> copierMap;
    static std::map<std::string, binaryOperatorT> binaryOperatorMap;
    static std::map<std::string, ternaryOperatorT> ternaryOperatorMap;

public:
    WrapperVirt() {
    }
    WrapperVirt( const WrapperVirt& wrapper ) {
    }
    virtual ~WrapperVirt() {
    }

    static int createConstructorMaps();

    static std::map<std::string, creatorFunctionT> &
    getCreatorMap() {
        return creatorMap;
    }
    static std::map<std::string, copierFunctionT>&
    getCopierMap() {
        return copierMap;
    }
    static std::map<std::string, binaryOperatorT>&
    getBinaryOperatorMap() {
        return binaryOperatorMap;
    }
    static std::map<std::string, ternaryOperatorT>&
    getTernaryOperatorMap() {
        return ternaryOperatorMap;
    }

    virtual TH1* getHist() = 0;
    virtual std::string getType() const = 0;
    virtual void* clone( const std::string aID = "", const std::string cID = "" ) const = 0;

    virtual WrapperVirt* getClassPointer() =0;

    static std::string constructHistoName( const std::string& aID, const std::string& cID ) {
        return aID + "_" + cID;
    }

    template<class WRAPPER>
    static WrapperVirt* create1D( const std::string& aID, const std::string& cID, int nX, double xMin, double xMax ) {
        return new WRAPPER( aID, cID, nX, xMin, xMax );
    }

    template<class WRAPPER>
    static WrapperVirt* create2D( const std::string& aID, const std::string& cID, int nX, double xMin, double xMax,
            int nY, double yMin, double yMax ) {
        return new WRAPPER( aID, cID, nX, xMin, xMax, nY, yMin, yMax );
    }
    template<class WRAPPER>
    static WrapperVirt* create3D( const std::string& aID, const std::string& cID, int nX, double xMin, double xMax,
            int nY, double yMin, double yMax, int nZ, double zMin, double zMax ) {
        return new WRAPPER( aID, cID, nX, xMin, xMax, nY, yMin, yMax, nZ, zMin, zMax );
    }

    template<class WRAPPER>
    static WrapperVirt* copy( const WrapperVirt& wrapper, const std::string& aID, const std::string& cID ) {
        return new WRAPPER( wrapper, aID, cID );
    }
    template<class WRAPPER>
    static WrapperVirt* binaryOperator( const WrapperVirt& wrapper1, const WrapperVirt& wrapper2,
            const std::string& aID, const std::string& cID ) {
        return new WRAPPER( wrapper1, wrapper2, aID, cID );
    }
    template<class WRAPPER>
    static WrapperVirt* ternaryOperator( const WrapperVirt& wrapper1, const WrapperVirt& wrapper2,
            const WrapperVirt& wrapper3, const std::string& aID, const std::string& cID ) {
        return new WRAPPER( wrapper1, wrapper2, wrapper3, aID, cID );
    }
};

} /* namespace Histo */

#endif /* WRAPPERVIRT_HH_ */
