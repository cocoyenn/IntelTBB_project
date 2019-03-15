#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#include <vector>
#include<iostream>
#define N 10000
using namespace tbb;

int main(){
    concurrent_vector<int> vector;
    
    tbb::parallel_for(0, N, 1, [&](int i){vector.push_back(i); });

    std::cout << vector.size() << std::endl;
    
    for(auto element: vector){
        std::cout << "Value: " <<  element  <<  std::endl;
    }
}