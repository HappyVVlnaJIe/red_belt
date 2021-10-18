#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <set>
#include "test_runner.h"
#include <random>

using namespace std;
//
//struct Request
//{
//    Request() = default;
//    Request(int64_t time,string name, int64_t client_id, int64_t room_count) :
//        time(time), hotel_name(name), client_id(client_id), room_count(room_count) {}
//	int64_t time;
//	std::string hotel_name;
//	int64_t client_id;
//	int64_t room_count;
//};
//
//struct Clients {
//    std::map<int64_t, int64_t> clients;
//    int unique_clients = 0;
//};
//
//class HRS {//Hotel reservation system
//public:
//	void BOOK(Request request);
//	int64_t CLIENTS(std::string hotel_name);
//	int64_t ROOMS(std::string hotel_name);
//private:
//	std::map<std::string, int64_t> rooms_in_hotels;
//    std::map<std::string, Clients> clients_in_hotels;
//	std::queue<Request> requests;
//    void ClearRequests();
//};
//
//void HRS::BOOK(Request request)
//{
//    requests.push(request);
//    std::string hotel_name = request.hotel_name;
//    if (rooms_in_hotels.find(hotel_name) == rooms_in_hotels.end()) {
//        rooms_in_hotels[hotel_name] = 0;
//    }
//    rooms_in_hotels[hotel_name] += request.room_count;
//    if (clients_in_hotels.find(hotel_name) == clients_in_hotels.end()) {
//        clients_in_hotels[hotel_name].clients[request.client_id] = 1;
//        clients_in_hotels[hotel_name].unique_clients++;
//    }
//    else {
//        if (clients_in_hotels[hotel_name].clients.find(request.client_id) == clients_in_hotels[hotel_name].clients.end()) {
//            clients_in_hotels[hotel_name].clients[request.client_id] = 1;
//            clients_in_hotels[hotel_name].unique_clients++;
//        }
//        else {
//            clients_in_hotels[hotel_name].clients[request.client_id]++;
//        }
//    }
//
//}
//
//int64_t HRS::CLIENTS(std::string hotel_name)
//{
//    ClearRequests();
//    if (clients_in_hotels.find(hotel_name) == clients_in_hotels.end()) {
//        return 0;
//    }
//    return clients_in_hotels[hotel_name].unique_clients;
//}
//
//int64_t HRS::ROOMS(std::string hotel_name)
//{
//    ClearRequests();
//    if (rooms_in_hotels.find(hotel_name) == rooms_in_hotels.end()) {
//        rooms_in_hotels[hotel_name] = 0;
//    }
//    return rooms_in_hotels[hotel_name];
//}
//
//void HRS::ClearRequests()
//{
//    if (requests.size() == 0) {
//        return;
//    }
//    int64_t cur_time = requests.back().time;
//    while (!requests.empty()&&requests.front().time<=cur_time-int64_t(86400)) {
//        auto curr = requests.front(); 
//        requests.pop();
//        rooms_in_hotels[curr.hotel_name] -= curr.room_count;
//        clients_in_hotels[curr.hotel_name].clients[curr.client_id]--;
//        if (clients_in_hotels[curr.hotel_name].clients[curr.client_id] == 0) {
//            clients_in_hotels[curr.hotel_name].unique_clients--;
//        }
//    }
//}
//
//


static const int DAY_SEC = 86400;

//сделать отдельно очередь запросов для каждого отеля

struct Request {
	int64_t time;
	int64_t client_id;
	int room_count;
};

class HotelInfo {
public:
	void Book(Request request);
	int64_t Rooms(int64_t curr_time);
	int64_t Clients(int64_t curr_time);
private:
	std::queue<Request> requests;
	int64_t rooms = 0;
	std::map<int64_t, int64_t> clients;
	void ClearRequests(int64_t curr_time);
};

void HotelInfo::Book(Request request) {
	requests.push(request);
	clients[request.client_id]++;
	rooms += request.room_count;
}

int64_t HotelInfo::Rooms(int64_t curr_time)
{
	ClearRequests(curr_time);
	return rooms;
}

