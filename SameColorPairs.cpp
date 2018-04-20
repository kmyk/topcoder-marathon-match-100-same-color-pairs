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

vector<array<int, 4> > solve_prepare(int H, int W, int C, vector<vector<int> > & board, int limit) {
    vector<vector<pair<int, int> > > color(C);
    REP (y, H) REP (x, W) {
        color[board[y][x]].emplace_back(y, x);
    }
    REP (c, C) {
        auto sq = [](int x) { return x * x; };
        sort(ALL(color[c]), [&](pair<int, int> const & p1, pair<int, int> const & p2) {
            int y1, x1; tie(y1, x1) = p1;
            int y2, x2; tie(y2, x2) = p2;
            int a1 = sq(2 * y1 - H) + sq(2 * x1 - W);
            int a2 = sq(2 * y2 - H) + sq(2 * x2 - W);
            return a1 < a2;
        });
    }
    vector<array<int, 4> > rects;
    while ((int)rects.size() < limit) {
        bool found = false;
        REP (c, C) {
            REP_R (i, color[c].size()) {  // use backward
                REP (j, i) {
                    if (not is_valid_pair(board, color[c][i], color[c][j])) continue;
                    use_pair(board, color[c][i], color[c][j], rects);
                    swap(color[c][i], color[c].back()); color[c].pop_back();  // drop i at first, since j < i
                    swap(color[c][j], color[c].back()); color[c].pop_back();
                    found = true;
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
vector<int> make_order(int H, int W, vector<vector<int> > & board, vector<pair<pair<int, int>, pair<int, int> > > const & pairs, RandomEngine & gen) {
    vector<int> result;
    map<pair<int, int>, vector<int> > depended;
    vector<pair<int, int> > cont(pairs.size(), make_pair(-1, -1));
    function<void (int)> go = [&](int i) {
        auto const & p1 = pairs[i].first;
        auto const & p2 = pairs[i].second;
        if (is_valid_pair(board, p1, p2, cont[i])) {
            use_pair(board, p1, p2);
            result.push_back(i);
            for (auto pi : { p1, p2 }) {
                auto it = depended.find(pi);
                if (it == depended.end()) continue;
                for (int j : it->second) {
                    go(j);
                }
                it->second = vector<int>();  // relase
            }
        } else {
            depended[cont[i]].push_back(i);
        }
    };
    REP (i, pairs.size()) {
        go(i);
    }
    return result;
}

template <class RandomEngine>
vector<array<int, 4> > solve_sa_body(int H, int W, int C, vector<vector<int> > const & original_board, double clock_end, RandomEngine & gen) {
    int already_removed = 0;
    REP (y, H) REP (x, W) {
        already_removed += original_board[y][x] == -1;
    }
    vector<vector<int> > board = original_board;
    vector<array<int, 4> > best_rects;
    vector<array<int, 4> > rects;

    int iteration = 0;
    for (; ; ++ iteration) {
        if (rdtsc() > clock_end) break;

        vector<pair<pair<int, int>, pair<int, int> > > pairs;
        vector<vector<pair<int, int> > > color(C);
        REP (y, H) REP (x, W) if (board[y][x] != -1) {
            color[board[y][x]].emplace_back(y, x);
        }
        REP (c, C) {
            shuffle(ALL(color[c]), gen);
        }
        while (true) {
            bool found = false;
            REP (c, C) {
                int k = color[c].size();
                if (k == 0) continue;
                found = true;
                pairs.emplace_back(color[c][k - 1], color[c][k - 2]);
                color[c].pop_back();
                color[c].pop_back();
            }
            if (not found) break;
        }
        vector<int> result = make_order(H, W, board, pairs, gen);
        for (int i : result) {
            rects.push_back(to_array(pairs[i]));
        }

        if (rects.size() > best_rects.size()) {
            best_rects = rects;
            if (already_removed + 2 * int(best_rects.size()) == H * W) return best_rects;
            cerr << "score = " << (already_removed + 2 * best_rects.size()) /(double) (H * W) << endl;
        }

        if (result.empty()) {
            double r = uniform_real_distribution<double>()(gen);
            int k = uniform_int_distribution<int>(0, r < 0.00001 ? 1000 : r < 0.001 ? 100 : 2)(gen);
            while (k -- and not rects.empty()) {
                int y1 = rects.back()[0];
                int x1 = rects.back()[1];
                int y2 = rects.back()[2];
                int x2 = rects.back()[3];
                rects.pop_back();
                board[y1][x1] = original_board[y1][x1];
                board[y2][x2] = original_board[y2][x2];
            }
        }
    }

    cerr << "iteration = " << iteration << endl;
    return best_rects;
}

template <class RandomEngine>
vector<array<int, 4> > solve_sa(int H, int W, int C, vector<vector<int> > board, double sec, RandomEngine & gen) {
    double clock_begin = rdtsc();
    vector<array<int, 4> > base = solve_prepare(H, W, C, board, (H * W - 1000) / 2);
    cerr << "prepare score = " << (base.size() /(double) (H * W / 2)) << endl;
#ifdef LOCAL
    REP (y, H) {
        REP (x, W) {
            if (board[y][x] == -1) {
                cerr << '.';
            } else {
                cerr << board[y][x];
            }
        }
        cerr << endl;
    }
#endif
    vector<array<int, 4> > result = solve_sa_body(H, W, C, board, clock_begin + sec, gen);
    return vector_concat(base, result);
}

template <class RandomEngine>
vector<array<int, 4> > solve_greedy_body(int H, int W, int C, vector<vector<int> > board, RandomEngine & gen) {
    vector<vector<pair<int, int> > > color(C);
    REP (y, H) REP (x, W) {
        color[board[y][x]].emplace_back(y, x);
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
vector<array<int, 4> > solve_greedy(int H, int W, int C, vector<vector<int> > const & board, double sec, RandomEngine & gen) {
    vector<array<int, 4> > result;
    int iteration = 0;
    double clock_begin = rdtsc();
    double last_t = rdtsc();
    double max_delta = 0;
    for (; ; ++ iteration) {
        double t = rdtsc();
        if (t + 2 * max_delta + 0.5 > clock_begin + sec) break;
        vector<array<int, 4> > it = solve_greedy_body(H, W, C, board, gen);
        if (result.size() < it.size()) {
            result = it;
            cerr << "score = " << (2 * result.size()) /(double) (H * W) << endl;
            if (2 * int(result.size()) == H * W) {
                ++ iteration;
                break;  // the optimal
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
    double t1, t2;  // sec
    if (C == 6) {
        t1 = 0.0;
        t2 = 9.0;
    } else if (C == 5) {
        t1 = 6.0;
        t2 = 3.0;
    } else {
        t1 = 8.0;
        t2 = 1.0;
    }
    auto result1 = (C != 6 ? solve_sa(H, W, C, board, t1, gen) : vector<array<int, 4> >());
    if (2 * int(result1.size()) == H * W) return result1;
    auto result2 = solve_greedy(H, W, C, board, t2, gen);
    return vector_max_size(result1, result2);
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
