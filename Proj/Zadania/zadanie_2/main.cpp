/*zadanie 2
Przedmiotem zadania jest implementacja:
1. klasy szablonowej Vector opakowywującej tbb:concurrent_vector oraz rozszerzającej go o metodę print().
2. algorytmu quick_sort z wykorzystaniem zarządzania task'ami implementowanymi przez klasę task_group.
Proszę nie modyfikować pliku main.cpp.
*/

#include "main.h"
#include <iostream>
#include "tbb/parallel_for.h"
#include "tbb/parallel_sort.h"
#include "tbb/tick_count.h"

using namespace tbb;
#define N 1000000

int main()
{
	Vector<int> vector;
	std::cout << "Vector maxsize: " << vector.max_size() << ", capacity " << vector.capacity() << std::endl << std::endl;

	tbb::parallel_for(1, 50, 1, [&](int i) { vector.push_back( 100%i ); });

	std::cout << "Nieposortowany vector: "<< std::endl;
	vector.print();

	quick_sort<int>(vector, 0, vector.size() - 1);
	
	std::cout << "Sprawdzenie poprawności algorytmu: "<< std::endl;
	vector.print();

	vector.clear();
	tbb::parallel_for(0, N, 1, [&](int i) { vector.push_back(i % 100 ); });
	std::cout << "\nVector size:" << vector.size() << ", capacity " << vector.capacity() << std::endl << std::endl;

	tick_count t0 = tick_count::now();
	quick_sort<int>(vector, 0, vector.size() - 1);
	tick_count t1 = tick_count::now();

	std::cout << "quick_sort: czas wykonania algorytmu: " << (t1 - t0).seconds() << std::endl << std::endl;

	vector.clear();

	std::cout << "Wypełniamy vector na nowo..." << std::endl;
	tbb::parallel_for(0, N / 4, 1, [&](int i) { vector.grow_by({i % 10 + 1, i % 100 + 2, i % 1000 + 3, i % 10000 + 4}); });

	std::cout << "i kopiujemy go." << std::endl;
	Vector<int> vector_copy(vector);

	std::cout << "Skopiowany vector: size " << vector_copy.size() << ", capacity " << vector_copy.capacity() << std::endl << std::endl;

	t0 = tick_count::now();
	parallel_sort(vector_copy.begin(), vector_copy.end());
	t1 = tick_count::now();

	std::cout << "parallel_sort: czas wykonania algorytmu: " << (t1 - t0).seconds() << std::endl << std::endl;

	return 0;
}

/*---OUTPUT---
Vector maxsize: 4611686018427387903, capacity 0

Nieposortowany vector:
0 0 1 0 0 4 2 4 1 0 1 4 9 2 10 4 15 10 5 0 16 12 8 4 0 22 19 16 13 10 7 4 1 32 30 28 26 24 22 20 18 16 14 12 10 8 6 4 2
Sprawdzenie poprawności algorytmu:
0 0 0 0 0 0 0 1 1 1 1 2 2 2 4 4 4 4 4 4 4 5 6 7 8 8 9 10 10 10 10 12 12 13 14 15 16 16 16 18 19 20 22 22 24 26 28 30 32

Vector size:1000000, capacity 1048576

quick_sort: czas wykonania algorytmu: 2.40162

Wypełniamy vector na nowo...
i kopiujemy go.
Skopiowany vector: size 1000000, capacity 1048576

parallel_sort: czas wykonania algorytmu: 1.68729

*/