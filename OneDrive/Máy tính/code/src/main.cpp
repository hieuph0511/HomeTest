#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

// Interface IGridManager to manage Grid
class IGridManager { 
public: 
    virtual bool checkGrid() = 0;
    virtual int setDimension(int n) = 0; 
    virtual int moveTo(int x, int y) = 0; 
    virtual int lineTo(int x, int y) = 0; 
    virtual void printGrid() = 0; 
    virtual ~IGridManager() = default;
};

/*--------- Manage Grid-------------//
*   bool checkGrid(): Check size Grid.
*   int setDimension(int n): Define the grid size n.
*   int moveTo(int x, int y): Robot moves to (x, y) without drawing.
*   int lineTo(int x, int y): Move from current position to (x, y) with drawing on the grid.
*   void printGrid(): print grid.
*   void drawLine(): draw line moved.
------------------------------------*/
class Grid : public IGridManager
{
public:
    // Constructor
    Grid() : dimension(0), currentX(0), currentY(0) {}

    bool checkGrid() override
    {
        return dimension > 0;
    }

    // Define the grid size n
    int setDimension(int n) override
    {
        if(n <= 0)
        {
            cerr << "Incorrect format for size of grid DIMENSION command." << endl;
            return -1;
        }
        
        dimension = n;
        grid.resize(n, vector<char>(n, '.'));
        return 0;
    }

    // Robot moves to (x, y) without drawing.
    int moveTo(int x, int y) override
    {
        if(!checkGrid())
        {
            cerr << "Grid size not set properly." << endl;
            return -1;
        }

        if (x >= 0 && x < dimension && y >= 0 && y < dimension)
        {
            currentX = x;
            currentY = y;
            grid[currentY][currentX] = 'x';
            return 0;
        }
        
        cerr << (x > dimension || y > dimension ? "Err: Position MOVE_TO > size grid." : "Err: Position MOVE_TO < 0.") << endl;
        return -1;
    }

    // Move from current position to (x, y) with drawing on the grid.
    int lineTo(int x, int y) override
    {
        if(!checkGrid())
        {
            cerr << "Grid size not set properly." << endl;
            return -1;
        }

        if (x >= 0 && x < dimension && y >= 0 && y < dimension)
        {
            drawLine(currentX, currentY, x, y);
            currentX = x;
            currentY = y;
            grid[currentY][currentX] = 'x';
            return 0;
        }
        
        cerr << (x > dimension || y > dimension ? "Err: Position LINE_TO > size grid." : "Err: Position LINE_TO < 0.") << endl;
        return -1;
    }

    // Print grid
    void printGrid() override
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
                cout << (grid[i][j] == 'x' ? "| x " : "|   ");
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

// Interface ICommandExecutor to manage Command
class ICommandExecutor 
{
public: 
    virtual int executeCommand(const string& command) = 0; 
    virtual void printGrid() = 0; 
    virtual ~ICommandExecutor() = default; 
}; 

class CommandExecutor : public ICommandExecutor 
{
public: 
    CommandExecutor(IGridManager* gridManager) : gridManager(gridManager) {}

    int executeCommand(const string& command) override
    {
        istringstream iss(command);
        string cmd;
        iss >> cmd;

        if (cmd == "DIMENSION")
        {
            return executeDimension(iss);
        } 
        else if (cmd == "MOVE_TO")
        {
            return executeMoveTo(iss);
        }
        else if (cmd == "LINE_TO")
        {
            return executeLineTo(iss);
        }
        else
        {
            cerr << "Undefined Command: " << cmd << endl;
            return -1;
        }
    }

    void printGrid() override 
    { 
        gridManager->printGrid(); 
    } 

private: 
    IGridManager* gridManager;

    int executeDimension(istringstream& iss) 
    { 
        int n;
        iss >> n; 
        int status = gridManager->setDimension(n); 
        if (status < 0) 
        { 
            cerr << "Incorrect format for DIMENSION command." << endl; 
        } 
        return status; 
    } 

    int executeMoveTo(istringstream& iss) 
    { 
        int x, y; 
        char comma; 
        iss >> x >> comma >> y;
        if (comma == ',') 
        { 
            int status = gridManager->moveTo(x, y); 
            if (status < 0) 
            { 
                cerr << "Incorrect format for MOVE_TO command." << endl; 
            } 
            return status; 
        }
        else 
        { 
            cerr << "Incorrect format for MOVE_TO command." << endl; 
            return -1; 
        } 
    } 

    int executeLineTo(istringstream& iss) 
    { 
        int x, y; 
        char comma;
        iss >> x >> comma >> y; 
        if (comma == ',') 
        { 
            int status = gridManager->lineTo(x, y);
            if (status < 0) 
            { 
                cerr << "Incorrect format for LINE_TO command." << endl; 
            } 
            return status; 
        } 
        else 
        { 
            cerr << "Incorrect format for LINE_TO command." << endl;
            return -1; 
        } 
    } 
};

int main() {
    int status = 0;
    Grid grid;
    CommandExecutor executor(&grid);

    // Open file command
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
