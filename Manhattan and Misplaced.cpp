#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>

using namespace std;

struct _8Puzzle {
    vector<vector<int>> board;
    int cost;
    int heuristic;
    int depth;
    string moves;
    string ManHa;

    bool operator<(const _8Puzzle& other) const {
        return (cost + heuristic) > (other.cost + other.heuristic);     // Initializing Priority Queue 
    }
};


int calculateMisplacedTiles(const vector<vector<int>>& board) {     // To calculate the misplaced tiles and return their count.
    int count = 0;
    int n = board.size();
    int num = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == n - 1 && j == n - 1) {
                if (board[i][j] != 0) {
                    count++;                    //Checking if the value of the cell is non-zero. If it is non-zero then it means the cell is not in correct position and the count is increamented
                }
            }
            else {
                if (board[i][j] != num) {
                    count++;
                }
                num++;
            }
        }
    }
    return count;
}


int calculateManhattanDistance(const vector<vector<int>>& board) { // to calculate Manhattan Distance and returns distance to goal state of each tile
    int distance = 0;
    int n = board.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int value = board[i][j];
            if (value != 0) {               // if the value is not zero then the distance to target row and column in goal state 
                int targetRow = (value - 1) / n;
                int targetCol = (value - 1) % n;
                distance += abs(i - targetRow) + abs(j - targetCol);
            }
        }
    }
    return distance;
}


bool isGoalState(const vector<vector<int>>& board) {// Receives the current board state and checks if is goal state. It returns true if it is a goal state. 
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


vector<_8Puzzle> generateMoves(const _8Puzzle& state) { // To genetate all the possible moves from current states for Misplaced Tile
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
            moves.push_back({ newBoard, state.cost + 1, calculateMisplacedTiles(newBoard), state.depth + 1, state.moves + directions[i] + " " });
        }
    }

    return moves;
}

vector<_8Puzzle> generateMovesManHa(const _8Puzzle& state) {        // To genetate all the possible moves from current states for Manhattn
    vector<_8Puzzle> ManHa;
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
            ManHa.push_back({ newBoard, state.cost + 1, calculateManhattanDistance(newBoard), state.depth + 1, state.moves + directions[i] + " ", state.ManHa + directions[i] + " " });
        }
    }

    return ManHa;
}



string solvePuzzle(const vector<vector<int>>& initialBoard, int& nodesExpanded) {
    int n = initialBoard.size();
    _8Puzzle initialState = { initialBoard, 0, calculateMisplacedTiles(initialBoard), 0, "" };  // calling Misplaced tile 

    priority_queue<_8Puzzle> pq;
    pq.push(initialState);

    map<vector<vector<int>>, bool> visited;
    visited[initialState.board] = true;

    nodesExpanded = 0;

    while (!pq.empty()) {
        _8Puzzle currentState = pq.top();
        pq.pop();
        nodesExpanded++;

        if (isGoalState(currentState.board)) {
            return currentState.moves;
        }

        vector<_8Puzzle> moves = generateMoves(currentState);
        for (const _8Puzzle& move : moves) {
            if (!visited[move.board]) {
                pq.push({ move.board, move.cost, move.heuristic, currentState.depth + 1, move.moves });
                visited[move.board] = true;
            }
        }
    }

    return "No solution found.";
}


string solvePuzzleManHa(const vector<vector<int>>& initialBoard, int& nodesExpanded) {
    int n = initialBoard.size();
    _8Puzzle initialState = { initialBoard, 0, calculateManhattanDistance(initialBoard), 0, "" };

    priority_queue<_8Puzzle> pq;        // Initialized Priority Queue 
    pq.push(initialState);              // Pusined Initial state 

    map<vector<vector<int>>, bool> visited; // Hashmap to keep track of visited nodes
    visited[initialState.board] = true;

    nodesExpanded = 0;  // To maintain No.of nodes expanded 

    while (!pq.empty()) {
        _8Puzzle currentState = pq.top(); // Extraction of highest priority state 
        pq.pop();      // it is removed after extraction
        nodesExpanded++;

        if (isGoalState(currentState.board)) {          // To check if the current state is goal state
            return currentState.ManHa;
        }

        vector<_8Puzzle> ManHa = generateMovesManHa(currentState);
        for (const _8Puzzle& move : ManHa) {
            if (!visited[move.board]) {                                                                             // Generate all the possible moves
                pq.push({ move.board, move.cost, move.heuristic, currentState.depth + 1, move.moves, move.ManHa });
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
        {1, 6, 7},
        {5, 0, 3},
        {4, 8, 2}
    };

    cout << "Initial Board:" << endl;
    printBoard(initialBoard);
    cout << endl;

    int nodesExpanded = 0;
    int nodesMan = 0;

    string solution = solvePuzzle(initialBoard, nodesExpanded);
    string ManHaSolution = solvePuzzleManHa(initialBoard, nodesMan);

    if (solution != "No solution found.") {
        cout << "Solution Found!" << endl;
        cout << "Moves: " << solution << endl;
        cout << "Total depth: " << (solution.size() / 5) << endl;
        cout << "Number of nodes expanded: " << nodesExpanded << endl;
    }
    else {
        cout << "No solution found." << endl;
    }

    if (ManHaSolution != "No solution found.") {
        cout << "Solution Found with Manhattan Distance!" << endl;
        cout << "Moves: " << ManHaSolution << endl;
        cout << "Total depth: " << (ManHaSolution.size() / 5) << endl;
        cout << "Number of nodes expanded: " << nodesMan << endl;
    }
    else {
        cout << "No solution found." << endl;
    }

    return 0;
}


