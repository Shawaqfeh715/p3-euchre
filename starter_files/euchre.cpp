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
                 << " have " << team1_score << " points" << endl << endl;
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
    int team1_score;
    int team2_score;
    int dealer_idx;

    void deal(Card &upcard) {
        if (shuffle) { pack.shuffle(); } 
        else { pack.reset(); }

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
        int t1_t = 0, t2_t = 0;
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
            (winner_idx % 2 == 0) ? t1_t++ : t2_t++;
        }
        
        bool t1_ord = (orderer_idx == 0 || orderer_idx == 2);
        if (t1_t > t2_t) {
            cout << players[0]->get_name() << " and " << players[2]->get_name() << " win the hand" << endl;
            if (t1_ord) {
                if (t1_t == 5) { cout << "march!" << endl; team1_score += 2; }
                else { team1_score += 1; }
            } else { cout << "euchred!" << endl; team1_score += 2; }
        } else {
            cout << players[1]->get_name() << " and " << players[3]->get_name() << " win the hand" << endl;
            if (!t1_ord) {
                if (t2_t == 5) { cout << "march!" << endl; team2_score += 2; }
                else { team2_score += 1; }
            } else { cout << "euchred!" << endl; team2_score += 2; }
        }
    }
};

int main(int argc, char **argv) {
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    string pack_fn = argv[1];
    string shuff_arg = argv[2];
    int p_win = stoi(argv[3]);
    if ((shuff_arg != "shuffle" && shuff_arg != "noshuffle") || (p_win < 1 || p_win > 100)) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    ifstream fin(pack_fn);
    if (!fin.is_open()) { cout << "Error opening " << pack_fn << endl; return 1; }
    vector<Player*> players;
    for (int i = 4; i < 12; i += 2) {
        string name = argv[i], type = argv[i+1];
        if (type != "Simple" && type != "Human") {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 NAME4 TYPE4" << endl;
            for (Player* p : players) delete p; return 1;
        }
        players.push_back(Player_factory(name, type));
    }
    for (int i = 0; i < argc; ++i) { cout << argv[i] << " "; }
    cout << endl;
    Pack pack(fin);
    Game game(players, pack, p_win, (shuff_arg == "shuffle"));
    game.play();
    for (Player* p : players) { delete p; }
    return 0;
}