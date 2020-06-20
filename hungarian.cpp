#include <iostream>
#include <vector>
#include <cassert>
#include <utility>
#include <algorithm>
#include <iomanip>
 
using namespace std;
 
using Matrix = vector<vector<int>>;
 
 
enum State {
    SIMPLE,
    STARRED,
    PRIMED
};
 
template <typename First, typename Second>
ostream& operator<<(ostream& stream, pair<First, Second> pair) {
    stream << "(" << pair.first << ", " << pair.second << ")";
    return stream;
}
 
template <typename T>
ostream& operator<<(ostream& stream, vector<T> container) {
    for (int i = 0; i < container.size(); i++) {
        stream << setw(6) << container[i] << " ";
    }
    stream << endl;
    return stream;
}
 
 
class HungarianAlgorithm {
public:
    explicit HungarianAlgorithm(Matrix& input_matrix) : cost_matrix(input_matrix) {
        rows = cost_matrix.size();
        cols = cost_matrix[0].size();
        mark_matrix.assign(rows, vector<int>(cols, State::SIMPLE));
        initial_cost_matrix = cost_matrix;
        row_cover.assign(rows, false);
        col_cover.assign(cols, false);
 
        solve();
    }
 
    vector<vector<int>> inputCostMatrix() const {
        return initial_cost_matrix;
    }
 
    vector<pair<int, int>> optimalAssignment() const {
        return assigned;
    }
 
    int optimalCost() const {
        return cost;
    }
 
private:
    Matrix initial_cost_matrix;
    Matrix cost_matrix;
    Matrix mark_matrix;
    int rows, cols;
    vector<bool> row_cover, col_cover;
 
    int cost = 0;
    vector<pair<int, int>> assigned;
 
    void solve() {
        for (int r = 0; r < rows; r++) {
            int min_val = cost_matrix[r][0];
            for (int c = 0; c < cols; c++) {
                min_val = min(min_val, cost_matrix[r][c]);
            }
 
            if (min_val != 0) {
                for (int c = 0; c < cols; c++) {
                    cost_matrix[r][c] -= min_val;
                }
            }
        }
 
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (cost_matrix[r][c] == 0 && !row_cover[r] && !col_cover[c]) {
                    mark_matrix[r][c] = State::STARRED;
                    row_cover[r] = true;
                    col_cover[c] = true;
                }
            }
        }
 
        row_cover.assign(rows, false);
        col_cover.assign(cols, false);
 
        bool is_match = false;
 
        while (!is_match) {
            for (int r = 0; r < rows; r++) {
                bool any_starred = any_of(begin(mark_matrix), end(mark_matrix), [r](vector<int> mrow) {
                    return mrow[r] == State::STARRED;
                });
 
                if (any_starred) {
                    col_cover[r] = any_starred;
                }
            }
 
            bool all_cols_covered = all_of(begin(col_cover), end(col_cover), [](int c) {
                return c != 0;
            });
 
            if (all_cols_covered) {
                is_match = true;
                continue;
            } else {
                pair<int, int> zero_index = coverZero();
 
                vector<pair<int, int>> primes{zero_index};
                vector<pair<int, int>> stars;
 
                while (zero_index.first != -1) {
                    zero_index = findStarInCol(zero_index.second);
                    if (zero_index.first != -1) {
                        stars.push_back(zero_index);
                        zero_index = findPrimeInRow(zero_index.first);
                        if (zero_index.first != -1) {
                            stars.push_back(zero_index);
                        }
 
                    }
                }
 
                for (auto &star : stars) {
                    mark_matrix[star.first][star.second] = State::SIMPLE;
                }
 
                for (auto &prime : primes) {
                    mark_matrix[prime.first][prime.second] = State::STARRED;
                }
 
                for (int r = 0; r < rows; r++) {
                    for (int c = 0; c < cols; c++) {
                        if (mark_matrix[r][c] == State::PRIMED) {
                            mark_matrix[r][c] = State::SIMPLE;
                        }
                    }
                }
 
                row_cover.assign(rows, false);
                col_cover.assign(cols, false);
 
            }
        }
 
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (mark_matrix[r][c] == State::STARRED) {
                    cost += initial_cost_matrix[r][c];
                    assigned.push_back({r, c});
                }
            }
        }
 
    };
 
    pair<int, int> coverZero() {
        while (true) {
            pair<int, int> zero {1, 1};
 
            while (zero.first != -1) {
                zero = findUncoveredZero();
                if (zero.first == -1) {
                    break;
                } else {
                    vector<int> row = mark_matrix[zero.first];
                    row[zero.second] = State::PRIMED;
 
                    int index = -1;
 
                    for (int c = 0; c < cols; c++) {
                        if (row[c] == State::STARRED) {
                            index = c;
                        }
                    }
 
                    if (index == -1) {
                        return zero;
                    }
 
                    row_cover[zero.first] = true;
                    col_cover[index] = false;
                }
            }
 
            vector<int> uncovered_values = findUncoveredValues();
 
            int min_uncovered = uncovered_values[0];
            for (auto& val : uncovered_values) {
                min_uncovered = min(min_uncovered, val);
            }
 
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (row_cover[r]) {
                        cost_matrix[r][c] += min_uncovered;
                    }
                    if (!col_cover[c]) {
                        cost_matrix[r][c] -= min_uncovered;
                    }
                }
            }
 
        }
    };
    pair<int, int> findUncoveredZero() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (cost_matrix[r][c] == 0 && !row_cover[r] && !col_cover[c]) {
                    return {r, c};
                }
            }
        }
        return {-1, -1};
    };
 
    vector<int> findUncoveredValues() {
        vector<int> uncovered_values;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (!row_cover[r] && !col_cover[c]) {
                    uncovered_values.push_back(cost_matrix[r][c]);
                }
            }
        }
        return uncovered_values;
    };
    pair<int, int> findStarInCol(int col) {
        for (int r = 0; r < rows; r++) {
            if (mark_matrix[r][col] == State::STARRED) {
                return {r, col};
            }
        }
        return {-1, -1};
    };
 
    pair<int, int> findPrimeInRow(int row) {
        for (int c = 0; c < cols; c++) {
            if (mark_matrix[row][c] == State::PRIMED) {
                return {row, c};
            }
        }
 
        return {-1, -1};
    };
 
};
 
 
int main() {
    int n;
    cin >> n;
 
    Matrix matrix(n, vector<int>(n));
 
    for (auto& row : matrix) {
        for (auto& value : row) {
            cin >> value;
        }
    }
 
 
    HungarianAlgorithm hga(matrix);
 
    cout << "INITIAL COSTS:" << endl;
    cout << hga.inputCostMatrix() << endl;
 
    cout << "OPTIMAL TOTAL COST: ";
    cout << hga.optimalCost() << endl;
 
    for (auto& assigned : hga.optimalAssignment()) {
        cout << "Worker " << assigned.first + 1 << " assigned to work " << assigned.second + 1 << endl;
    }
 
    return 0;
}
