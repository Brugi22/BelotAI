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

Card::Card(int value, Suit suit) : value(value), suit(suit) {}

int Card::getValue() const { return value; }

Suit Card::getSuit() const { return suit; }

Deck::Deck() {
    for (int i = 7; i <= 14; ++i) {
        cards.push_back(Card(i, SPADES));
        cards.push_back(Card(i, HEARTS));
        cards.push_back(Card(i, DIAMONDS));
        cards.push_back(Card(i, CLUBS));
    }
    shuffle();
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

        foundDeclaration.push_back(sameSuitCards.back());
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
        const Player& currentPlayer = players[(firstPlayer + i) % 4];
        std::cout << "Player " << currentPlayer.getName() << "'s turn:" << std::endl;
        std::cout << "Your hand: ";
        const std::vector<Card>& hand = currentPlayer.getHand();
        for (int j = 0; j < hand.size(); ++j) {
            std::cout << valueMapReverse.at(hand[j].getValue()) << " of " << suitMapReverse.at(hand[j].getSuit()) << "    ";
        }
        std::cout << std::endl;

        while (true) {
            if (i != 3) std::cout << "Choose: PASS, SPADES, HEARTS, DIAMONDS, CLUBS" << std::endl;
            else {
                validInput.erase(validInput.begin());
                std::cout << "Choose: SPADES, HEARTS, DIAMONDS, CLUBS" << std::endl;
            }
            std::cin >> inputTrump;
            if (std::find(validInput.begin(), validInput.end(), inputTrump) == validInput.end()) {
                std::cout << "Not a valid input, try again" << std::endl;
                continue;
            }
            if (inputTrump == "PASS") break;
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

    for(int i = 0; i < 4; i++) players[i].findDeclarations();

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
    int currentPlayerIndex = firstPlayer;
    int round = 0;
    std::vector<Card> roundCards;
    Card strongestCard = Card(0, SPADES);
    Card firstCard = Card(0, SPADES);
    int strongestPlayer = -1;

    while (round < 8) {
        if (currentPlayerIndex == firstPlayer) {
            roundCards.clear();
            strongestCard = firstCard = Card(0, SPADES);
        }

        const Player& currentPlayer = players[currentPlayerIndex];

        std::cout << "Player " << currentPlayer.getName() << "'s turn:" << std::endl;
        std::cout << "Your hand: ";
        const std::vector<Card>& hand = currentPlayer.getHand();
        for (int i = 0; i < hand.size(); ++i) {
            std::cout << i << ": " << valueMapReverse.at(hand[i].getValue()) << " of " << suitMapReverse.at(hand[i].getSuit()) << "    ";
        }
        std::cout << std::endl;

        // Valid cards to play:
        const std::vector<Card>& validHand = validCardsToPlay(hand, firstCard, strongestCard, trump);
        // std::cout << "----- PRVA ----- " << valueMapReverse.at(firstCard.getValue()) << " of " << suitMapReverse.at(firstCard.getSuit());
        // std::cout << "\n----- NAJJACA ----- " << valueMapReverse.at(strongestCard.getValue()) << " of " << suitMapReverse.at(strongestCard.getSuit());
        // std::cout << "\n----- MGUCE IGRATI -----\n";
        // for (Card c : validHand) std::cout << valueMapReverse.at(c.getValue()) << " of " << suitMapReverse.at(c.getSuit()) << "    ";
        // std::cout << std::endl;
        
        int chosenCardIndex;
        while (true) {
            std::cout << "Enter the position of the card to play (0-7): ";
            std::cin >> chosenCardIndex;

            if (chosenCardIndex < 0 || chosenCardIndex >= hand.size()) {
                std::cout << "Invalid input. Please enter a number between 0 and 7." << std::endl;
                continue;
            }

            if (std::find(validHand.begin(), validHand.end(), hand[chosenCardIndex]) == validHand.end()) {
                std::cout << "This card cannot be played according to the rules." << std::endl;
                continue;
            }

            break;
        }

        // Play the chosen card
        const Card& chosenCard = hand[chosenCardIndex];
        std::cout << "Player " << currentPlayer.getName() << " played: " << valueMapReverse.at(chosenCard.getValue()) << " of " << suitMapReverse.at(chosenCard.getSuit()) << std::endl;
        if (currentPlayerIndex == firstPlayer) firstCard = chosenCard;

        // Is the card winning the round?
        if (compareCards(chosenCard, strongestCard, trump)) {
            strongestCard = chosenCard;
            strongestPlayer = currentPlayerIndex;
        }

        // Remove the played card from the player's hand and add it to the Round Cards
        players[currentPlayerIndex].removeFromHand(chosenCard);
        roundCards.push_back(chosenCard);

        // Move to the next player
        currentPlayerIndex = (currentPlayerIndex + 1) % 4;

        // End of the round
        if (currentPlayerIndex == firstPlayer) {
            // Add round points to team winning the round
            points[strongestPlayer % 2] += countCardPoints(roundCards, trump);
            std::cout << "------ End round " << round + 1 << std::endl;
            std::cout << "------ Points: " << points[0] << " " << points[1] << std::endl;

            // First player is the strongest player of the round:
            firstPlayer = strongestPlayer;
            currentPlayerIndex = firstPlayer;

            round++;
            std::cout << "Round " << round + 1 << std::endl;
        }
    }

    // End of the game
    std::cout << "Game Over!" << std::endl;
}
