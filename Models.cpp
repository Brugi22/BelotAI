#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <random>
#include <set>
#include "Models.h"

static const std::map<std::string, Suit> suitMap = {
    {"SPADES", SPADES},
    {"HEARTS", HEARTS},
    {"DIAMONDS", DIAMONDS},
    {"CLUBS", CLUBS}
};

static const std::map<int, int> valueDeclaration = {
    {9, 150},
    {10, 100},
    {11, 200},
    {12, 100},
    {13, 100},
    {14, 100}
};

Card::Card(int value, Suit suit) : value(value), suit(suit) {}

int Card::getValue() const { return value; }

Suit Card::getSuit() const { return suit; }

Deck::Deck() {
    shuffle();
    for (int i = 7; i <= 14; ++i) {
        cards.push_back(Card(i, SPADES));
        cards.push_back(Card(i, HEARTS));
        cards.push_back(Card(i, DIAMONDS));
        cards.push_back(Card(i, CLUBS));
    }
}

void Deck::shuffle() {
    std::random_device rd;  // Use hardware entropy to generate a random seed
    std::mt19937 gen(rd()); // Standard Mersenne Twister engine
    std::shuffle(cards.begin(), cards.end(), gen);
}

Card Deck::drawCard() {
    Card drawnCard = cards.back();
    cards.pop_back();
    return drawnCard;
}

Player::Player(const std::string& name) : name(name) {}

void Player::addToHand(const Card& card) {
    hand.push_back(card);
}

const std::vector<Card>& Player::getHand() const {
    return hand;
}

const std::string Player::getName() const {
    return name;
}

void Player::removeFromHand(const Card& card) {
    std::vector<Card>::iterator it = std::find(hand.begin(), hand.end(), card);
    if(it != hand.end()) {
        hand.erase(it);
    }
}

void Player::sortHand() {
    std::sort(hand.begin(), hand.end(), [](const Card& a, const Card& b) {
        if (a.getSuit() < b.getSuit()) {
            return true;
        } else if (a.getSuit() > b.getSuit()) {
            return false;
        }

        return a.getValue() < b.getValue();
    });
}

int Player::declaration() {

}

BelaGame::BelaGame(const std::string& player1, const std::string& player2, const std::string& player3, const std::string& player4, const int firstPlayer)
    :  deck(), players{Player(player1), Player(player2), Player(player3), Player(player4)}, firstPlayer(firstPlayer), points{0, 0} {}

const Player& BelaGame::getPlayer(int index) const {
    return players[index];
}

const std::vector<Player>& BelaGame::getAllPlayers() const {
    return players;
}

void BelaGame::startGame() {
    dealCards();
    sortHands();
    chooseTrump();
    declarations();
    playGame();
}

void BelaGame::dealCards() {
    for (int i = 0; i < 8; i++) {
        for(int j = 0; j < 4; j++) {
            players[i].addToHand(deck.drawCard());
        }
    }
}

void BelaGame::sortHands() {
    for(int i = 0; i < 4; i++) {
        players[i].sortHand();
    }
}

void BelaGame::chooseTrump() {
    std::string inputTrump;
    std::vector<std::string> validInput = {"PASS", "SPADES", "HEARTS", "DIAMONDS", "CLUBS"};

    for(int i = 0 ; i < 4; i++) {
        std::cout << "Player " << players[(firstPlayer+i)%4].getName() << " is choosing trump" << std::endl;

        while(true) {
            if(i != 3) std::cout << "Choose: PASS, SPADES, HEARTS, DIAMONDS, CLUBS" << std::endl;
            else {
                validInput.erase(std::remove(validInput.begin(), validInput.end(), "PASS"), validInput.end());
                std::cout << "Choose: SPADES, HEARTS, DIAMONDS, CLUBS" << std::endl;
            }
            std::cin >> inputTrump;
            if(std::find(validInput.begin(), validInput.end(), inputTrump) == validInput.end()) {
                std::cout << "Not a valid input, try again";
                continue;
            }
            if(inputTrump == "PASS") break;
            else {
                trump = suitMap.at(inputTrump);
                trumpTeam = i % 2;
                return;
            }
        }
    }
}

void BelaGame::declarations() {
    for(int i = 0; i < 4; i++) {
        std::cout << "Player " << players[(firstPlayer + i) % 4].getName() << " is declaring" << std::endl;

        //4 same cards

        const std::vector<Card>& hand = players[(firstPlayer + i) % 4].getHand();

        auto isCardWithValue = [&hand](int value) {
            return std::any_of(hand.begin(), hand.end(), [value](const Card& c) { return c.getValue() == value; });
        };

        std::vector<int> valuesToCheck = { 9, 10, 11, 12, 13, 14 };
        std::set<Card> jokers;

        for (int value : valuesToCheck) {
            if (isCardWithValue(value)) {
                std::copy_if(hand.begin(), hand.end(), std::inserter(jokers, jokers.end()), [value](const Card& c) { return c.getValue() == value; });
            }
            if (jokers.size() == 4) {
                players[(firstPlayer + i) % 4].declarationValue = valueDeclaration.at(value);
            }
            jokers.clear();
        }

        //cards in a row

    }
}

void BelaGame::playGame() {
    
}
