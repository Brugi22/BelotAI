#ifndef MODELS_H
#define MODELS_H

#include <vector>
#include <string>
#include <map>
#include "Prozor.h"

enum Suit { SPADES, HEARTS, DIAMONDS, CLUBS };

static const std::map<std::string, Suit> suitMap = {
    {"SPADES", SPADES},
    {"HEARTS", HEARTS},
    {"DIAMONDS", DIAMONDS},
    {"CLUBS", CLUBS}
};

static const std::map<Suit, std::string> suitMapReverse = {
    {SPADES, "SPADES"},
    {HEARTS, "HEARTS"},
    {DIAMONDS, "DIAMONDS"},
    {CLUBS, "CLUBS"}
};

static const std::map<int, std::string> valueMapReverse = {
    {0, "TEST"},
    {7, "7"},
    {8, "8"},
    {9, "9"},
    {10, "10"},
    {11, "JACK"},
    {12, "QUEEN"},
    {13, "KING"},
    {14, "ACE"}
};

static const std::map<int, int> valueDeclaration = {
    {3, 20},
    {4, 50},
    {5, 100},
    {9, 150},
    {10, 100},
    {11, 200},
    {12, 100},
    {13, 100},
    {14, 100}
};

class Card {
public:
    Card(int value, Suit suit);
    Card(int n);
    int getValue() const;
    Suit getSuit() const;

    bool operator==(const Card& other) const { return value == other.value && suit == other.suit; }

    void Render(Prozor*, int n, bool t);
    void setSprite(int n);

private:
    int value;
    Suit suit;
    sf::Texture texture;
    sf::Sprite card;
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
    const std::vector<std::vector<Card>>& getDeclaration() const;
    const int getDeclarationValue() const;
    const std::vector<std::vector<Card>>& getInfo() const;
    const std::string getName() const;
    void removeFromHand(Card card);
    void sortHand();
    void updateInfo(int playerIndex, Card card);
    std::vector<Card> getAllCardOfSuit(Suit suit);
    void findDeclarations();
    void clearDeclarations();
    void displayDeclarations();
    void checkBela(Suit trump);
    void initializeInfo();

private:
    std::string name;
    std::vector<Card> hand;
    std::vector<std::vector<Card>> declaration;
    std::vector<std::vector<Card>> info;
    int declarationValue;
};

class BelaGame {
public:
    BelaGame(const std::string& player1, const std::string& player2, const std::string& player3, const std::string& player4, const int firstPlayer);
    const Player& getPlayer(int index) const;
    const std::vector<Player>& getAllPlayers() const;
    void startGame();

private:
    void dealCards();
    void sortHands();
    void chooseTrump();
    void declarations();
    void clearDeclarations(int playerIndex);
    void processDeclarations(int currentPlayer, bool& found4Same, bool& foundDeclaration, std::pair<int, int>& strongestDeclaration, int& strongestIndexPlayer);
    void removePlayedCardsFromInfo(std::vector<Card> roundCards);
    void initializeInfo();
    void playGame();
    void renderiraj();
    void sort();

    Deck deck;
    std::vector<Player> players;
    int firstPlayer;
    int points[2];                  // 0 and 2 (index 0)    vs     1 and 3 (index 1)
    int roundsWon[2];
    int trumpTeam;
    Suit trump;
    std::vector<int> allCards;
    Prozor P;
    bool trump_chosen;
};

#endif