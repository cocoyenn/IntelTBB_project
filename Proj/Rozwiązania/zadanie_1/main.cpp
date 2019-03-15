/*Zadanie 1. 
Proszę zaimplementować funckcję ParallelMonteCarlo,
która zwraca wyestymowaną wartość liczby PI dla N elementów zgodnie z metodą Monte Carlo.
Funkcja ParallelMonteCarlo wykorzystuje algorytm  tbb::paraller_reduce,
dla którego argumentem funckyjnym może być funktor albo lambda.
Pliku main.cpp proszę nie modyfikować.
Spodziewany(!) output programu podany jest na końcu pliku.

Generator liczb pseudolosowych:
float RandomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

*/

#include <iostream>
#include "monte_carlo.h"
#include "tbb/tick_count.h"
#define N 100000

int main(){
    tbb::tick_count t0 = tbb::tick_count::now();
    std::cout << "Parallel Monte Carlo: "<< ParallelMonteCarlo(N) << std::endl;
    tbb::tick_count t1 = tbb::tick_count::now();

    std::cout <<"Czas wykonania programu: " << (t1-t0).seconds() << std::endl;
}

/*---OUTPUT---
Parallel Monte Carlo: 3.13584
Czas wykonania programu: 1.79412
*/