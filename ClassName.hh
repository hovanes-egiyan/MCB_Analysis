/*
 * ClassName.hh
 *
 *  Created on: Aug 11, 2016
 *      Author: hovanes
 */

#ifndef CLASSNAME_HH_
#define CLASSNAME_HH_

#include <string>
#include <typeinfo>
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>


std::string demangle(const char* name);

template <class T>
std::string className(const T& t) {

    return demangle(typeid(t).name());
}

template <class T>
std::string replaceClassName( const T& t, const std::string& oldString, const std::string& newString ) {
    std::string cName = demangle(typeid(t).name());
    boost::replace_all( cName, string(oldString), string(newString) );
    return cName;
}


#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

#include "ClassName.hh"

using namespace std;

std::string demangle(const char* name) {

    int status = -4; // some arbitrary value to eliminate the compiler warning

    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : name ;
}

#else

// does nothing if not g++
std::string demangle(const char* name) {
    return name;
}

#endif


#endif /* CLASSNAME_HH_ */
