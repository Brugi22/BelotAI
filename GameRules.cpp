#include <iostream>
#include <vector>
#include <map>
#include "Models.h"
#include "GameRules.h"

const int trumpOrder[] = { 7, 8, 12, 13, 10, 14, 9, 11 };
const int normalOrder[] = { 7, 8, 9, 11, 12, 13, 10, 14 };
const std::map<int, int> cardValue = {
    {7, 0}, {8, 0}, {9, 0}, {11, 2}, {12, 3}, {13, 4}, {10, 10}, {14, 11}
};
const std::map<int, int> cardTrumpValue = {
    {7, 0}, {8, 0}, {12, 3}, {13, 4}, {10, 10}, {14, 11}, {9, 14}, {11, 20}
};

//returns true if card1 is stronger than card2
bool compareCards(const Card& card1, const Card& card2, Suit trump) {
    if (card2.getValue() == 0) return true;   // base case, first card of the round
    if (card1.getSuit() == trump && card2.getSuit() != trump) return true;
    if (card1.getSuit() != card2.getSuit()) return false;

    int index1, index2;

    for (int i = 0; i < 8; i++) {
        if (card1.getValue() == (card1.getSuit() == trump ? trumpOrder[i] : normalOrder[i])) {
            index1 = i;
        }
        if (card2.getValue() == (card1.getSuit() == trump ? trumpOrder[i] : normalOrder[i])) {
            index2 = i;
        }
    }

    return index1 > index2;
}

std::vector<Card> validCardsToPlay(const std::vector<Card>& playerHand, Card firstCard = Card(0, SPADES),
                                    Card strongestCard = Card(0, SPADES), Suit trump = SPADES) {
    if (firstCard.getValue() == 0) return playerHand;

    std::vector<Card> validCards;
    std::vector<Card> validStrongerCards;

    for (const auto& card : playerHand) {
        if (card.getSuit() == firstCard.getSuit()) validCards.push_back(card);
    }

    if (validCards.size() != 0) {
        for (const auto& card : validCards) if (compareCards(card, strongestCard, trump)) validStrongerCards.push_back(card);

        if (validStrongerCards.size() != 0) return validStrongerCards;
        return validCards;
    }

    for (const auto& card : playerHand) {
        if (card.getSuit() == trump) validCards.push_back(card);
    }

    if (validCards.size() != 0) {
        for (const auto& card : validCards) if (compareCards(card, strongestCard, trump)) validStrongerCards.push_back(card);

        if (validStrongerCards.size() != 0) return validStrongerCards;
        return validCards;
    }
    return playerHand;
}

int countCardPoints(const std::vector<Card>& cards, Suit trump) {
    int countPoints = 0;
    std::cout << "------ TEST count card points ------\n";
    for (const auto& card : cards) {
        std::cout << valueMapReverse.at(card.getValue()) << " of " << suitMapReverse.at(card.getSuit()) << std::endl;
        std::cout << "Adut? " << ((card.getSuit() != trump) ? "NE\n" : "DA\n");
        std::cout << "Bodovi: " << ((card.getSuit() != trump) ?
            cardValue.at(card.getValue()) : cardTrumpValue.at(card.getValue()))
            << std::endl;
        if (card.getSuit() != trump) countPoints += cardValue.at(card.getValue());
        else countPoints += cardTrumpValue.at(card.getValue());
    }
    std::cout << "------------\n";
    return countPoints;
}

void processSameSuitDeclaration(const std::vector<Card>& declaration, bool& found4Same, bool& foundDeclaration,
    std::pair<int, int>& strongestDeclaration, int& strongestIndexPlayer, int currentPlayer) {
    if (!found4Same) {
        found4Same = true;
        foundDeclaration = true;
        strongestDeclaration = { declaration[0].getValue(), declaration[0].getValue() };
        strongestIndexPlayer = currentPlayer;
    }
    else if (strongestDeclaration.first < declaration[0].getValue()) {
        strongestDeclaration = { declaration[0].getValue(), declaration[0].getValue() };
        strongestIndexPlayer = currentPlayer;
    }
}

void processDifferentSuitDeclaration(const std::vector<Card>& declaration, bool& found4Same, bool& foundDeclaration,
    std::pair<int, int>& strongestDeclaration, int& strongestIndexPlayer, int currentPlayer) {
    int score = std::min(5, static_cast<int>(declaration.size()));
    if (!foundDeclaration) {
        foundDeclaration = true;
        strongestDeclaration = { score, declaration.back().getValue() };
        strongestIndexPlayer = currentPlayer;
    }
    else {
        if (!found4Same && (strongestDeclaration.first < declaration.size() ||
            (strongestDeclaration.first == declaration.size() &&
                strongestDeclaration.second < declaration.back().getValue()))) {
            strongestDeclaration = { score, declaration.back().getValue() };
            strongestIndexPlayer = currentPlayer;
        }
    }
}