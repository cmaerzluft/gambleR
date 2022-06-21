#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List score_blackjack(IntegerVector x, IntegerVector y = IntegerVector::create(0)) {
  // Initializations
  int n_scores = x.size();
  int n_groups = y.size();
  IntegerVector group_ids;
  if (n_groups > 1) {
    group_ids = sort_unique(noNA(y));
    n_groups = group_ids.size();
  }
  IntegerVector value(n_scores);
  LogicalVector has_ace(n_groups);
  LogicalVector soft_value(n_groups);
  IntegerVector player_score(n_groups);

  for (int i1 = 0; i1 < n_scores; i1++) {
    value[i1] = x[i1] % 100;
    if (value[i1] == 11 | value[i1] == 12 | value[i1] == 13) {
      value[i1] = 10;
    }
    if (value[i1] == 14) {
      value[i1] = 1;
    }

    for (int i2 = 0; i2 < n_groups; i2++) {
      if (value[i1] == 1) {
        has_ace[i2] = true;
      }
      if (n_groups > 1) {
        if (y[i1] == group_ids[i2]) {
          player_score[i2] += value[i1];
        }
      } else {
        player_score[i2] += value[i1];
      }
    }
  }

  for (int i1 = 0; i1 < n_groups; i1++) {
    soft_value[i1] = has_ace[i1] & ((player_score[i1] + 10) <= 21);
    if (soft_value[i1]) {
      player_score[i1] += 10;
    }
  }

  return List::create(
    _["player_score"] = player_score,
    _["soft_value"] = soft_value
  );
}

/*** R
score_blackjack(x = as.integer(c(104, 114)))
score_blackjack(x = as.integer(c(104, 114, 108)))
score_blackjack(x = as.integer(c(104, 114, 104, 113)), y = c(1, 1, 2, 2))
score_blackjack(x = as.integer(c(104, 114, 108, 114, 102, 102)), y = c(1, 1, 1, 2, 2, 2))
*/
