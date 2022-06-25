#include <numeric>
#include <RcppArmadilloExtensions/sample.h>
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
IntegerVector score_blackjackC2(IntegerVector x, IntegerVector y = IntegerVector::create(0)) {
  // CM NOTE: For some reason calling this function directly in R as part of a group_by is very memory inefficient.
  //  need to find out why
  // Initializations
  int n_scores = x.size();
  int n_groups = y.size();
  LogicalVector has_ace(n_groups);
  LogicalVector soft_value(n_groups);
  IntegerVector player_score(n_groups);

  // Add scores and track aces
  for (int i1 = 0; i1 < n_scores; i1++) {
    for (int i2 = 0; i2 < n_groups; i2++) {
      if (x[i1] == 1) {
        has_ace[i2] = true;
      }
      if (n_groups > 1) {
        if (y[i1] == y[i2]) {
          player_score[i2] += x[i1];
        }
      } else {
        player_score[i2] += x[i1];
      }
    }
  }

  // Count soft-aces as 11 (1 + 10)
  for (int i1 = 0; i1 < n_groups; i1++) {
    soft_value[i1] = has_ace[i1] & ((player_score[i1] + 10) <= 21);
    if (soft_value[i1]) {
      player_score[i1] += 10;
    }
  }

  // return List::create(
  //   _["player_score"] = player_score,
  //   _["soft_value"] = soft_value
  // );
  return player_score;
}