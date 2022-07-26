#ifndef BJUTILS_H
#define BJUTILS_H

#include <numeric>
#include <Rcpp.h>

int id_to_value_blackjack(int x);

std::vector<int> ids_to_value_blackjack(std::vector<int> x);

std::vector<int> init_player_ids(int x);

#endif