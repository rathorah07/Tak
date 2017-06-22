#ifndef GVAR_H
#define GVAR_H
#include "Game.h"
#include <map>
#include <climits>
#include <sys/time.h>

extern FILE* fp;

extern int island_size_weight;
extern int flat_weight[];
extern int island_weights[];
extern int growth_weights[];
extern int hard_flats_weight[];
extern int soft_flats_weight[];
extern int wall_weight;
extern int capstone_weight;
extern int END_GAME_FLATSTONE_THRESHOLD[];
extern struct timeval startTime, EndTime;
extern float total_taken_time;
extern int total_allowed_time; 
bool isFlatTerminal(Game &state);

int flat_win_score(Game &state);

#endif
