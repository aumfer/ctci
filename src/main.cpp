#include <string>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <queue>
#include <intrin.h>
#include <vector>
#include <cassert>

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

struct dlist {
	int value;
	dlist *next, *prev;
};

bool ispalin(const dlist &list) {
	const dlist *a, *b;
	for (a = &list, b = list.prev; a != b; a = a->next, b = b->prev) {
		if (a->value != b->value) {
			return false;
		}
	}
	return true;
}

bool isect(const slist &alist, const slist &blist) {
	for (const slist *a = &alist; a; a = a->next) {
		for (const slist *b = &blist; b; b = b->next) {
			if (a == b) { // by ref
				return true;
			}
		}
	}
	return false;
}

struct sgraph {
	std::vector<sgraph*> children;
};

bool hasroute(const sgraph &s, const sgraph &e) {
	if (&s == &e) {
		return true;
	}
	std::queue<const sgraph *> q = std::queue<const sgraph *>();
	std::unordered_set<const sgraph *> visited = std::unordered_set<const sgraph *>();
	q.push(&s);
	while (!q.empty()) {
		const sgraph *n = q.front();
		q.pop();
		for (const sgraph *c : n->children) {
			if (c == &e) {
				return true;
			}
			if (visited.find(c) == visited.end()) {
				q.push(c);
			}
		}
		visited.insert(n);
	}
	return false;
}

struct btree {
	btree *a, *b;
	int v;
};

btree *mintree(int *vals, int len) {
	btree *t = new btree();
	int mid = len / 2;
	int val = vals[mid];
	t->v = val;
	if (mid) {
		t->a = mintree(vals + mid / 2, mid);
		t->b = mintree(vals + mid * 2, mid);
	}
	return t;
}

struct slist_btree {
	const btree *val;
	slist_btree *next;
};

std::vector<slist_btree *> *depthlists(const btree &tree, int depth = 0, std::vector<slist_btree *> *lists = nullptr) {
	if (lists == nullptr) {
		lists = new std::vector<slist_btree *>();
	}
	slist_btree *next = new slist_btree();
	next->val = &tree;
	if (lists->size() > depth) {
		(*lists)[depth]->next = next;
	} else {
		(*lists)[depth] = next;
	}
	if (tree.a) {
		depthlists(*tree.a, depth + 1, lists);
	}
	if (tree.b) {
		depthlists(*tree.b, depth + 1, lists);
	}
	return lists;
}

bool isbalanced(const btree &tree, int depth = 0, int *mindepth = nullptr, int *maxdepth = nullptr) {
	int mina, maxa;
	if (tree.a) {
		isbalanced(*tree.a, depth+1, &mina, &maxa);
	} else {
		mina = depth;
		maxa = depth;
	}
	int minb, maxb;
	if (tree.b) {
		isbalanced(*tree.b, depth + 1, &minb, &maxb);
	} else {
		minb = depth;
		maxb = depth;
	}
	int mind = depth + std::min(mina, minb);
	int maxd = depth + std::max(minb, maxb);
	if (mindepth) {
		*mindepth = mind;
	}
	if (maxdepth) {
		*maxdepth = maxd;
	}
	return maxd - mind < 2;
}

bool isbst(const btree &tree, const btree *parent = nullptr) {
	bool res;
	if (parent) {
		if (&tree == parent->a) {
			if (parent->b) {
				res = tree.v <= parent->b->v;
			} else {
				res = true;
			}
		} else {
			if (parent->a) {
				res = tree.v >= parent->a->v;
			} else {
				res = true;
			}
		}
	} else {
		res = true;
	}
	if (tree.a) {
		res = res && isbst(*tree.a, &tree);
	}
	if (tree.b) {
		res = res && isbst(*tree.b, &tree);
	}
	return res;
}

template<typename R>
void iot(const btree &tree, const R &receiver) {
	if (tree.a) {
		iot(*tree.a, receiver);
	}
	bool cont = receiver(tree);
	if (!cont) {
		return;
	}
	if (tree.b) {
		iot(*tree.b, receiver);
	}
}

const btree *ionext(const btree &tree, const btree &node) {
	bool isnext = false;
	const btree *next = nullptr;
	iot(tree, [&](const btree &visit) {
		if (isnext) {
			next = &visit;
			return false;
		} else if (&visit == &node) {
			isnext = true;
		}
		return true;
	});
	return next;
}

