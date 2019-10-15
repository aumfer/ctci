#include <string>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <list>

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

bool oneaway(const std::string &a, const std::string &b, int start = 0, int num_edits = 0) {
	if (num_edits > 1) {
		return false;
	}
	int len_dif = a.length() - b.length();
	if (std::abs(len_dif) > 1) {
		return false;
	}
	for (int i = start; i < a.length(); ++i) {
		// todo handle string lengths: we could be accessing b out-of-bounds here, or ignoring the remaining characters in b
		if (a[i] != b[i]) {
			const std::string add_b = b.substr(0, i - 1) + a[i] + b.substr(i, b.length() - i);
			const std::string rem_b = b.substr(0, i - 1) + b.substr(i + 1, b.length() - i + 1);
			const std::string alt_b = b.substr(0, i - 1) + a[i] + b.substr(i + 1, b.length() - i + 1);
			bool with_add = oneaway(a, add_b, i, num_edits + 1);
			bool with_rem = oneaway(a, rem_b, i, num_edits + 1);
			bool with_alt = oneaway(a, alt_b, i, num_edits + 1);
			return with_add || with_rem || with_alt;
		}
	}
	return true;
}

char *compress(char *out, int *out_len, char *in, int len) {
	char prev = 0;
	int count = 0;
	char *o = out;
	for (char *c = in; c < in + len; ++c) {
		if (*c != prev) {
			if (count) {
				*o++ = char(count); // todo int-to-ascii
				*o++ = prev;
				if (o - out >= len - 1) {
					return in;
				}
			}
			prev = *c;
			count = 0;
		} else {
			++count;
		}
	}
	*out_len = o - out;
	*o = '\0';
	return out;
}

std::list<int> removedupes(const std::list<int> &list) {
	std::unordered_set<int> set = std::unordered_set<int>(list.begin(), list.end());
	std::list<int> deduped = std::list<int>(set.begin(), set.end());
	return deduped;
}

struct slist {
	int val;
	slist *next;
};

int ktolast(const slist &list, int k) {
	//int buf[k];
	int *buf = (int *)alloca(k * sizeof(int));
	unsigned i = 0;
	for (const slist *n = &list; n; n = n->next) {
		buf[i++%k] = n->val;
	}
	return buf[i%k];
}

void delmid(slist &n) {
	n.val = n.next->val;
	n.next = n.next->next;
}

void partition(slist &head, int val) {
	for (slist *n = &head; n->next; n = n->next) {
		slist *m = n;
		while (n->val >= val && m) {
			std::swap(n->val, m->next->val);
			m = m->next;
		}
	}
}

slist *sumlists(slist &alist, slist &blist) {
	slist *head = nullptr;
	slist *prev = nullptr;

	slist *a, *b;
	int carry = 0;
	for (a = &alist, b = &blist; a || b; a = a ? a->next : nullptr, b = b ? b->next : nullptr) {
		int av = a ? a->val : 0;
		int bv = b ? b->val : 0;
		int sum = av + bv;
		if (carry) {
			sum += carry;
		}
		if (sum >= 10) {
			carry = 1;
			sum -= 10;
		} else {
			carry = 0;
		}

		slist *next = new slist();
		next->val = sum;
		if (!head) {
			head = next;
		}
		if (prev) {
			prev->next = next;
		}
		prev = next;
	}

	return head;
}

int main(int argc, char **argv) {

	return 0;
}