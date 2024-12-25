/******************
Name: Aviv Sayer
ID: 326552304
Assignment: ex4
*******************/

#include <stdio.h>
#include <string.h>

#define bool                int
#define true                1
#define false               0

#define HORIZONTAL          'H'
#define PYRAMID_SIZE        5
#define EMPTY               (-1)
#define MAX_BOARD_SIZE      20
#define MAX_WORD_LENGTH     15
#define MAX_SLOTS_NUMBER    100
#define MAX_GRID_SIZE       30

typedef struct Slot {
    int row, column, length;
    char direction;
} Slot;

int robotPaths(int i, int j);

float humanPyramid(int i, int j, float weights[PYRAMID_SIZE][PYRAMID_SIZE]);

char closingOf(char brace);

bool isClosing(char brace);

bool isOpening(char brace);

bool parenthesisValidator(char last);

bool checkBoard(int areaCounters[MAX_BOARD_SIZE],
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

bool placeWord(int GRID_SIZE, char grid[MAX_GRID_SIZE * MAX_GRID_SIZE],
               int row, int column, int length, char direction, char word[MAX_WORD_LENGTH + 1], int i);

bool crosswordWordFinder(int GRID_SIZE, int SLOTS, int WORDS,
                         char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], char orgGrid[MAX_GRID_SIZE * MAX_GRID_SIZE],
                         Slot slots[MAX_SLOTS_NUMBER], int currentSlot,
                         char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1],
                         int wordsAmounts[MAX_WORD_LENGTH], int currentWords[MAX_WORD_LENGTH], int word);

