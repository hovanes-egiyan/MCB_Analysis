/*
 * Container.hh
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#ifndef CONTAINER_HH_
#define CONTAINER_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>

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

#include "Wrapper.hh"

namespace Histo {

class Container {
protected:
    // string for axis name delimiter
    static std::string axisNameDelimiter;

    // Map of pointers to the histograms for this
    // scan point, like energy dependence etc. The first key is what on axes,
    // the second index is a string specifies the conditions/cuts.
    std::map<std::string, std::map<std::string, WrapperVirt*> > histoMap;

public:
    Container() {
    }
    Container( const Container& cont ) {
        *this = cont;
    }
    virtual ~Container() {
        histoMap.clear();
    }
    Container& operator=( const Container& cont );

    virtual void createHistograms() = 0;

    static const std::string& getAxisNameDelimiter() {
        return axisNameDelimiter;
    }
    static std::string& setAxisNameDelimiter( const std::string& axisNameDelimiter ) {
        return ( Container::axisNameDelimiter = axisNameDelimiter );
    }
    virtual std::map<std::string, std::map<std::string, WrapperVirt*> >& getHistoMap() {
        return histoMap;
    }
    virtual std::map<std::string, std::map<std::string, WrapperVirt*> >& setHistoMap(
            const std::map<std::string, std::map<std::string, WrapperVirt*> >& histoMap ) {
        return (this->histoMap = histoMap);
    }

    static std::string addHistoSuffix( const std::string& oldString , const std::string& siffix ) {
        return oldString + getAxisNameDelimiter() + siffix;
    }
    static std::string replaceHistoSuffix( std::string oldString , const std::string& oldSuffix, const std::string& newSuffix );
};

} /* namespace Histo */

#endif /* CONTAINER_HH_ */
