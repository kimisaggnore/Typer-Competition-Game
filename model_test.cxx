#include "model.hxx"
#include <catch.hxx>
static std::vector<std::string>
load_dictionary2(std::string const& filename)
{
    std::ifstream dictionary = ge211::open_resource_file(filename);
    std::vector<std::string> result;
    std::string buffer;

    while (std::getline(dictionary, buffer)) {
        result.push_back(buffer);
    }

    if (dictionary.bad()) {
        throw std::runtime_error("could not read dictionary: " + filename);
    }

    return result;
}
//test1: player only loses life if other player successfully spells while
//the current player can't!
TEST_CASE("test1")
{
    std::vector<std::string> words = load_dictionary2("dictionary.dat");
    Model m = Model(words);
    // a prefix is set when constructor is called.
    // cur dict is set to an empty dictionary.
    CHECK(m.player_turn_ == 1);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 0);
    CHECK(m.round_num == 0);
    CHECK(m.check_success() == 0);

    m.cur_dict = m.construct_dictionary(words, m.prefix_, true);
    // initialize current dictionary
    //std::cout << "here\n" << "";

    // initialize time mt. set rng_num to be based of the size of the CURRENT
    // DICTIONARY. rng_num is from 0- cur_dict.size().
    m.mt.seed(std::chrono::system_clock::now().time_since_epoch().count());
    int rng_num = m.mt() % m.cur_dict.size();

    std::string rand_valid = m.cur_dict[rng_num];
    // pick a valid random word.

    std::string typedword1 = rand_valid.substr(m.prefix_.size(),
                                                 rand_valid.size());

    m.typed_word = typedword1;

    CHECK(m.valid_input(m.prefix_ + m.typed_word, m.cur_dict, false));
    m.make_move();
    CHECK(m.check_success() == 1);
    CHECK(m.player_turn_ == 2);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 1);
    CHECK(m.round_num == 0);

    //std::string prefix2 = m.prefix_;
    //m.typed_word = typedword2.substr(m.prefix_.size(), typedword2.size());
    //m.prefix_ + m.typed_word
    m.typed_word =
            "abunchofnonsense";
    CHECK(m.valid_input
    (m.prefix_ +
    "abunchofnonsense", m.cur_dict, false) == false);
    CHECK(m.player_time == 21 * 60);
    m.make_move();
    CHECK(m.player_time == 18 * 60);
    CHECK(m.check_success() == 1);
    m.time_is_up();
    CHECK(m.check_success() == 0);
    CHECK(m.player_turn_ == 1);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 3);
    CHECK(m.counter == 0);
    CHECK(m.round_num == 0);

}
//test2: neither player loses lives if both players can't spell successfully
// within
//same round!
TEST_CASE("testcase2")
{

    std::vector<std::string> words = load_dictionary2("dictionary.dat");
    Model m = Model(words);
    CHECK(m.player_turn_ == 1);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 0);
    CHECK(m.round_num == 0);
    CHECK(m.check_success() == 0);

    std::string character = m.prefix_;
    m.cur_dict = m.construct_dictionary(words, m.prefix_, true);
    std::vector<std::string> sample_dict = m.cur_dict;

    m.mt.seed(std::chrono::system_clock::now().time_since_epoch().count());

    m.typed_word = "abunchofnonsense";
    CHECK(m.valid_input(character + m.typed_word, words, false) == false);
    CHECK(m.player_time == 21 * 60);
    m.make_move();
    CHECK(m.player_time == 18 * 60);
    CHECK(m.check_success() == 0);
    m.time_is_up();
    CHECK(m.player_turn_ == 2);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 1);
    CHECK(m.round_num == 0);

    m.typed_word = "abunchofnonsense2";
    CHECK(m.valid_input(character + m.typed_word, words, false) == false);
    CHECK(m.player_time == 21 * 60);
    m.make_move();
    CHECK(m.player_time == 18 * 60);
    CHECK(m.check_success() == 0);
    m.time_is_up();
    CHECK(m.check_success() == 0);
    CHECK(m.player_turn_ == 1);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 0);
    CHECK(m.round_num == 0);


}
//test case3: When current dictionary doesn't have enough valid words to
// continue the round, completely reset the prefix.
TEST_CASE("test case 3")
{
    std::vector<std::string> words = load_dictionary2("dictionary.dat");
    Model m = Model(words);
    CHECK(m.player_turn_ == 1);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 0);
    CHECK(m.round_num == 0);
    CHECK(m.check_success() == 0);
    m.prefix_ = "zyth";
    std::vector<std::string> sample_dict = m.construct_dictionary(words,
                                                                  m.prefix_,
                                                                  false);
    std::string new_prefix = m.set_new_prefix(sample_dict, m.prefix_);
    CHECK(new_prefix.size() == 1);
    /*std::vector<std::string> new_dict = m.construct_dictionary(sample_dict,
                                                               new_prefix,
                                                               false);*/
}

