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

    bool operator==(const Card& other) const {return value == other.value && suit == other.suit;}

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
    const std::vector<std::vector<Card>>& getDeclaration() const;
    const int getDeclarationValue() const;
    const std::string getName() const;
    void clearDeclarations();
    void removeFromHand(const Card& card);
    void sortHand();
    std::vector<Card> getAllCardOfSuit(Suit suit);
    void findDeclarations();

private:
    std::string name;
    std::vector<Card> hand;
    std::vector<std::vector<Card>> declaration;
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
    void playGame();

    Deck deck;
    std::vector<Player> players;    
    int firstPlayer;
    int points[2];                  // 0 and 2 (index 0)    vs     1 and 3 (index 1)
    int trumpTeam;
    Suit trump;
};

#endif