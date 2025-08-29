//.............MINE SWEEPER GAME................
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

// ===========================
//    SYMBOL DEFINITIONS
// ===========================
#define BOMB_SYMBOL "*"
#define FLAG_SYMBOL "F"
#define HIDDEN_SYMBOL "-"
#define EMPTY_SYMBOL " "

// ===========================
//    STRUCT DEFINITIONS
// ===========================
struct Player
{
    string name;
    string difficulty;
    int timeTaken; // in seconds
    string result; // "Win" or "Loss"

    void playGame(); // member function
};

struct GameSettings
{
    int boardSize; // Board size (depends on difficulty)
    int bombCount; // Number of bombs
    string difficultyName;
};

// ===========================
//    FUNCTION DECLARATIONS
// ===========================
void displayMenu();
// for input validation of every input,where prompt shows appropriate msg before taking input
int getValidatedInt(string prompt, int min, int max);
char **allocateBoard(int size); // according to difficulty level
void initializeBoards(char **actualBoard, char **visibleBoard, int size);
void placeBombs(char **board, int size, int bombCount, int safeRow, int safeCol);
string getDifficultyName(int difficulty);
void clearScreen();
void pauseGame();
void printBoard(char **visibleBoard, int size);
void gameLoop(char **actualBoard, char **visibleBoard, GameSettings settings, Player &player);
void revealCell(char **actualBoard, char **visibleBoard, int row, int col, int size);
void placeFlag(char **visibleBoard, int row, int col, int size);
bool checkWin(char **actualBoard, char **visibleBoard, int size, int bombCount);
void showHelp();
void showHighScores();
void updateHighScores(const Player &player);
void showPlayerStats();
void freeBoard(char **board, int size);
void setColor(int color);
string toLower(string str);

// ===========================
//      MAIN FUNCTION
// ===========================
int main()
{
    srand(time(0)); // for random bomb placement

    int choice;
    do
    {
        displayMenu();                      // Show menu
        choice = getValidatedInt("", 1, 5); // Get validated menu choice

        switch (choice)
        {
        case 1:
        {
            // Start new game
            Player p;
            p.playGame();
            break;
        }
        case 2:
            showHighScores(); // Show highscores
            break;
        case 3:
            showPlayerStats(); // Show stats
            break;
        case 4:
            showHelp(); // Show help
            break;
        case 5:
            cout << "Exiting the game. Goodbye!\n"; // Exit
            break;
        default:
            cout << "Invalid option.\n";
        }
        if (choice != 5)
        {
            pauseGame();   // Pause before clearing and showing press any key to continue ....
            clearScreen(); // Clear console
        }
    } while (choice != 5);

    return 0;
}

// ===========================
//  MAIN MENU FUNCTION
// ===========================
void displayMenu()
{
    cout << "\n===== MINESWEEPER MENU =====\n";
    cout << "1. Play Game\n";
    cout << "2. View High Scores\n";
    cout << "3. Player Stats\n";
    cout << "4. Help\n";
    cout << "5. Exit\n";
    cout << "=============================\n";
    cout << "Enter your choice: ";
}

// ===========================
// VALIDATED INTEGER INPUT
// ===========================
int getValidatedInt(string prompt, int min, int max)
{
    int value;
    while (true)
    {
        if (!prompt.empty())
            cout << prompt; // If prompt is not empty show the msg in prompt
        cin >> value;
        if (cin.fail() || value < min || value > max) // If input is not appropriate like for int type it is 123abc
        {
            cin.clear();            // Clear input buffer after input is failed
            cin.ignore(1000, '\n'); // Throw away at most 1000 characters but stop as soon as a newline is found
            cout << "Invalid input. Enter a number between " << min << " and " << max << ".\n";
        }
        else
        {
            return value; // Valid input
        }
    }
}

