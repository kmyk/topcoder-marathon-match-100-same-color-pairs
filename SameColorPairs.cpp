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
double clock_begin;


class Solver {
    const int H;
    const int W;
    const int C;
    const vector<vector<int> > original_board;
public:
    Solver(int H, int W, int C, vector<vector<int> > const & board)
            : H(H)
            , W(W)
            , C(C)
            , original_board(board) {
    }

    vector<array<int, 4> > solve1() const {
        vector<vector<int> > board = original_board;
        random_device device;
        xor_shift_128 gen(device());
        vector<vector<pair<int, int> > > color(C);
        REP (y, H) REP (x, W) {
            color[board[y][x]].emplace_back(y, x);
        }
        REP (c, C) {
            shuffle(ALL(color[c]), gen);
        }
        vector<array<int, 4> > rects;
        while (true) {
            bool found = false;
            REP (c, C) {
                REP (ci, color[c].size()) {
                    REP (cj, ci) {
                        int y1, x1; tie(y1, x1) = color[c][ci];
                        int y2, x2; tie(y2, x2) = color[c][cj];
                        if (y1 > y2) swap(y1, y2);
                        if (x1 > x2) swap(x1, x2);
                        REP3 (y, y1, y2 + 1) {
                            REP3 (x, x1, x2 + 1) {
                                if (board[y][x] != -1 and board[y][x] != c) {
                                    goto next_pair;
                                }
                            }
                        }
                        // found
                        found = true;
                        tie(y1, x1) = color[c][ci];  // undo the swaps
                        tie(y2, x2) = color[c][cj];
                        rects.push_back((array<int, 4>) { y1, x1, y2, x2 });
                        // cerr << "(" << y1 << ", " << x1 << ") (" << y2 << ", " << x2 << ")" << endl;
                        board[y1][x1] = -1;
                        board[y2][x2] = -1;
                        swap(color[c][ci], color[c].back()); color[c].pop_back();  // remove from list
                        swap(color[c][cj], color[c].back()); color[c].pop_back();
                        goto next_color;
next_pair: ;
                    }
                }
next_color: ;
            }
            if (not found) break;
        }
        return rects;
    }

    vector<array<int, 4> > operator () () const {
        vector<array<int, 4> > result;
        int iteration = 0;
        double last_t = (rdtsc() - clock_begin) / TLE;
        double max_delta = 0;
        for (; ; ++ iteration) {
            double t = (rdtsc() - clock_begin) / TLE;
            if (t + 2 * max_delta + 0.05 > 0.50) break;
            vector<array<int, 4> > it = solve1();
            if (result.size() < it.size()) {
                result = it;
                cerr << "score = " << (result.size() /(double) (H * W / 2)) << endl;
                if (int(result.size()) == H * W / 2) {
                    ++ iteration;
                    break;  // the optimal
                }
            }
            chmax(max_delta, t - last_t);
            last_t = t;
        }
        cerr << "iteration = " << iteration << endl;
        double t = (rdtsc() - clock_begin) / TLE;
        cerr << "time = " << t << endl;
        cerr << "max delta = " << max_delta << endl;
        return result;
    }

private:
    bool is_on_field(int y, int x) const {
        return 0 <= y and y < H and 0 <= x and x < W;
    }
};

class SameColorPairs {
public:
    vector<string> removePairs(vector<string> a_board) {
        clock_begin = rdtsc();

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
        REP (y, H) {
            REP (x, W) {
                cerr << board[y][x];
            }
            cerr << endl;
        }

        // solve
        vector<array<int, 4> > result = Solver(H, W, C, board)();

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
        return answer;
    }
};
