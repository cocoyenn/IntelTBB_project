#include "tbb/task_group.h"  
#include "tbb/parallel_invoke.h"
#include <iostream>
#include "tbb/tick_count.h"
using namespace tbb;

#include "tbb/task_group.h"

int Fib(int n) {
    if( n<2 ) {return n;} 
    int x, y;

    tbb::task_group g;
        g.run([&]{x=Fib(n-1);}); // spawn a task
        g.run([&]{y=Fib(n-2);}); // spawn another task
        g.wait();                // wait for both tasks to complete
    return x+y;
}

int main(){
         
    tick_count t0 = tick_count::now();
    std::cout << Fib(20) << std::endl;
    tick_count t1 = tick_count::now();
    std::cout << (t1 - t0).seconds() << std::endl;
    
}
