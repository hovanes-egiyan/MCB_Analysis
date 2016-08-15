/*
 * MyFun.hh
 *
 *  Created on: Aug 12, 2016
 *      Author: hovanes
 */

#ifndef MYFUN_HH_
#define MYFUN_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <regex>
#include <iterator>
#include <functional>
#include <memory>


struct MyFunBase {
    MyFunBase() {}
    virtual ~MyFunBase(){}
};

template <class T>
class MyFun : public MyFunBase {
private:
    std::function<T> func;
public:
    MyFun(){}
    virtual ~MyFun(){}
    virtual std::function<T>& getFunc() {
        return func;
    }
    virtual  std::function<T>& operator()() {
        return func;
    }
    virtual void setFunc( std::function<T>& funct ) {
        func = funct;
    }
};



#endif /* MYFUN_HH_ */
