// Name: Maribella Fues
// Class: Fundamentals of Computing
// Assignment: crossfunc.h
// Purpose: defines the macros, struct, and function prototypes used in crossword.c and crossfunc.c

#define WORDSIZE 16
#define MAXWORDS 20
#define BOARDSIZE 15
#define _GNU_SOURCE

// struct for the clues of each word
typedef struct {
    int col;
    int row;
    char dir[10];
    char hint[WORDSIZE];
} Word;

void playGame(char[][WORDSIZE], char[][BOARDSIZE], char[][BOARDSIZE], Word[], int);
int readWords(char[][WORDSIZE]);
int checkWord(char[]);
void toUpper(char[][WORDSIZE], int);
void display(char[][WORDSIZE], int, Word[], char[][BOARDSIZE], char[][BOARDSIZE]);
void makeBoards(char[][BOARDSIZE], char[][BOARDSIZE]);
int readFile(char[][WORDSIZE], char*);
void descendWords(char[][WORDSIZE], int);
void placeWords(char[][WORDSIZE], int, Word[], char[][BOARDSIZE], char[][BOARDSIZE]);
void generateClues(char[][WORDSIZE], Word[], int);
int checkEmpty(int, int, char[][WORDSIZE], Word[], char[][BOARDSIZE]);
void writeFile(char[][WORDSIZE], char[][BOARDSIZE], char[][BOARDSIZE], Word[], char*, int);
