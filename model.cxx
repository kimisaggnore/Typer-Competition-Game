#include "model.hxx"

Model::Model(std::vector<std::string> const& words)
        :
          dictionary_(words),
          used_words_(),
          prefix_(),
          typed_word_arr_(),
          typed_word(),
          max_word_length(),
          player_turn_(),
          cur_dict(),
          p1_lives(),
          p2_lives(),
          round_num(),
          game_started(),
          mt(),
          warning(),
          game_time(),
          flash(),
          p1_streak(),
          p2_streak(),
          counter(),
          player_time(),
          success(),
          reduce_time_counter(),
          time_decreases_count(),
          init_game_time(),
          p1_golden_heart(),
          p2_golden_heart(),
          p1_swap(),
          p2_swap(),
          prefix_swapped(),
          p1_drain(),
          p2_drain(),
          next_player_drained(),
          detection(),
          next_player_drained2(),
          active_drain(),
          max_prefix_size(),
          power_word_len(),
          streak_num_powerup()


{
    used_words_ = {};
    prefix_ = set_new_prefix(dictionary_, "");
    typed_word_arr_ = {};
    typed_word = "";
    max_word_length = 25;
    player_turn_ = 1;
    cur_dict = {};
    p1_lives = 4;
    p2_lives = 4;
    round_num = 0;
    game_started = false;
    warning = false;
    game_time = 0;
    flash = false;
    p1_streak = 0;
    p2_streak = 0;
    counter = 0;
    player_time = 21*60;
    success = {0,0};
    reduce_time_counter = 10;
    time_decreases_count = 0;
    init_game_time = player_time;
    p1_golden_heart = 1;
    p2_golden_heart = 1;
    p1_swap = 1;
    p2_swap = 1;
    prefix_swapped = 0;
    p1_drain = 1;
    p2_drain = 1;
    //next_player_drained = 0;
    detection = 1;
    //next_player_drained2 = 0;
    active_drain = 0;
    max_prefix_size = 2;
    power_word_len = 5;
    streak_num_powerup = 3;
}

//Sets a new prefix, and returns the new prefix. The prefix must be valid in
// order to be returned. If the prefix is invalid, constructs a new prefix
// until it is valid.
std::string
Model::set_new_prefix(std::vector<std::string> dict, std::string
cur_prefix)
{
    mt.seed(std::chrono::system_clock::now().time_since_epoch().count());
    int rng_num = mt() % dict.size();
    std::string new_prefix = dict[rng_num].substr(0, cur_prefix.size() + 1);
    if (valid_prefix(dict, new_prefix, false)){
        return new_prefix;
    } else {
        rng_num = rand() % (dictionary_.size());
        new_prefix = dictionary_[rng_num].substr(0,1);
    }
    return new_prefix;
}

//This method constructs and returns a sub dictionary that contains all the
// words that satisfy the current prefix.
std::vector<std::string>
Model::construct_dictionary(std::vector<std::string> dict, std::string
cur_prefix, bool index_tracker)
{
    if (cur_prefix.compare("") == 0){
        return dict;
    } else {
        std::vector<std::string> new_dict = {};
        unsigned int i = 0;
        int len = cur_prefix.size();
        while (i < dict.size() and dict[i].substr(0,len).compare(cur_prefix)
        != 0 ) {
            i = i + 1;
        }
        while (i < dict.size() and dict[i].substr(0,len).compare(cur_prefix)
        == 0 ){
            new_dict.push_back(dict[i]);
            i = i + 1;
        }
        return new_dict;
    }
}

//Checks whether the new assigned prefix actually has words that fit it. That
// is, there must be more than one unused word that fit the prefix, so then
// both players have the possibility of completing that round.
bool
Model::valid_prefix(std::vector<std::string> dict, std::string cur_prefix,
                    bool index_tracker)
{
    std::vector<std::string> cmp = construct_dictionary(dict, cur_prefix,
                                                        false);

    for (unsigned int i = 0; i < cmp.size(); i++) {
    }
    if (cmp.size() > 1){
        return true;
    } else {
        return false;
    }
}

