// Name: Maribella Fues
// Class: Fundamentals of Computing
// Assignment: crossfunc.c
// Purpose: define all the functions used to run and display the crossword that are used both in crossword.c and other functions

#define _GNU_SOURCE
# include <stdio.h>
# include <string.h>
# include <ctype.h>

# include "crossfunc.h"

// calls multiple functions to run the game by ordering the words, placing them on the correct boards, and generating the clues
void playGame(char words[][WORDSIZE], char solnBoard[][BOARDSIZE], char puzzleBoard[][BOARDSIZE], Word clues[], int count)
{
    makeBoards(solnBoard, puzzleBoard);
    toUpper(words, count);
    descendWords(words, count);
    generateClues(words, clues, count);
    placeWords(words, count, clues, solnBoard, puzzleBoard);
    generateClues(words, clues, count);
}

// reads in user input and if the word is valid, stores it in the words array
int readWords(char words[][WORDSIZE])
{
    printf("Enter a list of words followed by a \".\":\n");
    int count = 0;
    while(1) {
        char word[50];
        scanf("%s", &word);
        int valid = checkWord(word);
        if(valid < 0) break;
        else if(valid) {
            strcpy(words[count], word);
            count++;
        }
        if(count >= 20) {
            printf("The maximum number of words has been reached.\n");
            break;
        }
    }
    return count;
}

// checks if the word is a valid word to be used in the crossword
int checkWord(char word[])
{
    if(!strcmp(word, ".")) return -1;
    else if(strlen(word) < 2 || strlen(word) > 15) {
        printf("Word ignored because it is an improper length.\n");
        return 0;
    }
    for(int i = 0; i < strlen(word); i++)
        if(!isalpha(word[i])) {
            printf("Word ignored because it contains a non-letter.\n");
            return 0;
        }
    return 1;
}

// converts all the words to uppercase
void toUpper(char words[][WORDSIZE], int count)
{
    for(int i = 0; i < count; i++)
        for(int j = 0; j < strlen(words[i]); j++)
            if(words[i][j] > 96) words[i][j] -= 32;
}

// displays to standard output both boards and the clues
void display(char words[][WORDSIZE], int count, Word clues[], char solnBoard[][BOARDSIZE], char puzzleBoard[][BOARDSIZE])
{
    // for(int i = 0; i < count; i++)
        // printf("%s\n", words[i]);
    for(int i = 0; i < count; i++)
        if(clues[i].row < 0) printf("Could not place the word %s.\n", words[i]);

    printf("\nSolution:\n");
    for(int i = 0; i < BOARDSIZE+2; i++)
        printf("-");
    printf("\n");
    for(int i = 0; i < BOARDSIZE; i++)
    {
        printf("|");
        for(int j = 0; j < BOARDSIZE; j++)
            printf("%c", solnBoard[i][j]);
        printf("|\n");
    }
    for(int i = 0; i < BOARDSIZE+2; i++)
        printf("-");
    printf("\n");

    printf("\n\nCrossword Puzzle:\n");
    for(int i = 0; i < BOARDSIZE+2; i++)
        printf("-");
    printf("\n");
    for(int i = 0; i < BOARDSIZE; i++)
    {
        printf("|");
        for(int j = 0; j < BOARDSIZE; j++)
            printf("%c", puzzleBoard[i][j]);
        printf("|\n");
    }
    for(int i = 0; i < BOARDSIZE+2; i++)
        printf("-");
    printf("\n");

    printf("\n\nClues:\n\n");
    printf("Location | Direction | Anagram\n");
    for(int i = 0; i < count; i++)
        if(clues[i].row>-1)
            printf("%5d,%2d | %9s | %s\n", clues[i].col, clues[i].row, clues[i].dir, clues[i].hint);
    printf("\n");
}

// fills blank boards for the solution and puzzle boards
void makeBoards(char solnBoard[][BOARDSIZE], char puzzleBoard[][BOARDSIZE])
{
    for(int i = 0; i < BOARDSIZE; i++)
        for(int j = 0; j < BOARDSIZE; j++) {
            solnBoard[i][j] = '.';
            puzzleBoard[i][j] = '#'; }

}

// reads in the words from the file and if they are valid words, stores them in the words array
int readFile(char words[][WORDSIZE], char fileName[])
{
    int count = 0;
    FILE *fp = fopen(fileName, "r");
    if(fp == NULL) {
        printf("Error: could not open file\n");
        return -1;
    }
    while(1) {
        char word[50];
        fscanf(fp, "%s", word);
        int valid = checkWord(word);
        if(valid < 0) break;
        else if(valid) {
            strcpy(words[count], word);
            count++;
        }
        if(count >= 20) {
            printf("The maximum number of words has been reached.\n");
            break;
        }

    }
    return count;
}