//test case4: makes sure drain power is restored when a word is greater than
//power_word_len field of model(# of characters) and drain_power works as
//expected!
TEST_CASE("test case 4")
{
    std::vector<std::string> words = load_dictionary2("dictionary.dat");
    Model m = Model(words);
    CHECK(m.player_turn_ == 1);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 0);
    CHECK(m.round_num == 0);
    CHECK(m.p1_drain == 1);
    CHECK(m.p1_drain == 1);
    CHECK(m.active_drain == 0);
    CHECK(m.check_success() == 0);
    m.drain_next_player();
    CHECK(m.active_drain == 1);
    int orig_time = m.player_time;
    std::string character = m.prefix_;
    std::vector<std::string> sample_dict = m.construct_dictionary(words,
                                                                  character,
                                                                  true);
    m.mt.seed(std::chrono::system_clock::now().time_since_epoch().count());
    int rng_num = m.mt() % sample_dict.size();
    std::string rand_valid = sample_dict[rng_num];
    std::string typedword1 = rand_valid.substr(character.size(),
                                               rand_valid.size());
    m.typed_word = typedword1;

    CHECK(m.valid_input(character + m.typed_word, words, false));
    m.make_move();
    m.p1_drain = 0;
    CHECK(m.player_turn_ == 2);
    CHECK(m.player_time == orig_time / 2);
    CHECK(m.p1_drain == 0);
    CHECK(m.p2_drain == 1);

    m.typed_word = "abunchofnonsense2";
    CHECK(m.valid_input(character + m.typed_word, words, false) != true);
    m.make_move();
    m.time_is_up();
    CHECK(m.player_turn_ == 1);
    std::string last_prefix = m.prefix_;
    std::vector<std::string> last_dict = m.construct_dictionary(words,
                                                                last_prefix,
                                                                false);
    int current_char_count = 0;
    std::string rand_valid2 = "";
    while (current_char_count < m.power_word_len + 1) {
        int rng_num2 = m.mt() % last_dict.size();
        rand_valid2 = last_dict[rng_num2];
        current_char_count = rand_valid2.size();
    }
    CHECK(m.valid_input(rand_valid2, last_dict, false));
    m.typed_word = rand_valid2.substr(last_prefix.size(), rand_valid2.size());
    m.make_move();
    CHECK(m.p1_drain == 1);
}

//test case 5: if total number of words inputted is equal to ten over the
//course of game, the overall time allotted to each player is 1 less than
//before!
TEST_CASE("test case 5")
{
    std::vector<std::string> words = load_dictionary2("dictionary.dat");
    Model m = Model(words);
    // a prefix is set when constructor is called.
    // cur dict is set to an empty dictionary.
    m.reduce_time_counter = 1;
    CHECK(m.player_turn_ == 1);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 0);
    CHECK(m.round_num == 0);
    CHECK(m.check_success() == 0);
    CHECK(m.reduce_time_counter == 1);
    m.cur_dict = m.construct_dictionary(words, m.prefix_, true);
    // initialize current dictionary
    //std::cout << "here\n" << "";

    // initialize time mt. set rng_num to be based of the size of the CURRENT
    // DICTIONARY. rng_num is from 0- cur_dict.size().
    m.mt.seed(std::chrono::system_clock::now().time_since_epoch().count());
    int rng_num = m.mt() % m.cur_dict.size();

    std::string rand_valid = m.cur_dict[rng_num];
    // pick a valid random word.

    std::string typedword1 = rand_valid.substr(m.prefix_.size(),
                                               rand_valid.size());

    m.typed_word = typedword1;


    CHECK(m.valid_input(m.prefix_ + m.typed_word, m.cur_dict, false));
    int stored_time = m.player_time;

    m.make_move();

    CHECK(m.check_success() == 1);
    CHECK(m.player_turn_ == 2);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 1);
    CHECK(m.round_num == 0);
    CHECK(m.reduce_time_counter == 10);

    int updated_time = m.player_time;

    CHECK(updated_time == stored_time*.9);

}

//Tests the use of the swap prefix ability and properly tests the condition
// where the swap ability is replenished.
TEST_CASE("test case 6")
{
    std::vector<std::string> words = load_dictionary2("dictionary.dat");
    Model m = Model(words);
    // a prefix is set when constructor is called.
    // cur dict is set to an empty dictionary.
    CHECK(m.player_turn_ == 1);
    CHECK(m.p1_lives == 4);
    CHECK(m.p2_lives == 4);
    CHECK(m.counter == 0);
    CHECK(m.round_num == 0);
    CHECK(m.check_success() == 0);
    CHECK(m.p1_swap == 1);
    m.p1_streak = 2;

    m.cur_dict = m.construct_dictionary(words, m.prefix_, true);

    m.swap_prefix();
    m.p1_swap = 0;
    CHECK(m.p1_swap == 0);
    CHECK(m.prefix_swapped == 1);

    m.cur_dict = m.construct_dictionary(words, m.prefix_, true);
    m.mt.seed(std::chrono::system_clock::now().time_since_epoch().count());
    int rng_num = m.mt() % m.cur_dict.size();

    std::string rand_valid = m.cur_dict[rng_num];
    // pick a valid random word.

    std::string typedword1 = rand_valid.substr(m.prefix_.size(),
                                               rand_valid.size());

    m.typed_word = typedword1;
    CHECK(m.valid_input(m.prefix_ + m.typed_word, m.cur_dict, false));
    m.make_move();

    CHECK(m.p1_streak == 0);

    CHECK(m.p1_swap == 1);

}