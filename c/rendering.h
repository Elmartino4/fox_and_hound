#include "constants.h"

void render_board(coord characterPositions[], char nextPlayer);
void render_feedback(char nextPlayer, char* foxName, char* houndName, int foxScore, int houndScore);
char render_win(char winner, char* winnerName);