#include <string>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>

template<typename TKey, typename TValue>
TValue get_or_default(const std::unordered_map<TKey, TValue> &m, const TKey &k) {
	auto mf = m.find(k);
	TValue default_v = { 0 };
	TValue v = mf == m.end() ? default_v : mf->second;
	return v;
}

bool alluniq(const std::string &s) {
	std::unordered_set<char> uniq = std::unordered_set<char>(s.begin(), s.end());
	return uniq.size() == s.length();
}

bool ispermute(std::string &a, std::string &b) {
	if (a.length() != b.length()) {
		return false;
	}
	std::sort(a.begin(), a.end());
	std::sort(b.begin(), b.end());
	int len = a.length();
	for (int i = 0; i < len; ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}
bool ispermute2(const std::string &a, const std::string &b) {
	std::unordered_map<char, int> counts = std::unordered_map<char, int>();
	if (a.length() != b.length()) {
		return false;
	}
	for (char c : a) {
		int count = get_or_default(counts, c);
		counts[c] = count + 1;
	}
	for (char c : b) {
		int count = get_or_default(counts, c);
		if (count <= 0) {
			return false;
		}
		counts[c] = count - 1;
	}
	for (auto kvp : counts) {
		if (kvp.second != 0) {
			return false;
		}
	}
	return true;
}

void urlify(char *s, int len) {
	char *c = s;
	for (int i = 0; i < len; ++i) {
		if (*c == ' ') {
			memcpy(c + 3, c, len - i);
			*c++ = '%';
			*c++ = '2';
			*c++ = '0';
		}
		++c;
	}
}

bool ispalinpermute(const std::string &s) {
	std::unordered_map<char, int> counts = std::unordered_map<char, int>();
	for (char c : s) {
		int count = get_or_default(counts, c);
		counts[c] = count + 1;
	}
	bool is_odd = s.length() & 1;
	bool has_odd = false;
	for (auto kvp : counts) {
		if (kvp.second & 1) {
			if (!is_odd || has_odd) {
				return false;
			} else {
				has_odd = true;
			}
		}
	}
	return true;
}

int main(int argc, char **argv) {

	return 0;
}