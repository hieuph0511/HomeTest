// Robot Movement

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;


/*--------- Manage Grid-------------//
*   int setDimension(int n): Define the grid size n.
*   int moveTo(int x, int y): Robot moves to (x, y) without drawing.
*   int lineTo(int x, int y): Move from current position to (x, y) with drawing on the grid.
*   void printGrid(): print grid.
*   void drawLine(): draw line moved.
------------------------------------*/
class Grid
{
public:
    // Constructor
    Grid() : dimension(0), currentX(0), currentY(0) {}
    bool checkGrid()
    {
        bool check = false;
        if(dimension <= 0)
        {
            cerr << "Incorrect format for size of grid."<<endl;
            check = false;
        }
        else check = true;

        return check; 
    }
    // Define the grid size n
    int setDimension(int n)
    {
        int status = 0;
        if(n <= 0)
        {
            cerr << "Incorrect format for size of grid DIMENSION command."<<endl;
            status = -1;
        }
        else
        {
            dimension = n;
            grid.resize(n, vector<char>(n, '.'));
            status = 0;
        }
        return status;
    }

    //Robot moves to (x, y) without drawing.
    int moveTo(int x, int y)
    {
        if(!checkGrid())
        {
            return -1;
        }
        int status = 0;
        if (x >= 0 && x < dimension && y >= 0 && y < dimension)
        {
            currentX = x;
            currentY = y;
            grid[currentY][currentX] = 'x';
            status = 0;
        }
        else if(x > dimension || y > dimension )
        {
            cerr << "Err: Position MOVE_TO > size grid."<<endl;
            status = -1;
        }
        else
        {
            cerr << "Err: Position MOVE_TO < 0."<<endl;
            status = -1;
        }
        return status;
    }

    //Move from current position to (x, y) with drawing on the grid.
    int lineTo(int x, int y)
    {
        if(!checkGrid())
        {
            return -1;
        }
        int status = 0;
        if (x >= 0 && x < dimension && y >= 0 && y < dimension)
        {
            drawLine(currentX, currentY, x, y);
            currentX = x;
            currentY = y;
            grid[currentY][currentX] = 'x';
            status = 0;
        }
        else if(x > dimension || y > dimension )
        {
            cerr << "Err: Position LINE_TO > size grid."<<endl;
            status = -1;
        }
        else
        {
            cerr << "Err: Position LINE_TO < 0."<<endl;
            status = -1;
        }
        return status;
    }

    // Print grid
    void printGrid()
    {
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                cout << "+---";
            }
            cout << "+" << endl;

            for (int j = 0; j < dimension; ++j)
            {
                if(grid[i][j] == 'x')
                {
                    cout << "| x ";
                }
                else cout << "|   ";
            }
            cout << "|" << endl;
        }

        for (int j = 0; j < dimension; ++j)
        {
            cout << "+---";
        }
        cout << "+" << endl;
    }

private:
    int dimension;
    int currentX, currentY;
    vector<vector<char>> grid;

    // Draw line moved.
    void drawLine(int x1, int y1, int x2, int y2)
    {
        int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = dx + dy, e2;

        while (true)
        {
            grid[y1][x1] = 'x';
            if (x1 == x2 && y1 == y2) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x1 += sx; }
            if (e2 <= dx) { err += dx; y1 += sy; }
        }
    }
};


/*--------- Manage Command-------------//
*   int executeCommand(const string &command): execute command from txt file
*   void printGrid(): print grid
------------------------------------*/
class CommandExecutor : public Grid
{
public:
    CommandExecutor() {}
    //Execute command from txt file
    int executeCommand(const string &command)
    {
        istringstream iss(command);
        string cmd;
        iss >> cmd;
        int status = 0;

        // Command DIMENSION
        if (cmd == "DIMENSION")
        {
            int n;
            iss >> n;
            status = grid.setDimension(n);
            if(status < 0)
            {
                cerr << "Incorrect format for DIMENSION command."<<endl;
            }
        }

        // Command MOVE_TO
        else if (cmd == "MOVE_TO")
        {
            int x, y;
            char comma;
            iss >> x >> comma >> y;
            if(comma == ',')
            {
                status = grid.moveTo(x, y);
                if(status < 0)
                {
                    cerr << "Incorrect format for MOVE_TO command."<<endl;
                }
            }
            else
            {
                cerr << "Incorrect format for MOVE_TO command."<<endl;
            }
        }

        // Command LINE_TO
        else if (cmd == "LINE_TO")
        {
            int x, y;
            char comma;
            iss >> x >> comma >> y;
            if(comma == ',')
            {
                status = grid.lineTo(x, y);
                if(status < 0)
                {
                    cerr << "Incorrect format for LINE_TO command."<<endl;
                }
            }
            else
            {
                cerr << "Incorrect format for LINE_TO command."<<endl;
            }
           
        }
        else
        {
            cerr << "Unknown command: " << cmd << endl;
        }
        return status;
    }

    // Print grid
    void printGrid()
    {
        grid.printGrid();
    }

private:
    Grid grid;
};

int main() {
    
    int status = 0;

    CommandExecutor executor;

    // open file command
    std::string src = "C:\\Users\\vanga\\OneDrive\\Máy tính\\code\\commands.txt";

    ifstream file(src);
    string line;

    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    while (getline(file, line)) 
    {
       status = executor.executeCommand(line);
       if(status != 0) break;
    }
    if(status == 0) executor.printGrid();

    file.close();
    return status;
}
