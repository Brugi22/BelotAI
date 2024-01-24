#ifndef MODELS_H
#define MODELS_H

#include <vector>
#include <string>

enum Suit { SPADES, HEARTS, DIAMONDS, CLUBS };

class Card {
public:
    Card(int value, Suit suit);
    int getValue() const;
    Suit getSuit() const;

private:
    int value;
    Suit suit;
};

class Deck {
public:
    Deck();
    void shuffle();
    Card drawCard();

private:
    std::vector<Card> cards;
};

class Player {
public:
    Player(const std::string& name);
    void addToHand(const Card& card);
    const std::vector<Card>& getHand() const;
    const std::string getName() const;
    void removeFromHand(const Card& card);

private:
    std::string name;
    std::vector<Card> hand;
};

class BelaGame {
public:
    BelaGame(const std::string& player1, const std::string& player2, const std::string& player3, const std::string& player4, const int firstPlayer);
    const Player& getPlayer(int index) const;
    const std::vector<Player>& getAllPlayers() const;
    void startGame();

private:
    void dealCards();
    void chooseTrump();
    void declarations();
    void playGame();

    Deck deck;
    std::vector<Player> players;
    int firstPlayer;
    Suit trump;
};

#endif