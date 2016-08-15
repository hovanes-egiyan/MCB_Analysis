/*
 * Container.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "Container.hh"
using namespace std;

char Histo::rawString[] = "raw";
char Histo::scaledString[] = "scaled";
char Histo::paraString[] = "para";
char Histo::perpString[] = "perp";
char Histo::enancementString[] = "enhancement";
char Histo::diamondString[] = "diamond";
char Histo::polarizationString[] = "polarization";
char Histo::coherentPolarizationString[] = "coherentPolarization" ;
char Histo::scanString[] = "scan";

namespace Histo {

std::string Container::axisNameDelimiter = "_";

// copy assignment operator with deep copy of the map
Container& Container::operator =( const Container& cont ) {
    if( this != &cont ) {
        for( auto& axesIter : cont.histoMap ) {
            auto& axesID = axesIter.first;
            auto& cutMap = axesIter.second;
            for( auto& cutIter : cutMap ) {
                auto cutID = cutIter.first;
                auto& wrapper = cutIter.second;
                std::string oldName = reinterpret_cast<TH1*>(wrapper->getHist())->GetName();
                histoMap[axesID][cutID] =  reinterpret_cast<WrapperVirt*>( wrapper->clone( oldName ) );
            }
        }
    }
    return *this;
}

std::string Container::replaceHistoSuffix( string oldString , const string& oldSuffix, const string& newSufffix ) {
    boost::replace_all( oldString, string(oldSuffix), string(newSufffix) );
    return oldString;
}


} /* namespace Histo */