void buildorder(int *projects, int num_projects, std::pair<int, int> *dependencies, int num_dependencies) {
	for (int i = 0; i < num_dependencies; ++i) {
		const std::pair<int, int> &d = dependencies[i];
		int firsti, secondi;
		for (int j = 0; j < num_projects; ++j) {
			int p = projects[j];
			if (p == d.second) {
				secondi = j;
			}
			if (p == d.first) {
				firsti = j;
			}
		}
		if (secondi < firsti) {
			int first = projects[firsti];
			for (int j = firsti; j > secondi; --j) {
				projects[j] = projects[j - 1];
			}
			projects[secondi] = first;
		}
	}
}

struct btreep {
	btreep *a, *b, *parent;
	int v;
};

const btreep *fca(const btreep &a, const btreep &b) {
	if (!a.parent || !b.parent) {
		return nullptr;
	}
	if (a.parent == b.parent) {
		return a.parent;
	}
	return fca(*a.parent, *b.parent);
}

int countpathsum(const btree &tree, int sum) {
	int count = tree.v == sum ? 1 : 0;
	if (tree.a) {
		count += countpathsum(*tree.a, sum - tree.v);
	}
	if (tree.b) {
		count += countpathsum(*tree.b, sum - tree.v);
	}
	return count;
}

uint32_t insertbits(uint32_t n, uint32_t m, unsigned start, unsigned end) {
	uint32_t mask = -1 >> (32 - (end - start));
	return n | ((m&mask) << start);
}

void printbinarydouble(double v) {
	uint64_t u = *(uint64_t *)&v;
	for (unsigned i = 0; i < 64; ++i) {
		printf("%d", u & (1 << i) ? 1 : 0);
	}
}

int flipwin(uint32_t input) {
	int max = 0;
	for (unsigned i = 0; i < 32; ++i) {
		for (unsigned j = 32 - i; j > i; --j) {
			uint32_t mask = (-1 >> i) << j;
			uint32_t test = input & mask;
			int count = __popcnt(test);
			if (count + 1 <= (j - i) && count > max) {
				max = count;
			}
		}
	}
	return max;
}

uint32_t nextsmallest1(uint32_t v) {
	for (unsigned i = 0; i < 32; ++i) {
		if ((v & (1 << i)) == 0) {
			for (unsigned j = i; j < 32; ++j) {
				if ((v & (1 << j)) != 0) {
					return (v | (1 << i)) & ~(1 << j);
				}
			}
		}
	}
	return -1;
}
uint32_t nextlargest1(uint32_t v) {
	for (unsigned i = 0; i < 32; ++i) {
		if ((v & (1 << i)) != 0) {
			for (unsigned j = i; j < 32; ++j) {
				if ((v & (1 << j)) == 0) {
					return v & ~(1 << i) | (1 << j);
				}
			}
		}
	}
	return 0;
}

int conversion(uint32_t a, uint32_t b) {
	int res = __popcnt(a ^ b);
	return res;
}

constexpr uint32_t EVEN_BITS() {
	uint32_t v = 0;
	for (unsigned i = 0; i < 32; i += 2) {
		v |= (1 << i);
	}
	return v;
}
constexpr uint32_t ODD_BITS() {
	uint32_t v = 0;
	for (unsigned i = 1; i < 32; i += 2) {
		v |= (1 << i);
	}
	return v;
}
uint32_t pswap(uint32_t v) {
	return ((v & EVEN_BITS()) << 1) | ((v & ODD_BITS()) >> 1);
}

void draw_line(uint8_t *screen, int screen_len, int width, int x1, int x2, int y) {
	int height = (screen_len * 8) / width;
	for (int x = std::min(x1, x2); x < std::max(x1, x2); ++x) {
		int i = x * height + y;
		screen[i / 8] |= (1 << i);
	}
}

int triplestep(int nsteps, int cursteps = 0) {
	if (cursteps > nsteps) return 0;
	if (cursteps == nsteps) return 1;
	int one = triplestep(nsteps, cursteps + 1);
	int two = triplestep(nsteps, cursteps + 2);
	int tre = triplestep(nsteps, cursteps + 3);
	return one + two + tre;
}

bool robotgrid(bool *invalid, bool *onpath, int r, int c, int x = 0, int y = 0) {
	if (invalid[x * r + y]) {
		return false;
	}
	if (x == c && y == r) {
		onpath[x * r + y] = true;
		return true;
	}
	if (x < c) {
		if (robotgrid(invalid, onpath, r, c, x + 1, y)) {
			onpath[x * r + y] = true;
			return true;
		}
	}
	if (y < r) {
		if (robotgrid(invalid, onpath, r, c, x, y + 1)) {
			onpath[x * r + y] = true;
			return true;
		}
	}
	return false;
}

