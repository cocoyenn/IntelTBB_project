/*zadanie 2
Przedmiotem zadania jest implementacja:
1. klasy szablonowej Vector opakowywującej tbb:concurrent_vector oraz rozszerzająca go o metodę print().
2. algorytmu quick_sort z wykorzystanem zarządzanai task'ami implementowanymi przez klasę task_group.
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