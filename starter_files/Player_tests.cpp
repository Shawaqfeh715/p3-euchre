#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

// Add more tests here
TEST(test_simple_make_trump_round1_order_up){
    Player *p= Player_factory("Alice","Simple");
    p->add_card(Card(JACK,SPADES));
    p->add_card(Card(JACK,CLUBS));
    p->add_card(Card(ACE,HEARTS));
    p->add_card(Card(TWO,DIAMONDS));
    p->add_card(Card(THREE,HEARTS));

    Card upcard(TEN,SPADES);
    Suit ordered;
    bool result=p->make_trump(upcard,false,1,ordered);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(ordered,SPADES);
    delete p;
}

TEST_MAIN()
