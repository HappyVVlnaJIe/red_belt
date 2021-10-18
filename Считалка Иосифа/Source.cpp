#include "test_runner.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <iostream>
#include <list>

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    list<typename RandomIt::value_type> pool;
    move(first, last, back_inserter(pool));
    auto pool_it = pool.begin();
    while (!pool.empty()) {
        auto value = next(pool_it);
        if (next(pool_it) == pool.end()) {
            value = pool.begin();
        }
        *(first++) = move(*pool_it);
        pool.erase(pool_it);
        pool_it = value;
        if (pool.empty()) {
            break;
        }
        for (size_t i = 0; i < ((step_size - 1) % pool.size()); ++i) {
            if (++pool_it == pool.end()) pool_it = pool.begin();
        }
    }
}

vector<int> MakeTestVector() {
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        ASSERT_EQUAL(numbers_copy, numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        ASSERT_EQUAL(numbers_copy, vector<int>({ 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 }));
    }
}

// ��� ����������� ���, ������� ������� ��� ���������, ��� ���� ����������
// ������� MakeJosephusPermutation �� ��������� ����������� ��������.
// ������ ��, ��������, �� ��������� ��� �� �������, ������ �� ���������,
// ������ �� ������� ������ ���, ����� � ����� ��� move-��������� �
// � ����� ������������� �����

struct NoncopyableInt {
    int value;

    NoncopyableInt(int value) : value(value) {}

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

void TestAvoidsCopying() {
    vector<NoncopyableInt> numbers;
    numbers.push_back({ 1 });
    numbers.push_back({ 2 });
    numbers.push_back({ 3 });
    numbers.push_back({ 4 });
    numbers.push_back({ 5 });

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    vector<NoncopyableInt> expected;
    expected.push_back({ 1 });
    expected.push_back({ 3 });
    expected.push_back({ 5 });
    expected.push_back({ 4 });
    expected.push_back({ 2 });

    ASSERT_EQUAL(numbers, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    RUN_TEST(tr, TestAvoidsCopying);
    return 0;
}