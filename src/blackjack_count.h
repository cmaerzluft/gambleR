#ifndef BJCOUNTCARDS_H
#define BJCOUNTCARDS_H

#include <numeric>
#include <Rcpp.h>

int count_hilo(int x, bool start = false);

int count_omega2(int x, bool start = false);

int count_griffin_ultimate(int x, bool start = false);

int count_card(int x = 0, const std::string method = "none", const bool start = false);

std::vector<int> count_cards_by_group(std::vector<int> x, std::vector<int> y, const std::string method = "none");

#endif