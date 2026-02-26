#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"

using namespace std;

class Game {
public:
    Game(vector<Player*> players_in, Pack pack_in, int points_to_win_in, bool shuffle_in)
        : players(players_in), pack(pack_in), points_to_win(points_to_win_in), 
          shuffle(shuffle_in), team1_score(0), team2_score(0), dealer_idx(0) {}

    void play() {
        int num_hand = 0;
        while (team1_score < points_to_win && team2_score < points_to_win) {
            cout << "Hand " << num_hand << endl;
            play_hand();
            
            cout << players[0]->get_name() << " and " << players[2]->get_name()
                 << " have " << team1_score << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name()
                 << " have " << team2_score << " points" << endl << endl;

            dealer_idx = (dealer_idx + 1) % 4;
            num_hand++;
        }

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
    int team1_score; // Team 0: Players 0 & 2
    int team2_score; // Team 1: Players 1 & 3
    int dealer_idx;

    void deal(Card &upcard) {
        if (shuffle) { pack.shuffle(); } 
        else { pack.reset(); }

        // Exact 3-2-3-2 then 2-3-2-3 pattern from specs
        int batches[] = {3, 2, 3, 2, 2, 3, 2, 3};
        int current_player = (dealer_idx + 1) % 4;
        
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < batches[i]; ++j) {
                players[current_player]->add_card(pack.deal_one());
            }
            current_player = (current_player + 1) % 4;
        }
        
        upcard = pack.deal_one();
        cout << players[dealer_idx]->get_name() << " deals" << endl;
        cout << upcard << " turned up" << endl;
    }

    void play_hand() {
        Card upcard;
        deal(upcard);
        Suit trump;
        int orderer_idx = -1;

        // Round 1
        for (int i = 1; i <= 4; ++i) {
            int current = (dealer_idx + i) % 4;
            if (players[current]->make_trump(upcard, (current == dealer_idx), 1, trump)) {
                orderer_idx = current;
                cout << players[current]->get_name() << " orders up " << trump << endl << endl;
                players[dealer_idx]->add_and_discard(upcard);
                break;
            }
            cout << players[current]->get_name() << " passes" << endl;
        }

        // Round 2
        if (orderer_idx == -1) {
            for (int i = 1; i <= 4; ++i) {
                int current = (dealer_idx + i) % 4;
                if (players[current]->make_trump(upcard, (current == dealer_idx), 2, trump)) {
                    orderer_idx = current;
                    cout << players[current]->get_name() << " orders up " << trump << endl << endl;
                    break;
                }
                cout << players[current]->get_name() << " passes" << endl;
            }
        }
        play_tricks(trump, orderer_idx);
    }

    void play_tricks(Suit trump, int orderer_idx) {
        int team1_tricks = 0, team2_tricks = 0;
        int leader_idx = (dealer_idx + 1) % 4;

        for (int i = 0; i < 5; ++i) {
            Card led_card = players[leader_idx]->lead_card(trump);
            cout << led_card << " led by " << players[leader_idx]->get_name() << endl;
            
            Card highest = led_card;
            int winner_idx = leader_idx;

            for (int j = 1; j < 4; ++j) {
                int current = (leader_idx + j) % 4;
                Card played = players[current]->play_card(led_card, trump);
                cout << played << " played by " << players[current]->get_name() << endl;
                
                if (Card_less(highest, played, led_card, trump)) {
                    highest = played;
                    winner_idx = current;
                }
            }
            cout << players[winner_idx]->get_name() << " takes the trick" << endl << endl;
            leader_idx = winner_idx;
            (winner_idx % 2 == 0) ? team1_tricks++ : team2_tricks++;
        }
        score_hand(team1_tricks, team2_tricks, orderer_idx);
    }

    void score_hand(int t1_t, int t2_t, int orderer) {
        bool team1_ordered = (orderer == 0 || orderer == 2);
        if (t1_t > t2_t) {
            cout << players[0]->get_name() << " and " << players[2]->get_name() << " win the hand" << endl;
            if (team1_ordered) {
                if (t1_t == 5) { cout << "march!" << endl; team1_score += 2; }
                else { team1_score += 1; }
            } else { 
                cout << "euchred!" << endl; 
                team1_score += 2; 
            }
        } else {
            cout << players[1]->get_name() << " and " << players[3]->get_name() << " win the hand" << endl;
            if (!team1_ordered) {
                if (t2_t == 5) { cout << "march!" << endl; team2_score += 2; }
                else { team2_score += 1; }
            } else { 
                cout << "euchred!" << endl; 
                team2_score += 2; 
            }
        }
    }
};

int main(int argc, char **argv) {
    // 1. Check Argument Count
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    // 2. Extract and Validate Arguments
    string pack_filename = argv[1];
    string shuffle_arg = argv[2];
    int points_to_win = stoi(argv[3]);

    if ((shuffle_arg != "shuffle" && shuffle_arg != "noshuffle") || 
        (points_to_win < 1 || points_to_win > 100)) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    // 3. Open Pack File
    ifstream fin(pack_filename);
    if (!fin.is_open()) {
        cout << "Error opening " << pack_filename << endl;
        return 1;
    }

    // 4. Create Players and Validate Types
    vector<Player*> players;
    for (int i = 4; i < 12; i += 2) {
        string name = argv[i];
        string type = argv[i+1];
        if (type != "Simple" && type != "Human") {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                 << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                 << "NAME4 TYPE4" << endl;
            for (Player* p : players) delete p;
            return 1;
        }
        players.push_back(Player_factory(name, type));
    }

    // 5. Output Command Line Arguments
    for (int i = 0; i < argc; ++i) { cout << argv[i] << (i == argc - 1 ? "" : " "); }
    cout << " " << endl; // Spec says space at the end

    // 6. Run Game
    Pack pack(fin);
    bool do_shuffle = (shuffle_arg == "shuffle");
    Game game(players, pack, points_to_win, do_shuffle);
    game.play();

    // 7. Cleanup
    for (Player* p : players) { delete p; }
    return 0;