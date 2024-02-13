#include "rendering.h"
#include "constants.h"
#include "movement.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
#include <fcntl.h> // Also includes the non-standard file <io.h>
#endif

char check_fox_win(coord* characterPositions) {
    // a fox wins when a hound cannot move in front of it
    // returns 1 when true, 0 when false

    for (int x = 0; x < 8; x++)
    {
        // find the maximum possible y coordinate such that the hound can block the fox

        char depth = Y_CO(characterPositions[0]) - abs(x - X_CO(characterPositions[0]));

        for (int y = (x + 1) % 2; y <= depth; y += 2)
        {
            if (search_for_collision(characterPositions + 1, 4, COORDINATE(x, y)) != -1)
                return 0;
        }
    }
    
    return 1;
}

char* get_name(int playerNumber) {
    if (playerNumber == 1) {
        wprintf(L"What should Player 1 be called (starts as fox)? ");
    } else {
        wprintf(L"What should Player 2 be called (starts as hound)? ");
    }

    char *buff = malloc(162); // allocates 162 characters to the player name
    char pointerIndex = 1;
    buff[0] = '_';
    while (buff[pointerIndex - 1] != '\n' && pointerIndex < 160) {
        buff[pointerIndex] = getchar();
        pointerIndex++;
    }

    char *name = malloc(pointerIndex - 1); // stores the player name in the minimum sized character[]

    arrayCopy(buff + 1, name, pointerIndex - 2);
    name[pointerIndex - 2] = 0;
    free(buff); // frees the initially allocated 162 characters
    return name;
}

char runGame(playmode mode, char* foxName, char* houndName, int foxScore, int houndScore) {
    char playing = 1;

    coord characterPositions[] = {
        COORDINATE(0, 7), // fox position
        COORDINATE(1, 0), // hound 0 position
        COORDINATE(3, 0), // hound 1 position
        COORDINATE(5, 0), // hound 2 position
        COORDINATE(7, 0), // hound 3 position
    };

    char nextPlayer = 0;
    while(playing) {
        render_feedback(nextPlayer, foxName, houndName, foxScore, houndScore);
        render_board(characterPositions, nextPlayer);
        
        // check if win condition reached
        if (check_fox_win(characterPositions) == 1) {
            return 1;
        }
        
        if (nextPlayer == 0) {
            // request the movement of the fox

            coord foxMovementData = move_fox(characterPositions, mode);

            if (foxMovementData == NULL_COORD) {
                return 0;

            } else {
                characterPositions[0] = foxMovementData;
            }

            nextPlayer = 1;
        } else {
            // request the movement of the hounds

            int houndMovementData = move_hounds(characterPositions, mode);

            if (houndMovementData == HOUND_NO_MOVE) {
                return 1;

            } else {
                characterPositions[houndMovementData % 4 + 1]   // get the encoded index of the hound to be moved
                    = houndMovementData / 4;                    // get the encoded coordinates of the hound's new location
            }
            
            nextPlayer = 0;
        }
    }
}

int main(int argc, char const *argv[]) {
    playmode mode = LOCAL_MULTIPLAYER;
    char selectedFlag = 0;
    char* player1Name;
    char* player2Name;
    int* scores = malloc(2 * sizeof(int));
    scores[0] = 0;
    scores[1] = 0;

    _setmode(_fileno(stdout), _O_WTEXT);
    wprintf(L"testabc, █\n");

    wprintf(L"\x1b[2J██████████████████████████████████████████████████████████████████████████████\n");
    wprintf(L"█▄ ▄▄ █ ▄▄ █▄ ▀ ▄████▀▄ ██▄ ▀█▄ ▄█▄ ▄▄▀███ █ █ ▄▄ █▄ ██ ▄█▄ ▀█▄ ▄█▄ ▄▄▀█ ▄▄▄▄█\n");
    wprintf(L"██ ▄███ ██ ██▀ ▀█████ ▀ ███ █▄▀ ███ ██ ███ ▄ █ ██ ██ ██ ███ █▄▀ ███ ██ █▄▄▄▄ █\n");
    wprintf(L"█▄▄▄███▄▄▄▄█▄▄█▄▄███▄▄█▄▄█▄▄▄██▄▄█▄▄▄▄████▄█▄█▄▄▄▄██▄▄▄▄██▄▄▄██▄▄█▄▄▄▄██▄▄▄▄▄█\n\n");

    if (mode == LOCAL_MULTIPLAYER) {
        player1Name = get_name(1);
        player2Name = get_name(2);
    } else {
        player1Name = "Human";
        player2Name = "Robot";
    }

    wprintf(L"Initialising Game.\n");

    char exit = 1; // =1 if player1 starts, =-1 if player2 starts, =0 if the game should exit
    while(exit != 0) {
        char winner;
        if (exit == 1) {
            winner = runGame(mode, player1Name, player2Name, scores[0], scores[1]);
        } else {
            winner = runGame(mode, player2Name, player1Name, scores[1], scores[0]);
        }

        if (mode == AI_SINGLEPLAYER_F) {
            mode = AI_SINGLEPLAYER_H;
        } else if (mode == AI_SINGLEPLAYER_H) {
            mode = AI_SINGLEPLAYER_F;
        }
        
        // adds a score to the winner
        scores[((exit + 1) / 2 + winner) % 2]++;

        // when winner =0, hound won, otherwise fox won
        // if returns 0, exit =0, otherwise should return 1, exit doesnt change
        exit *= render_win(winner, ((exit + 1) / 2 + winner) % 2 == 0 ? player1Name : player2Name); 
    }
    
    return 0;
}


