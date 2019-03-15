#include "tbb/tbb.h"
#include <vector>
#include <iostream>
 
#define N 1000

using namespace tbb;
using namespace std;
 
vector<int> Data;
bool Data2d[N][N];
 
int main() {
    Data.resize(1000);
    try {
        parallel_for( blocked_range<int>(0, 2000), []( const blocked_range<int>& r ) {
        for( int i=r.begin(); i!=r.end(); ++i )
            Data.at(i) += 1;
        });
    } catch( captured_exception& ex ) {
       cout << "captured_exception: " << ex.what() << endl;
    } catch( out_of_range& ex ) {
       cout << "out_of_range: " << ex.what() << endl;
    }

    // try {
    //     parallel_for( 0, 1000, 1, 
    //         []( int i ) {
    //             task_group_context root(task_group_context::isolated);
    //             parallel_for( 0, 1000, 1,
    //                [&i]( int j ) {
    //                    Data[i][j] = true;
    //                },
    //                root);
    //             throw "oops";
    //         });
    // } catch(...) {
    // }
   


    return 0;
}