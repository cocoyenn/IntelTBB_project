#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include <math.h>
#include <iostream>
using namespace tbb;

#define N 1000000
class Sum {

    public:
    Sum() : value(0) {}

    Sum( Sum& s, split ) {value = 0;}
    void join( Sum& rhs ) {value += rhs.value;}

    void operator()( const blocked_range<float*>& r ) {
        float temp = value;
        for( float* a=r.begin(); a!=r.end(); ++a ) {
            temp += *a;
        }
        value = temp;
    }
    
    float getValue() const {return value;}
    private:
        float value;
};

float ParallelSum( float array[], size_t n ) {
    Sum total;
    parallel_reduce( blocked_range<float*>( array, array+n ),
                     total );
    return total.getValue();
}


int main(){
    float a[N];

    for( int i = 0; i < N; i++ ) {
        a[i] = i*0.0304;
    }

    std::cout << ParallelSum( a, N ) << std::endl;
}