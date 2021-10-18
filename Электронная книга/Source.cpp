#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>
#include "test_runner.h"

using namespace std;

class ReadingManager {
public:
    ReadingManager():page_rate(MAX_PAGE_COUNT_+1){}

    void Read(int user_id, int page_count) {
        if (user_page_counts_.find(user_id) != user_page_counts_.end()) {
            page_rate[user_page_counts_[user_id]].erase(user_id);
        }
        user_page_counts_[user_id] = page_count;
        page_rate[page_count].insert(user_id);
    }

    double Cheer(int user_id) const {
        if (user_page_counts_.find(user_id) == user_page_counts_.end()) {
            return 0;
        }
        int cur_users_count = user_page_counts_.size();
        if (cur_users_count == 1) {
            return 1;
        }
        int last_page = user_page_counts_.at(user_id);
        int users = 0;
        for (int i = 1; i < last_page; i++) {
            users += page_rate[i].size();
        }
        return users * 1.0 / (cur_users_count-1);
    }

private:
    static const int MAX_PAGE_COUNT_ = 1000;
    map<int,int> user_page_counts_;
    vector<set<int>> page_rate;
};

void TestonlyOneUser()
{
    {
        ReadingManager manager;
        manager.Read(4, 5);
        ASSERT_EQUAL(manager.Cheer(4), 1.0);
        manager.Read(4, 6);
        ASSERT_EQUAL(manager.Cheer(4), 1.0);
        manager.Read(4, 100);
        ASSERT_EQUAL(manager.Cheer(4), 1.0);
        ASSERT_EQUAL(manager.Cheer(3), 0.0);
        ASSERT_EQUAL(manager.Cheer(5), 0.0);
    }

}
void TestCheer()
{
    {
        ReadingManager manager;
        manager.Read(1, 1);
        ASSERT_EQUAL(manager.Cheer(1), 1.0);
        ASSERT_EQUAL(manager.Cheer(2), 0);
    }

}

//int main() {
//    TestRunner tr;
//    RUN_TEST(tr, TestonlyOneUser);
//    RUN_TEST(tr, TestCheer);
//    return 0;
//}

int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        }
        else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}