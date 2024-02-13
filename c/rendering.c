#include "constants.h"
#include "rendering.h"
#include <stdio.h>
#include <stdlib.h>

// ansi colour code definitions
#define BKG_RED   196
#define BKG_BLK   0
#define GREEN     28
#define ORANGE    208

void render_board(coord characterPositions[], char nextPlayer) {
    // for labelling columns A-H
    wprintf(L"  A B C D E F G H\n");

    // loops through each row on 8 tall board, occurs backwards if next player changes
    for (char yCoordinate = nextPlayer * 7;
        (yCoordinate < 8 && nextPlayer == 0) || (yCoordinate >= 0 && nextPlayer == 1);
        yCoordinate -= nextPlayer * 2 - 1)
    {
        // loops through each position on 8 wide board
        for (char xCoordinate = 0; xCoordinate < 8; xCoordinate++)
        {
            /* 
            prints two black or two red characters in a checkerbox pattern.
            \x1b[48;5;%dm represents a 256 colour ansi escape code.
            Where %d is substituted for a number representing one of 256 colours. either black or red.
            see https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit
            */

            // sets the background colour to red or black.
            if (xCoordinate > 0) {
                wprintf(L"\x1b[48;5;%dm\x1b[38;5;%dm▌",
                    ((yCoordinate + xCoordinate) % 2 == 0) ? BKG_RED:BKG_BLK,
                    ((yCoordinate + xCoordinate) % 2 == 0) ? BKG_BLK:BKG_RED);
            } else {
                wprintf(L"%d\x1b[48;5;%dm\x1b[30m▌",
                    yCoordinate + 1, // for labeling rows 1-8 
                    ((yCoordinate + xCoordinate) % 2 == 0) ? BKG_RED:BKG_BLK);
            }
            

            // is set to 1 if a fox is present,
            // is set to two if a hound is present in current square
            char characterFlag = 0;

            for (int characterIndex = 0; characterIndex < 5; characterIndex++)
            {
                if (characterPositions[characterIndex] == COORDINATE(xCoordinate, yCoordinate)) {
                    // sets the type of character based on its position in the coordinates array
                    characterFlag = (characterIndex > 0) ? 2 : 1;
                }
            }
            
            
            if (characterFlag == 0) { // prints a blank box if no character is within the square
                wprintf(L" ");
            } else { // prints a circle within a square
                wprintf(L"\x1b[38;5;%dm⬤", characterFlag == 1 ? ORANGE : GREEN);
            }
        }
        // clears the escape code to print to the screen as usual
        wprintf(L"\x1b[30m▐\x1b[39;49m\n");
    }
}

void render_feedback(char nextPlayer, char* foxName, char* houndName, int foxScore, int houndScore) {
    wprintf(L"\x1b[2J");
    wprintf(L"%s:%d - %s:%d\n", foxName, foxScore, houndName, houndScore);
    if (nextPlayer == 0) {
        wprintf(L"Fox (%s) next\n", foxName);
    } else {
        wprintf(L"Hound (%s) next\n", houndName);
    }

    wprintf(L"Press Ctrl+C at any time to exit.\n");

    // print information about the game state
}

char render_win(char winner, char* winnerName) {
    if (winner == 0) {
        wprintf(L"Hound, %s won!\n", winnerName);
    } else {
        wprintf(L"Fox, %s won!\n", winnerName);
    }

    while (1 == 1) {
        wprintf(L"Rematch (y,n)? ");
        
        char str[2];

        scanf("%1s", str);  // gets a string of length 1 as an input, written to str
        flush();            // if more text is inputted than nessecary, the remainder is flushed

        if (str[0] == 'y' || str[0]  == 'Y') {
            return -1;
        } else if (str[0] == 'n' || str[0] == 'N') {
            wprintf(L"Thanks for playing Fox and Hounds!\n");
            return 0;
        }
    }
}