#include "rzpch.h"

#ifndef _ARRAY_CPP_
#define _ARRAY_CPP_

namespace Razor {

	template<class T>
	Array<T>::Array() : std::vector<T>()
	{

	}

	template<class T>
	Array<T>::Array(std::initializer_list<T> args) : std::vector<T>(args)
	{

	}

	template<class T>
	Array<T>::~Array()
	{
	}

	template<class T>
	void Array<T>::push(const T& value)
	{
		std::vector<T>::push_back(value);
	}

	template<class T>
	void Array<T>::push(const Array<T>& values)
	{
		for (int i = 0; i < values.count(); i++)
			std::vector<T>::push_back(values[i]);
	}
		
	template<class T>
	void Array<T>::unshift(const T& value)
	{
		std::vector<T>::insert(std::vector<T>::begin(), value);
	}

	template<class T>
	void Array<T>::remove(const T& value)
	{
		std::vector<T>::erase(std::remove(begin(), end(), value), end());
	}

	template<class T>
	void Array<T>::removeAt(int index)
	{
		std::vector<T>::erase(begin() + index);
	}

	template<class T>
	void Array<T>::removeAll(const T& value)
	{
		for (int i = 0; i < size();)
		{
			if (at(i) == value)
				remove(i);
			else
				i++;
		}
	}

	template<class T>
	bool Array<T>::has(const T& value) const
	{
		for (int i = 0, c = size(); i < c; i++)
			if (at(i) == value)
				return true;

		return false;
	}

	template<class T>
	int Array<T>::size() const
	{
		return static_cast<int>(std::vector<T>::size());
	}

	template<class T>
	T& Array<T>::last()
	{
		return std::vector<T>::operator[](size() - 1);
	}

	template<class T>
	T& Array<T>::first()
	{
		return std::vector<T>::operator[](0);
	}

	template<class T>
	Array<T> Array<T>::reverse() const
	{
		Array<T> a;

		for (int i = size() - 1; i >= 0; i--)
			a.append(at(i));

		return a;
	}

	template<class T>
	T Array<T>::at(int index) const
	{
		return operator[](index);
	}

	template<class T>
	T Array<T>::operator[](int index) const
	{
		return std::vector<T>::operator[](index);
	}

	template<class T>
	T& Array<T>::operator[](int index)
	{
		return std::vector<T>::operator[](index);
	}

	template<class T>
	void Array<T>::operator+=(const T& value)
	{
		return push(value);
	}

	template<class T>
	void Array<T>::operator-=(const T& value)
	{
		return remove(value);
	}

}

#endif