#include "constants.h"
#include "movement.h"
#include <stdio.h>
#include <stdlib.h>

coord get_coord_input() {
    

    char str[3];

    scanf("%2s", str);  // gets a string of length 2 as an input, written to str
    flush();            // if more text is inputted than nessecary, the remainder is flushed

    char x = str[0] - 65, // converts an input letter into a 0-indexed value from 'A'
         y = str[1] - 49; // converts an input letter into a 0-indexed value from '1'

    if (INRANGE(x, y))
        return COORDINATE(x, y);
    
    x = str[0] - 97; // updates x using a lower case input, converted to a 0-indexed value from 'a'

    if (INRANGE(x, y))
        return COORDINATE(x, y);
    
    return NULL_COORD; // if both conversions fail, return a null coordinate
}

coord local_move_fox(coord* characterPositions) {
    coord nextPosition[] = { NULL_COORD, NULL_COORD, 
        NULL_COORD, NULL_COORD };
    
    char counter = 0; // used as a counter for potential positions to make later processing faster

    for (char i = 0; i < 4; i++)
    {
        // generate the options for the positions the fox may attempt to move to
        char x = (i % 2) * 2 - 1 + X_CO(characterPositions[0]),
             y = (i / 2) * 2 - 1 + Y_CO(characterPositions[0]);

        if (INRANGE(x, y)) {
            coord loc = COORDINATE(x, y);
            // make sure the fox wont collide with hounds
            if (search_for_collision(characterPositions, 5, loc) == -1) {
                nextPosition[i] = loc; // update the nextPosition array to contain legal movements
                counter++;
            }
        }
    }
    
    if (counter == 0) { // triggered when fox can move no where
        return NULL_COORD;
    } else if (counter == 1) { // only one move can be made
        char index = 0;

        while (nextPosition[index] == NULL_COORD) { // find the index of this one move
            index++;
        }

        wprintf(L"Can only move the Fox to %c%d. Type anything to continue.\n",
            X_CO(nextPosition[index]) + 65,
            Y_CO(nextPosition[index]) + 1);

        getchar();
        
        return nextPosition[index];
    } else {
        char index = -1;
        while (index == -1) {
            wprintf(L"Move to (");

            char count = 0;

            for (char i = 0; i < 4; i++)
            {
                // list the locations to which the fox can move

                if (nextPosition[i] != NULL_COORD) { // checks whether location is cached as moveable
                    wprintf(L"%s%c%d",
                        count == 0 ? "" : ", ",
                        X_CO(nextPosition[i]) + 65,
                        Y_CO(nextPosition[i]) + 1);
                    
                    count++;
                }
                
            }

            wprintf(L")? ");
            
            coord inputCoord = get_coord_input();

            // verify input using linear search
            index = search_for_collision(nextPosition, 4, inputCoord);

            if (index == -1) {
                wprintf(L"Invalid location, try again.\n");
            }
        }
        
        return nextPosition[index];
    }
}


coord move_fox(coord* characterPositions, playmode mode) {
    return local_move_fox(characterPositions);
}

// returns an encoded value consisting of the character index, and new position,
// or a constant that is outside the positible range for these values for other events
int local_move_hounds(coord* characterPositions) {
    char characterId = -1;
    coord nextPosition[] = { NULL_COORD, NULL_COORD, 
        NULL_COORD, NULL_COORD, 
        NULL_COORD, NULL_COORD, 
        NULL_COORD, NULL_COORD };
        // represents an overflown coordinate state (no movement) for a coordinate value that is unreachable

    char counter = 0; // used as a cache for movable pieces later
    

    // find where each character can move
    for (char i = 1; i < 5; i++)
    {
        char x = X_CO(characterPositions[i]),
             y = Y_CO(characterPositions[i]);

        if (y < 7) {
            if (x > 0) { // test moving left
                // test for collision
                if (search_for_collision(characterPositions, 5, COORDINATE(x - 1, y + 1)) == -1) {
                    // if none is found add new position to "nextPosition" array
                    nextPosition[2 * i - 1] = COORDINATE(x - 1, y + 1);

                    counter++; // for counting movable pieces
                }
            }

            if (x < 7) { // test moving right
                if (search_for_collision(characterPositions, 5, COORDINATE(x + 1, y + 1)) == -1) {
                    // if none is found add new position to "nextPosition" array
                    nextPosition[2 * i - 2] = COORDINATE(x + 1, y + 1);
                    
                    if (nextPosition[2 * i - 1] == NULL_COORD) {
                        counter++; // for counting movable pieces
                    }
                }
            }
        }
    }

    coord* legalMoves = malloc(counter);
    char* moveIndexs = malloc(counter);
    char moveIndex = 0;
    for (char i = 0; i < 4; i++)
    {
        if (nextPosition[2 * i] != NULL_COORD ||
            nextPosition[2 * i + 1] != NULL_COORD) {
            legalMoves[moveIndex] = characterPositions[i + 1];
            moveIndexs[moveIndex] = i;
            moveIndex++;
        }
    }
    

    if (counter == 0) { // indicates no piece can be moved.
        return HOUND_NO_MOVE;
    } else if (counter > 1) {
        while (characterId == -1) {
            wprintf(L"Move (");
            char count = 0;

            for (char i = 0; i < counter; i++)
            {
                // list the locations of each hound

                wprintf(L"%s%c%d",
                    count == 0 ? "" : ", ",
                    X_CO(legalMoves[i]) + 65,
                    Y_CO(legalMoves[i]) + 1);
                
                count++;
            }
            
            wprintf(L")? ");

            coord inputCoord = get_coord_input();

            // verify input using linear search
            characterId = search_for_collision(legalMoves, counter, inputCoord);

            if (characterId == -1) {
                wprintf(L"Invalid location, try again.\n");
            }
        }
    } else {
        // since only one hound can be moved the user is informed. 
        
        characterId = 0;
        
        wprintf(L"Can only move the Hound at %c%d. Type anything to continue.\n",
            X_CO(legalMoves[0]) + 65,
            Y_CO(legalMoves[0]) + 1);
        
        getchar();
    }

    characterId = moveIndexs[characterId] + 1;

    coord posA = nextPosition[characterId * 2 - 1],
          posB = nextPosition[characterId * 2 - 2];


    // check if theres just one position the character can move to
    if (posB == NULL_COORD) {
        wprintf(L"Can only move the Hound to %c%d. Type anything to continue.\n",
            X_CO(posA) + 65,
            Y_CO(posA) + 1);

        getchar();
        return posA * 4 + characterId - 1; // return where to move
    }
    
    if (posA == NULL_COORD) {
        wprintf(L"Can only move the Hound to %c%d. Type anything to continue.\n",
            X_CO(posB) + 65,
            Y_CO(posB) + 1);
        
        getchar();
        return posB * 4 + characterId - 1; // return where to move
    }
    
    // otherwise request where to move
    while (1) {
        wprintf(L"Move to (%c%d, %c%d)? ",
            X_CO(posA) + 65,
            Y_CO(posA) + 1,
            X_CO(posB) + 65,
            Y_CO(posB) + 1);

        coord inputCoord = get_coord_input();

        // verify input using highly simplified linear search
        
        if (inputCoord == posA || inputCoord == posB) {
            return inputCoord * 4 + characterId - 1;
        }

        wprintf(L"Invalid location, try again.\n");
    }
}

int move_hounds(coord* characterPositions, playmode mode) {
    return local_move_hounds(characterPositions);
}