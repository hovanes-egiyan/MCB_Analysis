/*
 * MCBHisto.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: Hovanes Egiyan
 */

#include "MCBHisto.hh"

//template<char* const TYPE, unsigned DIR>
templateClassImp(MCBHisto)

using namespace std;
using namespace MCBHistStringTypes;

template class MCBHisto<mtString,TH1D>;
template class MCBHisto<rawString,TH1D>;
template class MCBHisto<paraString,TH1D>;

//MCBHisto<paraString,TH1D>* dummyHist = MCBHisto<paraString,TH1D>::createHisto( paraString, "AA", 100, 10., 20 );

MCBHisto<paraString,TH1D> dummyHist( "aa", "AA", 100, 10., 20 );
