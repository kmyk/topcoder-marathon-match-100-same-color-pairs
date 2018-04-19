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

class SameColorPairs {
public:
    vector<string> removePairs(vector<string> board) {
        vector<string> ret;
        int H = board.size();
        int W = board[0].size();
        // find the first pair of horizontally adjacent tiles and remove them
        REP (i, H) {
            REP (j, W) {
                if (board[i][j] == board[i][j-1]) {
                    ret.push_back(to_string(i) + " " + to_string(j) + " " + to_string(i) + " " + to_string(j - 1));
                    return ret;
                }
            }
        }
        return ret;
    }
};
