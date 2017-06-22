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

Game :: Game()
{

}

Game :: Game(int board_len,int playerID)
{
  this->board_len = board_len;
  this->playerID = playerID;
  total_squares = board_len*board_len;
  free_square = total_squares;
  road_winner = -1;
  flat_winner = -1;
  island_score =0;
  board = vector<vector<pair<int,char> > > (total_squares);
  turn = 0;
  switch (board_len) {
    case 5:
            max_flats = 21;
            max_capstones = 1;
            break;
    case 6:
            max_flats = 30;
            max_capstones = 1;
            break;
    case 7:
            max_flats = 40;
            max_capstones = 1;
            break;
    default:
            max_flats = 21;
            max_capstones = 1;
    break;
  }
  max_movable = board_len;
  max_depth = 3;
  max_down = 1;
  max_up = board_len;
  max_left = 'a';
  max_right = (char)('a'+ board_len-1);
  moves = 0;
  players.push_back(Player(max_flats,max_capstones));
  players.push_back(Player(max_flats,max_capstones));
  all_squares = vector<string>(total_squares);
  for(int i =0;i<total_squares;i++)
  {
    all_squares[i] = square_to_string(i);
  }
}

int Game :: square_to_num(string square_string)
{
  if(square_string.length() != 2)
    return -1;
  if( !isalpha(square_string[0]) || !islower(square_string[0]) || !isdigit(square_string[1]) )
    return -1;
  int row = (int)square_string[0] - 96;
  int col = (int)square_string[1] - '0';
  if(row < 1 || row > board_len || col < 1 || col > board_len)
    return -1;
  return board_len * (col-1) + (row-1);
}



string Game :: square_to_string(int square)
{
  string s;
  if(square < 0 || square >= total_squares)
    return s;
  int row = square % board_len;
  int col = square / board_len;
  s += (char)(row+97);
  s += to_string(col+1);
  return s;
}



