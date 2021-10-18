#include <vector>
#include <stdexcept>
#include <iostream>
#include "test_runner.h"

using namespace std;

template <typename T>
class Deque {
public:
	Deque(size_t num = 0) :front(), back() {
		if (num != 0) Resize(num);
	}
	void Resize(size_t num) {
		front.resize(0);
		back.resize(num);
	}
	bool Empty() const;
	size_t Size() const;
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	T& At(size_t index);
	const T& At(size_t index) const;
	void PushFront(T elem);
	void PushBack(T elem);
	T& Front();
	const T& Front() const;
	T& Back();
	const T& Back() const;
private:
	std::vector<T> front, back;
};

template<typename T>
bool Deque<T>::Empty() const
{
	return front.empty() && back.empty();
}

template<typename T>
size_t Deque<T>::Size() const
{
	return front.size()+back.size();
}

template<typename T>
T& Deque<T>::operator[](size_t index)
{
	size_t value = front.size();
	return (index < value) ? front[value - 1 - index] : back[index - value];
}

template<typename T>
const T& Deque<T>::operator[](size_t index) const
{
	size_t value = front.size();
	return (index < value) ? front[value - 1 - index] : back[index - value];
}

template<typename T>
T& Deque<T>::At(size_t index)
{
	if (index > this->Size()) {
		throw std::out_of_range("out_of_range");
	}
	return this->operator[](index);
}

template<typename T>
const T& Deque<T>::At(size_t index) const
{
	if (index > this->Size()	) {
		throw std::out_of_range("out_of_range");
	}
	return this->operator[](index);
}

template<typename T>
void Deque<T>::PushFront(T elem)
{
	front.push_back(elem);
}

template<typename T>
void Deque<T>::PushBack(T elem)
{
	back.push_back(elem);
}

template<typename T>
T& Deque<T>::Front()
{
	return front.empty() ? back.front() : front.back();
}

template<typename T>
const T& Deque<T>::Front() const
{
	return front.empty() ? back.front() : front.back();
}

template<typename T>
T& Deque<T>::Back()
{
	return back.empty() ? front.front() : back.back();
	
}

template<typename T>
const T& Deque<T>::Back() const
{
	return back.empty() ? front.front() : back.back();
}

void TestFrontAfterPushBack() {
	Deque<int> dq;
	dq.PushBack(1);
	dq.PushBack(2);
	dq.PushBack(3);
	ASSERT_EQUAL(dq.Front(), 1);
	ASSERT_EQUAL(dq.Back(), 3);
	ASSERT_EQUAL(dq.Size(), 3);
}

void TestSize() {
	Deque<int> dq;
	dq.PushBack(11);
	dq.PushFront(21);
	dq.PushBack(12);
	dq.PushFront(22);
	dq.PushBack(13);
	ASSERT_EQUAL(dq.Size(), 5);
}

void TestTable() {
	try {
		Deque <int> t(3);
		ASSERT_EQUAL(t.Empty(), false);
		ASSERT_EQUAL(t.Size(), 3);
		t[0] = 1;
		ASSERT_EQUAL(t[0], 1);
		t[2] = 3;
		ASSERT_EQUAL(t[2], 3);
		t.At(2) = 41;
		ASSERT_EQUAL(t.At(2), 41);
		ASSERT_EQUAL(t.Front(), 1u);
		ASSERT_EQUAL(t.Back(), 41);
		t.PushFront(-2);
		ASSERT_EQUAL(t.Front(), -2);
		ASSERT_EQUAL(t.Size(), 4);
		t.PushBack(42);
		ASSERT_EQUAL(t.Back(), 42);
		ASSERT_EQUAL(t.Size(), 5);
		t.PushFront(-1);
		ASSERT_EQUAL(t.Front(), -1);
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestFrontAfterPushBack);
	RUN_TEST(tr, TestSize);
	RUN_TEST(tr, TestTable);
	return 0;
}