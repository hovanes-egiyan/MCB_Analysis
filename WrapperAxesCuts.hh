/*
 * WrapperAxesCuts.hh
 *
 *  Created on: Aug 8, 2016
 *      Author: hovanes
 */

#ifndef WRAPPERAXESCUTS_HH_
#define WRAPPERAXESCUTS_HH_

#include "WrapperVirt.hh"

namespace Histo {

class WrapperAxesCuts {
protected:
    std::string axesID;
    std::string cutID;
public:
    WrapperAxesCuts( const std::string aID = "", const std::string cID = "" ) :
           axesID( aID ), cutID( cID ) {
    }
    WrapperAxesCuts( const WrapperAxesCuts& wrapper ) :
           axesID( wrapper.axesID ), cutID( wrapper.cutID ) {
    }

    virtual ~WrapperAxesCuts() {
    }
    WrapperAxesCuts& operator=( const WrapperAxesCuts& wrapper ) {
        if ( this != &wrapper ) {
            axesID = wrapper.axesID;
            cutID = wrapper.cutID;
        }
        return *this;
    }

    virtual std::string getAxesID() const {
        return axesID;
    }
    virtual std::string setAxesID( const std::string& axesId ) {
        return ( axesID = axesId );
    }
    virtual std::string getCutID() const {
        return cutID;
    }
    virtual std::string setCutID( const std::string& cutId ) {
        return ( cutID = cutId );
    }
};

} /* namespace Histo */

#endif /* WRAPPERAXESCUTS_HH_ */