bool crosswordGenerator(int GRID_SIZE, int SLOTS, int WORDS,
                        char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], Slot slots[MAX_SLOTS_NUMBER], int currentSlot,
                        char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1],
                        int wordsAmounts[MAX_WORD_LENGTH], int currentWords[MAX_WORD_LENGTH]);

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

        if (scanf(" %d", &option)) {
            switch (option) {
                case 1: {
                    int i, j;

                    printf("Please enter the coordinates of the robot (column, row):\n");
                    scanf(" %d %d", &i, &j);

                    printf("The total number of paths the robot can take to reach home is: %d\n",
                           i < 0 || j < 0 ? 0 : robotPaths(i, j));

                    break;
                }
                case 2: {
                    float weights[PYRAMID_SIZE][PYRAMID_SIZE];
                    bool negativeValue = false;

                    printf("Please enter the weights of the cheerleaders:\n");
                    for (int i = 0; i < PYRAMID_SIZE && !negativeValue; ++i) {
                        for (int j = 0; j <= i && !negativeValue; ++j) {
                            scanf(" %f", &weights[i][j]);
                            if (weights[i][j] < 0) {
                                negativeValue = true;
                            }
                        }
                    }

                    if (negativeValue) {
                        printf("Negative weights are not supported.\n");
                    } else {
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
                    scanf(" ");
                    printf(parenthesisValidator('\0')
                           ? "The parentheses are balanced correctly.\n"
                           : "The parentheses are not balanced correctly.\n");
                    break;
                case 4: {
                    int N;
                    printf("Please enter the board dimensions:\n");
                    scanf(" %d", &N);
                    printf("Please enter a %d*%d puzzle board:\n", N, N);
                    bool board[MAX_BOARD_SIZE * MAX_BOARD_SIZE] = {};
                    int areas[MAX_BOARD_SIZE * MAX_BOARD_SIZE] = {};
                    char areasNames[MAX_BOARD_SIZE] = {};
                    int lastArea = -1;
                    int areaCounters[MAX_BOARD_SIZE] = {};
                    int columnCounters[MAX_BOARD_SIZE] = {};
                    for (int i = 0; i < N; ++i) {
                        for (int j = 0; j < N; ++j) {
                            char area;
                            scanf(" %c", &area);
                            int k = 0;
                            for (; k <= lastArea; ++k) {
                                if (areasNames[k] == area) {
                                    areas[N * i + j] = k;
                                    break;
                                }
                            }
                            if (k == lastArea + 1) {
                                areasNames[++lastArea] = area;
                                areas[N * i + j] = lastArea;
                            }
                        }
                    }
                    // if N == 2 or 3 it can never be solved.
                    if (N != 2 && N != 3 &&
                        queensBattle(board, areas, areaCounters, columnCounters, -N, N)) {
                        printf("Solution:\n");
                        for (int i = 0; i < N; ++i) {
                            for (int j = 0; j < N; ++j)
                                printf(board[N * i + j] ? "X " : "* ");
                            printf("\n");
                        }
                    } else {
                        printf("This puzzle cannot be solved.\n");
                    }
                    break;
                }
                case 5: {
                    int GRID_SIZE, SLOTS, WORDS;
                    char grid[MAX_GRID_SIZE * MAX_GRID_SIZE] = {};
                    Slot slots[MAX_SLOTS_NUMBER] = {};
                    char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1] = {};
                    int wordsAmounts[MAX_WORD_LENGTH] = {};
                    int currentWords[MAX_WORD_LENGTH] = {};
                    printf("Please enter the dimensions of the crossword grid:\n");
                    scanf(" %d", &GRID_SIZE);
                    initGrid(grid, GRID_SIZE, 0);
                    printf("Please enter the number of slots in the crossword:\n");
                    scanf(" %d", &SLOTS);
                    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
                    for (int i = 0; i < SLOTS; ++i) {
                        int row, column, length;
                        char direction;
                        scanf(" %d %d %d %c", &row, &column, &length, &direction);
                        Slot slot = {row, column, length, direction};
                        slots[i] = slot;
                        for (int j = 0; j < length; ++j) {
                            grid[GRID_SIZE * row + column + ((direction == HORIZONTAL) ? 1 : GRID_SIZE) * j] = '\0';
                        }
                    }
                    printf("Please enter the number of words in the dictionary:\n");
                    scanf(" %d", &WORDS);
                    while (WORDS < SLOTS) {
                        printf("The dictionary must contain at least %d words. "
                               "Please enter a valid dictionary size:\n", SLOTS);
                        scanf(" %d", &WORDS);
                    }
                    printf("Please enter the words for the dictionary:\n");
                    for (int i = 0; i < WORDS; ++i) {
                        char word[MAX_WORD_LENGTH + 1];
                        scanf(" %s", word);
                        int len = (int) strlen(word) - 1;
                        strcpy(words[len][wordsAmounts[len]++], word);
                    }

                    if (crosswordGenerator(GRID_SIZE, SLOTS, WORDS,
                                           grid, slots, 0, words, wordsAmounts, currentWords)) {
                        for (int i = 0; i < GRID_SIZE; ++i) {
                            for (int j = 0; j < GRID_SIZE; ++j)
                                printf("| %c ", grid[GRID_SIZE * i + j]);
                            printf("|\n");
                        }
                    } else {
                        printf("This crossword cannot be solved.\n");
                    }
                    break;
                }
                case 6:
                    printf("Goodbye!\n");
                    return 0;
                default:
                    printf(" Please choose a task number from the list.\n");
                    break;
            }
        } else {
            scanf("%*[^\n]");
        }
    }
}

int robotPaths(int i, int j) {
    if (i == 0 || j == 0)
        return 1;
    if (i == j)
        return 2 * robotPaths(i - 1, j);
    return robotPaths(i - 1, j) + robotPaths(i, j - 1);
}

float humanPyramid(int i, int j, float weights[PYRAMID_SIZE][PYRAMID_SIZE]) {
    if (j > i || j < 0)
        return 0;
    if (i == 0)
        return weights[0][0];
    return .5f * humanPyramid(i - 1, j, weights) + .5f * humanPyramid(i - 1, j - 1, weights)
           + weights[i][j];
}