int magicindex(int *sdi, int len, int offset = 0) {
	if (len == 0) {
		return -1;
	}
	int mid = len / 2;
	if (sdi[mid] == mid + offset) {
		return mid + offset;
	} else if (sdi[mid] < mid + offset) {
		return magicindex(sdi, mid, offset);
	} else {
		return magicindex(sdi + mid, mid, offset+mid);
	}
}

unsigned recurmult(unsigned a, unsigned b, unsigned sofar = 0) {
	if (b >= 2) {
		return recurmult(a, b - 2, sofar + (a << 1));
	} else if (b) {
		return sofar + a;
	} else {
		return sofar;
	}
}

std::vector<std::string> *permute(const std::string &str) {
	std::vector<std::string> *result = new std::vector<std::string>();
	if (str.length() == 0) {
		result->push_back(str);
	} else {
		char next = str[0];
		std::string rest = str.substr(1);
		std::vector<std::string> *perms = permute(rest);
		for (const std::string &s : *perms) {
			for (int i = 0; i <= s.length(); ++i) {
				std::string p = s;
				p.insert(i, 1, next);
				result->push_back(p);
			}
		}
		delete perms;
	}
	return result;
}

std::unordered_set<std::string> *permutenodupe(const std::string &str) {
	std::unordered_set<std::string> *result = new std::unordered_set<std::string>();
	if (str.length() == 0) {
		result->insert(str);
	}
	else {
		char next = str[0];
		std::string rest = str.substr(1);
		std::unordered_set<std::string> *perms = permutenodupe(rest);
		for (const std::string &s : *perms) {
			for (int i = 0; i <= s.length(); ++i) {
				std::string p = s;
				p.insert(i, 1, next);
				result->insert(p);
			}
		}
		delete perms;
	}
	return result;
}

void allparen(int n, int cur = 0, const std::string &s = std::string("")) {
	if (cur < n) {
		allparen(n, cur + 1, s + "(");
	}
	if (cur > 0) {
		allparen(n - 1, cur - 1, s + ")");
	}
	if (cur == 0) {
		puts(s.c_str());
	}
}

void paintfill(unsigned *screen, int w, int h, int x, int y, unsigned from, unsigned to) {
	unsigned *p = &screen[x * h + y];
	if (*p == from) {
		*p = to;
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				paintfill(screen, w, h, x + i, y + j, from, to);
			}
		}
	}
}

std::vector<std::vector<int>> *coins(int cents, int index = 0, std::vector<std::vector<int>> *result = nullptr) {
	if (result == nullptr) {
		result = new std::vector<std::vector<int>>();
		(*result).push_back(std::vector<int>());
	}
	if (cents == 0) {
		return result;
	}
	if (cents >= 1) {
		(*result)[index].push_back(1);
		coins(cents - 1, index, result);
	}
	if (cents >= 5) {
		(*result).push_back(std::vector<int>());
		(*result)[result->size()-1].push_back(5);
		coins(cents - 5, result->size() - 1, result);
	}
	if (cents >= 10) {
		(*result).push_back(std::vector<int>());
		(*result)[result->size() - 1].push_back(10);
		coins(cents - 10, result->size() - 1, result);
	}
	if (cents >= 25) {
		(*result).push_back(std::vector<int>());
		(*result)[result->size() - 1].push_back(25);
		coins(cents - 25, result->size() - 1, result);
	}
	return result;
}

struct box_t {
	int w, h, d;
};
int stackboxes(std::vector<box_t> &boxes) {
	std::sort(boxes.begin(), boxes.end(), [](const box_t &a, const box_t &b) {
		return (a.w + a.h + a.d) > (b.w + b.h + b.d);
	});
	int count = 0;
	const box_t *b;
	for (int i = 0; i < boxes.size(); ++i) {
		if (!count) {
			++count;
			b = &boxes[i];
		}
		if (boxes[i].w < b->w && boxes[i].h < b->h && boxes[i].d < b->d) {
			++count;
			b = &boxes[i];
		}
	}
	return count;
}

bool tictacwin(int *board, int n) {
	int i, j;

	// diag
	if (board[0]) {
		int player = board[0];
		for (i = 0; i < n; ++i) {
			if (board[i * n + i] != player) {
				break;
			}
		}
		if (i == n) {
			return true;
		}
	}

	// vert
	for (i = 0; i < n; ++i) {
		int player = board[i * n + 0];
		if (player) {
			for (j = 0; j < n; ++j) {
				if (board[i * n + j] != player) {
					break;
				}
			}
			if (j == n) {
				return true;
			}
		}
	}

	// hori
	for (i = 0; i < n; ++i) {
		int player = board[i];
		if (player) {
			for (j = 0; j < n; ++j) {
				if (board[j * n + i] != player) {
					break;
				}
			}
			if (j == n) {
				return true;
			}
		}
	}

	return false;
}

