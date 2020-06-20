#include <iostream>
#include <vector>
#include <cassert>
#include <utility>
#include <algorithm>
#include <climits>
#include <set>
#include <iomanip>
 
using namespace std;
 
using Matrix = vector<vector<int>>;
 
 
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
 
 
class AssignmentProblemBruteForce {
public:
    AssignmentProblemBruteForce (Matrix& cost_matrix) : initial_cost_matrix(cost_matrix) {
        rows = cost_matrix.size();
        cols = cost_matrix[0].size();
 
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
    int cost = INT_MAX;
    Matrix initial_cost_matrix;
    vector<pair<int, int>> assigned;
    int rows, cols;
 
    void solve() {
        set<int> assigned_tasks;
        vector<pair<int, int>> current_assignment;
        return dfs(0, assigned_tasks, current_assignment, 0);
    };
    void dfs(int current_worker, set<int>& assigned_tasks, vector<pair<int, int>>& current_assignment, int current_cost) {
        if (current_worker == rows && current_cost < cost) {
            assigned = current_assignment;
            cost = current_cost;
            return;
        }
 
        for (int j = 0; j < cols; j++) {
            if (assigned_tasks.count(j) == 0) {
                assigned_tasks.insert(j);
                current_assignment.push_back({current_worker, j});
                int cost = initial_cost_matrix[current_worker][j];
                dfs(current_worker + 1, assigned_tasks, current_assignment, current_cost + cost);
                current_assignment.pop_back();
                assigned_tasks.erase(j);
            }
        }
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
 
    AssignmentProblemBruteForce apbf(matrix);
 
    cout << "INITIAL COSTS:" << endl;
    cout << apbf.inputCostMatrix() << endl;
 
    cout << "OPTIMAL TOTAL COST: ";
    cout << apbf.optimalCost() << endl;
 
    for (auto& assigned : apbf.optimalAssignment()) {
        cout << "Worker " << assigned.first + 1 << " assigned to work " << assigned.second + 1 << endl;
    }
    return 0;
}