int64_t HotelInfo::Clients(int64_t curr_time)
{
	ClearRequests(curr_time);
	return clients.size();
}

void HotelInfo::ClearRequests(int64_t curr_time)
{
	while (!requests.empty() && requests.front().time <= curr_time - DAY_SEC) {
		auto curr_request = requests.front();
		requests.pop();
		rooms -= curr_request.room_count;
		clients[curr_request.client_id]--;
		if (clients[curr_request.client_id] == 0) {
			clients.erase(curr_request.client_id);
		}
	}
}

class HRS {
public:
	void BOOK(std::string hotel_name, Request request);
	int64_t ROOMS(const std::string& hotel_name);
	int64_t CLIENTS(const std::string& hotel_name);
private:
	std::map<std::string, HotelInfo> hotels;
	bool FindHotel(std::string hotel_name);
	int64_t curr_time;
};

void HRS::BOOK(std::string hotel_name, Request request)
{
	curr_time = request.time;
	hotels[hotel_name].Book(request);
}

int64_t HRS::ROOMS(const std::string& hotel_name)
{
	if (!FindHotel(hotel_name)) {
		return 0;
	}
	return hotels.at(hotel_name).Rooms(curr_time);
}

int64_t HRS::CLIENTS(const std::string& hotel_name)
{
	if (!FindHotel(hotel_name)) {
		return 0;
	}
	return hotels.at(hotel_name).Clients(curr_time);
}

bool HRS::FindHotel(std::string hotel_name)
{
	return hotels.find(hotel_name) != hotels.end();
}