void Game :: execute_move(string move_string)
{
  int owner;
  int current_piece;
  if(turn == 0)
    moves += 1;
  if(moves != 1)
    current_piece = turn;
  else
    current_piece = 1 - turn;
  if(isalpha(move_string[0]))
  {
    free_square--;
    int square = square_to_num(move_string.substr(1));
    if(move_string[0] == 'F' || move_string[0] == 'S')
    {
      board[square].push_back({current_piece,move_string[0]});
      players[current_piece].flats -= 1;
      if(move_string[0] == 'S')
      {
        players[current_piece].walls += 1;
      }
      else
      {
          players[current_piece].upper_flat += 1;
      }
    }
    else if(move_string[0] == 'C')
    {
      board[square].push_back({current_piece,move_string[0]});
      players[current_piece].capstone_pos = square;
      players[current_piece].capstones -= 1;
      players[current_piece].upper_flat += 1;
    }
  }

  else if(isdigit(move_string[0]))
  {
    int count = (int)(move_string[0] - '0');
    int square = square_to_num(move_string.substr(1,2));
    char direction = move_string[3];
    int change;
    switch (direction)
    {
      case '+'  :
                    change = board_len;
                    break;
      case '-' :
                    change = -board_len;
                    break;
      case '>' :
                    change = 1;
                    break;
      case '<' :
                    change = -1;
                    break;
    }
    int prev_square = square;
    vector<int> stack_pos;
    stack_pos.push_back(square);
    for(int i =4;i<move_string.length();i++)
    {
      int next_square = prev_square +change;
      stack_pos.push_back(next_square);
      prev_square = next_square;
    }

    for(vector<int>::iterator it = stack_pos.begin(); it != stack_pos.end(); it++)
    {
      if(board[*it].size() > 0)
      {
        int pos_owner = board[*it][board[*it].size()-1].first;
        int type;
        switch (board[*it][board[*it].size()-1].second)
        {
          case 'F':
            type =0;
            break;
          case 'S':
            type = 1;
            break;
          case 'C':
            type = 2;
            break;
        }
        for(int i = 0;i<board[*it].size()-1;i++)
        {
          if(board[*it][i].first == pos_owner)
          {
            players[pos_owner].soft_flats[type]--;
          }
          else
          {
            players[1 - pos_owner].hard_flats[type]--;
          }
        }
      }
    }

    prev_square = square;
    for(int i=4;i<move_string.length();i++)
    {

      int next_count = (int)(move_string[i]-'0');
      int next_square = prev_square + change;
      if(board[next_square].size() == 0)
        free_square--;
      if(board[next_square].size() > 0  && board[next_square][board[next_square].size()-1].second == 'S' )
      {
        players[current_piece].capstone_pos = next_square;
        owner = board[next_square][board[next_square].size()-1].first;
        board[next_square][board[next_square].size()-1].second = 'F';
        players[owner].upper_flat += 1;
        players[owner].walls -= 1;
      }
      if(next_count - count == 0)
      {
        if(board[next_square].size() > 0)
        {
          owner =  board[next_square][board[next_square].size()-1].first;
          players[owner].upper_flat -= 1;
        }

        for(int k = board[square].size()-count; k < board[square].size() ;k++)
        {
          board[next_square].push_back(board[square][k]);
        }
        if(board[next_square][board[next_square].size()-1].second == 'C')
          players[current_piece].capstone_pos = next_square;

        owner =  board[next_square][board[next_square].size()-1].first;
        switch (board[next_square][board[next_square].size()-1].second)
        {
          case 'F':
                  players[owner].upper_flat += 1;
                  break;
          case 'S':
                  players[owner].walls += 1;
                  break;
          default :
                  break;
        }
      }

      else
      {
        if(board[next_square].size() > 0)
        {
          owner =  board[next_square][board[next_square].size()-1].first;
          players[owner].upper_flat -= 1;
        }

        for(int k = board[square].size()-count; k < board[square].size()-count+next_count;k++)
        {
          board[next_square].push_back(board[square][k]);
        }

        owner =  board[next_square][board[next_square].size()-1].first;
        players[owner].upper_flat += 1;
      }
      prev_square = next_square;
      count -= next_count;
    }
    count = (int)(move_string[0]-'0');
    owner = board[square][board[square].size()-1].first;
    switch (board[square][board[square].size()-1].second)
    {
      case 'F':
              players[owner].upper_flat -= 1;
              break;
      case 'S':
              players[owner].walls -= 1;
              break;
      default:
              break;
    }

    board[square].erase(board[square].end()-count,board[square].end());
    if(board[square].size() == 0)
      free_square++;
    if(board[square].size() > 0)
    {
      owner = board[square][board[square].size()-1].first;
      switch (board[square][board[square].size()-1].second)
      {
        case 'F':
                players[owner].upper_flat += 1;
                break;
        case 'S':
                players[owner].walls += 1;
                break;
        default:
                break;
      }
    }

    for(vector<int>::iterator it = stack_pos.begin(); it != stack_pos.end(); it++)
    {
      if(board[*it].size() > 0)
      {
        int type;
        switch (board[*it][board[*it].size()-1].second)
        {
          case 'F':
            type =0;
            break;
          case 'S':
            type = 1;
            break;
          case 'C':
            type = 2;
            break;
        }
        int pos_owner = board[*it][board[*it].size()-1].first;
        for(int i = 0;i<board[*it].size()-1;i++)
        {
          if(board[*it][i].first == pos_owner)
          {
            players[pos_owner].soft_flats[type]++;
          }

          else
          {
            players[1 - pos_owner].hard_flats[type]++;
          }

        }
      }
    }

  }
  turn = 1-turn;

}



vector<vector<int> > Game :: partition(int n)
{
  vector<vector<int> > part_list;
  part_list.push_back(vector<int>(1,n));
  for(int x = 1;x < n; x++)
  {
    vector<vector<int> > temp_partition = partition(n-x);
    for(int y =0; y < temp_partition.size(); y++)
    {
      temp_partition[y].insert(temp_partition[y].begin(),x);
      part_list.push_back(temp_partition[y]);
    }
  }
  return part_list;
}

bool Game :: check_valid(int square, char direction, vector<int> &partition)
{
  int change;
  if (direction == '+')
    change = board_len;
  else if (direction == '-')
    change = -board_len;
  else if (direction == '>')
    change = 1;
  else if (direction == '<')
    change = -1;

  for(int i =0;i<partition.size();i++)
  {
    int next_square = square + change * (i + 1);
    if(board[next_square].size() > 0 && board[next_square][board[next_square].size()-1].second == 'C')
      return false;
    if(board[next_square].size() > 0 && board[next_square][board[next_square].size()-1].second == 'S' && i != (partition.size()-1))
      return false;
    if(i == partition.size()-1 && board[next_square].size() > 0 && board[next_square][board[next_square].size()-1].second == 'S' && partition[i] > 1 )
      return false;
    if(i == partition.size()-1 && board[next_square].size() > 0 && board[next_square][board[next_square].size()-1].second == 'S' && board[square][board[square].size()-1].second != 'C' )
      return false;
  }
  return true;
}

