#########################################################################################################################
#                                                                                                                       #
#  Title:
#  Author: Chris Maerzluft
#  Last Edit:
#                                                                                                                       #
#########################################################################################################################
# Description ###########################################################################################################
# Setup R ###############################################################################################################
# Clean environment
rm(list = ls(all = TRUE))
gc()

# Libraries
library(R6)
library(dplyr)
library(ggplot2)
library(Rcpp)
library(RcppArmadillo)

# Rcpp::compileAttributes()
devtools::document()
devtools::load_all()

# sourceCpp("src/simulate_blackjack.cpp")

#########################################################################################################################
# Blackjack Simulation ##################################################################################################
set.seed(12345)

# Options
n_decks <- 1
n_players <- 5
n_hands <- 10000
count_method <- "hi-lo" # "griffin_ultimate"
bj_sim_hands <- simulate_blackjack(
  n_decks = n_decks, n_players = n_players, n_hands = n_hands, count_method = count_method
)

results_individual <- bj_sim_hands %>%
  group_by(hand_id, player_id) %>%
  summarise(
    faceup_card = card_value[card_number == 2] %% 100,
    n_cards = n(),
    score = max(running_score)
  )
results_overall <- results_individual %>%
  group_by(hand_id) %>%
  summarise(
    dealer_faceup = if_else(faceup_card[player_id == 101] %in% 11:13, 10, faceup_card[player_id == 101]),
    dealer_score = min(score[player_id == 101], 22),
    dealer_outcome = if_else(
      score[player_id == 101] > 21, "Bust", if_else(
        score[player_id == 101] == 21 & n_cards[player_id == 101] == 2, "Blackjack",
        as.character(score[player_id == 101])
      )),
    wins = sum((score[player_id != 101] > dealer_score | dealer_outcome == "Bust") & score[player_id != 101] <= 21),
    pushes = sum(score[player_id != 101] == dealer_score & dealer_outcome != "Bust"),
    losses = sum((score[player_id != 101] < dealer_score & dealer_outcome != "Bust") | score[player_id != 101] > 21)
  )

ggplot(data = results_overall %>% filter(dealer_faceup == 2)) +
  geom_bar(aes(x = dealer_outcome, y = (..count..)/sum(..count..))) +
  scale_y_continuous(labels = scales::percent) +
  labs(
    title = sprintf("Dealer score when face-up card is a 2 (N = %s)", sum(results_overall$dealer_faceup == 2)),
    x = NULL, y = NULL
  )

ggplot(data = results_overall %>% filter(dealer_faceup == 10)) +
  geom_bar(aes(x = dealer_outcome, y = (..count..)/sum(..count..))) +
  scale_y_continuous(labels = scales::percent) +
  # scale_x_discrete(labels = c("17" = 17, "18" = 18, "19" = 19, "20" = 20, "21" = 21, "22" = "Bust")) +
  labs(
    title = sprintf("Dealer score when face-up card is a 10 (N = %s)", sum(results_overall$dealer_faceup == 10)),
    x = NULL, y = NULL
  )

ggplot(data = results_overall %>% filter(dealer_faceup == 14)) +
  geom_bar(aes(x = dealer_outcome, y = (..count..)/sum(..count..))) +
  scale_y_continuous(labels = scales::percent) +
  # scale_x_discrete(labels = c("17" = 17, "18" = 18, "19" = 19, "20" = 20, "21" = 21, "22" = "Bust")) +
  labs(
    title = sprintf("Dealer score when face-up card is a 14 (N = %s)", sum(results_overall$dealer_faceup == 14)),
    x = NULL, y = NULL
  )


results_sum <- results_overall %>%
  group_by(dealer_faceup, dealer_outcome) %>%
  summarise(n = n()) %>%
  mutate(
    dealer_faceup = factor(dealer_faceup),
    dealer_outcome = factor(dealer_outcome, levels = rev(c("17", "18", "19", "20", "21", "Blackjack", "Bust"))),
    pct = n/sum(n)
  )
new_percent <- scales::label_percent(accuracy = 2)
ggplot(results_sum) +
  geom_area(
    aes(x = dealer_faceup, y = pct, fill = dealer_outcome, group = dealer_outcome),
    size = 1, colour = "black", alpha = 0.6
  ) +
  scale_y_continuous(labels = new_percent, breaks = seq(0, 1, by = 0.1)) +
  scale_fill_discrete(name = "Dealer Outcome") +
  labs(title = "Dealer outcomes by face-up card", x = NULL, y = NULL)

# profvis::profvis(simulate_blackjack(n_decks = n_decks, n_players = 5, n_hands = 10000))
# CM NOTE: For deciding if someone won. To be used outside of simulation
# game$outcome <-
#   1*(game$player_score > game$player_score[n_players_dealer] & game$player_score <= 21) +
#   -1*(game$player_score < game$player_score[n_players_dealer] | game$player_score > 21)

# set.seed(12345)
# games <- deal_hands(n_decks = n_decks, n_players = n_players, n_hands = n_hands)
# games <- play_hands(games)
#########################################################################################################################



set.seed(12345)
my_seat <- 3
game1 <- blackjack$new(number_of_decks = n_decks, number_of_players = n_players, player_seat = my_seat)
game1$deal()
game1$hit(player_id = "Player 1")
game1$hit(player_id = "Player 2")
game1$hit(player_id = "Player 4")
game1$hit(player_id = "Player 4")
game1$hit(player_id = "Player 4")
game1$hit(player_id = "Player 4")
game1


self <- game1
