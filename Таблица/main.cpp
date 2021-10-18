#include "test_runner.h"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

template <typename T> 
class Table {
public:
	Table(size_t numb_rows, size_t numb_columns);
	void Resize(size_t numb_rows, size_t numb_columns);
	pair<size_t, size_t> Size() const;
	vector<T>& operator[] (const int index);
	const vector<T>& operator[] (const int index) const;
private:
	vector<vector<T>> data;
};

template<typename T>
Table<T>::Table(size_t numb_rows, size_t numb_columns) {
	Resize(numb_rows, numb_columns);
}

template<typename T>
void Table<T>::Resize(size_t rows, size_t columns)
{
	data.resize(rows);
	for (auto& item : data) {
		item.resize(columns);
	}
}

template<typename T>
pair<size_t, size_t> Table<T>::Size() const{	
	if (!data.empty() && !data[0].empty()) {
		return { data.size(), data[0].size() };
	}

	return  { 0, 0 };
}

template<typename T>
vector<T>& Table<T>::operator[](const int index)
{
	return data[index];
}

template<typename T>
const vector<T>& Table<T>::operator[](const int index) const
{
	return data[index];
}

void TestTable() {
	Table<int> t(1, 1);
	ASSERT_EQUAL(t.Size().first, 1u);
	ASSERT_EQUAL(t.Size().second, 1u);
	t[0][0] = 42;
	ASSERT_EQUAL(t[0][0], 42);
	t.Resize(3, 4);
	ASSERT_EQUAL(t.Size().first, 3u);
	ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestTable);
	return 0;
}

