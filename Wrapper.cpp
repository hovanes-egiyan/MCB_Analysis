/*
 * Wrapper.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#include <iostream>

#include "TH1.h"

#include "WrapperType.hh"
#include "Wrapper.hh"

namespace Histo {
//template class Wrapper<rawString, TH1D>;
//template class Wrapper<rawString, TH2D>;
//template class Wrapper<rawString, TH3D>;
//
//
//template class Wrapper<perpString, TH1D>;
//template class Wrapper<perpString, TH2D>;
//template class Wrapper<perpString, TH3D>;

template class Wrapper<scanString, TH2D>;

} /* namespace Histo */
