#pragma once

#ifndef ARRAY_H
#define ARRAY_H

namespace Razor {

	template <class T>
	class Array : private std::vector<T>
	{
	public:
		using std::vector<T>::clear;
		using std::vector<T>::begin;
		using std::vector<T>::end;

		Array();
		Array(std::initializer_list<T> args);
		~Array();

		void push(const T& value);
		void push(const Array<T>& values);
		void unshift(const T& value);
		void remove(const T& value);
		void removeAt(int index);
		void removeAll(const T& value);
		bool has(const T& value) const;
		int size() const;

		T& last();
		T& first();
		Array<T> reverse() const;
		T at(int index) const;
		T operator[](int index) const;
		T& operator[](int index);
		void operator+=(const T& value);
		void operator-=(const T& value);
	};

}

#include "Razor/Types/Array.cpp"

#endif