#include <iostream>
#include <vector>
#include <algorithm>

enum Suit { SPADES, HEARTS, DIAMONDS, CLUBS };

class Card {
public:
    Card(int value, Suit suit) : value(value), suit(suit) {}

    int getValue() const { return value; }
    Suit getSuit() const { return suit; }

private:
    int value;
    Suit suit;
};

class Deck {
public:
    Deck() {
        for (int i = 7; i <= 14; ++i) {
            cards.push_back(Card(i, SPADES));
            cards.push_back(Card(i, HEARTS));
            cards.push_back(Card(i, DIAMONDS));
            cards.push_back(Card(i, CLUBS));
        }
    }

    void shuffle() {
        std::random_shuffle(cards.begin(), cards.end());
    }

    Card drawCard() {
        Card drawnCard = cards.back();
        cards.pop_back();
        return drawnCard;
    }

private:
    std::vector<Card> cards;
};

class Player {
public:
    Player(const std::string& name) : name(name) {}

    void addToHand(const Card& card) {
        hand.push_back(card);
    }

    const std::vector<Card>& getHand() const {
        return hand;
    }

    const std::string getName() const {
        return name;
    }

    void removeFromHand(const Card& card) {
        std::vector<Card>::iterator it = std::find(hand.begin(), hand.end(), card);
        if(it != hand.end()) {
            hand.erase(it);
        }
    }

    int calculatePoints() const {
        int points = 0;
        for (const auto& card : hand) {
            points += card.getValue();
        }
        return points;
    }

private:
    std::string name;
    std::vector<Card> hand;
};

class BelaGame {
public:
    BelaGame(const std::string& player1, const std::string& player2)
        : player1(player1), player2(player2) {}

    void startGame() {
        deck.shuffle();
        dealCards();
        playGame();
        displayWinner();
    }

private:
    void dealCards() {
        for (int i = 0; i < 8; ++i) {
            player1.addToHand(deck.drawCard());
            player2.addToHand(deck.drawCard());
        }
    }

    void playGame() {
    }

    void displayWinner() {
        int points1 = player1.calculatePoints();
        int points2 = player2.calculatePoints();

        std::cout << "Game Over\n";
        std::cout << player1.getName() << " points: " << points1 << "\n";
        std::cout << player2.getName() << " points: " << points2 << "\n";

        if (points1 > points2)
            std::cout << player1.getName() << " wins!\n";
        else if (points2 > points1)
            std::cout << player2.getName() << " wins!\n";
        else
            std::cout << "It's a tie!\n";
    }

private:
    Deck deck;
    Player player1;
    Player player2;
};