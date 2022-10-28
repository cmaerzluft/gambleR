#include <RcppArmadilloExtensions/sample.h>
#include <Rcpp.h>
using namespace Rcpp;

std::vector<int> sample(std::vector<int> base_deck) {
  return RcppArmadillo::sample(base_deck, base_deck.size(), false);
}