vector<string> Game :: generate_stack_moves(int square)
{
  vector<string> all_moves;
  int r = square % board_len;
  int c = square / board_len;
  int size = board[square].size();
  char dirs[4] = {'+','-','<','>'};
  int up = board_len - 1 - c;
  int down = c;
  int right = board_len - 1 - r;
  int left = r;
  int rem_squares[4] = {up,down,left,right};
  int lifted_pieces = min(size,board_len);
  for(int num =0; num < lifted_pieces;num++)
  {
    vector<vector<int> > part_list = partition(num+1);
    for(int di = 0;di<4;di++)
    {
      vector<vector<int> > part_dir;
      for(int part =0;part < part_list.size();part++)
      {
        if(part_list[part].size() <= rem_squares[di])
          part_dir.push_back(part_list[part]);
      }

      for (size_t part = 0; part < part_dir.size(); part++)
      {
        if(check_valid(square,dirs[di],part_dir[part]))
        {
          string part_string;
          for (size_t i = 0; i < part_dir[part].size(); i++)
          {
            part_string += to_string(part_dir[part][i]);
          }
          string temp;
          temp += to_string(std::accumulate(part_dir[part].begin(), part_dir[part].end(), 0));
          temp += all_squares[square];
          temp += dirs[di];
          temp += part_string;

          all_moves.push_back(temp);
        }
      }
    }
  }
  return all_moves;

}

vector<string> Game :: generate_all_moves(int player)
{
  vector<string> all_moves;
  for (size_t i = 0; i < board.size(); i++)
  {
    if(board[i].size() == 0)
    {
      if(players[player].flats > 0)
      {
        string temp;
        temp += 'F';
        temp += all_squares[i];
        all_moves.push_back(temp);
      }

      if(moves != player  && players[player].flats > 0)
      {
        string temp;
        temp += 'S';
        temp += all_squares[i];
        all_moves.push_back(temp);
      }
      if(moves != player && players[player].capstones > 0)
      {
        string temp;
        temp += 'C';
        temp += all_squares[i];
        all_moves.push_back(temp);
      }
    }
  }
  for (size_t i = 0; i < board.size(); i++)
  {
    if(board[i].size() > 0 && board[i][board[i].size()-1].first == player && this->moves != player)
    {
      vector<string> stack_moves = generate_stack_moves(i);
      all_moves.reserve(all_moves.size() + stack_moves.size());
      all_moves.insert(all_moves.end(),stack_moves.begin(),stack_moves.end());
    }
  }
  return all_moves;
}

Game Game :: get_successor(string move)
{
  Game successor = *this;
  successor.execute_move(move);
  return successor;
}

pair<int,string> Game :: minVal(int alpha,int beta,int depth)
{
    if(depth ==0 || terminal())
    {
      return {heuristic_evaluation(),""};
    }

    pair<int,string> best_child = {INT_MAX,""};
    vector<string> all_moves = generate_all_moves(1-playerID);
    best_child.second = all_moves[0];
    random_shuffle(all_moves.begin(),all_moves.end());
    for(int i = 0;i<all_moves.size();i++)
    {
      Game successor_child = get_successor(all_moves[i]);
      pair<int,string> child = successor_child.maxVal(alpha,beta,depth-1);
      child.second = all_moves[i];
      if(child.first < best_child.first)
        best_child = child;

      beta = min(beta,child.first);
      if(alpha >= beta)
        return child;
    }
    return best_child;
}
pair<int,string> Game :: maxVal(int alpha,int beta,int depth)
{
    if(depth ==0 || terminal())
    {
      return {heuristic_evaluation(),""};
    }

    pair<int,string> best_child = {INT_MIN,""};
    vector<string> all_moves = generate_all_moves(playerID);
    random_shuffle(all_moves.begin(),all_moves.end());
    best_child.second = all_moves[0];

    for(int i = 0;i<all_moves.size();i++)
    {
      Game successor_child = get_successor(all_moves[i]);
      pair<int,string> child = successor_child.minVal(alpha,beta,depth-1);


      child.second = all_moves[i];
      if(child.first > best_child.first)
        best_child = child;

      alpha = max(alpha,child.first);
      if(alpha >= beta)
        return child;
    }
    return best_child;
}


vector<island> Game :: find_island(int player_num)
{
  vector<island> total_island;
  vector<bool> visited(board_len*board_len,false);
  for (size_t i = 0; i < visited.size(); i++)
  {
    if(board[i].size() == 0)
    {
      continue;
    }
    else if((visited[i] == false) && (board[i][board[i].size()-1].first == player_num) && (board[i][board[i].size()-1].second != 'S'))
    {
      island new_found(board_len);
      new_found.offer(i);
      do_island_dfs(i,new_found,visited,player_num);
      handle_island_growth(new_found,helper_set,player_num);
      total_island.push_back(new_found);
      helper_set.clear();
    }

  }
  return total_island;
}