// rearrange the words array to have the words in descending order of their length
void descendWords(char words[][WORDSIZE], int count)
{
    char temp[WORDSIZE];
    for(int i = 0; i < count; i++)
        for(int j = i+1; j < count; j++) {
            if(strlen(words[i]) < strlen(words[j])) {
                strcpy(temp, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], temp);
            }
        }
}

// places all the words on the board - manually places the first and second word, then loops through all the rest of the words to place them after
void placeWords(char words[][WORDSIZE], int count, Word clues[], char solnBoard[][BOARDSIZE], char puzzleBoard[][BOARDSIZE])
{
    // places the first word
    int start;
    if(count > 0) {
        start = (BOARDSIZE-strlen(words[0]))/2;
        for(int i = 0; i < strlen(words[0]); i++){
            solnBoard[7][i+start] = words[0][i];
            puzzleBoard[7][i+start] = ' '; }
        clues[0].col = start;
        clues[0].row = 7;
        strcpy(clues[0].dir, "Across");
    } 

    // places the second word
    if(count > 1) {
        int index1 = -1;
        int index2;
        for(int i = 0; i < strlen(words[0]); i++) {
            for(int j = 0; j < strlen(words[1]); j++)
                if(words[0][i] == words[1][j]) 
                    if(((strlen(words[1])-j-1) < (BOARDSIZE-clues[0].row)) && (j < clues[0].row)) {
                        index1 = i;
                        index2 = j;
                    }
        } 
        if(index1 > 0) {
            clues[1].col = clues[0].col + index1;
            clues[1].row = clues[0].row - index2;
            for(int i = 0; i < strlen(words[1]); i++) {
                solnBoard[clues[1].row + i][clues[1].col] = words[1][i];
                puzzleBoard[clues[1].row + i][clues[1].col] = ' '; }
            strcpy(clues[1].dir, "Down"); }
        else clues[1].row = -1;
    }  

    // places the rest of the words
    for(int cur = 2; cur < count; cur++) {
        clues[cur].row = -1;
        for(int check = 0; check < cur; check++) {
            if((clues[check].row>-1) && checkEmpty(cur, check, words, clues, puzzleBoard)) {
                if(!strcmp(clues[check].dir, "Across"))
                    for(int i = 0; i < strlen(words[cur]); i++) {
                        solnBoard[clues[cur].row + i][clues[cur].col] = words[cur][i];
                        puzzleBoard[clues[cur].row + i][clues[cur].col] = ' '; }
                if(!strcmp(clues[check].dir, "Down"))
                    for(int i = 0; i < strlen(words[cur]); i++) {
                        solnBoard[clues[cur].row][clues[cur].col + i] = words[cur][i];
                        puzzleBoard[clues[cur].row][clues[cur].col + i] = ' '; }
                break;
            }
        }
        // checks if can place words now that had been previously skipped
        for(int test = 0; test < cur; test++)
            if(clues[test].row < 0)
                for(int check = 0; check <= cur; check++)
                    if((clues[check].row>-1) && checkEmpty(test, check, words, clues, puzzleBoard)) {
                        if(!strcmp(clues[check].dir, "Across"))
                            for(int i = 0; i < strlen(words[test]); i++) {
                                solnBoard[clues[test].row + i][clues[test].col] = words[test][i];
                                puzzleBoard[clues[test].row + i][clues[test].col] = ' '; }
                        if(!strcmp(clues[check].dir, "Down"))
                            for(int i = 0; i < strlen(words[test]); i++) {
                                solnBoard[clues[test].row][clues[test].col + i] = words[test][i];
                                puzzleBoard[clues[test].row][clues[test].col + i] = ' '; }
                        break;
            }
    }
    
        
}

// generates the anagrams and orders the clues in ascending order of their columns
void generateClues(char words[][WORDSIZE], Word clues[], int count)
{
    for(int i = 0; i < count; i++) {
        strcpy(clues[i].hint, words[i]);
        strfry(clues[i].hint);
    }
    Word temp;
    for(int i = 0; i < count; i++)
        for(int j = i+1; j < count; j++)
            if((clues[i].row>-1) && clues[i].col > clues[j].col) {
                temp = clues[i];
                clues[i] = clues[j];
                clues[j] = temp; }
    for(int i = 0; i < count; i++)
        for(int j = i+1; j < count; j++)
            if((clues[i].row>-1) && clues[i].col == clues[j].col)
                if(clues[i].row > clues[j].row) {
                    temp = clues[i];
                    clues[i] = clues[j];
                    clues[j] = temp; }

}

