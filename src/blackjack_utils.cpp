#include <numeric>
#include <Rcpp.h>
using namespace Rcpp;

int id_to_value_blackjack(int x) {
  x = x % 100;
  if (x == 11 | x == 12 | x == 13) { x = 10; }
  if (x == 14) { x = 1; }
  if (x < 1 | x > 14) { stop("Wrong Card IDs"); }

  return x;
}

// [[Rcpp::export]]
std::vector<int> ids_to_value_blackjack(std::vector<int> x) {
  int n = x.size();
  std::vector<int> y(n);

  for (int i1 = 0; i1 < n; i1++) {
    y[i1] = id_to_value_blackjack(x[i1]);
  }

  return y;
}

std::vector<int> init_player_ids(int x) {
  std::vector<int> player_ids(x);
  std::iota(std::begin(player_ids), std::end(player_ids), 1);
  player_ids[x - 1] = 101;

  return player_ids;
}