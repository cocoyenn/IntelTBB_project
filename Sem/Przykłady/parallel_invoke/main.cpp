#include "tbb/parallel_invoke.h"
#include <iostream>
using namespace tbb;

void function(){
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

class MyFunctor {
    int arg ;
public:
    MyFunctor(int a) : arg(a) {}
    void operator()() const { std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
    void fun() const { std::cout <<  __PRETTY_FUNCTION__ << std::endl;}
};



int main() {
    MyFunctor functor(3);
    parallel_invoke(function,[]() { std::cout << __PRETTY_FUNCTION__ << std::endl;},  functor );

}