#pragma once
#ifndef GAMERULES_H
#define GAMERULES_H

#include <vector>
#include <utility>
#include <algorithm>
#include "Models.h"

bool compareCards(const Card& card1, const Card& card2, Suit trump);

std::vector<Card> validCardsToPlay(const std::vector<Card>& playerHand, Card firstCard, Card strongestCard, Suit trump);

int getStrongestPositionedCard(const std::vector<Card>& cards, Suit trump);

int countCardPoints(const std::vector<Card>& cards, Suit trump);

void processSameSuitDeclaration(const std::vector<Card>& declaration, bool& found4Same, bool& foundDeclaration,
    std::pair<int, int>& strongestDeclaration, int& strongestIndexPlayer, int currentPlayer);

void processDifferentSuitDeclaration(const std::vector<Card>& declaration, bool& found4Same, bool& foundDeclaration,
    std::pair<int, int>& strongestDeclaration, int& strongestIndexPlayer, int currentPlayer);

#endif