//Checks whether the word that is submitted is valid. A word is valid if it
// is contained in the existing dictionary.
bool
Model::valid_input(std::string word, std::vector<std::string> dict, bool
index_tracker)
{
    unsigned int i = 0;
    while (i < dict.size() and dict[i].compare(word) != 0){
        i = i + 1;
    }
    if (i == dict.size()){
        return false;
    }

    if (dict[i].compare(word) == 0){
        if (index_tracker) {
            used_words_.push_back(word);
            for (i = 0; i < dictionary_.size(); i++) {
                if (dictionary_[i].compare(word) == 0) {
                    dictionary_.erase(dictionary_.begin() + i);
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

//Updates the total game time and player time. Used for determining how the
// view should appear.
void
Model::on_frame()
{
    game_time += 1;
    player_time = player_time - 1;
}

//appends the currently typed word onto the back of the words array.
void
Model::push_letter(std::string character)
{
    int tmp = typed_word.length() + prefix_.length();
    if (tmp < max_word_length){
        typed_word_arr_.push_back(character);
        typed_word = typed_word + character;
    }

}

//erases the typed word.
void
Model::erase_word()
{
    typed_word = "";
}

//returns the other player.
int
Model::other_player(int turn, bool detect){

    if (detect){
        detection = 0;
    }
    if (turn == 1){
        return 2;
    } else {
        return 1;
    }
}

//checks whether the word has already been sucessfully inputted before.
bool
Model::in_used_words(std::string word)
{
    for (unsigned int i = 0; i < used_words_.size(); i++){
        if (used_words_[i].compare(word) == 0){
            return true;
        }
    }
    return false;
}

//determines how the view should appear for the flashing objects.
void
Model::flashing()
{
    if (game_time % 120 == 0 and game_time % 240 != 0) {
        flash = true;
    }
    if (game_time % 240 == 0) {
        flash = false;
    }
}

//adds to the streak of the current player.
void
Model::add_to_streak()
{
    if (player_turn_ == 1){
        p1_streak += 1;
    } else {
        p2_streak += 1;
    }
}

//resets the streak of the current player.
void
Model::reset_streak()
{
    if (player_turn_ == 1){
        p1_streak = 0;
    } else {
        p2_streak = 0;
    }
}

//makes the move. that is, does all the operations related to entering the
// word. Determines whether the input is valid, then updates the player's
// lives, special powerups, player time, and other parameters as required.
void
Model::make_move()
{
    if (!game_started){
        game_started = true;

        cur_dict = construct_dictionary(dictionary_, prefix_, true);
    }

    if (in_used_words(prefix_ + typed_word) and valid_input(prefix_ +
    typed_word,cur_dict, false)) {
        warning = true;
        erase_word();
    }
    else if ((!in_used_words(prefix_ + typed_word)) and valid_input(prefix_ +
    typed_word,cur_dict, true)){
        prefix_swapped = 0;
        if (counter < 1) {
            int tmp = (prefix_ + typed_word).size();
            if (tmp > power_word_len){
                if (player_turn_ == 1){
                    p1_drain = 1;
                } else {
                    p2_drain = 1;
                }
            }
            player_time = calculate_player_time();
            warning = false;
            counter = counter + 1;
            add_to_streak();
            update_success();
            player_turn_ = other_player(player_turn_, true);
            erase_word();
            reduce_time_counter -= 1;
        } else {
            int tmp100 = (prefix_ + typed_word).size();
            if (tmp100 > power_word_len){
                if (player_turn_ == 1){
                    p1_drain = 1;
                } else {
                    p2_drain = 1;
                }
            }
            reduce_time_counter -= 1;
            player_time = calculate_player_time();
            warning = false;
            add_to_streak();
            update_success();
            if (check_success() == 2) {
                int tmp5 = prefix_.size();
                if (tmp5 < max_prefix_size) {
                    cur_dict = construct_dictionary(cur_dict,prefix_, true);
                    prefix_ = set_new_prefix(cur_dict, prefix_);
                    round_num = round_num + 1;
                } else {
                    cur_dict = dictionary_;
                    prefix_ = set_new_prefix(cur_dict, "");
                    round_num = 0;
                    p1_golden_heart = 1;
                    p2_golden_heart = 1;
                    if (p1_lives < 4){
                        p1_lives += 1;
                    }
                    if (p2_lives < 4){
                        p2_lives += 1;
                    }
                }
                success[0] = 0;
                success[1] = 0;

            } else if (check_success() == 1) {
                if (other_player(player_turn_, true) == 1){
                    p1_lives = p1_lives - 1;
                    if (p1_golden_heart == 1){
                        p1_golden_heart = 0;
                    }
                } else {
                    p2_lives = p2_lives - 1;
                    if (p2_golden_heart == 1){
                        p2_golden_heart = 0;
                    }
                }

                success[0] = 0;
                success[1] = 0;
                cur_dict = construct_dictionary(dictionary_, "", true);
                prefix_ = set_new_prefix(cur_dict, "");
                round_num = 0;
            }
            success[0] = 0;
            success[1] = 0;
            counter = 0;
            player_turn_ = other_player(player_turn_, true);
            erase_word();
        }
        if (p1_streak == streak_num_powerup){
            p1_swap = 1;
            p1_streak = 0;
        }
        if (p2_streak == streak_num_powerup){
            p2_swap = 1;
            p2_streak = 0;
        }
    } else {
        player_time = player_time - 60*3;

    }
    if (reduce_time_counter == 0){
        reduce_time_counter = 10;
        time_decreases_count += 1;
        player_time = calculate_player_time();
    }
}

//function that is called when the time runs out for the player. Determines
// whether a life should be decreased from the player, and properly resets
// certain player variables.
void
Model::time_is_up()
{
    warning = false;
    if (check_success() == 1){
            if (player_turn_ == 1){
                p1_lives = p1_lives - 1;
                if (p1_golden_heart == 1){
                    p1_golden_heart = 0;
                }
            } else {
                p2_lives = p2_lives - 1;
                if (p2_golden_heart == 1){
                    p2_golden_heart = 0;
                }
            }


        cur_dict = dictionary_;
        prefix_ = set_new_prefix(dictionary_, "");
        reset_streak();
        erase_word();
        counter = -1;
    }
    if (check_success() == 0 and counter == 1){
        cur_dict = construct_dictionary(dictionary_, "", true);
        prefix_ = set_new_prefix(cur_dict, "");
        counter = -1;
        erase_word();
    }
    if (player_turn_ == 1){
        p1_streak = 0;
    } else {
        p2_streak = 0;
    }
    player_turn_ = other_player(player_turn_, true);
    player_time = calculate_player_time();
    counter = counter + 1;
    success[0] = 0;
    success[1] = 0;
    erase_word();
    prefix_swapped = 0;
}

//updates whether a certain player succeeded in inputting a valid word in the
// alloted time.
void
Model::update_success()
{
    if (player_turn_ == 1){
        success[0] = 1;
    } else {
        success[1] = 1;
    }
}

//Checks which players successfully entered a valid word in a given round.
int
Model::check_success()
{
    int sum = 0;
    for (int i = 0; i < 2; i++){
        sum = sum + success[i];
    }
    return sum;
}

//Calculates the next players time they have to enter in a valid word.
int
Model::calculate_player_time()
{
    player_time = init_game_time;
    if (time_decreases_count > 0) {
        player_time = init_game_time * pow(.9, time_decreases_count);
    }
    if (next_player_drained2 == 1){
        next_player_drained2 = 0;
        next_player_drained = 0;
        active_drain -= 1;
        if (active_drain > 0){
            next_player_drained = 1;
        }
    }
    if (next_player_drained == 1){
        player_time = player_time/2;
        next_player_drained2 = 1;
    }


    return player_time;
}

//swaps the last letter of the current prefix.
void
Model::swap_prefix()
{
    cur_dict = construct_dictionary(dictionary_,prefix_
    .substr(0,prefix_.size() -1),true);
    prefix_ = set_new_prefix(cur_dict, prefix_.substr(0,prefix_.size() -1));
    prefix_swapped = 1;
}

//sets up the proper conditions to drain the time of the next player and show
//it on the view.
void
Model::drain_next_player()
{
    if (!(next_player_drained == 1 and detection == 0)){
        detection = 1;
    }
    next_player_drained = 1;
    active_drain += 1;
}



//Write a function that chooses a prefix given some precondition
//Conditions:
// - 1st prefix is chosen completely at random from the dictionary by a word
// being chosen at random from the dictionary and the first letter is used as
// the prefix. This makes it so prefixes that are chosen are naturally
// selected to represent the majority of prefixes that will exist.
// - remaining prefixes are chosen based on randomly selecting from all the
// words that contain the current prefix, then selecting the "nth" letter of
// the word to be the new prefix.
// -These two conditions guarantees that there is at least one word that
// satisfies the prefix that is given. Another function also needs to check
// that 1. The word has not been used before previously and 2. If there are
// physically no words left in the dictionary that satisfies the right
// conditions (that is, if either there are actually no words left in the
// dictionary or if all the words left in the dictionary have already been
// inputted before). If 1., then throw a visible warning message and deduct
// no time. If 2., reset the round.

//Write a function that checks whether a word has already been inputted in
//the game (a conditions, the word first needs contain the valid prefix of
// course)

// Write a function that checks whether a prefix actually has words that
// satisfies the condition.

// Write a function that correctly updates the time of that the current
// player has.

// Write a function for giving the extra life

// Write a function for eliminating half the time of the other player

/*
1. Player sprites (an image of some color) and their lives (hearts) are placed at the top of the screen.
2. Timer is placed at the center of the screen and the prefix is placed below the timer. The timer indicates the amount of time left the current player has to input a valid string that includes the prefix of the word that is shown below the timer.
3. Whenever a player loses a life, the life decreases by one and is shown by the heart image turning into a transparent heart for that player.
4. The word that the user is inputting is shown live on the screen next to the prefix.
5. The current round number is increased every time all players have successfully inputted a valid string without losing a life.
6. Whenever a player puts in an invalid string (a word that doesn't fit the prefix), the current player loses a set amount of time on their counter and the timer flashes red to indicate that the string does not fit the requirement.
7. Whenever a set number of words has been inputted successfully in the overall game (not just in a specific round) decrease the amount of time each player has to come up with a new string.
8. The current number of successful words that were inputted in the overall game are displayed, and a counter shows the amount of words that left to be inputted in order for condition "7" to be activated.
9. Once the max length of the prefix is reached, the round restarts and all players that do not have the max number of lives gets an additional life.
10. If a player inputs a string that is at least 10 characters long, the next player has 80% of the total time to input a valid string.
11.  If the string is not unique (has already been inputted before) a warning is given but no time is taken from the player.
12. If a player has inputted consecutive valid strings without losing a life, their maximum amount of time to input a valid string increases by 20%.
 */