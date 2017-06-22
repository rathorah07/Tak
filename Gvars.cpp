#ifndef GVAR_H
#define GVAR_H
#include "Game.h"
#include "Gvars.h"
#include <map>
#include <climits>


int flat_win_holding_points = 1000000;
int flat_win_unused_points =  10;

bool isFlatTerminal(Game &state)
{
  if(state.free_square == 0 || (state.players[state.playerID].flats == 0 ) || (state.players[1-state.playerID].flats == 0 ))
  {
    if(state.players[state.playerID].upper_flat > state.players[1-state.playerID].upper_flat)
    {
      state.flat_winner = state.playerID;
      return true;
    }
    if(state.players[state.playerID].upper_flat < state.players[1-state.playerID].upper_flat)
    {
      state.flat_winner = 1-state.playerID;
      return true;
    }
    if(state.players[state.playerID].flats > state.players[1-state.playerID].flats)
    {
      state.flat_winner =  state.playerID;
      return true;
    }
    if(state.players[state.playerID].flats < state.players[1-state.playerID].flats)
    {
      state.flat_winner = 1-state.playerID;
      return true;
    }
    else
    {
      state.flat_winner = 3;
      return true;
    }
  }
  return false;
}

int flat_win_score(Game &state)
{
  int my_holdings = flat_win_holding_points *state.players[state.playerID].upper_flat;
  int enemy_holdings = flat_win_holding_points *state.players[1-state.playerID].upper_flat;

  my_holdings += flat_win_unused_points* state.players[state.playerID].flats;
  enemy_holdings += flat_win_unused_points* state.players[1-state.playerID].flats;

   if(my_holdings - enemy_holdings > 0)
   {
      return INT_MAX/2;
   }
   else
      return INT_MIN/2;
}

#endif
