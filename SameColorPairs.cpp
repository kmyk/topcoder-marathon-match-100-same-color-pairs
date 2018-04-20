#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0; (i) < int(n); ++ (i))
#define REP3(i, m, n) for (int i = (m); (i) < int(n); ++ (i))
#define REP_R(i, n) for (int i = int(n) - 1; (i) >= 0; -- (i))
#define REP3R(i, m, n) for (int i = int(n) - 1; (i) >= int(m); -- (i))
#define ALL(x) begin(x), end(x)
using ll = long long;
using namespace std;
template <class T> using reversed_priority_queue = priority_queue<T, vector<T>, greater<T> >;
template <class T> inline void chmax(T & a, T const & b) { a = max(a, b); }
template <class T> inline void chmin(T & a, T const & b) { a = min(a, b); }
const int dy[] = { -1, 1, 0, 0 };
const int dx[] = { 0, 0, 1, -1 };

class xor_shift_128 {
public:
    typedef uint32_t result_type;
    xor_shift_128(uint32_t seed) {
        set_seed(seed);
    }
    void set_seed(uint32_t seed) {
        a = seed = 1812433253u * (seed ^ (seed >> 30));
        b = seed = 1812433253u * (seed ^ (seed >> 30)) + 1;
        c = seed = 1812433253u * (seed ^ (seed >> 30)) + 2;
        d = seed = 1812433253u * (seed ^ (seed >> 30)) + 3;
    }
    uint32_t operator() () {
        uint32_t t = (a ^ (a << 11));
        a = b; b = c; c = d;
        return d = (d ^ (d >> 19)) ^ (t ^ (t >> 8));
    }
    static constexpr uint32_t max() { return numeric_limits<result_type>::max(); }
    static constexpr uint32_t min() { return numeric_limits<result_type>::min(); }
private:
    uint32_t a, b, c, d;
};

constexpr double ticks_per_sec = 2800000000;
constexpr double ticks_per_sec_inv = 1.0 / ticks_per_sec;
inline double rdtsc() { // in seconds
    uint32_t lo, hi;
    asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo) * ticks_per_sec_inv;
}
constexpr int TLE = 10; // sec

inline array<int, 4> to_array(pair<int, int> const & p1, pair<int, int> const & p2) {
    return (array<int, 4>) { p1.first, p1.second, p2.first, p2.second };
}
inline array<int, 4> to_array(pair<pair<int, int>, pair<int, int> > const & p12) {
    return to_array(p12.first, p12.second);
}

template <class T>
vector<T> vector_concat(vector<T> const & a, vector<T> const & b) {
    vector<T> c;
    copy(ALL(a), back_inserter(c));
    copy(ALL(b), back_inserter(c));
    return c;
}

template <class T>
vector<T> const & vector_max_size(vector<T> const & a, vector<T> const & b) {
    return a.size() >= b.size() ? a : b;
}

bool is_valid_pair(vector<vector<int> > const & board, pair<int, int> const & p1, pair<int, int> const & p2, pair<int, int> & cont) {
    int y1, x1; tie(y1, x1) = p1;
    int y2, x2; tie(y2, x2) = p2;
    int c = board[y1][x1];
    if (y1 > y2) swap(y1, y2);
    if (x1 > x2) swap(x1, x2);
    int & y = cont.first;
    int & x = cont.second;
    if (y == -1) y = y1;
    if (x == -1) x = x1;
    for (; y < y2 + 1; ++ y) {
        for (; x < x2 + 1; ++ x) {
            if (board[y][x] != -1 and board[y][x] != c) {
                return false;
            }
        }
        x = x1;
    }
    return true;
}

inline bool is_valid_pair(vector<vector<int> > const & board, pair<int, int> const & p1, pair<int, int> const & p2) {
    pair<int, int> cont = { -1, -1 };
    return is_valid_pair(board, p1, p2, cont);
}

