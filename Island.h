#ifndef ISLAND_H
#define ISLAND_H
#include <iostream>
#include <vector>
using namespace std;

class island{
  public:
    int elem;
    int board_size;
    int bottom;
    int top;
    int left;
    int right;
    int elem_count;
    float left_grow;
    float right_grow;
    float bottom_grow;
    float top_grow;

    island(int board_size);
    int vertical_threat();
    int horizontal_threat();
    int offer(int i);
    string to_binary(int x);
    void print_island_stat();
    int update_growable(int square,int status);  // -1 capstone or wall but our capstone is not close , 0 capstone is near , 1 growable , 2 our square are near
    int grow_factor();

};

#endif
