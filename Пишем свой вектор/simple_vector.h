#pragma once

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
	SimpleVector()=default;
	explicit SimpleVector(size_t size);
	~SimpleVector();

	T& operator[](size_t index);

	T* begin();
	T* end();

	size_t Size() const;
	size_t Capacity() const;
	void PushBack(const T& value);

private:
	// Добавьте поля для хранения данных вектора
	size_t capacity{};
	size_t size{};
	T* data{};
};

template<typename T>
SimpleVector<T>::SimpleVector(size_t size) :size(size), capacity(size), data(new T[size]{}) {}

template<typename T>
SimpleVector<T>::~SimpleVector()
{
	delete[] data;
}

template<typename T>
T& SimpleVector<T>::operator[](size_t index)
{
	return data[index];
}

template<typename T>
T* SimpleVector<T>::begin()
{
	return data;
}

template<typename T>
T* SimpleVector<T>::end()
{
	return data+size;
}

template<typename T>
size_t SimpleVector<T>::Size() const
{
	return size;
}

template<typename T>
size_t SimpleVector<T>::Capacity() const
{
	return capacity;
}

template<typename T>
void SimpleVector<T>::PushBack(const T& value)
{
	if (Capacity() == 0) {
		T* new_data = new T[++capacity]{ value };
		++size;
		data = new_data;
	}
	else {
		if (Size() == Capacity()) {
			T* new_data = new T[2 * Capacity()]{};
			capacity *= 2;
			for (size_t i = 0; i <= Size(); i++) {
				new_data[i] = data[i];
			}
			delete[] data;
			data = new_data;
		}
		data[size++] = value;
	}
}
