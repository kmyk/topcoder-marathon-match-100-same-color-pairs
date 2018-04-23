#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0; (i) < int(n); ++ (i))
#define REP3(i, m, n) for (int i = (m); (i) < int(n); ++ (i))
#define REP_R(i, n) for (int i = int(n) - 1; (i) >= 0; -- (i))
#define REP3R(i, m, n) for (int i = int(n) - 1; (i) >= int(m); -- (i))
#define ALL(x) begin(x), end(x)
using namespace std;
template <class T> inline void chmax(T & a, T const & b) { a = max(a, b); }
template <class T> inline void chmin(T & a, T const & b) { a = min(a, b); }

constexpr int MAX_H = 100;
constexpr int MAX_W = 100;
constexpr int MAX_C = 6;

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

vector<array<int, 4> > with_compress(int H, int W, int C, vector<vector<int> > const & board, function<vector<array<int, 4> > (int, int, int, vector<vector<int> > const &)> cont) {
    // check empty rows and cols
    vector<bool> is_empty_row(H, true);
    vector<bool> is_empty_col(W, true);
    REP (y, H) REP (x, W) {
        if (board[y][x] >= 0) {
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
    vector<array<int, 4> > rects = cont(map_row.size(), map_col.size(), C, nboard);

    // make the return value
    for (auto & rect : rects) {
        int & y1 = rect[0];
        int & x1 = rect[1];
        int & y2 = rect[2];
        int & x2 = rect[3];
        y1 = map_row[y1];
        x1 = map_col[x1];
        y2 = map_row[y2];
        x2 = map_col[x2];
    }
    return rects;
}

vector<array<int, 4> > with_transpose(int H, int W, int C, vector<vector<int> > const & original_board, function<vector<array<int, 4> > (int, int, int, vector<vector<int> > const &)> cont) {
    vector<vector<int> > board(W, vector<int>(H));
    REP (x, W) REP (y, H) {
        board[x][y] = original_board[y][x];
        if (board[x][y] < 0) board[x][y] = -1;
    }
    vector<array<int, 4> > rects = cont(W, H, C, board);
    for (auto & rect : rects) {
        swap(rect[0], rect[1]);
        swap(rect[2], rect[3]);
    }
    return rects;
}

vector<array<int, 4> > with_landscape(int H, int W, int C, vector<vector<int> > const & board, function<vector<array<int, 4> > (int, int, int, vector<vector<int> > const &)> cont) {
    if (H > W) {
        return with_transpose(H, W, C, board, cont);
    } else {
        return cont(H, W, C, board);
    }
}

vector<array<int, 4> > with_crop(int H, int W, int C, vector<vector<int> > const & original_board, int ly, int lx, int ry, int rx, function<vector<array<int, 4> > (int, int, int, vector<vector<int> > const &)> cont) {
    int h = ry - ly;
    int w = rx - lx;
    vector<vector<int> > board(h, vector<int>(w));
    REP (y, h) REP (x, w) {
        board[y][x] = original_board[ly + y][lx + x];
    }
    vector<array<int, 4> > rects = cont(h, w, C, board);
    for (auto & rect : rects) {
        rect[0] += ly;
        rect[1] += lx;
        rect[2] += ly;
        rect[3] += lx;
    }
    return rects;
}


array<int8_t, MAX_H * MAX_W> pack_board(int H, int W, vector<vector<int> > const & original_board) {
    array<int8_t, MAX_H * MAX_W> board;
    REP (y, H) REP (x, W) board[y * W + x] = original_board[y][x];
    return board;
}

void update_compression(int H, int W, array<int8_t, MAX_H * MAX_W> & board) {
    vector<int> stk;
    REP (y, H) {
        for (int x = 0; x < W; ) {
            if (board[y * W + x] < 0) {
                int nx = x - board[y * W + x];
                if (nx < W and board[y * W + nx] < 0) {
                    while (x < W and board[y * W + x] < 0) {
                        stk.push_back(x);
                        x -= board[y * W + x];
                    }
                    while (not stk.empty()) {
                        int px = stk.back();
                        stk.pop_back();
                        board[y * W + px] = px - x;
                    }
                } else {
                    x = nx;
                }
            } else {
                ++ x;
            }
        }
    }
}

bool is_valid_pair(int W, array<int8_t, MAX_H * MAX_W> const & board, pair<int, int> const & p1, pair<int, int> const & p2) {
    int y1, x1; tie(y1, x1) = p1;
    int y2, x2; tie(y2, x2) = p2;
    int c = board[y1 * W + x1];
    if (y1 > y2) swap(y1, y2);
    if (x1 > x2) swap(x1, x2);
    REP3 (y, y1, y2 + 1) {
        for (int x = x1; x < x2 + 1; ) {
            if (board[y * W + x] < 0) {
                x -= board[y * W + x];
            } else if (board[y * W + x] == c) {
                ++ x;
            } else {
                return false;
            }
        }
    }
    return true;
}

template <class InputIterator>
void apply_rects(vector<vector<int> > & board, InputIterator first, InputIterator last) {
    for (; first != last; ++ first) {
        int y1 = (*first)[0];
        int x1 = (*first)[1];
        int y2 = (*first)[2];
        int x2 = (*first)[3];
        board[y1][x1] = -1;
        board[y2][x2] = -1;
    }
}

template <class InputIterator>
void apply_rects(int W, array<int8_t, MAX_H * MAX_W> & board, InputIterator first, InputIterator last) {
    for (; first != last; ++ first) {
        int y1 = (*first)[0];
        int x1 = (*first)[1];
        int y2 = (*first)[2];
        int x2 = (*first)[3];
        board[y1 * W + x1] = -1;
        board[y2 * W + x2] = -1;
    }
}

template <class InputIterator>
InputIterator apply_unreliable_rects(int W, array<int8_t, MAX_H * MAX_W> & board, InputIterator first, InputIterator last) {
    InputIterator it = first;
    for (; first != last; ++ first) {
        int y1 = (*first)[0];
        int x1 = (*first)[1];
        int y2 = (*first)[2];
        int x2 = (*first)[3];
        // assume all conditions except is_valid_pair()
        if (not is_valid_pair(W, board, make_pair(y1, x1), make_pair(y2, x2))) continue;
        board[y1 * W + x1] = -1;
        board[y2 * W + x2] = -1;
        *(it ++) = *first;
    }
    return it;
}

template <class InputIterator, class OutputIterator>
void apply_rects_with_preceding_cells(int W, array<int8_t, MAX_H * MAX_W> & board, vector<pair<int, int> > preciding, InputIterator first, InputIterator last, int skip_i, OutputIterator dest) {
    auto check_use = [&](int y1, int x1, int y2, int x2) {
        if (not is_valid_pair(W, board, make_pair(y1, x1), make_pair(y2, x2))) return false;
        board[y1 * W + x1] = -1;
        board[y2 * W + x2] = -1;
        *(dest ++) = { y1, x1, y2, x2 };
        return true;
    };
    for (; first != last; ++ first) {
        if ((skip_i --) == 0) continue;
        int y1 = (*first)[0];
        int x1 = (*first)[1];
        int y2 = (*first)[2];
        int x2 = (*first)[3];
        REP (i, preciding.size()) {
            int y, x; tie(y, x) = preciding[i];
            if (check_use(y, x, y1, x1)
                    or check_use(y, x, y2, x2)) {
                swap(preciding[i], preciding.back());
                // preciding.pop_back();
                int k = preciding.size();
                preciding.resize(min(k - 1, max(3, k / 2)));
                // preciding.clear();
                goto next;
            }
        }
        check_use(y1, x1, y2, x2);
next: ;
    }
}

template <class RandomEngine>
vector<array<int, 4> > solve_greedy_once(int H, int W, int C, array<int8_t, MAX_H * MAX_W> & board, RandomEngine & gen) {
    // prepare lists of living cells
    vector<pair<int, int> > color(C);
    REP (y, H) {
        for (int x = 0; x < W; ) {
            if (board[y * W + x] < 0) {
                x -= board[y * W + x];
            } else {
                color.emplace_back(y, x);
                ++ x;
            }
        }
    }

    // remove greedily
    vector<array<int, 4> > rects;
    vector<array<int, 4> > removable;
    for (int iteration = 0; ; ++ iteration) {
        update_compression(H, W, board);
        removable.clear();
        for (int i = 0; i < int(color.size()); ++ i) {
            int y1, x1; tie(y1, x1) = color[i];
            int c = board[y1 * W + x1];
            if (c < 0) {
                swap(color[i], color.back());
                color.pop_back();
                -- i;
                continue;
            }
            int min_x2 = W;
            auto func = [&](int y2) {
                int x2 = x1 + (y2 == y1);
                while (x2 < W and board[y2 * W + x2] < 0) {
                    x2 -= board[y2 * W + x2];
                }
                if (x2 == W) return;
                if (board[y2 * W + x2] == c) {
                    if (x2 < min_x2) {
                        removable.push_back((array<int, 4>) { y1, x1, y2, x2 });
                    }
                    chmin(min_x2, x2 + 1);
                } else {
                    chmin(min_x2, x2);
                }
            };
            REP3 (y2, y1, H) {
                func(y2);
                if (min_x2 == x1) break;
            }
            min_x2 = W;
            REP_R (y2, y1 + 1) {
                func(y2);
                if (min_x2 == x1) break;
            }
        }
        if (removable.empty()) break;
        shuffle(ALL(removable), gen);
        for (auto const & rect : removable) {
            int y1 = rect[0];
            int x1 = rect[1];
            int y2 = rect[2];
            int x2 = rect[3];
            if (board[y1 * W + x1] < 0) continue;
            if (board[y2 * W + x2] < 0) continue;
            board[y1 * W + x1] = -1;
            board[y2 * W + x2] = -1;
            rects.push_back(rect);
        }
    }
    return rects;
}

template <class RandomEngine>
vector<array<int, 4> > solve_sa_greedy(int H, int W, int C, vector<vector<int> > const & original_original_board, double clock_end, RandomEngine & gen) {
    array<int8_t, MAX_H * MAX_W> original_board = pack_board(H, W, original_original_board);
    update_compression(H, W, original_board);
    vector<array<int, 4> > result;
    vector<array<int, 4> > cur;
    array<int8_t, MAX_H * MAX_W> saved_board = original_board;
    int saved_size = 0;
    vector<pair<int, int> > remaining;

    // values for partialy solved board
    int initial_removed = 0;
    REP (y, H) REP (x, W) {
        initial_removed += (original_board[y * W + x] < 0);
    }
    int optimal_size = 0; {  // since the numbers of cells sometimes are odd
        vector<bool> dangling(C);
        REP (y, H) REP (x, W) {
            int c = original_board[y * W + x];
            if (c >= 0) {
                optimal_size += dangling[c];
                dangling[c] = not dangling[c];
            }
        }
    }

    int iteration = 0;
    double clock_begin = rdtsc();
    double last_t = rdtsc();
    double max_delta = 0;
    array<int8_t, MAX_H * MAX_W> board;
    for (; ; ++ iteration) {
        double t = rdtsc();
        if (t + 2 * max_delta + 0.2 > clock_end) break;
        double temperature = (1 - (t - clock_begin) / (clock_end - clock_begin));

        // prepare board and the prefix of nxt
        char type;
        vector<array<int, 4> > nxt;
        int modified_i = 0;

        if (cur.empty() or bernoulli_distribution(0.01 * temperature)(gen)) {
            // forget everything and restart from zero
            type = 'A';
            memcpy(board.data(), original_board.data(), H * W);

        } else if (not remaining.empty() and bernoulli_distribution(0.5)(gen)) {
            // make neighborhood removing last remaining cells
            type = 'B';
            memcpy(board.data(), original_board.data(), H * W);
            int k = nxt.size();
            int limit = max(0, k - 500);
            int i = uniform_int_distribution<int>(bernoulli_distribution(0.2)(gen) ? 0 : limit, k - 1)(gen);
            shuffle(ALL(remaining), gen);
            apply_rects_with_preceding_cells(W, board, remaining, ALL(cur), i, back_inserter(nxt));

        } else {
            // make neighborhood only removing a pair
            type = 'C';
            nxt = cur;
            int k = nxt.size();
            int limit = max(0, k - 500);
            if (saved_size < limit) {
                apply_rects(W, saved_board, nxt.begin() + saved_size, nxt.begin() + limit);
                update_compression(H, W, saved_board);
                saved_size = limit;
            }
            modified_i = uniform_int_distribution<int>(bernoulli_distribution(0.2)(gen) ? 0 : limit, k - 1)(gen);
            swap(nxt[modified_i], nxt.back());
            if (modified_i >= saved_size) {
                memcpy(board.data(), saved_board.data(), H * W);
                apply_rects(W, board, nxt.begin() + saved_size, nxt.begin() + modified_i);
            } else {
                board = original_board;
                apply_rects(W, board, nxt.begin(), nxt.begin() + modified_i);
            }
            nxt.erase(apply_unreliable_rects(W, board, nxt.begin() + modified_i, nxt.end()), nxt.end());
        }

        // run greedy
        vector<array<int, 4> > delta = solve_greedy_once(H, W, C, board, gen);
        copy(ALL(delta), back_inserter(nxt));

        // update cur and result
        int size_delta = int(nxt.size()) - int(cur.size());
        double prob = exp(0.3 * size_delta / temperature);
        if (cur.size() <= nxt.size() or bernoulli_distribution(prob)(gen)) {
            cur = nxt;
            if (modified_i < saved_size) {
                memcpy(saved_board.data(), original_board.data(), H * W);
                saved_size = 0;
            }
            remaining.clear();
            if (H * W - initial_removed - 2 * int(cur.size()) <= 100) {
                REP (y, H) {
                    for (int x = 0; x < W; ) {
                        if (board[y * W + x] < 0) {
                            x -= board[y * W + x];
                        } else {
                            remaining.emplace_back(y, x);
                            ++ x;
                        }
                    }
                }
            }
            if (result.size() < cur.size()) {
                result = nxt;
#ifdef LOCAL
                cerr << type << " : ";
                cerr << "score = " << H * W - initial_removed - 2 * int(result.size()) << " / " << H * W << endl;
#endif
                if (int(result.size()) == optimal_size) {
                    ++ iteration;
                    break;
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

vector<int> split_to_chunks(int w, int k) {
    vector<int> ws(k + 1);
    REP (i, k) {
        ws[i + 1] = w * (i + 1) / k;
    }
    return ws;
}

vector<array<int, 4> > solve(int H, int W, int C, vector<vector<int> > const & original_board) {
    double clock_begin = rdtsc();
    random_device device;
    xor_shift_128 gen(device());
    vector<vector<int> > board = original_board;

    // split the board to blocks
    vector<int> h = split_to_chunks(H, H / 30 + 1);
    vector<int> w = split_to_chunks(W, W / 30 + 1);
    int hk = h.size() - 1;
    int wk = w.size() - 1;
    int k = hk * wk;

    // solve each black
    vector<tuple<int, int, int> > order;
    REP (y, hk) REP (x, wk) {
        order.emplace_back(y + x, y, x);
    }
    order.pop_back();
    sort(ALL(order));
    vector<array<int, 4> > rects;
    REP (i, k - 1) {
        int y, x; tie(ignore, y, x) = order[i];
        cerr << "[" << h[y] << ", " << h[y + 1] << ") * [" << w[x] << ", " << w[x + 1] << ")" << endl;
        vector<array<int, 4> > delta =
            with_crop(H, W, C, board, 0, 0, h[y + 1], w[x + 1], [&](int H, int W, int C, vector<vector<int> > const & board) {
                return with_compress(H, W, C, board, [&](int H, int W, int C, vector<vector<int> > const & board) {
                    return with_landscape(H, W, C, board, [&](int H, int W, int C, vector<vector<int> > const & board) {
                        double clock_end = min(rdtsc() + min(1.0, 3 * 5.0 / (k - 1)), clock_begin + 5.0 * (i + 1) / (k - 1));
                        return solve_sa_greedy(H, W, C, board, clock_end, gen);
                    });
                });
            });
        apply_rects(board, ALL(delta));
        copy(ALL(delta), back_inserter(rects));
    }

    // solve the entire board
    vector<array<int, 4> > delta =
        with_compress(H, W, C, board, [&](int H, int W, int C, vector<vector<int> > const & board) {
            return with_landscape(H, W, C, board, [&](int H, int W, int C, vector<vector<int> > const & board) {  // use landscape mode to make linked lists more efficient
                return solve_sa_greedy(H, W, C, board, clock_begin + 9.0, gen);
            });
        });
    apply_rects(board, ALL(delta));
    copy(ALL(delta), back_inserter(rects));
    return rects;
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
