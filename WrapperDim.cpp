/*
 * WrapperDim.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#include "WrapperDim.hh"
#include "WrapperPhysics.hh"

using namespace std;

namespace Histo {


template class WrapperDim<TH1D>;
template class WrapperDim<TH2D>;
template class WrapperDim<TH3D>;


} /* namespace Histo */
