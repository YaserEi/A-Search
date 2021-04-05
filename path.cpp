#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::sort;
using std::abs;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};


vector<State> ParseLine(string line)
{

    istringstream my_stream(line);
    int n;
    char c;
    vector<State> vector;

    while (my_stream >> n >> c && c == ','){
        if (n ==0)
        {

        vector.push_back(State::kEmpty);
        }
        else{
        vector.push_back(State::kObstacle);
        }
    }

    return vector;

}

vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
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

bool compare(vector<int> node1, vector<int> node2){
    int f1 = node1[2]+node1[3];
    int f2 = node2[2]+node2[3];

    return f1>f2;
}

void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), compare);
}
int Heurstic (int x1,  int y1, int x2, int y2)
{
    return abs(x2-x1)+abs(y2-y1);
}
bool checkValidCell(int x, int y , vector<vector<State>> &table){

  bool on_grid_x = (x >= 0 && x < table.size());
  bool on_grid_y = (y >= 0 && y < table.size());
  if (on_grid_x && on_grid_y)
    return table[x][y] == State::kEmpty;
  return false;
}
void addToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  // Add node to open vector, and mark grid cell as closed.
  openlist.push_back(vector<int>{x, y, g, h});
  grid[x][y] = State::kClosed;
}
void ExpandNeighbors (const vector<int> &current, int goal[2], vector<vector<int>> &open, vector<vector<State>> &grid ) // current node, x and y
{
    int Xcurrent = current[0];
    int Ycurrent = current[1];
    int g = current[2];
    int h;
    int Xneighbor;
    int Yneighbor;

    if (checkValidCell(Xcurrent+1,Ycurrent,grid))
    {
       Xneighbor = Xcurrent+1;
       Yneighbor = Ycurrent;
       h =  Heurstic(Xcurrent, Ycurrent, goal[0], goal[2]);
       addToOpen (Xneighbor, Yneighbor , g, h,  open, grid);
    }
    if (checkValidCell(Xcurrent-1,Ycurrent,grid))
    {
       Xneighbor = Xcurrent+1;
       Yneighbor = Ycurrent;
       h =  Heurstic(Xcurrent, Ycurrent, goal[0], goal[2]);
       addToOpen (Xneighbor, Yneighbor , g, h,  open, grid);
    }
    if (checkValidCell(Xcurrent,Ycurrent+1,grid))
    {
       Xneighbor = Xcurrent+1;
       Yneighbor = Ycurrent;
       h =  Heurstic(Xcurrent, Ycurrent, goal[0], goal[2]);
       addToOpen (Xneighbor, Yneighbor , g, h,  open, grid);
    }
    if (checkValidCell(Xcurrent,Ycurrent-1,grid))
    {
       Xneighbor = Xcurrent+1;
       Yneighbor = Ycurrent;
       h =  Heurstic(Xcurrent, Ycurrent, goal[0], goal[2]);
       addToOpen (Xneighbor, Yneighbor , g, h,  open, grid);
    }
}

vector<vector<State>> search (vector<vector<State>> grid, int start[2], int goal[2])
{
    // Create the vector of open nodes.
    vector<vector<int>> open {};
    
    // Initialize the starting node.
    int x = start[0];
    int y = start[1];
    int g = 0;
    int h = Heurstic(x, y, goal[0],goal[1]);
    addToOpen(x, y, g, h, open, grid);

    while (open.size() > 0) {
        // Get the next node
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        grid[x][y] = State::kPath;

        // Check if we're done.
        if (x == goal[0] && y == goal[1]) {
        grid[start[0]][start[1]] = State::kStart;
        grid[goal[0]][goal[1]] = State::kFinish;
        return grid;
        }
        
        // If we're not done, expand search to current node's neighbors.
        ExpandNeighbors(current, goal, open, grid);
    }
    
    // We've run out of new nodes to explore and haven't found a path.
    cout << "No path found!" << "\n";
    return std::vector<vector<State>>{};
}

string CellString(State cell)
{
    switch (cell)
    {
    case State::kObstacle: return"⛰️   " ;
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
    default: return "0   "; 
    }
}



void printBoard (vector<vector<State>> table)
{
    for (int i =0; i <table.size(); i++)
       {
           for (int j =0; j<table[i].size(); j++)
           {
           cout << CellString(table[i][j]);
           }
           cout <<"\n";
       }
}

int main(){
  int init[2]{0, 0};
  int goal[2]{4, 5};
  auto board = ReadBoardFile("1.board");
  auto solution = search(board, init, goal);
  printBoard(solution);



}