void Game :: do_island_dfs(int square,island& myisland,vector<bool> &visited,int player_num)
{
  visited[square] = true;
  vector<int> childs = nbr_square(square);
  for (size_t i = 0; i < childs.size(); i++)
  {
    int next_square = childs[i];

    if(board[next_square].size() == 0)
    {
      helper_set.insert(next_square);
      continue;
    }

    if((visited[next_square] == false) && (board[next_square][board[next_square].size()-1].first == player_num) && (board[next_square][board[next_square].size()-1].second != 'S'))
    {
      myisland.offer(next_square);
      do_island_dfs(next_square,myisland,visited,player_num);
    }
    else if(!(myisland.elem & (1<<next_square)) )
    {
      helper_set.insert(next_square);
    }
  }
}


void Game :: handle_island_growth(island& myisland,set<int>nbrs,int player_num)
{
  for(set<int>::iterator it = nbrs.begin(); it != nbrs.end();it++)
  {
    if(!(myisland.elem & 1<<(*it)) )  // potential nbr is not in island itself
    {
      int status = 5;
      if(board[*it].size() == 0)  // empty square;
      {
        vector<int> next_nbrs = nbr_square(*it);
        for(int i = 0;i<next_nbrs.size();i++)
        {
          int temp = next_nbrs[i];
          if( (board[temp].size() > 0) && (board[temp][board[temp].size()-1].first == player_num)  && (!(myisland.elem & 1<<temp)) )
          {
              status = 2;
          }
          else
            status = 1;
        }
      }
      else if(board[*it].size() > 0 && board[*it][board[*it].size()-1].second == 'C')
          status = -1;
      else if(board[*it].size() > 0 && board[*it][board[*it].size()-1].second == 'S')
      {
      	if((abs(*it-players[player_num].capstone_pos) == board_len || abs(*it-players[player_num].capstone_pos) == 1) )
        	status = 0;
        else
        	status = -1;
      }
      if(status != 5)
        myisland.update_growable(*it,status);
    }
  }
}

unsigned int Game :: getFirstSetBitPos(int n)
{
   return log2(n&-n)+1;
}

vector<int> Game :: nbr_square(int square)
{
  vector<int> childs;
  int row = square % board_len;
  int col = square / board_len;
    if(row == 0)
    {
      childs.push_back(col*board_len + row + 1);
    }
    else if(row == board_len -1)
    {
      childs.push_back(col*board_len + row - 1);
    }

    else
    {
      childs.push_back(col*board_len + row - 1);
      childs.push_back(col*board_len + row + 1);
    }

    if(col == 0)
    {
      childs.push_back((col+1)*board_len + row);
    }
    else if(col == board_len -1)
    {
      childs.push_back((col-1)*board_len + row);
    }

    else
    {
      childs.push_back((col-1)*board_len + row);
      childs.push_back((col+1)*board_len + row);
    }

    return childs;
}

void Game :: print_statistics()
{
  fprintf(fp, "My walls = %d \n",players[playerID].walls);
  fprintf(fp, "My upper_flat = % d\n",players[playerID].upper_flat );
  fprintf(fp, "Enemy walls = %d \n",players[1-playerID].walls);
  fprintf(fp, "Enemy upper_flat = % d\n",players[1-playerID].upper_flat );

  vector<island> myisland = find_island(playerID);
  fprintf(fp, "My island\n");
  for (int i = 0; i < myisland.size(); i++)
  {
    myisland[i].print_island_stat();
  }

  vector<island> enemyisland = find_island(playerID);
  fprintf(fp, "enemyisland \n");
  for (int i = 0; i < enemyisland.size(); i++)
  {
    enemyisland[i].print_island_stat();
  }
}

pair<int,int> Game :: get_flatstone_weight()
{
  int threshold = END_GAME_FLATSTONE_THRESHOLD[board_len];
  int mypos = min(players[playerID].upper_flat + 1 - players[playerID].capstones,threshold);
  int enemypos = min(players[1-playerID].upper_flat + 1 - players[1-playerID].capstones,threshold);

  pair<int,int> weights;
  weights.first = (flat_weight[0]*mypos)/threshold + (flat_weight[1]*(threshold-mypos))/threshold;
  weights.second = (flat_weight[0]*enemypos)/threshold + (flat_weight[1]*(threshold-enemypos))/threshold;

  return weights;
}
