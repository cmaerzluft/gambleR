#ifndef BJSCORE_H
#define BJSCORE_H

#include <numeric>
#include <Rcpp.h>

Rcpp::List score_blackjack(std::vector<int> player_hand);

Rcpp::List score_blackjack_by_group(std::vector<int> player_hands, std::vector<int> player_hand_ids, std::vector<int> player_ids);

#endif