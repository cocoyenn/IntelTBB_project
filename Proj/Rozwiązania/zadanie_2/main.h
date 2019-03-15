#pragma once
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include "tbb/task_group.h"
#include<iostream>
using namespace tbb;

template <typename T>
class Vector{
	
	typedef size_t size_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
	typedef typename concurrent_vector<value_type>::iterator iterator;

	public:
		Vector() = default;

		Vector(const Vector& vector){ m_vector = vector.m_vector;}

		size_type size() const { return m_vector.size();}

		size_type max_size() const {return m_vector.max_size();} 

		size_type capacity() const  {return m_vector.capacity();}

		bool empty() const {return  m_vector.empty();}

		auto push_back(T value){ return m_vector.push_back(value); }
		
		auto grow_by( std::initializer_list<value_type>& il ){return m_vector.grow_by(il); }

		//auto grow_by( std::initializer_list<value_type>&& il ){return m_vector.grow_by(il); }
		
		auto begin()  {return m_vector.begin();}

		auto cbegin() const {return m_vector.cbegin();}

        auto end()  {return m_vector.end();}

		auto cend() const {return m_vector.cend();}

		void clear() {m_vector.clear();}

		reference operator[](size_type idx){return m_vector[idx];}

		const_reference at(size_type idx) const {return m_vector.at(idx);}

		void print(){
			for(auto element: m_vector){
				std::cout << element << " ";
			}
			std::cout << std::endl;
		}
			
	private:
		concurrent_vector<value_type> m_vector;
};

template<typename T>
void quick_sort(Vector<T>& vec, int left, int right)
{
	if(right <= left) return;
    
	int i = left - 1, j = right + 1, pivot = vec.at((left+right)/2); 
	
	while(1)
	{
		while(pivot>vec[++i]);
		
		while(pivot<vec[--j]);
		
		if(i <= j )
            std::swap(vec[i],vec[j]);
        else
            break ;	
	}
    tbb::task_group group;
	if(j > left){
		 group.run([&]{quick_sort<T>(vec, left, j);});
	}   
	if(i < right){
		group.run([&]{quick_sort<T>(vec, i, right);});
	}
	    
    group.wait();
}
