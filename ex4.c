/******************
Name: Aviv Sayer
ID: 326552304
Assignment: ex4
*******************/

#include <stdio.h>
#include <string.h> // for task 5

// defining bool for boolean-like usages
#define bool                int
#define true                1
#define false               0

// define constants for all of the tasks
#define HORIZONTAL          'H'
#define PYRAMID_SIZE        5
#define EMPTY               (-1)
#define MAX_BOARD_SIZE      20
#define MAX_WORD_LENGTH     15
#define MAX_SLOTS_NUMBER    100
#define MAX_GRID_SIZE       30

// define a word slot for task 5
typedef struct Slot {
    int row, column, length;
    char direction;
} Slot;

// pre-declare all of the methods
int robotPaths(int i, int j);

float humanPyramid(int i, int j, float weights[PYRAMID_SIZE][PYRAMID_SIZE]);

char closingOf(char brace);

bool isClosing(char brace);

bool isOpening(char brace);

bool parenthesisValidator(char last);

bool checkMove(int areaCounters[MAX_BOARD_SIZE],
               int columnCounters[MAX_BOARD_SIZE],
               int column, int area);

bool queensColumn(bool board[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areas[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areaCounters[MAX_BOARD_SIZE],
                  int columnCounters[MAX_BOARD_SIZE],
                  int position, int j, int N);

bool queensBattle(bool board[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areas[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areaCounters[MAX_BOARD_SIZE],
                  int columnCounters[MAX_BOARD_SIZE],
                  int position, int N);

void initGrid(char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], int N, int i);

void clearLength(int GRID_SIZE, char grid[MAX_GRID_SIZE * MAX_GRID_SIZE],
                 int row, int column, int length, char direction, int i);

bool placeWord(int GRID_SIZE, char grid[MAX_GRID_SIZE * MAX_GRID_SIZE],
               int row, int column, int length, char direction, char word[MAX_WORD_LENGTH + 1], int i);

bool crosswordWordFinder(int GRID_SIZE, int SLOTS, int WORDS,
                         char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], char orgGrid[MAX_GRID_SIZE * MAX_GRID_SIZE],
                         Slot slots[MAX_SLOTS_NUMBER], int currentSlot,
                         char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1],
                         int wordsAmounts[MAX_WORD_LENGTH], bool usedWords[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER], int word);

bool crosswordGenerator(int GRID_SIZE, int SLOTS, int WORDS,
                        char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], Slot slots[MAX_SLOTS_NUMBER], int currentSlot,
                        char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1],
                        int wordsAmounts[MAX_WORD_LENGTH], bool usedWords[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER]);

