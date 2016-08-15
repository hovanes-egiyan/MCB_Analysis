#include "TROOT.h"
#include "TSystem.h"\

extern TSystem* gSystem;

void loadLibraries() {
    (TSystem*)gSystem->Load("/home/hovanes/ROOT/slib/libTUserMCBIO.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/WrapperAxesCuts.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/Container.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/WrapperVirt.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/WrapperDim.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/WrapperType.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/WrapperPhysics.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/Wrapper.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/ScanPositionData.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/ScanMCB.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/mcbTreeSelector.so");
//    gSystem->Load("/home/hovanes/ROOT/slib/ScanPositionPlots.so");
}
