#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Player.hpp"
#include "Pack.hpp"
using namespace std;

int main(int argc, char **argv) {
    // 1. Check argument count
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    // 2. getting basic arguments
    string pack_filename = argv[1];
    string shuffle_arg = argv[2];
    int points_to_win = stoi(argv[3]);

    // 3. Error check shuffle and points
    if ((shuffle_arg != "shuffle" && shuffle_arg != "noshuffle") || 
        (points_to_win < 1 || points_to_win > 100)) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }