// Name: Maribella Fues
// Class: Fundamentals of Computing
// Assignment: crossword.c
// Purpose: depending on the number of command line arguments, run the correct mode for the crossword

# include <stdio.h>
# include <string.h>

# include "crossfunc.h"

int main(int argc, char *argv[])
{
    char words[MAXWORDS][WORDSIZE];
    char slnBoard[BOARDSIZE][BOARDSIZE];
    char puzzleBoard[BOARDSIZE][BOARDSIZE];
    // run interactive mode that takes in user input and has standard ouput
    if(argc == 1) {
        int count = readWords(words);
        printf("\nAnagram Crossword Puzzle Generator\n");
        printf("----------------------------------\n\n");
        Word clues[count];
        playGame(words, slnBoard, puzzleBoard, clues, count);
        display(words, count, clues, slnBoard, puzzleBoard);
    }
    // run mode that takes in file input and has standard output
    else if(argc == 2) {
        int count = readFile(words, argv[1]);
        if(count > 0) {
            printf("\nAnagram Crossword Puzzle Generator\n");
            printf("----------------------------------\n\n");
            Word clues[count];
            playGame(words, slnBoard, puzzleBoard, clues, count); 
            display(words, count, clues, slnBoard, puzzleBoard); }
    }
    // run mode that takes in file input and has file ouput
    else if(argc == 3) {
        int count = readFile(words, argv[1]);
        if(count > 0) { 
            printf("\nAnagram Crossword Puzzle Generator\n");   
            printf("----------------------------------\n\n");
            Word clues[count];
            playGame(words, slnBoard, puzzleBoard, clues, count); 
            writeFile(words, slnBoard, puzzleBoard, clues, argv[2], count); }
    }
    else printf("Invalid number of inputs.\n");
    return 0;
}
