#include <iostream>
#include <vector>
#include <map>
#include "Models.h"

using namespace std;

const int trumpOrder[] = {7, 8, 12, 13, 10, 14, 9, 11};
const int normalOrder[] = {7, 8, 9, 11, 12, 13, 10, 14};
const map<int, int> cardValue = {
    {7, 0}, {8, 0}, {9, 0}, {11, 2}, {12, 3}, {13, 4}, {10, 10}, {14, 11}
};
const map<int, int> cardTrumpValue = {
    {7, 0}, {8, 0}, {12, 3}, {13, 4}, {10, 10}, {14, 11}, {9, 14}, {11, 20}
};

//returns true if card1 is stronger than card2
bool compareCards(const Card& card1, const Card& card2, Suit trump) {
    if(card1.getSuit() == trump && card2.getSuit() != trump) return true;
    if(card1.getSuit() != card2.getSuit()) return false;

    int index1,index2;

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

const vector<Card>& validCardsToPlay(const vector<Card>& playerHand, Card firstCard = Card(0, SPADES), Card strongestCard = Card(0, SPADES), Suit trump = SPADES) {
    if(firstCard.getValue() == 0) return playerHand;

    vector<Card> validCards;

    for(const auto& card : playerHand) {
        if(card.getSuit() == firstCard.getSuit()) validCards.push_back(card);
    }

    if(validCards.size() != 0) {
        vector<Card> validStrongerCards;
        
        for(const auto& card : validCards) if(compareCards(card,strongestCard,trump)) validStrongerCards.push_back(card);

        if(validStrongerCards.size() != 0) return validStrongerCards;
        return validCards;
    }

    for(const auto& card : playerHand) {
        if(card.getSuit() == trump) validCards.push_back(card);
    }

    if(validCards.size() != 0) {
        vector<Card> validStrongerCards;
        
        for(const auto& card : validCards) if(compareCards(card,strongestCard,trump)) validStrongerCards.push_back(card);

        if(validStrongerCards.size() != 0) return validStrongerCards;
        return validCards;
    }

    return playerHand;
}

int countCardPoints(const vector<Card>& cards, Suit trump) {
    int countPoints = 0;
    
    for(const auto& card : cards) {
        if(card.getSuit() != trump) countPoints += cardValue.at(card.getValue());
        else countPoints += cardTrumpValue.at(card.getValue());
    }

    return countPoints;
}