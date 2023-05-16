#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>

using namespace std;


struct _8Puzzle {
    vector<vector<int>> board;
    int cost;
    int depth;
    string moves;

    bool operator<(const _8Puzzle& other) const {  // Initializing Priority Queue 
        return cost > other.cost;
    }
};

bool isGoalState(const vector<vector<int>>& board) { // Receives the current board state and checks if is goal state. It returns true if it is a goal state. 
    int n = board.size();
    int num = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == n - 1 && j == n - 1) {
                if (board[i][j] != 0) {
                    return false;
                }
            }
            else {
                if (board[i][j] != num) {
                    return false;
                }
                num++;
            }
        }
    }
    return true;
}


vector<_8Puzzle> generateMoves(const _8Puzzle& state) { // To genetate all the possible moves from current states
    vector<_8Puzzle> moves;
    int n = state.board.size();
    int dx[] = { 1, -1, 0, 0 };
    int dy[] = { 0, 0, 1, -1 };
    string directions[] = { "DOWN", "UP", "RIGHT", "LEFT" };

    int zeroRow, zeroCol;
  
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (state.board[i][j] == 0) {
                zeroRow = i;
                zeroCol = j;
                break;
            }
        }
    }

    
    for (int i = 0; i < 4; ++i) {
        int newRow = zeroRow + dx[i];
        int newCol = zeroCol + dy[i];

        if (newRow >= 0 && newRow < n && newCol >= 0 && newCol < n) {
            vector<vector<int>> newBoard = state.board;
            swap(newBoard[zeroRow][zeroCol], newBoard[newRow][newCol]);
            moves.push_back({ newBoard, state.cost + 1, state.depth + 1, state.moves + directions[i] + " " });  // New state is created and pused 
        }
    }

    return moves;
}

string solvePuzzle(const vector<vector<int>>& initialBoard, int& expandedNodes) {
    int n = initialBoard.size();
    _8Puzzle initialState = { initialBoard, 0, 0, "" };     // Performing Uniform Cost Search

    priority_queue<_8Puzzle> pq;        // Initialized Priority Queue 
    pq.push(initialState);      // Pushed Initial state 

    map<vector<vector<int>>, bool> visited; // to keep track of visited nodes
    visited[initialState.board] = true;

    expandedNodes = 0;       // To maintain No.of nodes expanded 

    while (!pq.empty()) {
        _8Puzzle currentState = pq.top();
        pq.pop();
        expandedNodes++;

        if (isGoalState(currentState.board)) {  // To check if the current state is goal state
            return currentState.moves;
        }

        vector<_8Puzzle> moves = generateMoves(currentState);   // Generate all the possible moves
        for (const _8Puzzle& move : moves) {
            if (!visited[move.board]) {
                pq.push(move);
                visited[move.board] = true;
            }
        }
    }

    return "No solution found.";
}


void printBoard(const vector<vector<int>>& board) {
    int n = board.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> initialBoard = {
        {0, 7, 2},
        {4, 6, 1},
        {3, 5, 8}
    };

    cout << "Initial Board:" << endl;
    printBoard(initialBoard);
    cout << endl;

    int expandedNodes;
    string solution = solvePuzzle(initialBoard, expandedNodes);

    if (solution != "No solution found.") {
        cout << "Solution Found!" << endl;
        cout << "Moves: " << solution << endl;
        cout << "Total Moves: " << solution.size() / 5 << endl;
        cout << "Nodes Expanded: " << expandedNodes << endl;
    }
    else {
        cout << "No solution found." << endl;
        cout << "Nodes Expanded: " << expandedNodes << endl;
    }

    return 0;
}

