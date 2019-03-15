#include "tbb/parallel_sort.h"
#include "tbb/tick_count.h"
#include <algorithm>
#include <math.h>
#include <iostream>
#define N 100000

using namespace tbb;

int main() {

    float a[N], e[N];
    int b[N];
    short c[N];
    double d[N];
    for( int i = 0; i < N; i++ ) {
        a[i] = sin(i);
        b[i] = N%(i+1);
        c[i] = i % 100;
        d[i] = 1/cos(i);
        e[i] = cos(i);
    }
   
    tick_count t2 , t1 ;
    tick_count t0 = tick_count::now();
    parallel_sort(a, a + N);
    t1 = tick_count::now();
    std::cout << "Czas sortowania nr 1: " << (t1 - t0).seconds() << " s." << std::endl;

    t1 = tick_count::now();
    parallel_sort(b, b + N, std::greater<int>());
    t2 = tick_count::now();
    std::cout << "Czas sortowania nr 2: " << (t2 - t1).seconds() << " s." << std::endl;

    t1 = tick_count::now();
    parallel_sort(c);
    t2 = tick_count::now();
    std::cout << "Czas sortowania nr 3: " << (t2 - t1).seconds() << " s." << std::endl;

    t1 = tick_count::now();
    parallel_sort(d, std::greater<float>());
    t2 = tick_count::now();
    std::cout << "Czas sortowania nr 4: " << (t2 - t1).seconds() << " s." << std::endl;
    std::cout << "Całkowity czas sortowań: " << (t2 - t0).seconds() << " s." << std::endl;
    
    t1 = tick_count::now();
    std::sort(e, e + N, std::greater<float>());
    t2 = tick_count::now();
    std::cout << "Czas sortowania nierównoległego: " << (t2 - t1).seconds() << " s." << std::endl;

    return 0;
}