// checks if the the two words share a common character and if so, if there is room to place the new word intersecting the old word
int checkEmpty(int cur, int check, char words[][WORDSIZE], Word clues[], char board[][BOARDSIZE]) {
    int row, col, empty = 1;
    for(int i = 0; i < strlen(words[cur]); i++) 
        for(int j = 0; j < strlen(words[check]); j++) 
            if(words[cur][i] == words[check][j]) {
                // checking area around a horizontal word to place a vertical word
                if(!strcmp(clues[check].dir, "Across"))
                    if(((strlen(words[cur])-i-1) < (BOARDSIZE-clues[check].row)) && (i <= clues[check].row)) {
                        row = clues[check].row - i;
                        col = clues[check].col + j;
                        for(int k = 0; k < strlen(words[cur]); k++) {
                            if(k != i && board[row + k][col] != '#') {
                                empty = 0;
                                break; }
                            if(((col+1)<BOARDSIZE) && k != i && board[row + k][col+1] != '#') {
                                empty = 0;
                                break; }
                            if((col>0) && k != i && board[row + k][col-1] != '#') {
                                empty = 0;
                                break; }
                            if(row > 0 && board[row-1][col] != '#') {
                                empty = 0; 
                                break; }
                            if((row+strlen(words[cur])<BOARDSIZE) && board[row+strlen(words[cur])][col] != '#') {
                                empty = 0;
                                break; }
                            empty = 1;
                        }
                        if(empty > 0) {
                            clues[cur].row = row;
                            clues[cur].col = col;
                            strcpy(clues[cur].dir, "Down");
                            return 1; }
                    }
                // checking area around a vertical word to place a horizontal word
                if(!strcmp(clues[check].dir, "Down"))
                    if((strlen(words[cur])-i-1) < (BOARDSIZE-clues[check].col) && (i <= clues[check].col)) {
                        row = clues[check].row + j ;
                        col = clues[check].col - i;
                        for(int k = 0; k < strlen(words[cur]); k++) {
                            if(k != i && board[row][col+k] != '#') {
                                empty = 0;
                                break; }
                            if(((row+1)<BOARDSIZE) && k != i && board[row+1][col+k] != '#') {
                                empty = 0;
                                break; }
                            if((row>0) && k != i && board[row-1][col+k] != '#') {
                                empty = 0;
                                break; }
                            if(col > 0 && board[row][col-1] != '#') {
                                empty = 0;
                                break; }
                            if((col+strlen(words[cur])<BOARDSIZE) && board[row][col+strlen(words[cur])] != '#') {
                                empty = 0;
                                break; }
                            empty = 1;
                        }
                        if(empty > 0) {
                            clues[cur].row = row;
                            clues[cur].col = col;
                            strcpy(clues[cur].dir, "Across");
                            return 1; }
                    }
            }
    return 0;
    
}

// writes to the new file the output of the program including both boards and the  clues    
void writeFile(char words[][WORDSIZE], char sln[][BOARDSIZE], char puzzle[][BOARDSIZE], Word clues[], char* fileName, int count) {
    FILE *fp = fopen(fileName, "w");
    if(fp == NULL) {
        printf("Error: could not create new file\n");
        return; }

    fprintf(fp, "Anagram Crossword Puzzle Generator\n");
    fprintf(fp, "----------------------------------\n");
    for(int i = 0; i < count; i++)
        if(clues[i].row < 0) fprintf(fp, "Could not place the word %s.\n", words[i]);

    fprintf(fp, "\nSolution:\n");
    for(int i = 0; i < BOARDSIZE+2; i++)
        fprintf(fp, "-");
    fprintf(fp, "\n");
    for(int i = 0; i < BOARDSIZE; i++) {
        fprintf(fp, "|");
        for(int j = 0; j < BOARDSIZE; j++)
            fprintf(fp, "%c", sln[i][j]);
        fprintf(fp, "|\n"); }
    for(int i = 0; i < BOARDSIZE+2; i++)
        fprintf(fp, "-");
    fprintf(fp, "\n\n\nCrossword Puzzle:\n");
    for(int i = 0; i < BOARDSIZE+2; i++)
        fprintf(fp, "-");
    fprintf(fp, "\n");
    for(int i = 0; i < BOARDSIZE; i++) {
        fprintf(fp, "|");
        for(int j = 0; j < BOARDSIZE; j++)
            fprintf(fp, "%c", puzzle[i][j]);
        fprintf(fp, "|\n"); }
    for(int i = 0; i < BOARDSIZE+2; i++)
        fprintf(fp, "-");
    fprintf(fp, "\n\n\nClues:\n\n");
    fprintf(fp, "Location | Direction | Anagram\n");
    for(int i = 0; i < count; i++)
        if(clues[i].row>-1)
            fprintf(fp, "%5d,%2d | %9s | %s\n", clues[i].col, clues[i].row, clues[i].dir, clues[i].hint);


    fclose(fp);
}