// ===========================
// PLAY GAME FUNCTION
// ===========================
void Player::playGame()
{
    /*
    Steps:
    1. Ask player name
    2. Select difficulty(easy,difficult,hard)
    3. Auto-assign bombs (based on difficulty)
    4. Allocate and initialize boards
    5. Pick a safe start cell and place bombs elsewhere
    6. Reveal safe cell before showing board
    7. Run game loop until player Win or Loss
    8. Record end time and calculate total time
    9. Update players score in high score list
    10. Free memory for board
    */

    Player player;
    cout << "Enter your name: ";
    cin >> player.name;

    while (true)
    {
        int difficulty = getValidatedInt("Select difficulty (1: Easy, 2: Medium, 3: Hard): ", 1, 3);

        GameSettings settings;
        if (difficulty == 1)
            settings.boardSize = 6;
        else if (difficulty == 2)
            settings.boardSize = 8;
        else
            settings.boardSize = 12;

        settings.difficultyName = getDifficultyName(difficulty);
        player.difficulty = settings.difficultyName;

        //  Auto bomb count based on difficulty
        if (difficulty == 1) // Easy = 15%
            settings.bombCount = (settings.boardSize * settings.boardSize) * 15 / 100;
        else if (difficulty == 2) // Medium = 18%
            settings.bombCount = (settings.boardSize * settings.boardSize) * 18 / 100;
        else // Hard = 20%
            settings.bombCount = (settings.boardSize * settings.boardSize) * 20 / 100;

        // Allocate memory
        char **actualBoard = allocateBoard(settings.boardSize);
        char **visibleBoard = allocateBoard(settings.boardSize);

        // Initialize both boards
        initializeBoards(actualBoard, visibleBoard, settings.boardSize);

        //  Random safe starting cell
        int startRow = rand() % settings.boardSize;
        int startCol = rand() % settings.boardSize;

        // Place bombs except safe cell
        placeBombs(actualBoard, settings.boardSize, settings.bombCount, startRow, startCol);

        // Reveal safe starting cell immediately
        revealCell(actualBoard, visibleBoard, startRow, startCol, settings.boardSize);

        // Start timer
        time_t startTime = time(0);

        // Run game loop
        gameLoop(actualBoard, visibleBoard, settings, player);

        // End timer
        time_t endTime = time(0);
        player.timeTaken = static_cast<int>(endTime - startTime); // Calculate timetaken and doing typecasting

        // Save highscore
        updateHighScores(player);

        // Free memory
        freeBoard(actualBoard, settings.boardSize);
        freeBoard(visibleBoard, settings.boardSize);

        // After one game: ask to play again or go back to main menu
        cout << "\nWhat do you want to do next?\n";
        cout << "1. Play Again\n";
        cout << "2. Go Back to Main Menu\n";
        int nextChoice = getValidatedInt("Enter choice: ", 1, 2);
        if (nextChoice == 2)
            break; // go back to main menu
        // if 1, loop continues and difficulty will be asked again
    }
}

// ===========================
// BOARD INITIALIZATION
// ===========================
char **allocateBoard(int size)
{
    char **board = new char *[size];
    for (int i = 0; i < size; ++i)
        board[i] = new char[size];
    return board;
}

void initializeBoards(char **actualBoard, char **visibleBoard, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            actualBoard[i][j] = EMPTY_SYMBOL[0];
            visibleBoard[i][j] = HIDDEN_SYMBOL[0];
        }
    }
}

// ===========================
// PLACE BOMBS
// ===========================
void placeBombs(char **board, int size, int bombCount, int safeRow, int safeCol)
{
    int placed = 0;
    while (placed < bombCount)
    {
        int row = rand() % size;
        int col = rand() % size;

        // Skip safe cell
        if ((row == safeRow && col == safeCol) || board[row][col] == BOMB_SYMBOL[0])
            continue;

        board[row][col] = BOMB_SYMBOL[0];
        placed++;
    }
}

// ===========================
// DIFFICULTY NAME
// ===========================
string getDifficultyName(int difficulty)
{
    if (difficulty == 1)
        return "Easy";
    if (difficulty == 2)
        return "Medium";
    return "Hard";
}

// ===========================
// SCREEN CONTROL
// ===========================
void clearScreen()
{
    system("cls"); // clear console
}

void pauseGame()
{
    system("pause");
}