int main() {
    int option;
    while (true) {
        printf("Choose an option:\n"
               "1. Robot Paths\n"
               "2. The Human Pyramid\n"
               "3. Parenthesis Validation\n"
               "4. Queens Battle\n"
               "5. Crossword Generator\n"
               "6. Exit\n");

        if (scanf(" %d", &option)) { // get the option chosen by the user
            switch (option) {
                case 1: { // task 1: robot paths
                    int i, j; // i = row, j= column

                    // get the input
                    printf("Please enter the coordinates of the robot (column, row):\n");
                    scanf(" %d %d", &i, &j);

                    // if i or j < 0 print 0. otherwise print the no. of paths from (i,j) to (0,0)
                    printf("The total number of paths the robot can take to reach home is: %d\n",
                           i < 0 || j < 0 ? 0 : robotPaths(i, j));

                    break;
                }
                case 2: { // task 2: weight calculation
                    float weights[PYRAMID_SIZE][PYRAMID_SIZE]; // the array which will store the weights
                    bool negativeValue = false;

                    // get all the weights into the array
                    printf("Please enter the weights of the cheerleaders:\n");
                    for (int i = 0; i < PYRAMID_SIZE && !negativeValue; ++i) {
                        for (int j = 0; j <= i && !negativeValue; ++j) {
                            scanf(" %f", &weights[i][j]);
                            if (weights[i][j] < 0) {
                                negativeValue = true;
                            }
                        }
                    }

                    // if got a negative value, get out.
                    if (negativeValue) {
                        printf("Negative weights are not supported.\n");
                    } else { // else, foreach person print his weight
                        printf(" The total weight on each cheerleader is:\n");
                        for (int i = 0; i < PYRAMID_SIZE; ++i) {
                            for (int j = 0; j <= i; ++j)
                                printf("%.2f ", humanPyramid(i, j, weights));
                            printf("\n");
                        }
                    }

                    break;
                }
                case 3:
                    printf("Please enter a term for validation:\n");
                    scanf(" "); // sub-clear the buffer. full clear is not needed.
                    // the method is getting the input. print the response accordingly
                    printf(parenthesisValidator('\0')
                           ? "The parentheses are balanced correctly.\n"
                           : "The parentheses are not balanced correctly.\n");
                    break;
                case 4: { // task 4: a variation of the N-queens problem
                    int N; // size of the board
                    printf("Please enter the board dimensions:\n");
                    scanf(" %d", &N); // ge the size of the board
                    printf("Please enter a %d*%d puzzle board:\n", N, N);
                    // stores the queens. true = queen, false = empty
                    bool board[MAX_BOARD_SIZE * MAX_BOARD_SIZE] = {};
                    int areas[MAX_BOARD_SIZE * MAX_BOARD_SIZE] = {}; // stores the areas
                    // stores the areas' names.
                    // the area in index i in this array will be expressed as i in {areas[][]}
                    char areasNames[MAX_BOARD_SIZE] = {};
                    int lastArea = -1; // last area which was putted into {areasNames[]}
                    // for each area with index i, areaCounter[i] is the no. of queens in it.
                    int areaCounters[MAX_BOARD_SIZE] = {};
                    // for each column with index i, columnCounter[i] is the no. of queens in it.
                    int columnCounters[MAX_BOARD_SIZE] = {};

                    // input areas into {areas[][]}
                    for (int i = 0; i < N; ++i) {
                        for (int j = 0; j < N; ++j) {
                            char area;
                            scanf(" %c", &area);

                            // serialize the area
                            int k = 0;
                            // if area was serialized, don't serialize
                            // (and put the existing serialization into {areas[][]})
                            for (; k <= lastArea; ++k) {
                                if (areasNames[k] == area) {
                                    areas[N * i + j] = k;
                                    break;
                                }
                            }
                            // else, serialize the area
                            // (and put the new serialization into {areas[][]})
                            if (k == lastArea + 1) {
                                areasNames[++lastArea] = area;
                                areas[N * i + j] = lastArea;
                            }
                        }
                    }

                    // try to generate a response
                    // if N == 2 or 3 it can never be solved.
                    if (N != 2 && N != 3 &&
                        queensBattle(board, areas, areaCounters, columnCounters, -N, N)) {
                        // if can be solved, print the solution
                        printf("Solution:\n");
                        for (int i = 0; i < N; ++i) {
                            for (int j = 0; j < N; ++j)
                                printf(board[N * i + j] ? "X " : "* ");
                            printf("\n");
                        }
                    } else {
                        // else print accordingly
                        printf("This puzzle cannot be solved.\n");
                    }
                    break;
                }
                case 5: { // task 5: crossword solving
                    int GRID_SIZE, SLOTS, WORDS; // size of the grid, no. of slots, no. of words
                    char grid[MAX_GRID_SIZE * MAX_GRID_SIZE] = {}; // the grid itself
                    Slot slots[MAX_SLOTS_NUMBER] = {}; // all the slots for the words to be filled in
                    // the array with all the possible words to fill in the slots.
                    // words[i] is the array of all the words with length i
                    char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1] = {};
                    int wordsAmounts[MAX_WORD_LENGTH] = {}; // wordsAmounts[i] is the no. of words in words[i]
                    // currentWords[i] is the first word that was not filled in a slot in words[i]
                    int usedWords[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER] = {};
                    // get the input
                    printf("Please enter the dimensions of the crossword grid:\n");
                    scanf(" %d", &GRID_SIZE);
                    initGrid(grid, GRID_SIZE, 0);
                    printf("Please enter the number of slots in the crossword:\n");
                    scanf(" %d", &SLOTS);
                    // get the info about the slots for the words
                    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
                    for (int i = 0; i < SLOTS; ++i) {
                        int row, column, length;
                        char direction;
                        scanf(" %d %d %d %c", &row, &column, &length, &direction);
                        Slot slot = {row, column, length, direction};
                        slots[i] = slot;
                        // set all chars in the slot region to '\0'
                        clearLength(GRID_SIZE, grid, row, column, length, direction, 0);
                    }
                    // input the no. of the possible words to choose between to put in the slots
                    printf("Please enter the number of words in the dictionary:\n");
                    scanf(" %d", &WORDS);
                    while (WORDS < SLOTS) { // validate that the no. of words in not less that the number of slots
                        printf("The dictionary must contain at least %d words. "
                               "Please enter a valid dictionary size:\n", SLOTS);
                        scanf(" %d", &WORDS);
                    }
                    // scan all the words
                    printf("Please enter the words for the dictionary:\n");
                    for (int i = 0; i < WORDS; ++i) {
                        char word[MAX_WORD_LENGTH + 1];
                        scanf(" %s", word);
                        int len = (int) strlen(word) - 1;
                        // put the received word in the array of words
                        strcpy(words[len][wordsAmounts[len]++], word);
                    }

                    // if the crossword can be solved, print the solution.
                    if (crosswordGenerator(GRID_SIZE, SLOTS, WORDS,
                                           grid, slots, 0,
                                           words, wordsAmounts, usedWords)) {
                        for (int i = 0; i < GRID_SIZE; ++i) {
                            for (int j = 0; j < GRID_SIZE; ++j)
                                printf("| %c ", grid[GRID_SIZE * i + j]);
                            printf("|\n");
                        }
                    } else { // else, inform the user.
                        printf("This crossword cannot be solved.\n");
                    }
                    break;
                }
                case 6: // get out of the program
                    printf("Goodbye!\n");
                    return 0;
                default: // invalid task number.
                    printf(" Please choose a task number from the list.\n");
                    break;
            }
        } else {
            scanf("%*[^\n]");
        }
    }
}

