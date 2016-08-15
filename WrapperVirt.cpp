/*
 * WrapperVirt.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: hovanes
 */

#include "WrapperVirt.hh"
#include "Wrapper.hh"

using namespace std;
using namespace std::placeholders;
// for _1, _2, _3...

namespace Histo {

typedef Wrapper<perpString, TH1D> perp1D;

//map<std::string, WrapperVirt* (WrapperVirt::*)( const string&, const string&, int, double, double )> WrapperVirt::creatorMap;
//map<std::string, WrapperVirt* (WrapperVirt::*)( const WrapperVirt&, const string&, const string& )> WrapperVirt::copierMap;
//map<std::string, WrapperVirt* (WrapperVirt::*)( const WrapperVirt&, const WrapperVirt&, const std::string&, const std::string& )> WrapperVirt::binaryOperatorMap;
//map<std::string,
//        WrapperVirt* (WrapperVirt::*)( const WrapperVirt&, const WrapperVirt&, const WrapperVirt&, const std::string&,
//                const std::string& )> WrapperVirt::ternaryOperatorMap;
map<std::string, creatorFunctionT> WrapperVirt::creatorMap;
map<std::string, copierFunctionT> WrapperVirt::copierMap;
map<std::string, binaryOperatorT> WrapperVirt::binaryOperatorMap;
map<std::string, ternaryOperatorT> WrapperVirt::ternaryOperatorMap;

volatile int dummyInt = WrapperVirt::createConstructorMaps();

int WrapperVirt::createConstructorMaps() {
    MyFun   <WrapperVirt* ( const std::string&, const std::string&, int, double, double )> f;
    function<WrapperVirt* ( const std::string&, const std::string&, int, double, double )> tmpFun = bind(
            &WrapperVirt::create1D<Wrapper<rawString, TH1D>>, _1, _2, _3, _4, _5 );
    f.setFunc( tmpFun );
    // Constructors
    creatorMap["Histo::Wrapper<&Histo::rawString, TH1D>"] = f;
    // Unary operators
    copierMap["Histo::Wrapper<&Histo::rawString, TH1D>"] = &copy<Wrapper<rawString, TH1D>>;
    copierMap["Histo::Wrapper<&Histo::scaledString, TH1D>"] = &copy<Wrapper<scaledString, TH1D>>;
    copierMap["Histo::Wrapper<&Histo::paraString, TH1D>"] = &copy<Wrapper<paraString, TH1D>>;
    copierMap["Histo::Wrapper<&Histo::perpString, TH1D>"] = &copy<Wrapper<perpString, TH1D>>;
    copierMap["Histo::Wrapper<&Histo::scanString, TH1D>"] = &copy<Wrapper<scanString, TH1D>>;
    // Binary operators
    binaryOperatorMap["Histo::Wrapper<&Histo::paraString, TH1D>"] = &binaryOperator<Wrapper<paraString, TH1D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::perpString, TH1D>"] = &binaryOperator<Wrapper<perpString, TH1D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::diamondString, TH1D>"] = &binaryOperator<Wrapper<diamondString, TH1D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::enancementString, TH1D>"] = &binaryOperator<
            Wrapper<enancementString, TH1D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::coherentPolarizationString, TH1D>"] = &binaryOperator<
            Wrapper<coherentPolarizationString, TH1D>>;

    // Ternary operators
    ternaryOperatorMap["Histo::Wrapper<&Histo::polarizationString, TH1D>"] = &ternaryOperator<
            Wrapper<polarizationString, TH1D>>;

    // 2D operators
    // Constructors
    MyFun   <WrapperVirt* ( const std::string&, const std::string&, int, double, double, int, double, double )> f2;
    function<WrapperVirt* ( const std::string&, const std::string&, int, double, double, int, double, double)> tmpFun2 = bind(
            &WrapperVirt::create2D<Wrapper<scanString, TH2D>>, _1, _2, _3, _4, _5, _6, _7, _8 );
    f2.setFunc( tmpFun2 );
    creatorMap["Histo::Wrapper<&Histo::scanString, TH2D>"] = f2;
    // Unary operators
    copierMap["Histo::Wrapper<&Histo::scaledString, TH2D>"] = &copy<Wrapper<scaledString, TH2D>>;
    copierMap["Histo::Wrapper<&Histo::scanString, TH2D>"] = &copy<Wrapper<scanString, TH2D>>;
    // Binary operators
    binaryOperatorMap["Histo::Wrapper<&Histo::paraString, TH2D>"] = &binaryOperator<Wrapper<paraString, TH2D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::perpString, TH2D>"] = &binaryOperator<Wrapper<perpString, TH2D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::diamondString, TH2D>"] = &binaryOperator<Wrapper<diamondString, TH2D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::enancementString, TH2D>"] = &binaryOperator<
            Wrapper<enancementString, TH2D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::coherentPolarizationString, TH2D>"] = &binaryOperator<
            Wrapper<coherentPolarizationString, TH2D>>;
    // Ternary operators
    ternaryOperatorMap["Histo::Wrapper<&Histo::polarizationString, TH2D>"] = &ternaryOperator<
            Wrapper<polarizationString, TH2D>>;

    // 3D operators
    // Constructors
    MyFun   <WrapperVirt* ( const std::string&, const std::string&, int, double, double, int, double, double, int, double, double )> f3;
    function<WrapperVirt* ( const std::string&, const std::string&, int, double, double, int, double, double, int, double, double)> tmpFun3 = bind(
            &WrapperVirt::create3D<Wrapper<scanString, TH3D>>, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11 );
    f3.setFunc( tmpFun3 );
    creatorMap["Histo::Wrapper<&Histo::scanString, TH3D>"] = f3;
    // Unary operators
    copierMap["Histo::Wrapper<&Histo::scaledString, TH3D>"] = &copy<Wrapper<scaledString, TH3D>>;
    copierMap["Histo::Wrapper<&Histo::scanString, TH3D>"] = &copy<Wrapper<scanString, TH3D>>;
    // Binary operators
    binaryOperatorMap["Histo::Wrapper<&Histo::paraString, TH3D>"] = &binaryOperator<Wrapper<paraString, TH3D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::perpString, TH3D>"] = &binaryOperator<Wrapper<perpString, TH3D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::diamondString, TH3D>"] = &binaryOperator<Wrapper<diamondString, TH3D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::enancementString, TH3D>"] = &binaryOperator<
            Wrapper<enancementString, TH3D>>;
    binaryOperatorMap["Histo::Wrapper<&Histo::coherentPolarizationString, TH3D>"] = &binaryOperator<
            Wrapper<coherentPolarizationString, TH3D>>;
    // Ternary operators
    ternaryOperatorMap["Histo::Wrapper<&Histo::polarizationString, TH3D>"] = &ternaryOperator<
            Wrapper<polarizationString, TH3D>>;


    return 0;
}

} /* namespace Histo */
