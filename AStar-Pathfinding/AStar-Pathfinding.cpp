#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

struct Node {
    int x, y;
    double g, h;
    Node* parent;

    Node(int x, int y, double g, double h, Node* parent)
        : x(x), y(y), g(g), h(h), parent(parent) {}

    double f() const {
        return g + h;
    }
};

struct CompareNodes {
    bool operator()(const Node* a, const Node* b) {
        return a->f() > b->f();
    }
};

class AStar {
public:
    AStar(vector<vector<int>>& grid)
        : grid(grid), rows(grid.size()), cols(grid[0].size()) {}

    vector<pair<int, int>> findPath(pair<int, int> start, pair<int, int> goal) {
        priority_queue<Node*, vector<Node*>, CompareNodes> openSet;
        vector<vector<Node>> nodes(rows, vector<Node>(cols, Node(0, 0, 0, 0, nullptr)));

        Node* startNode = &nodes[start.first][start.second];
        startNode->x = start.first;
        startNode->y = start.second;

        openSet.push(startNode);

        while (!openSet.empty()) {
            Node* current = openSet.top();
            openSet.pop();

            if (current->x == goal.first && current->y == goal.second) {
                return reconstructPath(current);
            }

            for (const auto& neighbor : getNeighbors(current)) {
                double tentative_g = current->g + calculateDistance(current, neighbor);

                if (tentative_g < neighbor->g || neighbor->g == 0) {
                    neighbor->g = tentative_g;
                    neighbor->h = calculateDistance(neighbor, &nodes[goal.first][goal.second]);
                    neighbor->parent = current;

                    if (!find(openSet, neighbor)) {
                        openSet.push(neighbor);
                    }
                }
            }
        }

        return vector<pair<int, int>>(); // No path found
    }

private:
    vector<vector<int>>& grid;
    int rows, cols;

    bool find(priority_queue<Node*, vector<Node*>, CompareNodes>& openSet, Node* node) {
        priority_queue<Node*, vector<Node*>, CompareNodes> tempSet = openSet;

        while (!tempSet.empty()) {
            if (tempSet.top() == node) {
                return true;
            }
            tempSet.pop();
        }

        return false;
    }

    vector<Node*> getNeighbors(Node* node) {
        vector<Node*> neighbors;
        int x = node->x;
        int y = node->y;

        if (isValid(x - 1, y)) neighbors.push_back(new Node(x - 1, y, 0, 0, nullptr));
        if (isValid(x + 1, y)) neighbors.push_back(new Node(x + 1, y, 0, 0, nullptr));
        if (isValid(x, y - 1)) neighbors.push_back(new Node(x, y - 1, 0, 0, nullptr));
        if (isValid(x, y + 1)) neighbors.push_back(new Node(x, y + 1, 0, 0, nullptr));

        return neighbors;
    }

    bool isValid(int x, int y) {
        return x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == 0;
    }

    double calculateDistance(Node* a, Node* b) {
        return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
    }

    vector<pair<int, int>> reconstructPath(Node* node) {
        vector<pair<int, int>> path;
        while (node != nullptr) {
            path.emplace_back(node->x, node->y);
            node = node->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

int main() {

    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0}
    };

    AStar astar(grid);

    pair<int, int> start = { 0, 0 };
    pair<int, int> goal = { 4, 4 };

    vector<pair<int, int>> path = astar.findPath(start, goal);

    if (!path.empty()) {
        cout << "Path found: ";
        for (const auto& point : path) {
            cout << "(" << point.first << ", " << point.second << ") ";
        }
        cout << endl;
    }
    else {
        cout << "No path found." << endl;
    }

    // Add this line to pause and wait for user input
    cout << "Press Enter to exit...";
    cin.get();

    return 0;
}