void use_pair(vector<vector<int> > & board, pair<int, int> const & p1, pair<int, int> const & p2) {
    int y1, x1; tie(y1, x1) = p1;
    int y2, x2; tie(y2, x2) = p2;
    board[y1][x1] = -1;
    board[y2][x2] = -1;
}
void use_pair(vector<vector<int> > & board, pair<int, int> const & p1, pair<int, int> const & p2, vector<array<int, 4> > & rects) {
    int y1, x1; tie(y1, x1) = p1;
    int y2, x2; tie(y2, x2) = p2;
    rects.push_back((array<int, 4>) { y1, x1, y2, x2 });
    board[y1][x1] = -1;
    board[y2][x2] = -1;
}

void apply_rects(vector<vector<int> > & board, vector<array<int, 4> > const & rects) {
    for (auto const & rect : rects) {
        int y1 = rect[0];
        int x1 = rect[1];
        int y2 = rect[2];
        int x2 = rect[3];
        board[y1][x1] = -1;
        board[y2][x2] = -1;
    }
}

void apply_unreliable_rects(vector<vector<int> > & board, vector<array<int, 4> > & rects) {
    int l = 0;
    REP (r, rects.size()) {
        int y1 = rects[r][0];
        int x1 = rects[r][1];
        int y2 = rects[r][2];
        int x2 = rects[r][3];
        // assume all conditions except is_valid_pair()
        if (not is_valid_pair(board, make_pair(y1, x1), make_pair(y2, x2))) continue;
        board[y1][x1] = -1;
        board[y2][x2] = -1;
        rects[l ++] = rects[r];
    }
    rects.resize(l);
}

vector<array<int, 4> > with_prefix(int H, int W, int C, vector<vector<int> > const & original_board, vector<array<int, 4> > const & prefix, function<vector<array<int, 4> > (int, int, int, vector<vector<int> > const &)> cont) {
    // make new board
    vector<vector<int> > board = original_board;
    apply_rects(board, prefix);

    // check empty rows and cols
    vector<bool> is_empty_row(H, true);
    vector<bool> is_empty_col(W, true);
    REP (y, H) REP (x, W) {
        if (board[y][x] != -1) {
            is_empty_row[y] = false;
            is_empty_col[x] = false;
        }
    }

    // construct maps
    vector<int> map_row;
    vector<int> map_col;
    REP (y, H) if (not is_empty_row[y]) map_row.push_back(y);
    REP (x, W) if (not is_empty_col[x]) map_col.push_back(x);

    // construct the compressed board
    vector<vector<int> > nboard(map_row.size(), vector<int>(map_col.size()));
    REP (y, map_row.size()) {
        REP (x, map_col.size()) {
            nboard[y][x] = board[map_row[y]][map_col[x]];
        }
    }

    // call the continuation
    vector<array<int, 4> > delta = cont(map_row.size(), map_col.size(), C, nboard);

    // make the return value
    vector<array<int, 4> > result = prefix;
    for (auto const & rect : delta) {
        int y1 = rect[0];
        int x1 = rect[1];
        int y2 = rect[2];
        int x2 = rect[3];
        y1 = map_row[y1];
        x1 = map_col[x1];
        y2 = map_row[y2];
        x2 = map_col[x2];
        result.push_back((array<int, 4>) { y1, x1, y2, x2 });
    }
    return result;
}

template <class RandomEngine>
vector<array<int, 4> > solve_greedy_once(int H, int W, int C, vector<vector<int> > board, RandomEngine & gen) {
    vector<vector<pair<int, int> > > color(C);
    REP (y, H) REP (x, W) {
        if (board[y][x] != -1) {
            color[board[y][x]].emplace_back(y, x);
        }
    }
    REP (c, C) {
        shuffle(ALL(color[c]), gen);
    }
    vector<array<int, 4> > rects;
    vector<int> i_of(C);
    while (true) {
        bool found = false;
        REP (c, C) {
            REP (count, color[c].size()) {
                if (i_of[c] >= int(color[c].size())) i_of[c] = 0;
                int i = i_of[c] ++;
                REP (j, color[c].size()) if (i < j) {
                    if (not is_valid_pair(board, color[c][i], color[c][j])) continue;
                    found = true;
                    use_pair(board, color[c][i], color[c][j], rects);
                    swap(color[c][j], color[c].back()); color[c].pop_back();  // drop j at first, since i < j
                    swap(color[c][i], color[c].back()); color[c].pop_back();
                    goto next_color;
                }
            }
next_color: ;
        }
        if (not found) break;
    }
    return rects;
}

