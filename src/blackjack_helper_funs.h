#ifndef BJUTILS_H
#define BJUTILS_H

#include <numeric>
#include <Rcpp.h>

int id_to_value_blackjack(int x);

std::vector<int> ids_to_value_blackjack(std::vector<int> x);

std::vector<int> init_player_ids(int x);

int count_hilo(int x, bool start = false);

int count_omega2(int x, bool start = false);

int count_griffin_ultimate(int x, bool start = false);

int count_card(int x = 0, const std::string method = "none", const bool start = false);

std::vector<int> count_cards_by_group(std::vector<int> x, std::vector<int> y, const std::string method = "none");

Rcpp::List score_blackjack(std::vector<int> player_hand);

Rcpp::List score_blackjack_by_group(std::vector<int> player_hands, std::vector<int> player_hand_ids, std::vector<int> player_ids);

#endif