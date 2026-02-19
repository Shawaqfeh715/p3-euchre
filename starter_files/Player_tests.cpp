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

TEST(test_simple_make_trump_round1_pass_not_enough_face_trump){
    Player *p=Player_factory("Alice","Simple");

    p->add_card(Card(ACE,SPADES));
    p->add_card(Card(NINE,SPADES));
    p->add_card(Card(TWO,HEARTS));
    p->add_card(Card(THREE,DIAMONDS));
    p->add_card(Card(FOUR,CLUBS));

    Card upcard(TEN,SPADES);
    Suit ordered;
    bool result=p->make_trump(upcard,false,1,ordered);

    ASSERT_FALSE(result);
    delete p;
}

TEST(test_play_card_cannot_follow_suit_plays_lowest){
    Player *p =Player_factory("Dave","Simple");
    p->add_card(Card(ACE,SPADES));
    p->add_card(Card(KING,CLUBS));
    p->add_card(Card(TWO,CLUBS));
    p->add_card(Card(THREE,DIAMONDS));
    p->add_card(Card(FOUR,CLUBS));

    Card led_card(TEN,HEARTS);
    Card played=p->play_card(led_card,SPADES);

    ASSERT_EQUAL(played,Card(TWO,CLUBS));
}

TEST(test_play_card_left_bower_cannot_follow_hearts){
    Player *p=Player_factory("Dave","Simple");
    p->add_card(Card(JACK,CLUBS));
    p->add_card(Card(TWO,DIAMONDS));
    p->add_card(Card(THREE,CLUBS));

    delete p;

    p=Player_factory("Dave","Simple");
    p->add_card(Card(JACK,DIAMONDS));
    p->add_card(Card(TWO,SPADES));
    p->add_card(Card(THREE,CLUBS));
    p->add_card(Card(FOUR,SPADES));
    p->add_card(Card(FIVE,CLUBS));

    Card led_card(TEN,SPADES);
    Card played=p->play_card(led_card,HEARTS);

    ASSERT_EQUAL(played,Card(FOUR,SPADES));
    delete p;
}

TEST(test_simple_player_screw_the_dealer) {
    Player * alice = Player_factory("Alice", "Simple");
    // Give Alice 5 cards that are NOT Diamonds
    alice->add_card(Card(NINE, SPADES));
    alice->add_card(Card(TEN, SPADES));
    alice->add_card(Card(NINE, CLUBS));
    alice->add_card(Card(TEN, CLUBS));
    alice->add_card(Card(NINE, HEARTS));

    Card upcard(NINE, DIAMONDS); // Round 2 candidate will be hearts
    Suit ordered;
    // Round 2, Alice is the dealer
    bool result = alice->make_trump(upcard, true, 2, ordered);

    ASSERT_TRUE(result);
    ASSERT_EQUAL(ordered, HEARTS);
    delete alice;
}

TEST(test_simple_player_lead_highest_non_trump) {
    Player * bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, SPADES));  // trump
    bob->add_card(Card(TEN, SPADES));  // trump
    bob->add_card(Card(ACE, HEARTS));  // high non-trump
    bob->add_card(Card(NINE, CLUBS));  // low non-trump
    bob->add_card(Card(TEN, CLUBS));

    // Spades are trump so Bob should lead with  ace of hearts.
    Card led = bob->lead_card(SPADES);
    ASSERT_EQUAL(led, Card(ACE, HEARTS));
    delete bob;
}

TEST_MAIN()