int factzeros(int n) {
	unsigned long long fact = 1;
	while (n > 0) {
		fact *= n--;
	}
	int count = 0;
	while (fact % 10 == 0) {
		fact /= 10;
		++count;
	}
	return count;
}

int smalldiff(int *a, int alen, int *b, int blen) {
	std::sort(a, a + alen);
	std::sort(b, b + blen);
	int dif = INT_MAX;
	int ai, bi;
	for (ai = 0, bi = 0; ai < alen && bi < blen;) {
		int d = a[ai] - b[bi];
		if (d < 0) {
			++ai;
			continue;
		}
		dif = std::min(dif, d);
		++bi;

	}
	return dif;
}

int nmax(int a, int b) {
	unsigned k = unsigned(b - a) >> 31;
	return a * k + b * ((~k)&1);
}

void englishint(int n) {
	static const char *names[] = { "zero",
		"one", "two", "three", "four", "five",
		"six", "seven", "eight", "nine", "ten"
	};
	int thou;
	int hund;
	int tens;
	int ones;

	if (n > 10000) {
		thou = n / 1000;
		englishint(thou);
		printf(" thousand");
		n -= thou * 1000;
	} else if (n >= 1000) {
		thou = n / 1000;
		printf("%s thousand", names[thou]);
		n -= thou * 1000;
	} else {
		thou = 0;
	}
	if (n >= 100) {
		hund = n / 100;
		printf("%s%s hundred", thou ? ", " : "", names[hund]);
		n -= hund * 100;
	} else {
		hund = 0;
	}
	if (n >= 10) {
		tens = n / 10;
		printf("%s%s", hund ? " " : "", names[tens]);
		n -= tens * 10;
	} else {
		tens = 0;
	}
	if (n) {
		ones = n;
		printf("%s%s", tens ? "-" : "", names[ones]);
		n -= ones * 1;
	}
	assert(!n);
}

struct person_t {
	int birth, death;
};
int livingppl(const person_t *people, int count) {
	std::unordered_map<int, int> births = std::unordered_map<int, int>();
	std::unordered_map<int, int> deaths = std::unordered_map<int, int>();
	for (int i = 0; i < count; ++i) {
		const person_t &p = people[i];
		int b = get_or_default(births, p.birth);
		births[p.birth] = b + 1;
		int d = get_or_default(deaths, p.death);
		deaths[p.death] = d + 1;
	}
	int alive = 0;
	for (int y = 1900; y < 2000; ++y) {
		alive += get_or_default(births, y);
		alive -= get_or_default(deaths, y);
	}
	return alive;
}

void divingboard(int k, int *count) {
	if (k == 0) {
		++*count;
		return;
	}
	divingboard(k - 1, count);
	divingboard(k - 1, count);
}

struct square_t {
	int x, y, w, h;
};
struct line_t {
	float x0, x1, y0, y1;
};
line_t bisectsq(const square_t &a, const square_t &b) {
	float midax = a.x + (a.w / 2.0f);
	float miday = a.y + (a.h / 2.0f);
	float midbx = b.x + (b.w / 2.0f);
	float midby = b.y + (b.h / 2.0f);
	line_t l = {
		midax, midbx,
		miday, midby
	};
	return l;
}

int main(int argc, char **argv) {
	int projects[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	int num_projects = _countof(projects);
	std::pair<int, int> dependencies[] = { { 'a', 'd' }, { 'f', 'b' }, { 'b', 'd' }, { 'f', 'a' }, { 'd', 'c' } };
	int num_dependencies = _countof(dependencies);
	buildorder(projects, num_projects, dependencies, num_dependencies);
	int expected[] = { 'f', 'e', 'a', 'b', 'd', 'c' };

	uint32_t n = insertbits(1024, 19, 2, 6);

	printbinarydouble(3.1415);

	int win = flipwin(1775);

	uint32_t s = nextsmallest1(11);
	uint32_t l = nextlargest1(11);

	unsigned m1 = recurmult(3, 3);
	unsigned m2 = recurmult(3, 4);
	unsigned m3 = recurmult(4, 3);

	auto perms = permute(std::string("world"));
	auto permsnodupe = permutenodupe(std::string("Hello"));

	allparen(3);

	auto coin = coins(6);

	int zeros = factzeros(19);

	int a[] = { 1,3,15,11,2 };
	int b[] = { 23,127,235,19,8 };
	int dif = smalldiff(a, 5, b, 5);

	int nm = nmax(10, 5);

	englishint(1337);
	puts("");
	englishint(119500);
	return 0;
}