#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"

using namespace std;

class Game {
public:
    Game(vector<Player*> players_in, Pack pack_in, int p_win, bool shuff)
        : players(players_in), pack(pack_in), points_to_win(p_win), 
          shuffle(shuff), team1_score(0), team2_score(0), dealer_idx(0) {}

    void play() {
        int hand_num = 0;
        while (team1_score < points_to_win && team2_score < points_to_win) {
            cout << "Hand " << hand_num << endl;
            play_hand();
            cout << players[0]->get_name() << " and " << players[2]->get_name()
                 << " have " << team1_score << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name()
                 << " have " << team2_score << " points" << endl << endl;
            dealer_idx = (dealer_idx + 1) % 4;
            hand_num++;
        }
        announce_winner();
    }

private:
    vector<Player*> players;
    Pack pack;
    int points_to_win;
    bool shuffle;
    int team1_score, team2_score, dealer_idx;

    // Helper to shorten the play() function and handle long lines
    void announce_winner() {
        if (team1_score >= points_to_win) {
            cout << players[0]->get_name() << " and " 
                 << players[2]->get_name() << " win!" << endl;
        } else {
            cout << players[1]->get_name() << " and " 
                 << players[3]->get_name() << " win!" << endl;
        }
    }

    void deal(Card &upcard) {
        if (shuffle) pack.shuffle(); else pack.reset();
        int batches[] = {3, 2, 3, 2, 2, 3, 2, 3};
        int cur = (dealer_idx + 1) % 4;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < batches[i]; ++j) {
                players[cur]->add_card(pack.deal_one());
            }
            cur = (cur + 1) % 4;
        }
        upcard = pack.deal_one();
        cout << players[dealer_idx]->get_name() << " deals" << endl;
        cout << upcard << " turned up" << endl;
    }

    void play_hand() {
        Card upcard;
        deal(upcard);
        Suit trump;
        int orderer = -1;

        for (int round = 1; round <= 2 && orderer == -1; ++round) {
            for (int i = 1; i <= 4 && orderer == -1; ++i) {
                int p = (dealer_idx + i) % 4;
                if (players[p]->make_trump(upcard, (p == dealer_idx), round, trump)) {
                    orderer = p;
                    cout << players[p]->get_name() << " orders up " << trump 
                    << endl << endl;
                    if (round == 1) players[dealer_idx]->add_and_discard(upcard);
                } else {
                    cout << players[p]->get_name() << " passes" << endl;
                }
            }
        }
        play_tricks(trump, orderer);
    }

    void update_score(int t1_tricks, int orderer) {
        bool t1_ord = (orderer == 0 || orderer == 2);
        if (t1_tricks > 2) {
            cout << players[0]->get_name() << " and " 
                 << players[2]->get_name() << " win the hand" << endl;
            if (!t1_ord) { cout << "euchred!" << endl; team1_score += 2; }
            else if (t1_tricks == 5) { cout << "march!" << endl; team1_score += 2; }
            else team1_score += 1;
        } else {
            cout << players[1]->get_name() << " and " 
                 << players[3]->get_name() << " win the hand" << endl;
            if (t1_ord) { cout << "euchred!" << endl; team2_score += 2; }
            else if (t1_tricks == 0) { cout << "march!" << endl; team2_score += 2; }
            else team2_score += 1;
        }
    }

    void play_tricks(Suit trump, int orderer) {
        int t1_tricks = 0, leader = (dealer_idx + 1) % 4;
        for (int i = 0; i < 5; ++i) {
            Card led = players[leader]->lead_card(trump);
            cout << led << " led by " << players[leader]->get_name() << endl;
            Card high = led; 
            int winner = leader;
            for (int j = 1; j < 4; ++j) {
                int p = (leader + j) % 4;
                Card played = players[p]->play_card(led, trump);
                cout << played << " played by " << players[p]->get_name() << endl;
                if (Card_less(high, played, led, trump)) { 
                    high = played; 
                    winner = p; 
                }
            }
            cout << players[winner]->get_name() << " takes the trick" << endl << endl;
            leader = winner;
            if (winner % 2 == 0) t1_tricks++;
        }
        update_score(t1_tricks, orderer);
        cout << endl;
    }
};

int main(int argc, char **argv) {
    if (argc != 12 || (string(argv[2]) != "shuffle" && string(argv[2]) != "noshuffle")) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    for (int i = 0; i < argc; ++i) cout << argv[i] << " ";
    cout << endl;
    ifstream fin(argv[1]);
    if (!fin.is_open()) { 
        cout << "Error opening " << argv[1] << endl; 
        return 1; 
    }
    Pack pack(fin);
    vector<Player*> players;
    for (int i = 4; i < 12; i += 2) {
        players.push_back(Player_factory(argv[i], argv[i+1]));
    }
    
    Game game(players,
          pack,
          stoi(argv[3]),
          (string(argv[2]) == "shuffle"));
    game.play();
    for (Player* p : players) delete p;
    return 0;