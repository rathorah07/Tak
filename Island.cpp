#include "Island.h"
#include <iostream>
#include <vector>
#include <climits>
#include <string.h>
#include "Gvars.h"
#include <math.h>
using namespace std;

island :: island(int board_size)
{
  elem = 0;
  this->board_size = board_size;
  elem_count = 0;
  left = INT_MAX;
  right = INT_MIN;
  top = INT_MIN;
  bottom = INT_MAX;
  float left_grow = 0;
  float right_grow = 0;
  float bottom_grow = 0;
  float top_grow = 0;
}
int island :: vertical_threat()
{
  if(elem_count > 0)
    return top - bottom + 1;
  else
    return 0;
}

int island :: horizontal_threat()
{
  if(elem_count > 0)
    return right - left + 1;
  else
    return 0;
}

int island :: offer(int square)
{
  if(elem & (1 << square))   // element already exist
  {
    return -1;
  }
  elem |= 1 << square;
  int row = square % board_size;
  int col = square / board_size;
  if(row < left)
  {
    left_grow = 0;
    left = row;
  }
  if(row > right)
  {
    right = row;
    right_grow = 0;
  }

  if(col > top)
  {
    top_grow = 0;
    top = col;
  }

  if(col < bottom)
  {
    bottom_grow =0;
    bottom = col;
  }
  elem_count++;
}

int island :: update_growable(int square,int status)
{
  int row = square % board_size;
  int col = square / board_size;

  switch (status)
  {
    case -1:

    case 0:

        if(row < left)
          left_grow += 0.5;
        if(row > right)
          right_grow += 0.5;

        if(col < bottom)
          bottom_grow += 0.5;
        if(col > top)
          top_grow += 0.5;
        break;

    case 1:
          if(row < left)
            left_grow += 1;
          if(row > right)
            right_grow += 1;

          if(col < bottom)
            bottom_grow += 1;
          if(col > top)
            top_grow += 1;
          break;

    case 2:
        if(row < left)
          left_grow += 1.5;
        if(row > right)
          right_grow += 1.5;

        if(col < bottom)
          bottom_grow += 1.5;
        if(col > top)
          top_grow += 1.5;
        break;
  }

}

string island :: to_binary(int x)
{
  string ans;
  while(x > 0)
  {
    ans += to_string(x%2);
    x = x/2;
  }
  return ans;
}


int island :: grow_factor()
{
  int growth_factor = 0;
  int hThreat = horizontal_threat();
  int vThreat = vertical_threat();

  if(hThreat >= vThreat)
  {
    growth_factor += left_grow + right_grow;
  }

  else if(vThreat >= hThreat)
  {
    growth_factor += top_grow + bottom_grow;
  }
  return growth_factor;
}


void island :: print_island_stat()
{
  fprintf(fp, "island size = %d\n",elem_count);
  fprintf(fp, "horizontal_threat = %d \n",horizontal_threat());
  fprintf(fp, "vertical_threat = %d\n",vertical_threat());
  fprintf(fp, "parts = %s\n", to_binary(elem).c_str());
  fprintf(fp, "left_grow = %f\n",left_grow);
  fprintf(fp, "right_grow = %f\n",right_grow);
  fprintf(fp, "bottom_grow = %f\n",bottom_grow);
  fprintf(fp, "top_grow = %f\n",top_grow);
}
