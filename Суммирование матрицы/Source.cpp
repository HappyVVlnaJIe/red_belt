#include "test_runner.h"
#include <vector>
#include <future>
using namespace std;
#include <algorithm>
#include <cstdint>
#include <vector>
#include <future>
using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
        : first(begin)
        , last(end)
        , size_(distance(first, last))
    {
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

    size_t size() const {
        return size_;
    }

private:
    Iterator first, last;
    size_t size_;
};

template <typename Iterator>
class Paginator {
private:
    vector<IteratorRange<Iterator>> pages;

public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (size_t left = distance(begin, end); left > 0; ) {
            size_t current_page_size = min(page_size, left);
            Iterator current_page_end = next(begin, current_page_size);
            pages.push_back({ begin, current_page_end });

            left -= current_page_size;
            begin = current_page_end;
        }
    }

    auto begin() const {
        return pages.begin();
    }

    auto end() const {
        return pages.end();
    }

    size_t size() const {
        return pages.size();
    }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}
template <typename Container>
int64_t VectorSum(const Container& v) {
    int64_t result = 0;
    for (const auto& row : v) {
        for (const auto& item : row) {
            result += item;
        }
    }
    return result;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    // Реализуйте эту функцию
    int64_t result = 0;
    vector<future<int64_t>> line_sum;
    
    for (auto page : Paginate(matrix, 2000)) {
        line_sum.push_back(async([page] {return VectorSum(page);}));
    }
    
    for (auto& f : line_sum) {
        result += f.get();
    }
    return result;
    
}

void TestCalculateMatrixSum() {
    {
        const vector<vector<int>> matrix = {
          {1, 2, 3, 4},
          {5, 6, 7, 8},
          {9, 10, 11, 12},
          {13, 14, 15, 16}
        };
        ASSERT_EQUAL(CalculateMatrixSum(matrix), 136); 
    }
    /*{
        const vector<vector<int>> matrix = {
          {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 0, 0}
        };
        ASSERT_EQUAL(CalculateMatrixSum(matrix), 34359738352);
    }
    {
        for (int i = 0; i < 10;i++) {
            int size = rand()%9000;
            const vector<vector<int>> matrix = GenerateMatrix(size);
            ASSERT_EQUAL(CalculateMatrixSum(matrix), static_cast<int64_t>(0) * size * size);
        }
    }*/
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}