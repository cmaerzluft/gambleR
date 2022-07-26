#include <numeric>
#include <Rcpp.h>
using namespace Rcpp;

int count_hilo(int x, bool start = false) {
  // Hi-lo method, neutral deck is 0, 2-6 = +1, 7-9 = 0, 10-Ace = -1
  int res = 0;
  if (start)                { res =  0; }
  else if (x >= 2 & x < 7)  { res =  1; }
  else if (x >= 7 & x < 10) { res =  0; }
  else if (x >= 10 | x < 2) { res = -1; }

  return res;
}

int count_omega2(int x, bool start = false) {
  // Omega II method, neutral deck is 0, 2-6 = +1, 7-9 = 0, 10-Ace = -1
  int res = 0;
  if (start)                         { res =  0; }
  else if (x == 4 | x == 5 | x == 6) { res =  2; }
  else if (x == 2 | x == 3 | x == 7) { res =  1; }
  else if (x == 1 | x == 8)          { res =  0; }
  else if (x == 9)                   { res = -1; }
  else if (x >= 10)                  { res = -2; }

  return res;
}

int count_griffin_ultimate(int x, bool start = false) {
  // Griffin Ultimate method, neutral deck is 0, values are below
  int res = 0;
  if (start)        { res =   0; }
  else if (x == 1)  { res = -60; }
  else if (x == 2)  { res =  37; }
  else if (x == 3)  { res =  45; }
  else if (x == 4)  { res =  52; }
  else if (x == 5)  { res =  70; }
  else if (x == 6)  { res =  46; }
  else if (x == 7)  { res =  27; }
  else if (x == 8)  { res =   0; }
  else if (x == 9)  { res = -17; }
  else if (x == 10) { res = -50; }

  return res;
}

int count_card(int x = 0, const std::string method = "none", const bool start = false) {
  int res = 0;
  if (method.compare("hi-lo") == 0)                 { res = count_hilo(x, start); }
  else if (method.compare("omega2") == 0)           { res = count_omega2(x, start); }
  else if (method.compare("griffin_ultimate") == 0) { res = count_griffin_ultimate(x, start); }

  return res;
}

// [[Rcpp::export]]
std::vector<int> count_cards_by_group(std::vector<int> x, std::vector<int> y, const std::string method = "none") {
  int n_cards = x.size();
  std::vector<int> res(n_cards);
  int start_value = count_card(0, method, true);

  res[0] = start_value + count_card(x[0], method, false);
  for (int i1 = 1; i1 < n_cards; i1++) {
    if (y[i1] == y[i1 - 1]) {
      res[i1] = res[i1 - 1] + count_card(x[i1], method, false);
    } else {
      res[i1] = start_value + count_card(x[i1], method, false);
    }
  }

  return res;
}