//void Test() {
//    HRS hotels;
//    ASSERT_EQUAL(hotels.CLIENTS("Marriott"), 0u);
//    ASSERT_EQUAL(hotels.ROOMS("Marriott"), 0u);
//
//    hotels.BOOK({ 10, "FourSeasons", 1, 2u });
//    ASSERT_EQUAL(hotels.CLIENTS("FourSeasons"), 1u);
//    ASSERT_EQUAL(hotels.ROOMS("FourSeasons"), 2u);
//
//    hotels.BOOK({ 10, "Marriott", 1, 1u });
//    hotels.BOOK({ 86409, "FourSeasons", 2, 1u });
//    ASSERT_EQUAL(hotels.CLIENTS("FourSeasons"), 2u);
//    ASSERT_EQUAL(hotels.ROOMS("FourSeasons"), 3u);
//    ASSERT_EQUAL(hotels.CLIENTS("Marriott"), 1u);
//    hotels.BOOK({ 86410, "Marriott", 2, 10u });
//    ASSERT_EQUAL(hotels.ROOMS("FourSeasons"), 1u);
//    ASSERT_EQUAL(hotels.ROOMS("Marriott"), 10u);
//}
//
//void Test1() {
//    HRS booker;
//   /* ASSERT_EQUAL(booker.CLIENTS("M"), 0);
//    ASSERT_EQUAL(booker.ROOMS("M"), 0);
//    booker.BOOK({ 10, "F", 1,2 });
//    booker.BOOK({ 10, "M", 1,1 });
//    ASSERT_EQUAL(booker.CLIENTS("F"), 1);
//    ASSERT_EQUAL(booker.ROOMS("F"), 2);
//    booker.BOOK({ 86409, "F", 2,1 });
//    ASSERT_EQUAL(booker.CLIENTS("F"), 2);
//    ASSERT_EQUAL(booker.ROOMS("F"), 3);
//    ASSERT_EQUAL(booker.CLIENTS("M"), 1);
//    booker.BOOK({ 86410, "M", 2,10 });
//    ASSERT_EQUAL(booker.ROOMS("F"), 1);
//    ASSERT_EQUAL(booker.ROOMS("M"), 10);*/
//    long tm = 86410;
//    tm *= 3;
//    booker.BOOK({ tm, "M", 2, 8 });
//    ASSERT_EQUAL(booker.ROOMS("M"), 8);
//
//    booker.BOOK({ tm, "M", 1, 8 });
//    cout << "OK" << endl;
//    ASSERT_EQUAL(booker.CLIENTS("M"), 2);
//    ASSERT_EQUAL(booker.ROOMS("M"), 16);
//
//    booker.BOOK({ 86410 * 2 + 1, "M", 1, 1 });
//    ASSERT_EQUAL(booker.ROOMS("M"), 17);
//    //---------------------------------
//    booker.BOOK({ 86410 * 3, "F", 2, 8 });
//    ASSERT_EQUAL(booker.ROOMS("F"), 8);
//
//    booker.BOOK({ 86410 * 3, "F", 1, 8 });
//    ASSERT_EQUAL(booker.ROOMS("F"), 16);
//
//    booker.BOOK({ 86410 * 2 + 1, "F", 1, 1 });
//    ASSERT_EQUAL(booker.ROOMS("F"), 17);
//}
//
//void Test2() {
//    HRS booker;
//    booker.BOOK({ 1, "a", 100, 1 });
//    booker.BOOK({ 1, "a", 10, 2 });
//    ASSERT_EQUAL(booker.ROOMS("a"), 3);
//    ASSERT_EQUAL(booker.CLIENTS("a"), 2);
//    booker.BOOK({ 1, "b", 100, 1 });
//    booker.BOOK({ 1, "b", 10, 2 });
//    ASSERT_EQUAL(booker.ROOMS("b"), 3);
//    ASSERT_EQUAL(booker.CLIENTS("b"), 2);
//    HRS booker2;
//    ASSERT_EQUAL(booker2.ROOMS("a"), 0);
//    ASSERT_EQUAL(booker2.CLIENTS("a"), 0);
//    ASSERT_EQUAL(booker2.ROOMS("b"), 0);
//    ASSERT_EQUAL(booker2.CLIENTS("b"), 0);
//    booker2.BOOK({ 10, "xxx", 777, 3 });
//    booker2.BOOK({ 11, "xxx", 777, 4 });
//    ASSERT_EQUAL(booker2.ROOMS("xxx"), 7);
//    ASSERT_EQUAL(booker2.CLIENTS("xxx"), 1);
//    HRS booker3;
//    for (int i = 0; i < 86400 * 10; i++) {
//        booker3.BOOK({ i, "m", i, 1 });
//    }
//    ASSERT_EQUAL(booker3.ROOMS("m"), 86400);
//    ASSERT_EQUAL(booker3.CLIENTS("m"), 86400);
//    HRS booker4;
//    for (int i = 0; i < 86400 * 10; i++) {
//        booker4.BOOK({i, "a", i, 1});
//        booker4.BOOK({ i, "a", i, 1 });
//    }
//    ASSERT_EQUAL(booker4.ROOMS("a"), 172800);
//    ASSERT_EQUAL(booker4.CLIENTS("a"), 86400);
//}
//
//void Test3() {
//    HRS manager;
//    // Test empty bookings
//    ASSERT_EQUAL(manager.CLIENTS("mariott"), 0);
//    ASSERT_EQUAL(manager.ROOMS("mariott"), 0);
//    manager.BOOK({ 0, "mariott", 1, 10 });
//    manager.BOOK({ 0, "mariott", 2, 1 });
//    manager.BOOK({ 0, "mariott", 3, 1 });
//    manager.BOOK({ 0, "mariott", 4, 1 });
//    manager.BOOK({ 0, "hilton", 1, 1 });
//    manager.BOOK({ 1, "hilton", 2, 1 });
//    // Test correctness
//    ASSERT_EQUAL(manager.CLIENTS("mariott"), 4);
//    ASSERT_EQUAL(manager.ROOMS("mariott"), 13);
//    ASSERT_EQUAL(manager.CLIENTS("hilton"), 2);
//    ASSERT_EQUAL(manager.ROOMS("hilton"), 2);
//    // Test event past 1 day resets statics
//    manager.BOOK({ 86400, "mariott", 1, 1 });
//    ASSERT_EQUAL(manager.CLIENTS("mariott"), 1);
//    ASSERT_EQUAL(manager.ROOMS("mariott"), 1);
//    ASSERT_EQUAL(manager.CLIENTS("hilton"), 1);
//    ASSERT_EQUAL(manager.ROOMS("hilton"), 1);
//    // Test no clients and room for the last day
//    manager.BOOK({ 86401, "mariott", 5, 1 });
//    ASSERT_EQUAL(manager.CLIENTS("mariott"), 2);
//    ASSERT_EQUAL(manager.ROOMS("mariott"), 2);
//    ASSERT_EQUAL(manager.CLIENTS("hilton"), 0);
//    ASSERT_EQUAL(manager.ROOMS("hilton"), 0);
//}
//
//void Test4() {
//    HRS booker;
//    for (int i = 0; i < 10000; i++)
//        booker.BOOK({ 1, "m", 777, i });
//    ASSERT_EQUAL(booker.ROOMS("m"), 49'995'000);
//    ASSERT_EQUAL(booker.CLIENTS("m"), 1);
//    for (int i = 0; i < 10000; i++)
//        booker.BOOK({ 86401, "m", i, 1 });
//    ASSERT_EQUAL(booker.ROOMS("m"), 10000);
//    ASSERT_EQUAL(booker.CLIENTS("m"), 10000);
//}
//
//void Test5() {
//    HRS booker;
//    int j = 1;
//    int i_max = 80000;
//    for (int i = 0; i < i_max; i++) {
//        booker.BOOK({ -1'000'000'000'000'000'000 + i, "a", i, j });
//        j++;
//    }
//    int64_t result = (i_max + 1)*1.0 / 2 * i_max;
//    ASSERT_EQUAL(booker.ROOMS("a"), result);
//    ASSERT_EQUAL(booker.CLIENTS("a"), i_max);
//}
//
////void TimeTest() {
////    ios::sync_with_stdio(false);
////    cin.tie(nullptr);
////    size_t Q = 100'000;
////    int64_t t1 = -1000'000'000'000'000'000;
////    int64_t t2 = 1000'000'000'000'000'000;
////    std::random_device rd;  //Will be used to obtain a seed for the random number engine
////    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
////    std::uniform_int_distribution<int64_t> random_time(t1, t2);
////    std::uniform_int_distribution<long> random_id(1, 1000'000'000);
////    std::uniform_int_distribution<long> random_room(1, 1000);
////    std::uniform_int_distribution<size_t> random_query_index(0, Q - 1);
////    vector<int64_t> v_time;
////    vector<string> v_hotel = { "a", "abcdye", "jhbdsfvkb", "dnlkenrkj", "a","b",
////                              "b", "kjnfvjnl", "jn;kelr", "a", "cccc", "xxxx", "csc", "xxxx" };
////    std::uniform_int_distribution<size_t> random_query_index_hotel(0, v_hotel.size() - 1);
////    vector<long> v_id;
////    vector<long> v_room;
////    vector<long> v_index_hotel;
////    vector<size_t> v_query_index;
////    for (size_t i = 0; i < Q; i++) {
////        v_time.push_back(random_time(gen));
////        v_id.push_back(random_id(gen));
////        v_room.push_back(random_room(gen));
////        v_index_hotel.push_back(random_query_index_hotel(gen));
////        v_query_index.push_back(random_query_index(gen));
////    }
////    sort(v_time.begin(), v_time.end());
////    for (auto& i : v_time)
////        cout << i << " ";
////    cout << endl;
////    //  cout <<" v_time.size()=" << v_time.size() << endl;
////    //  cout <<" v_id.size()=" << v_id.size() << endl;
////    //  cout <<" v_hotel.size()=" << v_hotel.size() << endl;
////    //  cout <<" v_room.size()=" << v_room.size() << endl;
////    //  cout <<" v_index_hotel.size()=" << v_index_hotel.size() << endl;
////    LOG_DURATION("Booker");
////    HRS booker;
////    for (size_t i = 0; i < double(2 * Q) / 3; ++i) {
////        booker.Book(v_time.at(v_query_index.at(i)), v_hotel.at(v_index_hotel.at(i)),
////            v_id.at(v_query_index.at(i)), v_room.at(v_query_index.at(i)));
////    }
////    for (size_t i = 0; i < double(Q) / 6; ++i) {
////        cout << booker.Rooms(v_hotel.at(v_index_hotel.at(i))) << " ";
////    }
////    cout << "\n";
////    for (size_t i = 0; i < double(Q) / 6; ++i) {
////        cout << booker.Clients(v_hotel.at(v_index_hotel.at(i))) << " ";
////    }
////    cout << "\n";
////}
//
//void Test6() {
//    HRS manager;
//    // Test empty bookings
//    ASSERT_EQUAL(manager.CLIENTS("mariott"), 0);
//    ASSERT_EQUAL(manager.ROOMS("mariott"), 0);
//    manager.BOOK({ 0, "mariott", 1, 10 });
//    manager.BOOK({ 0, "mariott", 2, 1 });
//    manager.BOOK({ 0, "mariott", 3, 1 });
//    manager.BOOK({ 0, "mariott", 4, 1 });
//    manager.BOOK({ 0, "hilton", 1, 1 });
//    manager.BOOK({ 1, "hilton", 2, 1 });
//    // Test correctness
//    ASSERT_EQUAL(manager.CLIENTS("mariott"), 4);
//    ASSERT_EQUAL(manager.ROOMS("mariott"), 13);
//    ASSERT_EQUAL(manager.CLIENTS("hilton"), 2);
//    ASSERT_EQUAL(manager.ROOMS("hilton"), 2);
//    // Test event past 1 day resets statics
//    manager.BOOK({86400, "mariott", 1, 1});
//    ASSERT_EQUAL(manager.CLIENTS("mariott"), 1);
//    ASSERT_EQUAL(manager.ROOMS("mariott"), 1);
//    ASSERT_EQUAL(manager.CLIENTS("hilton"), 1);
//    ASSERT_EQUAL(manager.ROOMS("hilton"), 1);
//    // Test no clients and room for the last day
//    manager.BOOK({ 86401, "mariott", 5, 1  });
//    ASSERT_EQUAL(manager.CLIENTS("mariott"), 2);
//    ASSERT_EQUAL(manager.ROOMS("mariott"), 2);
//    ASSERT_EQUAL(manager.CLIENTS("hilton"), 0);
//    ASSERT_EQUAL(manager.ROOMS("hilton"), 0);
//    manager.BOOK({ 999999999999999000, "hilton", 3, 5 });
//    manager.BOOK({ 999999999999999900, "hilton", 4, 3 });
//    manager.BOOK({ 999999999999999990, "mariot", 6, 1 });
//    manager.BOOK({ 999999999999999999, "mariot", 6, 2 });
//    ASSERT_EQUAL(manager.ROOMS("hilton"), 8);
//    ASSERT_EQUAL(manager.CLIENTS("hilton"), 2);
//    ASSERT_EQUAL(manager.ROOMS("mariot"), 3);
//    ASSERT_EQUAL(manager.CLIENTS("mariot"), 1);
//}

int main() {
    TestRunner tr;
    /*RUN_TEST(tr, Test);
    RUN_TEST(tr, Test1);
    RUN_TEST(tr, Test2);
    RUN_TEST(tr, Test3);
    RUN_TEST(tr, Test4);
    RUN_TEST(tr, Test5);
    RUN_TEST(tr, Test6);*/
    //RUN_TEST(tr, TimeTest);
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    HRS hrs;

    int query_count;
    std::cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;
        if (query_type == "BOOK") {
            Request request;
			std::string hotel_name;
            std::cin >> request.time >> hotel_name >> request.client_id >> request.room_count;
            hrs.BOOK(hotel_name, request);
        }
        else if (query_type == "CLIENTS") {
            std::string hotel_name;
            std::cin >> hotel_name;
            std::cout<<hrs.CLIENTS(hotel_name)<< "\n";
        }
        else if (query_type == "ROOMS") {
            std::string hotel_name;
            std::cin >> hotel_name;
            std::cout << hrs.ROOMS(hotel_name) << "\n";
        }
    }

    return 0;
}