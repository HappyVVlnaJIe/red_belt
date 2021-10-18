#include "stats.h"

HttpRequest ParseRequest(string_view line) {
	size_t first_letter = line.find_first_not_of(' ');
	size_t method_end = line.find(' ', first_letter);
	size_t URI_end = line.find(' ', method_end + 1);
	//size_t protocol_end = line.find(' ', URI_end + 1); //mb URI_end+1?
	return { line.substr(first_letter,method_end-first_letter),line.substr(method_end+1,URI_end-method_end-1),line.substr(URI_end + 1,line.size()-URI_end-1) };
}

void Stats::AddMethod(string_view method) {
	if (method_count.find(method) == method_count.end()) {
		method_count["UNKNOWN"]++;
	}
	else {
		method_count[method]++;
	}
}

void Stats::AddUri(string_view uri) {
	if (url_count.find(uri) == url_count.end()) {
		url_count["unknown"]++;
	}
	else {
		url_count[uri]++;
	}
}

const map<string_view, int>& Stats::GetMethodStats() const {
	return method_count;
}

const map<string_view, int>& Stats::GetUriStats() const {
	return url_count;
}