int robotPaths(int i, int j) {
    if (i == 0 || j == 0) // if i or j are 0 the number of ways to get to (0,0) is 1
        return 1;
    if (i == j) // if i equals to j than robotPaths(i - 1, j) equals to robotPaths(i, j - 1) (optimisation)
        return 2 * robotPaths(i - 1, j);
    // the number of ways to get to (i,j) is the no. of ways to get to (i, j-1)
    // + the no. of ways to get to (i-1,j) because you can go down ((i-1,j)) or left ((i,j-1))
    return robotPaths(i - 1, j) + robotPaths(i, j - 1);
}

float humanPyramid(int i, int j, float weights[PYRAMID_SIZE][PYRAMID_SIZE]) {
    if (j > i || j < 0) // if out of the pyramid, return 0 (no weight)
        return 0;
    if (i == 0) // the top of the pyramid has no one above
        return weights[0][0];
    return .5f * humanPyramid(i - 1, j, weights) + .5f * humanPyramid(i - 1, j - 1, weights)
           + weights[i][j]; // calculate the weight on the (i,j) person with the provided equation
}

char closingOf(char brace) { // gives the closing brace of the given brace
    switch (brace) {
        case '(':
            return ')';
        case '[':
            return ']';
        case '{':
            return '}';
        case '<':
            return '>';
        default:
            return '\0';
    }
}

bool isClosing(char brace) { // returns if the given char is a closing brace
    return brace == ')' || brace == ']' || brace == '}' || brace == '>';
}

bool isOpening(char brace) {// returns if the given char is an opening brace
    return brace == '(' || brace == '[' || brace == '{' || brace == '<';
}

bool parenthesisValidator(char last) {
    char c;
    // scan the next char
    scanf("%c", &c);
    // if the char is enter, the input is finished
    if (c == '\n') {
        return last == '\0'; // if last opened brace is the null char, there is no more brace to close.
    } else if (c == closingOf(last)) { // else, if the char closes the last opened brace
        return EMPTY; // return EMPTY - meaning that the input is not finished, but the brace is closed
    } else if (isClosing(c)) {
        // if the char is a closing brace, but dont closes the last brace, the char is invalid.
        scanf("%*[^\n]"); // empty the buffer
        return false;
    } else if (isOpening(c)) { // else, if the char opens a new brace, open a new query for it.
        bool out = parenthesisValidator(c);
        if (out != EMPTY) // if out equals EMPTY, we need to continue scanning
            return out;
        // else, the function will be called again
    }
    return parenthesisValidator(last); // open the next query
}

bool checkMove(int areaCounters[MAX_BOARD_SIZE],
               int columnCounters[MAX_BOARD_SIZE],
               int column, int area) { // checks if the last move is a valid move
    return columnCounters[column] <= 1 && areaCounters[area] <= 1;
}

