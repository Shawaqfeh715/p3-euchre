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

    // 4. Verify player types and create them
    vector<Player*> players;
    for (int i = 4; i < 12; i += 2) {
        string name = argv[i];
        string type = argv[i+1];
        if (type != "Simple" && type != "Human") {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                 << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                 << "NAME4 TYPE4" << endl;
            // Clean up any players already created before exiting
            for (Player* p : players) delete p;
            return 1;
        }
        players.push_back(Player_factory(name, type));
    }

    // 5. Check if pack file opens
    ifstream fin(pack_filename);
    if (!fin.is_open()) {
        cout << "Error opening " << pack_filename << endl;
        for (Player* p : players) delete p;
        return 1;
    }

    // Print arguments 
    for (int i = 0; i < argc; ++i) { cout << argv[i] << " "; }
    cout << endl;

    // TODO: Create game object and play
    
    // Cleanup
    for (Player* p : players) delete p;
    return 0;
}

class Game {
    public:
    Game(vector<Player*> players_in, Pack pack_in, int points_to_win_in,
         bool shuffle_in) : players(players_in), pack(pack_in),
         points_to_win(points_to_win_in), shuffle(shuffle_in),
         team1_score(0), team2_score(0), dealer_idx(0) {}

    void play() {
        int num_hand = 0;
        while (team1_score < points_to_win && team2_score < points_to_win) {
            cout << players[0]->get_name() << " and " << players[2]->get_name()
                 << " have " << team1_score << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name()
                 << " have " << team2_score << " points" << endl << endl;

            // preparing for the next hand
            dealer_idx = (dealer_idx + 1) % 4;
            num_hand++;
        }

        // announcing winner of the hand
        if (team1_score >= points_to_win) {
            cout << players[0]->get_name() << " and " << players[2]->get_name() << " win!" << endl;
        } else {
            cout << players[1]->get_name() << " and " << players[3]->get_name() << " win!" << endl;
        }
    }

    private:
    vector<Player*> players;
    Pack pack;
    int points_to_win;
    bool shuffle;
    int team1_score;
    int team2_score;
    int dealer_idx;

    void play_hand();

    void deal(Card &upcard);
    if (shuffle) {
        pack.shuffle();

    } else {
        pack.reset();
    }

    int batches_to_deal[] = {3, 2, 3, 2, 2, 3, 2, 3};
    int current_player = (dealer_idx + 1) % 4;

    for(int i = 0; i < 8; ++i) {
        for (int j = 0; j < batches_to_deal[i]; ++j) {
            players[current_player]->add_card(pack.deal_one());
        }
        current_player = (current_player + 1) % 4;
    }
    upcard = pack.deal_one();
    cout << players[dealer_idx]->get_name() << " deals" << endl;
    cout << upcard << " turned up" << endl;

    void Game::play_hand() {
    Card upcard;
    deal(upcard);

    Suit trump;
    int orderer_idx = -1; // Track who made trump (-1 means nobody yet)

    // Round 1
    for (int i = 1; i <= 4; ++i) {
        int current = (dealer_idx + i) % 4;
        if (players[current]->make_trump(upcard, (current == dealer_idx), 1, trump)) {
            orderer_idx = current;
            cout << players[current]->get_name() << " orders up " << trump << endl << endl;
            players[dealer_idx]->add_and_discard(upcard);
            break;
        } else {
            cout << players[current]->get_name() << " passes" << endl;
        }
    }

    // Round 2 (Only happens if everyone passed round 1)
    if (orderer_idx == -1) {
        for (int i = 1; i <= 4; ++i) {
            int current = (dealer_idx + i) % 4;
            if (players[current]->make_trump(upcard, (current == dealer_idx), 2, trump)) {
                orderer_idx = current;
                cout << players[current]->get_name() << " orders up " << trump << endl << endl;
                break;
            } else {
                cout << players[current]->get_name() << " passes" << endl;
            }
        }
    }

    // play_tricks(trump, orderer_idx); 
}
};