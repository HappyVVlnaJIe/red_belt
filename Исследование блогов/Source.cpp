#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <sstream>
#include <future>
using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other);
};

void Stats::operator+=(const Stats& other) {
    for (const auto& [word, numb] : other.word_frequences) {
        word_frequences[word] += numb;
    }
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
    stringstream stream;
    stream << line;
    string word;
    Stats result;
    while (stream >> word) {
        if (key_words.count(word) >0) {
            result.word_frequences[word] ++;
        }
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
    Stats result;
    for (string line; getline(input, line); ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

//Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
//    // Реализуйте эту функцию
//    vector<future<Stats>> futures;
//    Stats result;
//    for (string line; getline(input, line); ) {
//        futures.push_back(async(ExploreLine, ref(key_words), line));
//    }
//    for (auto& f : futures) {
//        result += f.get();
//    }
//    return result;
//}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    // Реализуйте эту функцию
    vector<future<Stats>> futures;
    Stats result;
    string words;
    for (string line; input >> line; ) {
        words += line + " ";
    }
    futures.push_back(async(ExploreLine, ref(key_words), words));
    for (auto& f : futures) {
        result += f.get();
    }
    return result;
}

void TestBasic() {
    const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
      {"yangle", 6},
      {"rocks", 2},
      {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}