// ===========================
// PRINT BOARD
// ===========================
void printBoard(char **visibleBoard, int size)
{
    /*  Sample Visible Board Output

      0  1  2  3  4
   0 | -  -  -  -  -
   1 | -  F  -  -  -
   2 | -  -  2  -  -
   3 | -  -  -  -  -
   4 | -  -  -  -  -
   */
    cout << "\n   ";
    for (int col = 0; col < size; ++col)
        cout << (col < 10 ? " " : "") << col << " ";
    cout << "\n";

    for (int row = 0; row < size; ++row)
    {
        cout << (row < 10 ? " " : "") << row << " |";
        for (int col = 0; col < size; ++col)
        {
            char cell = visibleBoard[row][col];
            if (cell == BOMB_SYMBOL[0])
                setColor(12); // red for bombs
            else if (cell == FLAG_SYMBOL[0])
                setColor(14); // yellow for flags
            else if (cell >= '1' && cell <= '8')
                setColor(9); // blue for numbers(1-8)
            else if (cell == EMPTY_SYMBOL[0])
                setColor(10); // green for empty revealed cells
            else
                setColor(7); // gray for othr cases

            cout << cell << "  ";
        }
        setColor(7); // reset color back to default after finishing the row
        cout << "\n";
    }
}

// ===========================
// GAME LOOP
// ===========================
void gameLoop(char **actualBoard, char **visibleBoard, GameSettings settings, Player &player)
{
    int row, col;
    char action;
    bool gameOver = false;
    /*
  Game runs until player wins (all safe cells revealed) or loses (hits bomb).

  Loop steps:
  1. Show board.
  2. Get row & column (skip if already revealed/flagged).
  3. Get action (reveal or flag).
  4. If flag : place/remove flag.
  5. If reveal:
     - Bomb : game over (loss).
     - Safe : reveal cells, then check win.
  6. If win/loss : exit loop.
  */

    while (!gameOver)
    {
        printBoard(visibleBoard, settings.boardSize); // show current board state

        // Get row and col input
        row = getValidatedInt("\nEnter row: ", 0, settings.boardSize - 1);
        col = getValidatedInt("Enter column: ", 0, settings.boardSize - 1);

        // error handling for row and col
        if (visibleBoard[row][col] != HIDDEN_SYMBOL[0] && visibleBoard[row][col] != FLAG_SYMBOL[0])
        {
            cout << "This cell is already revealed. Try again.\n";
            continue;
        }

        cout << "Enter action (r to reveal, f to flag): ";
        cin >> action;
        action = tolower(action);

        if (action == 'r')
        {
            if (actualBoard[row][col] == BOMB_SYMBOL[0])
            {
                cout << "\nBOOM! You hit a bomb!\n";
                printBoard(actualBoard, settings.boardSize); // reveal full board
                gameOver = true;
                player.result = "Loss";
            }
            else
            {
                revealCell(actualBoard, visibleBoard, row, col, settings.boardSize);
                if (checkWin(actualBoard, visibleBoard, settings.boardSize, settings.bombCount))
                {
                    cout << "\nCongratulations! You cleared the minefield!\n";
                    printBoard(actualBoard, settings.boardSize); // reveal full board
                    gameOver = true;
                    player.result = "Win";
                }
            }
        }
        else if (action == 'f')
        {
            placeFlag(visibleBoard, row, col, settings.boardSize);
        }
        else
        {
            cout << "Invalid action!\n";
        }
    }
}

// ===========================
// REVEAL CELL
// ===========================
void revealCell(char **actualBoard, char **visibleBoard, int row, int col, int size)
{
    if (row < 0 || col < 0 || row >= size || col >= size || visibleBoard[row][col] != HIDDEN_SYMBOL[0])
        return;

    int bombCount = 0;

    for (int rowChange = -1; rowChange <= 1; ++rowChange)
    {
        for (int colChange = -1; colChange <= 1; ++colChange)
        {
            int neighborRow = row + rowChange;
            int neighborCol = col + colChange;
            if (neighborRow >= 0 && neighborCol >= 0 && neighborRow < size && neighborCol < size && actualBoard[neighborRow][neighborCol] == BOMB_SYMBOL[0])
                bombCount++;
        }
    }

    visibleBoard[row][col] = (bombCount == 0) ? EMPTY_SYMBOL[0] : '0' + bombCount;

    if (bombCount == 0)
    {
        for (int rowChange = -1; rowChange <= 1; ++rowChange)
            for (int colChange = -1; colChange <= 1; ++colChange)
                revealCell(actualBoard, visibleBoard, row + rowChange, col + colChange, size);
    }
}

