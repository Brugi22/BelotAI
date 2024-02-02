#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <random>
#include <utility>
#include "Models.h"
#include "GameRules.h"

static const std::map<std::string, Suit> suitMap = {
    {"SPADES", SPADES},
    {"HEARTS", HEARTS},
    {"DIAMONDS", DIAMONDS},
    {"CLUBS", CLUBS}
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
    std::random_device rd;  
    std::mt19937 gen(rd());
    std::shuffle(cards.begin(), cards.end(), gen);
}

Card Deck::drawCard() {
    Card drawnCard = cards.back();
    cards.pop_back();
    return drawnCard;
}

Player::Player(const std::string& name) : name(name), declarationValue(0) {}

void Player::addToHand(const Card& card) {
    hand.push_back(card);
}

const std::vector<Card>& Player::getHand() const {
    return hand;
}

const std::vector<std::vector<Card>>& Player::getDeclaration() const {
    return declaration;
}

const int Player::getDeclarationValue() const {
    return declarationValue;
}

const std::string Player::getName() const {
    return name;
}

void Player::clearDeclarations() {
    declaration.clear();
    declarationValue = 0;
}

void Player::removeFromHand(const Card& card) {
    auto it = find(hand.begin(), hand.end(), card);
 
    if (it != hand.end()) {
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

std::vector<Card> Player::getAllCardOfSuit(Suit suit) {
    std::vector<Card> foundCards;

    for(const auto& card : hand) {
        if(card.getSuit() == suit) foundCards.push_back(card);
    }

    return foundCards;
}

void Player::findDeclarations() {
    int iterationList[6] = {12, 13, 10, 14, 9, 11};
    std::vector<Card> foundDeclaration;

    for (const int& value : iterationList) {  
        int count = std::count_if(hand.begin(), hand.end(), [value](const Card& card) { return card.getValue() == value; });

        if (count == 4) {
            foundDeclaration.clear();
            for (int suit = SPADES; suit <= CLUBS; suit++) foundDeclaration.push_back(Card(value, static_cast<Suit>(suit)));
            declaration.push_back(foundDeclaration);
            declarationValue += valueDeclaration.at(value);
        }
    }

    for (int suit = SPADES; suit <= CLUBS; suit++) {
        std::vector<Card> sameSuitCards = getAllCardOfSuit(static_cast<Suit>(suit));
        foundDeclaration.clear();

        if(sameSuitCards.size() <= 2) continue;

        for (int i = 0; i < sameSuitCards.size() - 1; i++) {
            if (sameSuitCards[i].getValue() == sameSuitCards[i + 1].getValue() - 1) {
                foundDeclaration.push_back(sameSuitCards[i]);
            } else {
                foundDeclaration.push_back(sameSuitCards[i]);

                if(foundDeclaration.size() >= 3) {
                    int score = (foundDeclaration.size() > 5) ? 5 : foundDeclaration.size();
                    declaration.push_back(foundDeclaration);
                    declarationValue += valueDeclaration.at(score);
                }

                foundDeclaration.clear();
            }
        }

        foundDeclaration.push_back(sameSuitCards[sameSuitCards.size() - 1]);
        if(foundDeclaration.size() >= 3) {
            int score = (foundDeclaration.size() > 5) ? 5 : foundDeclaration.size();
            declaration.push_back(foundDeclaration);
            declarationValue += valueDeclaration.at(score);
        }
    }
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
            players[j].addToHand(deck.drawCard());
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

void BelaGame::clearDeclarations(int playerIndex) {
    players[playerIndex].clearDeclarations();
}

void BelaGame::processDeclarations(int currentPlayer, bool& found4Same, bool& foundDeclaration,
                            std::pair<int, int>& strongestDeclaration, int& strongestIndexPlayer) {
    for (const auto& declaration : players[currentPlayer].getDeclaration()) {
        if (declaration[0].getSuit() == declaration[1].getSuit()) {
            processSameSuitDeclaration(declaration, found4Same, foundDeclaration, strongestDeclaration, strongestIndexPlayer, currentPlayer);
        } else {
            processDifferentSuitDeclaration(declaration, found4Same, foundDeclaration, strongestDeclaration, strongestIndexPlayer, currentPlayer);
        }
    }
}

void BelaGame::declarations() {
    int strongestIndexPlayer;
    std::pair<int, int> strongestDeclaration = {0, 0};
    bool found4Same = false;
    bool foundDeclaration = false;

    for (int i = 0; i < 4; i++) {
        int currentPlayer = (firstPlayer + i) % 4;
        processDeclarations(currentPlayer, found4Same, foundDeclaration, strongestDeclaration, strongestIndexPlayer);
    }

    if (foundDeclaration) {
        clearDeclarations((strongestIndexPlayer + 1) % 4);
        clearDeclarations((strongestIndexPlayer + 3) % 4);
    }
}

void BelaGame::playGame() {
    
}
