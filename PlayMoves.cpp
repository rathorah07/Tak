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
#include <fstream>
#include <sys/time.h>

void Game :: alpha_beta_play()
{
  int alpha = INT_MIN;
  int beta = INT_MAX;
  int eval;

  if(playerID == 1)
  {
    string move;
    getline(cin,move);
    gettimeofday (&startTime, NULL);
    execute_move(move);
  }

  while(true)
  {
      string move;
      pair<int,string> score_move = maxVal(alpha,beta,max_depth);
      move = score_move.second;
      move = move + '\n';
      printf("%s",move.c_str());
      if(this->moves != 0)
      {
        gettimeofday(&EndTime,NULL);
        float elapsed = (EndTime.tv_sec - startTime.tv_sec) + (EndTime.tv_usec - startTime.tv_usec)/(1.0*1000000L);
        total_taken_time += elapsed;

        if(total_taken_time/total_allowed_time < .3)
        {
            max_depth = 4;
        }

        else if(total_taken_time/total_allowed_time < .9)
        {
          max_depth = 3;
        }

        else
          max_depth = 2;
      }
      execute_move(score_move.second);
      getline(cin,move);
      gettimeofday(&startTime,NULL);
      execute_move(move);
  }
}
