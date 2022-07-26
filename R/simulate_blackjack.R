#' @export
simulate_blackjack <- function(n_players, n_hands, n_decks = 1, count_method = "hi-lo") {
  if (n_players < 1) { stop("n_players must be a positive value") }
  if (n_hands < 1) { stop("n_hands must be a positive value") }
  if (n_decks < 1) { stop("n_decks must be a positive value") }
  count_method <- match.arg(count_method, c("hi-lo", "omega2", "griffin_ultimate"))
  card_deck <- rep(gambleR::card_deck$card_id, n_decks)

  res <- tryCatch(
    {
      simulate_blackjackC(
        n_players = n_players, n_hands = n_hands, card_deck = card_deck, count_method = count_method
      ) %>% tibble()
    },
    error = function(cond) {
      message(paste0(cond))
      return(NA)
    }
  )

  return(res)
}


# CM NOTE: Idea, run game as a nested tibble:
#   game = tibble(game_id = numeric, cards = vector, hands = tibble())
#     hands = tibble(player_id, card_number, card_value)
simulate_blackjackR <- function(n_decks, n_players, n_hands, progress = TRUE) {
  # Logic checks
  # if (n_players >= 101L) { stop("Can't have more than 100 players") }

  # Initializations
  game <- list()
  player_ids <- as.integer(c(1:n_players, 101L))
  n_players_dealer <- n_players + 1
  fresh_deck <- rep.int(1, n_hands)
  empty_game_start <- data.frame(
    hand_id = 0L, player_id = rep(player_ids, 2),
    card_number = rep(1L:2L, each = n_players_dealer),
    card_value = NA_integer_
  )
  cards <- rep(gambleR::card_deck$card_id, n_decks)
  current_deck <- cards[-sample(length(cards), 1)]

  # Simulate all games
  if (progress) { pb <- txtProgressBar(max = n_hands, style = 3) }
  for (i1 in seq.int(length.out = n_hands)) {
    # Store game ID
    empty_game_start$hand_id <- i1

    # Simulate a single hand
    res <- sim_hand_blackjack(
      game = empty_game_start, cards = current_deck,
      hand_id = i1, player_ids = player_ids, n_players_dealer = n_players_dealer
    )

    # Decide and track whether to reshuffle the deck
    if (length(res$cards) < n_players_dealer*(4 + 4 %/% n_players) | i1 == n_hands) {
      current_deck <- cards[-sample(length(cards), 1)]
    } else {
      fresh_deck[i1 + 1] <- 0
      current_deck <- res$cards
    }

    # Store results
    game[[i1]] <- res$game
    if (progress) { setTxtProgressBar(pb, i1) }
  }
  if (progress) { close(pb) }

  # Join games
  game <- do.call(rbind, game)

  # Add shuffle numbers to results
  game$shuffle_number <- rep.int(as.integer(cumsum(fresh_deck)), times = table(game$hand_id))

  return(game)
}