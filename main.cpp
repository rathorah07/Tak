#include "Game.h"
#include "Island.h"
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cctype>
#include <stdlib.h>
#include "Gvars.h"
#include <fstream>
using namespace std;

int playerID;
int board_size;

FILE * fp;
int island_size_weight = 80;
int island_weights[]  = {0,100,200,350,500,0};
int growth_weights[] = {0,0,0,0,200,0};
int hard_flats_weight[] = {-50,-10,-10};
int soft_flats_weight[] = {110,200,250};
int flat_weight[] = {400,600};
int wall_weight = 250;
int capstone_weight = 300;
int END_GAME_FLATSTONE_THRESHOLD[] ={0, 0, 0, 5, 4, 8};
float total_taken_time = 2;
int total_allowed_time;
struct timeval startTime, EndTime;

int main()
{
  srand (time(NULL));
  string data;
  getline(cin,data);
  std::stringstream stream(data);
  stream >> playerID;
  playerID--;
  stream >> board_size;
  Game game(board_size,playerID);
  stream >> total_allowed_time;

  if(playerID == 1)
  {
        island_size_weight = 10;
        int motu_island_weights[]  = {0,0,0,0,1000,0};
        int motu_growth_weights[] = {0,0,0,0,0,0};
        int motu_hard_flats_weight[] = {-75,-25,-25};
        int motu_soft_flats_weight[] = {125,200,200};
        wall_weight = 300;
        flat_weight[1] = 400;
        int motu_END_GAME_FLATSTONE_THRESHOLD[] ={0, 0, 0, 5, 8, 10};

        for (size_t i = 0; i < 6; i++)
        {
          island_weights[i] = motu_island_weights[i];
          growth_weights[i] = motu_growth_weights[i];
          END_GAME_FLATSTONE_THRESHOLD[i] = motu_END_GAME_FLATSTONE_THRESHOLD[i];
        }

        for (size_t i = 0; i < 3; i++)
        {
          hard_flats_weight[i] = motu_hard_flats_weight[i];
          soft_flats_weight[i] = motu_soft_flats_weight[i];
        }


  }

  game.alpha_beta_play();
  return 0;
}
