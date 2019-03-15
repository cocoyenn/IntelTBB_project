#pragma once

#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include <random>
#include <math.h>
#include <iostream>

using namespace tbb;

float RandomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

class MonteCarlo
{
  public:
    MonteCarlo(long n) : number(n), countCircle(0) {}

    MonteCarlo(MonteCarlo &s, split) { countCircle = 0; }

    void join(MonteCarlo &rhs) { countCircle += rhs.countCircle; }

    void operator()(const blocked_range<int> &range)
    {
        long temp = countCircle;
        for (int a = range.begin(); a != range.end(); ++a)
        {
            float t1 = RandomFloat(-1.0, 1.0);
            float t2 = RandomFloat(-1.0, 1.0);
            if (sqrt(t1 * t1 + t2 * t2) < 1.0)
            {
                temp++;
            }
        }
        countCircle = temp;
    }

    float getPI() const
    {
        return (4.0 * countCircle) / number;
    }

  private:
    long number;
    long countCircle;
};

float ParallelMonteCarlo(size_t n)
{
    MonteCarlo total(n);
    simple_partitioner ap;
    parallel_reduce(blocked_range<int>(0, n, 100), total, ap);

    return total.getPI();
}