bool queensColumn(bool board[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areas[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areaCounters[MAX_BOARD_SIZE],
                  int columnCounters[MAX_BOARD_SIZE],
                  int position, int j, int N) { // put queens in each colum and check if the board is valid
    int row = position / N;
    int column = position % N;
    if (j >= N)
        return false; // if we got to the last column with no success
    if (column - 1 > j || j > column + 1 || position == 0) {
        // put the queen
        areaCounters[areas[N * row + j]]++;
        columnCounters[j]++;
        board[N * row + j] = true;
        // continue the recursion
        if (queensBattle(board, areas, areaCounters, columnCounters, N * row + j, N))
            return true;
        // un-put the queen
        areaCounters[areas[N * row + j]]--;
        columnCounters[j]--;
        board[N * row + j] = false;
    }
    // continue to the next column
    return queensColumn(board, areas, areaCounters, columnCounters, position, j + 1, N);
}

bool queensBattle(bool board[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areas[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areaCounters[MAX_BOARD_SIZE],
                  int columnCounters[MAX_BOARD_SIZE],
                  int position, int N) { // put queens in each row and check if the board is valid
    int row = position / N;
    int column = position % N;
    int area = areas[position];
    if (position >= 0 && !checkMove(areaCounters, columnCounters, column, area)) // if last move was valid
        return false;
    if (++row == N)
        return true; // if we got to the last row with no failure
    // for each row, go over the columns
    return queensColumn(board, areas, areaCounters, columnCounters, N * row + column, 0, N);
}

void initGrid(char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], int N, int i) {
    // set every char in the grid to '#'
    if (i == N * N)
        return;
    grid[i] = '#';
    initGrid(grid, N, i + 1);
}

void clearLength(int GRID_SIZE, char grid[MAX_GRID_SIZE * MAX_GRID_SIZE],
                 int row, int column, int length, char direction, int i) {
    // clear the given slot in {grid[]}
    if (length == 0)
        return;
    grid[GRID_SIZE * row + column] = '\0';

    if (direction == HORIZONTAL)
        clearLength(GRID_SIZE, grid, row, column + 1, length - 1, direction, i + 1);
    else clearLength(GRID_SIZE, grid, row + 1, column, length - 1, direction, i + 1);
}

bool placeWord(int GRID_SIZE, char grid[MAX_GRID_SIZE * MAX_GRID_SIZE],
               int row, int column, int length, char direction, char word[MAX_WORD_LENGTH + 1], int i) {
    // place a word in grid
    if (length == 0)
        return true;
    if (grid[GRID_SIZE * row + column] != '\0' && grid[GRID_SIZE * row + column] != word[i])
        return false;
    grid[GRID_SIZE * row + column] = word[i];

    if (direction == HORIZONTAL)
        return placeWord(GRID_SIZE, grid, row, column + 1, length - 1, direction, word, i + 1);
    return placeWord(GRID_SIZE, grid, row + 1, column, length - 1, direction, word, i + 1);
}

bool crosswordGenerator(int GRID_SIZE, int SLOTS, int WORDS,
                        char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], Slot slots[MAX_SLOTS_NUMBER], int currentSlot,
                        char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1],
                        int wordsAmounts[MAX_WORD_LENGTH], bool usedWords[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER]) {
    // generate the solution to the crossword if exists
    if (currentSlot == SLOTS)
        return true;
    char orgGrid[MAX_GRID_SIZE * MAX_GRID_SIZE];
    strcpy(orgGrid, grid);
    return crosswordWordFinder(GRID_SIZE, SLOTS, WORDS, grid, orgGrid,
                               slots, currentSlot, words, wordsAmounts, usedWords, 0);
}

bool crosswordWordFinder(int GRID_SIZE, int SLOTS, int WORDS,
                         char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], char orgGrid[MAX_GRID_SIZE * MAX_GRID_SIZE],
                         Slot slots[MAX_SLOTS_NUMBER], int currentSlot,
                         char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1],
                         int wordsAmounts[MAX_WORD_LENGTH], bool usedWords[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER],
                         int word) {
    // finds a word that fits the given slot
    if (word >= wordsAmounts[slots[currentSlot].length - 1])
        return false;
    if (!usedWords[slots[currentSlot].length - 1][word] &&
        placeWord(GRID_SIZE, grid, slots[currentSlot].row, slots[currentSlot].column,
                  slots[currentSlot].length, slots[currentSlot].direction,
                  words[slots[currentSlot].length - 1][word], 0)) { // if we can place a word there
        usedWords[slots[currentSlot].length - 1][word] = true;
        if (crosswordGenerator(GRID_SIZE, SLOTS, WORDS,
                               grid, slots, currentSlot + 1,
                               words, wordsAmounts, usedWords))
            return true; // continue the recursion
        usedWords[slots[currentSlot].length - 1][word] = false;
    }
    strcpy(grid, orgGrid); // un-place the word (restore the original grid)
    // try the next word
    return crosswordWordFinder(GRID_SIZE, SLOTS, WORDS, grid, orgGrid,
                               slots, currentSlot, words, wordsAmounts, usedWords, word + 1);
}