#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <deque>
#include <stdexcept>
#include <set>
#include "test_runner.h"
using namespace std;

template <class T>
class ObjectPool {
public:
	T* Allocate();
	T* TryAllocate();

	void Deallocate(T* object);

	~ObjectPool();

private:
	// Добавьте сюда поля
	std::set<T*> allocated;
	std::deque<T*> liberated;
};

template<class T>
T* ObjectPool<T>::Allocate()
{
	if (!liberated.empty()) {
		T* ptr = liberated.front();
		allocated.insert(ptr);
		liberated.pop_front();
		return ptr;
	}
	T* result = new T;
	allocated.insert(result);
	return result;
}

template<class T>
T* ObjectPool<T>::TryAllocate()
{
	if (!liberated.empty()) {
		T* ptr = liberated.front();
		allocated.insert(ptr);
		liberated.pop_front();
		return ptr;
	}
	return nullptr;
}

template<class T>
void ObjectPool<T>::Deallocate(T* object)
{
	auto ptr = allocated.find(object);
	if (ptr == allocated.end()) {
		throw invalid_argument("");
	}
	liberated.push_back(object);
	allocated.erase(ptr);

}

template<class T>
ObjectPool<T>::~ObjectPool()
{
	for (auto& elem : liberated) {
		delete elem;
	}
	for (auto& elem : allocated) {
		delete elem;
	}
}


void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestObjectPool);
	return 0;
}