// ===========================
// FLAG CELL
// ===========================
void placeFlag(char **visibleBoard, int row, int col, int size)
{
    if (visibleBoard[row][col] == HIDDEN_SYMBOL[0])
        visibleBoard[row][col] = FLAG_SYMBOL[0];
    else if (visibleBoard[row][col] == FLAG_SYMBOL[0])
        visibleBoard[row][col] = HIDDEN_SYMBOL[0];
}

// ===========================
// CHECK WIN
// ===========================
bool checkWin(char **actualBoard, char **visibleBoard, int size, int bombCount)
{
    int revealed = 0;
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            if (visibleBoard[i][j] != HIDDEN_SYMBOL[0] && visibleBoard[i][j] != FLAG_SYMBOL[0])
                revealed++;
    return revealed == (size * size - bombCount);
}

// ===========================
// HELP
// ===========================
void showHelp()
{
    cout << "\n===== HOW TO PLAY =====\n";
    cout << "- Goal: Reveal all non-bomb cells.\n";
    cout << "- 'r' to reveal a cell.\n";
    cout << "- 'f' to flag/unflag a cell.\n";
    cout << "- Avoid bombs!\n";
}

// ===========================
// HIGHSCORES
// ===========================
void updateHighScores(const Player &player)
{
    ofstream file("highscore.txt", ios::app);
    if (file.is_open())
    {
        file << player.name << " " << player.difficulty << " " << player.timeTaken << " " << player.result << "\n";
        file.close();
    }
}

string toLower(string str)
{
    for (int i = 0; i < str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
}

void showHighScores()
{
    string difficulty;
    cout << "Enter difficulty to filter (Easy/Medium/Hard): ";
    cin >> difficulty;
    difficulty = toLower(difficulty);

    ifstream file("highscore.txt");
    if (!file.is_open())
    {
        cout << "No high scores available.\n";
        return;
    }

    Player *players = new Player[100];
    int count = 0;
    while (file >> players[count].name >> players[count].difficulty >> players[count].timeTaken >> players[count].result)
        if (toLower(players[count].difficulty) == difficulty && players[count].result == "Win")
            count++;
    file.close();

    if (count == 0)
    {
        cout << "No winners for " << difficulty << ".\n";
        delete[] players;
        return;
    }

    int topPlayers = getValidatedInt("Enter number of top players to display (1-" + to_string(count) + "): ", 1, count);

    for (int i = 0; i < count - 1; ++i)
        for (int j = i + 1; j < count; ++j)
            if (players[j].timeTaken < players[i].timeTaken)
                swap(players[i], players[j]);

    cout << "\nTop " << topPlayers << " players for " << difficulty << ":\n";
    for (int i = 0; i < topPlayers; ++i)
        cout << players[i].name << " - Time: " << players[i].timeTaken << "s\n";
    delete[] players;
}
// ===========================
// PLAYER STATS
// ===========================
void showPlayerStats()
{
    ifstream file("highscore.txt");
    if (!file.is_open())
    {
        cout << "No player stats available.\n";
        return;
    }

    string name, difficulty, result;
    int timeTaken;
    int totalGames = 0, wins = 0, losses = 0, totalTime = 0;

    cout << "Enter player name to view stats: ";
    string searchName;
    cin >> searchName;
    searchName = toLower(searchName);

    while (file >> name >> difficulty >> timeTaken >> result)
    {
        if (toLower(name) == searchName)
        {
            totalGames++;
            totalTime += timeTaken;
            if (result == "Win")
                wins++;
            else
                losses++;
        }
    }
    file.close();

    if (totalGames == 0)
    {
        cout << "No records found for player " << searchName << ".\n";
        return;
    }

    cout << "\nStats for player " << searchName << ":\n";
    cout << "Total Games Played: " << totalGames << "\n";
    cout << "Wins: " << wins << "\n";
    cout << "Losses: " << losses << "\n";
    cout << "Average Time: " << totalTime / totalGames << "s\n";
}

// ===========================
// FREE BOARD MEMORY
// ===========================
void freeBoard(char **board, int size)
{
    for (int i = 0; i < size; ++i)
        delete[] board[i];
    delete[] board;
}

// ===========================
// SET CONSOLE COLOR
// ===========================
void setColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