char closingOf(char brace) {
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

bool isClosing(char brace) {
    return brace == ')' || brace == ']' || brace == '}' || brace == '>';
}

bool isOpening(char brace) {
    return brace == '(' || brace == '[' || brace == '{' || brace == '<';
}

bool parenthesisValidator(char last) {
    char c;
    scanf("%c", &c);
    if (c == '\n') {
        return last == '\0';
    } else if (c == closingOf(last)) {
        return EMPTY;
    } else if (isClosing(c)) {
        scanf("%*[^\n]");
        return false;
    } else if (isOpening(c)) {
        bool out = parenthesisValidator(c);
        if (out != EMPTY)
            return out;
    }
    return parenthesisValidator(last);
}

bool checkBoard(int areaCounters[MAX_BOARD_SIZE],
                int columnCounters[MAX_BOARD_SIZE],
                int column, int area) {
    return columnCounters[column] <= 1 && areaCounters[area] <= 1;
}

bool queensColumn(bool board[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areas[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areaCounters[MAX_BOARD_SIZE],
                  int columnCounters[MAX_BOARD_SIZE],
                  int position, int j, int N) {
    int row = position / N;
    int column = position % N;
    if (j >= N)
        return false;
    if (column - 1 > j || j > column + 1 || position == 0) {
        areaCounters[areas[N * row + j]]++;
        columnCounters[j]++;
        board[N * row + j] = true;
        if (queensBattle(board, areas, areaCounters, columnCounters, N * row + j, N))
            return true;
        areaCounters[areas[N * row + j]]--;
        columnCounters[j]--;
        board[N * row + j] = false;
    }
    return queensColumn(board, areas, areaCounters, columnCounters, position, j + 1, N);
}

bool queensBattle(bool board[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areas[MAX_BOARD_SIZE * MAX_BOARD_SIZE],
                  int areaCounters[MAX_BOARD_SIZE],
                  int columnCounters[MAX_BOARD_SIZE],
                  int position, int N) {
    int row = position / N;
    int column = position % N;
    int area = areas[position];
    if (position >= 0 && !checkBoard(areaCounters, columnCounters, column, area))
        return false;
    if (++row == N)
        return true;
    return queensColumn(board, areas, areaCounters, columnCounters, N * row + column, 0, N);
}

void initGrid(char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], int N, int i) {
    if (i == N * N)
        return;
    grid[i] = '#';
    initGrid(grid, N, i + 1);
}

bool placeWord(int GRID_SIZE, char grid[MAX_GRID_SIZE * MAX_GRID_SIZE],
               int row, int column, int length, char direction, char word[MAX_WORD_LENGTH + 1], int i) {
    if (length == 0)
        return true;
    if (grid[GRID_SIZE * row + column] != '\0' && grid[GRID_SIZE * row + column] != word[i])
        return false;
    grid[GRID_SIZE * row + column] = word[i];

    if (direction == HORIZONTAL)
        return placeWord(GRID_SIZE, grid, row, column + 1, length - 1, direction, word, i + 1);
    return placeWord(GRID_SIZE, grid, row + 1, column, length - 1, direction, word, i + 1);
}

bool crosswordWordFinder(int GRID_SIZE, int SLOTS, int WORDS,
                         char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], char orgGrid[MAX_GRID_SIZE * MAX_GRID_SIZE],
                         Slot slots[MAX_SLOTS_NUMBER], int currentSlot,
                         char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1],
                         int wordsAmounts[MAX_WORD_LENGTH], int currentWords[MAX_WORD_LENGTH], int word) {
    if (word >= wordsAmounts[slots[currentSlot].length - 1])
        return false;
    if (placeWord(GRID_SIZE, grid, slots[currentSlot].row, slots[currentSlot].column,
                  slots[currentSlot].length, slots[currentSlot].direction,
                  words[slots[currentSlot].length - 1][word], 0)) {
        currentWords[slots[currentSlot].length - 1]++;
        if (crosswordGenerator(GRID_SIZE, SLOTS, WORDS,
                               grid, slots, currentSlot + 1, words, wordsAmounts, currentWords))
            return true;
        currentWords[slots[currentSlot].length - 1]--;
    }
    strcpy(grid, orgGrid);
    return crosswordWordFinder(GRID_SIZE, SLOTS, WORDS, grid, orgGrid,
                               slots, currentSlot, words, wordsAmounts, currentWords, word + 1);
}

bool crosswordGenerator(int GRID_SIZE, int SLOTS, int WORDS,
                        char grid[MAX_GRID_SIZE * MAX_GRID_SIZE], Slot slots[MAX_SLOTS_NUMBER], int currentSlot,
                        char words[MAX_WORD_LENGTH][MAX_SLOTS_NUMBER][MAX_WORD_LENGTH + 1],
                        int wordsAmounts[MAX_WORD_LENGTH], int currentWords[MAX_WORD_LENGTH]) {
    if (currentSlot == SLOTS)
        return true;
    char orgGrid[MAX_GRID_SIZE * MAX_GRID_SIZE];
    strcpy(orgGrid, grid);
    return crosswordWordFinder(GRID_SIZE, SLOTS, WORDS, grid, orgGrid,
                               slots, currentSlot, words, wordsAmounts, currentWords,
                               currentWords[slots[currentSlot].length - 1]);
}
