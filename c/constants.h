// defines some datatypes and functions for the co-ordinate system
#define coord char
#define COORDINATE(x, y) ((x) * 8 + (y))
#define X_CO(n) ((n) / 8)
#define Y_CO(n) ((n) % 8)
#define NULL_COORD 0x7f
#define INRANGE(x,y) ((x) >= 0 && (x) < 8 && (y) >= 0 && (y) < 8)

// defines game state constants
#define HOUND_NO_MOVE   -1

// defines play modes
// determine whether characters use a remote connection, local ai, or a local player
#define playmode                char
#define LOCAL_MULTIPLAYER       0
#define ONLINE_MULTIPLAYER_F    1
#define AI_SINGLEPLAYER_F       2
#define ONLINE_MULTIPLAYER_H    3
#define AI_SINGLEPLAYER_H       4


int search_for_collision(coord* positions, char length, coord find);
void flush(void );
void arrayCopy(char* arrIn, char* arrOut, int length);