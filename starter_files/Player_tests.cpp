#include "Player.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());
    delete alice;
}

TEST(test_simple_make_trump_round1_order_up) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(JACK, SPADES));   
    p->add_card(Card(JACK, CLUBS));    
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(TWO, DIAMONDS));
    p->add_card(Card(THREE, HEARTS));
    Card upcard(TEN, SPADES);
    Suit ordered;
    ASSERT_TRUE(p->make_trump(upcard, false, 1, ordered));
    ASSERT_EQUAL(ordered, SPADES);
    delete p;
}

TEST(test_simple_make_trump_round2_dealer_forced) {
    Player * p = Player_factory("Dealer", "Simple");
    p->add_card(Card(NINE, SPADES));
    Card upcard(NINE, DIAMONDS); 
    Suit ordered;
    ASSERT_TRUE(p->make_trump(upcard, true, 2, ordered));
    ASSERT_EQUAL(ordered, HEARTS); // Same color
    delete p;
}

TEST(test_play_card_left_bower_as_trump) {
    Player * p = Player_factory("Bob", "Simple");
    p->add_card(Card(JACK, DIAMONDS)); // Left Bower if Hearts are trump
    p->add_card(Card(NINE, CLUBS));
    Card led(ACE, DIAMONDS);
    Card played = p->play_card(led, HEARTS);
    ASSERT_EQUAL(played, Card(NINE, CLUBS)); // Shouldn't play Left Bower as Diamond
    delete p;
}

TEST(test_simple_player_add_and_discard_upcard_is_lowest) {
    Player * p = Player_factory("Dealer", "Simple");
    p->add_card(Card(TEN, DIAMONDS));
    p->add_card(Card(JACK, DIAMONDS));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(KING, DIAMONDS));
    p->add_card(Card(ACE, DIAMONDS));

    Card upcard(NINE, DIAMONDS); // Lowest card, same suit
    p->add_and_discard(upcard);
    
    // Check if the Nine was discarded by trying to lead everything
    for (int i = 0; i < 5; ++i) {
        Card led = p->lead_card(HEARTS);
        ASSERT_NOT_EQUAL(led, Card(NINE, DIAMONDS));
    }
    delete p;
}

TEST(test_simple_player_lead_card_highest_non_trump) {
    Player * p = Player_factory("Alice", "Simple");
    p->add_card(Card(NINE, SPADES)); // Trump
    p->add_card(Card(ACE, HEARTS));  // High non-trump
    Card led = p->lead_card(SPADES);
    ASSERT_EQUAL(led, Card(ACE, HEARTS));
    delete p;
}

TEST_MAIN()