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
int flagsLeft;

// ===========================
//    STRUCT DEFINITIONS
// ===========================
struct Player
{
    string name;
    string difficulty;
    int timeTaken;
    string result;

    void playGame();
};

struct GameSettings
{
    int boardSize;
    int bombCount;
    string difficultyName;
};

// ===========================
//    FUNCTION DECLARATIONS
// ===========================
void displayMenu();
int getValidatedInt(string prompt, int min, int max);
char **allocateBoard(int size);
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
    srand(time(0));

    int choice;
    do
    {
        displayMenu();
        choice = getValidatedInt("", 1, 5);

        switch (choice)
        {
        case 1:
        {
            Player p;
            p.playGame();
            break;
        }
        case 2:
            showHighScores();
            break;
        case 3:
            showPlayerStats();
            break;
        case 4:
            showHelp();
            break;
        case 5:
            cout << "Exiting the game. Goodbye!\n";
            break;
        default:
            cout << "Invalid option.\n";
        }
        if (choice != 5)
        {
            pauseGame();
            clearScreen();
        }
    } while (choice != 5);

    return 0;
}

// ===========================
//  MAIN MENU FUNCTION
// ===========================
void displayMenu()
{
    setColor(11);
    cout << "\n===== MINESWEEPER MENU =====\n";
    setColor(14);
    cout << "1. Play Game\n";
    setColor(10);
    cout << "2. View High Scores\n";
    setColor(13);
    cout << "3. Player Stats\n";
    setColor(9);
    cout << "4. Help\n";
    setColor(12);
    cout << "5. Exit\n";
    setColor(11);
    cout << "=============================\n";
    setColor(7);
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
            cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter a number between " << min << " and " << max << ".\n";
        }
        else
        {
            return value;
        }
    }
}

// ===========================
// PLAY GAME FUNCTION
// ===========================
void Player::playGame()
{
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

        if (difficulty == 1)
            settings.bombCount = (settings.boardSize * settings.boardSize) * 15 / 100;
        else if (difficulty == 2)
            settings.bombCount = (settings.boardSize * settings.boardSize) * 18 / 100;
        else
            settings.bombCount = (settings.boardSize * settings.boardSize) * 20 / 100;
        flagsLeft = settings.bombCount;

        if (settings.difficultyName == "Easy")
            setColor(10);
        else if (settings.difficultyName == "Medium")
            setColor(14);
        else
            setColor(12);

        cout << "\nStarting Game: " << settings.difficultyName << " Mode\n";
        setColor(7);

        char **actualBoard = allocateBoard(settings.boardSize);
        char **visibleBoard = allocateBoard(settings.boardSize);

        initializeBoards(actualBoard, visibleBoard, settings.boardSize);

        int startRow = rand() % settings.boardSize;
        int startCol = rand() % settings.boardSize;

        placeBombs(actualBoard, settings.boardSize, settings.bombCount, startRow, startCol);

        revealCell(actualBoard, visibleBoard, startRow, startCol, settings.boardSize);

        time_t startTime = time(0);

        gameLoop(actualBoard, visibleBoard, settings, player);

        time_t endTime = time(0);
        player.timeTaken = static_cast<int>(endTime - startTime);

        updateHighScores(player);

        freeBoard(actualBoard, settings.boardSize);
        freeBoard(visibleBoard, settings.boardSize);

        cout << "\nWhat do you want to do next?\n";
        setColor(10);
        cout << "1. Play Again\n";
        setColor(14);
        cout << "2. Go Back to Main Menu\n";
        setColor(7);
        int nextChoice = getValidatedInt("Enter choice: ", 1, 2);

        if (nextChoice == 2)
            break;
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
    system("cls");
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
                setColor(12);
            else if (cell == FLAG_SYMBOL[0])
                setColor(14);
            else if (cell >= '1' && cell <= '8')
                setColor(9);
            else if (cell == EMPTY_SYMBOL[0])
                setColor(10);
            else
                setColor(7);

            cout << cell << "  ";
        }
        setColor(7);
        cout << "\n";
    }
}

// ===========================
// GAME LOOP
// ===========================
time_t startTime = time(0);

void gameLoop(char **actualBoard, char **visibleBoard, GameSettings settings, Player &player)
{
    time_t startTime = time(0);
    int row, col;
    char action;
    bool gameOver = false;

    while (!gameOver)
    {
        clearScreen();
        time_t currentTime = time(0);
        int elapsed = static_cast<int>(currentTime - startTime);

        setColor(11);
        cout << "Time: " << elapsed << "s";
        cout << "   Flags Left: " << flagsLeft << "/" << settings.bombCount << "\n";
        setColor(7);
        printBoard(visibleBoard, settings.boardSize);

        row = getValidatedInt("\nEnter row: ", 0, settings.boardSize - 1);
        col = getValidatedInt("Enter column: ", 0, settings.boardSize - 1);

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
                setColor(12);
                cout << "\nBOOM! You hit a bomb!\n";
                setColor(7);
                printBoard(actualBoard, settings.boardSize);
                gameOver = true;
                player.result = "Loss";
            }

            else
            {
                revealCell(actualBoard, visibleBoard, row, col, settings.boardSize);
                if (checkWin(actualBoard, visibleBoard, settings.boardSize, settings.bombCount))
                {
                    cout << "\nCongratulations! You cleared the minefield!\n";
                    printBoard(actualBoard, settings.boardSize);
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
    player.timeTaken = static_cast<int>(time(0) - startTime);
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
    if (visibleBoard[row][col] == HIDDEN_SYMBOL[0] && flagsLeft > 0)
    {
        visibleBoard[row][col] = FLAG_SYMBOL[0];
        flagsLeft--;
    }
    else if (visibleBoard[row][col] == FLAG_SYMBOL[0])
    {
        visibleBoard[row][col] = HIDDEN_SYMBOL[0];
        flagsLeft++;
    }
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
    setColor(11);
    cout << "\nTop " << topPlayers << " players for " << difficulty << ":\n";
    setColor(7);

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
    setColor(14);
    cout << "\nStats for player " << searchName << ":\n";
    setColor(7);

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
