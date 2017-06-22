#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <cstdio>
#include <vector>
#include <cctype>
#include <time.h>
#include <stdlib.h>
#include "Island.h"
#include <set>

using namespace std;

class Game{
  public:

    class Player{
      public:
        int flats;
        int capstones;
        int walls;
        int upper_flat;
        int hard_flats[3] = {0,0,0};
        int soft_flats[3] = {0,0,0};
        int capstone_pos;
        Player(int flats,int capstones)
        {
          this->flats = flats;
          this->capstones = capstones;
          this->walls = 0;
          this->upper_flat = 0;
          capstone_pos = 50000;
        }
    };
    set<int> helper_set;
    int free_square;
    int road_winner;
    int flat_winner;
    int island_score;
    int board_len;
    int max_depth;
    int playerID;
    int total_squares;
    vector<vector<pair<int,char> > > board;
    int turn;
    int max_movable;
    int max_flats;
    int max_capstones;
    int max_down;
    int max_up;
    char max_left;
    char max_right;
    int moves;
    vector<Player>players;
    vector<string> all_squares;
    Game(int board_len,int playerID);
    Game();
    int square_to_num(string square_string);
    string square_to_string(int square);
    void execute_move(string move_string);
    vector<vector<int> > partition(int board_len);
    bool check_valid(int square, char direction, vector<int> &partition);
    vector<string> generate_stack_moves(int square);
    vector<string> generate_all_moves(int player);
    Game get_successor(string move);
    pair<int,string> minVal(int alpha,int beta,int depth);
    pair<int,string> maxVal(int alpha,int beta,int depth);
    bool terminal();
    int terminalUtility();
    void alpha_beta_play();
    vector<island> find_island (int player_num);
    vector<int> nbr_square(int square);
    void do_island_dfs(int square,island& myisland,vector<bool> &visited,int player_num);
    int heuristic_evaluation();
    void print_statistics();
    unsigned int getFirstSetBitPos(int n);
    void handle_island_growth(island& myisland,set<int>nbrs,int player_num);
    pair<int,int> get_flatstone_weight();
};

#endif