template <class RandomEngine>
vector<array<int, 4> > solve_sa_greedy(int H, int W, int C, vector<vector<int> > const & original_board, double sec, RandomEngine & gen) {
    vector<array<int, 4> > result;
    vector<array<int, 4> > cur;

    int iteration = 0;
    double clock_begin = rdtsc();
    double last_t = rdtsc();
    double max_delta = 0;
    for (; ; ++ iteration) {
        double t = rdtsc();
        if (t + 2 * max_delta + 0.5 > clock_begin + sec) break;

        // prepare board and the prefix of nxt
        vector<array<int, 4> > nxt = cur;
        if (not nxt.empty()) {
            int i = uniform_int_distribution<int>(0, nxt.size() - 1)(gen);
            nxt.erase(nxt.begin() + i);
        }
        vector<vector<int> > board = original_board;
        apply_unreliable_rects(board, nxt);

        // run greedy
        vector<array<int, 4> > delta = solve_greedy_once(H, W, C, board, gen);
        copy(ALL(delta), back_inserter(nxt));

        // update cur and result
        int size_delta = int(nxt.size()) - int(cur.size());
        if (cur.size() <= nxt.size() or bernoulli_distribution((exp(size_delta) / (t - clock_begin) * sec))(gen)) {
            cur = nxt;
            if (result.size() < cur.size()) {
                result = nxt;
                cerr << "score = " << (2 * result.size()) /(double) (H * W) << endl;
                if (2 * int(result.size()) == H * W) {
                    ++ iteration;
                    break;  // the optimal
                }
            }
        }

        chmax(max_delta, t - last_t);
        last_t = t;
    }
    cerr << "iteration = " << iteration << endl;
    cerr << "time = " << (rdtsc() - clock_begin) << endl;
    cerr << "max delta = " << max_delta << endl;
    return result;
}

vector<array<int, 4> > solve(int H, int W, int C, vector<vector<int> > const & board) {
    random_device device;
    xor_shift_128 gen(device());
    return solve_sa_greedy(H, W, C, board, 9.0, gen);
}

class SameColorPairs {
public:
    vector<string> removePairs(vector<string> a_board) {
        double clock_begin = rdtsc();

        // parse input
        int H = a_board.size();
        int W = a_board[0].size();
        vector<vector<int> > board(H, vector<int>(W));
        int C = 0;
        REP (y, H) REP (x, W) {
            board[y][x] = a_board[y][x] - '0';
            chmax(C, board[y][x] + 1);
        }

        // assertion
        assert (10 <= H and H <= 100);
        assert (10 <= W and W <= 100);
        assert (2  <= C and C <= 6);

        // debug output
        cerr << "H = " << H << endl;
        cerr << "W = " << W << endl;
        cerr << "C = " << C << endl;
#ifdef LOCAL
        REP (y, H) {
            REP (x, W) {
                cerr << board[y][x];
            }
            cerr << endl;
        }
#endif

        // solve
        vector<array<int, 4> > result = solve(H, W, C, board);

        // make output
        vector<string> answer;
        for (auto rect : result) {
            ostringstream oss;
            oss << rect[0] << ' ';
            oss << rect[1] << ' ';
            oss << rect[2] << ' ';
            oss << rect[3];
            answer.push_back(oss.str());
        }
        double t = rdtsc() - clock_begin;
        cerr << "time = " << t << endl;
        return answer;
    }
};
