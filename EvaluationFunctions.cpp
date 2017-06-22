#include "Game.h"
#include "Island.h"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <time.h>
#include <sstream>
#include <cstdio>
#include <cctype>
#include <climits>
#include <unistd.h>
#include "Gvars.h"


bool Game :: terminal()
{
  island_score = 0;
  vector<island> myisland = find_island(playerID);
  vector<island> enemyisland = find_island(1-playerID);

  for(int i =0 ;i<myisland.size();i++)
  {
    int hThreat = myisland[i].horizontal_threat();
    int vThreat = myisland[i].vertical_threat();

    int growth_factor = myisland[i].grow_factor();

    if(hThreat == board_len || vThreat == board_len)
    {
      road_winner = playerID;
      return true;
    }
    if(turn != playerID)
      growth_factor--;
    int max_length = max(hThreat,vThreat);
    island_score += island_weights[max_length];
    island_score += island_size_weight*myisland[i].elem_count;
    island_score += growth_factor*growth_weights[max_length];
  }

  for(int i =0 ;i<enemyisland.size();i++)
  {
    int hThreat = enemyisland[i].horizontal_threat();
    int vThreat = enemyisland[i].vertical_threat();

    int growth_factor = enemyisland[i].grow_factor();

    if(hThreat == board_len || vThreat == board_len)
    {
      road_winner = 1-playerID;
      return true;
    }
    int max_length = max(hThreat,vThreat);
    island_score -= island_weights[max_length];
    island_score -= island_size_weight*enemyisland[i].elem_count;
    island_score -= growth_factor*growth_weights[max_length];
  }

  if(isFlatTerminal(*this))
    return true;

  return false;
}

int Game:: terminalUtility()
{
  if(road_winner == playerID)
    {
      //fprintf(fp, "winning terminal found\n");
      return INT_MAX ;
    }
  else
  {
    //fprintf(fp, "loosing terminal found\n");
    return INT_MIN ;
  }

}


int Game :: heuristic_evaluation()
{
  int myscore =0;
  int enemyscore =0;
  if(road_winner == playerID)
  {
    return INT_MAX;
  }
  if(road_winner == 1-playerID)
  {
    return INT_MIN;
  }
  if(flat_winner == playerID)
  {
    return flat_win_score(*this);
  }

  if(flat_winner == 1-playerID)
  {
    return flat_win_score(*this);
  }
  if(flat_winner == 3)
    return 0;

  pair<int,int> flatstone_weight = get_flatstone_weight();

  myscore += players[playerID].upper_flat * flatstone_weight.first;
  enemyscore += players[1-playerID].upper_flat *flatstone_weight.second;

  myscore += players[playerID].walls*wall_weight;
  enemyscore += players[1-playerID].walls*wall_weight;

  for (int i = 0; i < 3; i++)
  {
    myscore += ( players[playerID].soft_flats[i]*soft_flats_weight[i] + players[1-playerID].hard_flats[i]*hard_flats_weight[i] );

    enemyscore += (players[1-playerID].soft_flats[i]*soft_flats_weight[i] + players[playerID].hard_flats[i]*hard_flats_weight[i]);
  }

  return myscore - enemyscore + island_score;
}
