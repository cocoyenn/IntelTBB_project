#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include "tbb/tick_count.h"
#include <math.h>
#include <iostream>
#include <functional>

using namespace tbb;
#define L 350
#define M 625
#define N 500

void SerialMatrixMultiply( float c[M][N], float a[M][L], float b[L][N] ) {
 for( size_t i=0; i<M; ++i ) {
    for( size_t j=0; j<N; ++j ) {
    float sum = 0.0;
        for( size_t k=0; k<L; ++k ){
            sum += a[i][k]*b[k][j];
        }
        c[i][j] = sum;
    }
 }
}

class MatrixMultiplyBody2D {
 float (*m_a)[L];
 float (*m_b)[N];
 float (*m_c)[N];
public:
    MatrixMultiplyBody2D( float c[M][N], float a[M][L], float b[L][N] ) :
        m_a(a), m_b(b), m_c(c)
    {}

 void operator()( const blocked_range2d<size_t>& r ) const {
    for( size_t i=r.rows().begin(); i!=r.rows().end( ); ++i ){
        for( size_t j=r.cols().begin(); j!=r.cols().end( ); ++j ) {
        float sum = 0;
            for( size_t k=0; k<L; ++k ){
                sum += m_a[i][k]*m_b[k][j];
            }
        m_c[i][j] = sum;
        }
    }
}
 
};
void ParallelMatrixMultiply(float c[M][N], float a[M][L], float b[L][N]){
 parallel_for( blocked_range2d<size_t>(0, M, 0, N), MatrixMultiplyBody2D(c,a,b) );
}

int main(){
    float a[M][L], b[L][N], c[M][N];
    tick_count t2 , t1 ;

  // setMatrix(a, M, L, sin);
   parallel_for( blocked_range2d<size_t>(0, M, 0, L), [&](const blocked_range2d<size_t> r ){
      for( size_t i=r.rows().begin(); i!=r.rows().end( ); ++i ){
        for( size_t j=r.cols().begin(); j!=r.cols().end( ); ++j ) {
            a[i][j] = sin(j);
            }
        }
   });
    parallel_for( blocked_range2d<size_t>(0, L, 0, N), [&](const blocked_range2d<size_t> r ){
      for( size_t i=r.rows().begin(); i!=r.rows().end( ); ++i ){
        for( size_t j=r.cols().begin(); j!=r.cols().end( ); ++j ) {
            b[i][j] = cos(j);
            }
        }
   });
   parallel_for( blocked_range2d<size_t>(0, M, 0, N), [&](const blocked_range2d<size_t> r ){
      for( size_t i=r.rows().begin(); i!=r.rows().end( ); ++i ){
        for( size_t j=r.cols().begin(); j!=r.cols().end( ); ++j ) {
            c[i][j] = 0.0;
            }
        }
   });
    
    t1 = tick_count::now();
    ParallelMatrixMultiply(c,a,b);
    t2 = tick_count::now();
    std::cout << "Parallel martix multiple time: " << (t2 - t1).seconds() << " s." << std::endl;

    t1 = tick_count::now();
    SerialMatrixMultiply( c, a, b);
    t2 = tick_count::now();
    std::cout << "Serial matrix multiple time: " << (t2 - t1).seconds() << " s." << std::endl;
}