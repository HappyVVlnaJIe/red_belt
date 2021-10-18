#pragma once
#include <array>
#include <stdexcept>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
	explicit StackVector(size_t a_size = 0);

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	auto begin();
	auto end();
	auto begin() const;
	auto end() const;

	size_t Size() const;
	size_t Capacity() const;

	void PushBack(const T& value);
	T PopBack();

private:
	array<T, N> data;
	size_t size = 0;
};

template<typename T, size_t N>
inline StackVector<T, N>::StackVector(size_t a_size)
{
	if (a_size > N) {
		throw invalid_argument("error");
	}
	size = a_size;
}

template<typename T, size_t N>
inline T& StackVector<T, N>::operator[](size_t index)
{
	return data[index];
	// TODO: вставьте здесь оператор return
}

template<typename T, size_t N>
inline const T& StackVector<T, N>::operator[](size_t index) const
{
	return data[index];
	// TODO: вставьте здесь оператор return
}

template<typename T, size_t N>
inline auto StackVector<T, N>::begin()
{
	return data.begin();
}

template<typename T, size_t N>
inline auto StackVector<T, N>::end()
{
	return data.begin() + size;
}

template<typename T, size_t N>
inline auto StackVector<T, N>::begin() const
{
	return data.begin();
}

template<typename T, size_t N>
inline auto StackVector<T, N>::end() const
{
	return data.begin()+size;
}

template<typename T, size_t N>
inline size_t StackVector<T, N>::Size() const
{
	return size;
}

template<typename T, size_t N>
inline size_t StackVector<T, N>::Capacity() const
{
	return N;
}

template<typename T, size_t N>
inline void StackVector<T, N>::PushBack(const T& value)
{
	if (size >= N) {
		throw overflow_error("Full");
	}
	else {
		data[size++] = value;
	}
}

template<typename T, size_t N>
inline T StackVector<T, N>::PopBack()
{
	if (size == 0) {
		throw underflow_error("Empty");
	}
	else {
		--size;
		return data[size];
	}
}
