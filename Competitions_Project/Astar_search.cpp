#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State { kEmpty, kObstacle, kClosed, kPath, kStart, kFinish };

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
        row.push_back(n == 0 ? State::kEmpty : State::kObstacle);
    }
    return row;
}

int Heuristic(const int x1, const int y1, const int x2, const int y2)
{
    return abs(x2 - x1) + abs(y2 - y1);
}


bool CheckValidCell(int x, int y, vector<vector<State>> grid)
{
    bool flag1 = (x >= 0 && grid.size() > x);
    bool flag2 = (y >= 0 && grid[0].size() > y);
    if (flag1 && flag2) return grid[x][y] == State::kEmpty;
    return false;
}

vector<vector<State>> ReadBoardFile(string path) {
    ifstream myfile(path);

    vector<vector<State>> board{};
    if (myfile) {
        string line;
        while (getline(myfile, line)) {
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}

void AddToOpen(const int x, const int y, const int g, const int h, vector<vector<int>>& open, vector<vector<State>>& grid)
{
    vector<int> node = { x, y, g, h };
    open.push_back(node);
    grid[x][y] = State::kClosed;
}


const int delta[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

void ExpandNeighbors(vector<int> current, vector<vector<int>> &open, vector<vector<State>> &grid, int goal[2])
{
    auto x = current[0];
    auto y = current[1];
    auto g = current[2];

    for(auto i = 0; i < 4; i++)
    {
        auto x2 = x + delta[i][0];
        auto y2 = y + delta[i][1];

    	if(CheckValidCell(x2, y2, grid))
    	{
            auto g2 = g + 1;
            auto h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, open, grid);
    	}
    }
}

string CellString(State cell) {
    switch (cell) {
		case State::kObstacle: return "ob   ";
        case State::kPath: return "p   ";
        case State::kStart: return "st   ";
		case State::kFinish: return "fi   ";
		default: return "0   ";
    }
}

bool Compare(vector<int> first, vector<int> second)
{
    auto f1 = first[2] + first[3];
    auto f2 = second[2] + second[3];
    return f1 > f2;
}

void CellSort(vector<vector<int>> *v)
{
    std::sort(v -> begin(), v -> end(), Compare);
}

void PrintBoard(const vector<vector<State>> board) {
    for (auto i = 0; i < board.size(); i++) {
        for (auto j = 0; j < board[i].size(); j++) {

            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}




vector<vector<State>> Search(vector<vector<State>> board, int start[2], int finish[2])
{
    vector<vector<State>> solution{};
    vector<vector<int>> open{};
    auto x = start[0];
    auto y = start[1];
    auto g = 0;
    auto h = Heuristic(x, y, finish[0], finish[1]);
    AddToOpen(x, y, g, h, open, board);
	while (open.size() > 0)
	{
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        board[x][y] = State::kPath;
        if (x == finish[0] && y == finish[1])
        {
            board[start[0]][start[1]] = State::kStart;
            board[finish[0]][finish[1]] = State::kFinish;
            return board;
        }
        ExpandNeighbors(current, open, board, finish);

	}
    cout << "No path found!" << "\n";
    return solution;
}

int main() {
    auto board = ReadBoardFile("1.board");
    int init[2]{ 0, 0 };
    int goal[2]{ 4, 5 };
    auto solution = Search(board, init, goal);
    PrintBoard(solution);
}