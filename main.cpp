#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include <vector>

using namespace testing;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/// 1 ...  2 abc 3 def 
/// 4 ghi  5 jkl 6 mno
/// 7 pqrs 8 tuv 9 wxyz

// could use inheritance?

struct DigitLetter
{
    DigitLetter() = default;

    DigitLetter(
        int8_t digit_, int8_t letter_, std::vector<DigitLetter> children_)
        : digit(digit_), letter(letter_), children(std::move(children_))
    {}

    int8_t digit = 0;
    char letter = '\0';
    std::vector<DigitLetter> children;
};

class T9
{
public:
    std::vector<std::string> words_from_digits(
        const std::vector<size_t>& digits)
    {
        DigitLetter* current_digit_letter = &root_;

        std::string word;
        std::vector<std::string> words;

        struct DigitLetterTouched
        {
            DigitLetter* dl;
            size_t child_index;
        };

        std::vector<DigitLetterTouched> walked;

        size_t char_index = 0;
        size_t digit_index = 0;
        while (digit_index < digits.size()) {
            while (true) {
                if (char_index < current_digit_letter->children.size()) {
                    if (current_digit_letter->children[char_index].digit == digits[digit_index]) {
                        word.push_back(current_digit_letter->children[char_index].letter);
                        walked.push_back({current_digit_letter, char_index});
                        current_digit_letter = &current_digit_letter->children[char_index];
                        digit_index++;

                        if (digit_index == digits.size()) {
                            words.push_back(word);
                            word.pop_back();
                            current_digit_letter = walked.back().dl;
                            char_index = ++walked.back().child_index;
                            walked.pop_back();
                            digit_index--;
                        } else {
                            char_index = 0;
                            break;
                        }
                    } else {
                        char_index++;
                    }
                } else {
                    if (word.empty())
                        return words;

                    word.pop_back();
                    current_digit_letter = walked.back().dl;
                    char_index = ++walked.back().child_index;
                    walked.pop_back();
                    digit_index--;
                    break;
                }
            }
        }

        return words;
    }

    void add_word(const std::string& word)
    {
        DigitLetter* current = &root_;
        for (char char_index : word) {
            auto digitLetter = std::find_if(
                current->children.begin(), current->children.end(),
                [&word, &char_index](const auto& digit_letter) {
                    return digit_letter.letter == char_index;
                });

            if (digitLetter == current->children.end()) {
                current = &current->children.emplace_back(
                    number_from_letter(char_index), char_index,
                    std::vector<DigitLetter>{});
            } else {
                current = &*digitLetter;
            }
        }
    }

    uint8_t number_from_letter(const char letter) const
    {
        const uint8_t letter_to_numbers[] =
            {2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5,
             6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9};
        const uint8_t offset = letter - 'a';
        return letter_to_numbers[offset];
    }

private:
    DigitLetter root_;
};

class T9Fixture : public Test
{
public:
    T9 t9;
};

 TEST_F(T9Fixture, SingleDigitTwoGivesABC)
 {
     t9.add_word("ask");
     t9.add_word("ball");
     t9.add_word("cat");
     const auto words = t9.words_from_digits( { 2 } );
     ASSERT_THAT(words, ElementsAre( "a", "b", "c" ));
 }

 TEST_F(T9Fixture, SingleDigitThreeGivesDEF)
 {
     t9.add_word("dent");
     t9.add_word("east");
     t9.add_word("fast");
     const auto words = t9.words_from_digits( { 3 } );
     ASSERT_THAT(words, ElementsAre( "d", "e", "f" ));
 }

 TEST_F(T9Fixture, DoubleDigitTwoGivesAAABBACBACA)
 {
     t9.add_word("aardvark");
     t9.add_word("abacus");
     t9.add_word("ball");
     t9.add_word("cat");
     const auto words = t9.words_from_digits( { 2, 2 } );
     ASSERT_THAT(words, ElementsAre( "aa", "ab", "ba", "ca" ));
 }

TEST_F(T9Fixture, HelloInDictionary)
{
    t9.add_word("hello");
    const auto words = t9.words_from_digits({4, 3, 5, 5, 6});

    ASSERT_THAT(words, ElementsAre("hello"));
}

TEST_F(T9Fixture, HelloAndGekkoInDictionary)
{
    t9.add_word("gekko");
    t9.add_word("hello");
    const auto words = t9.words_from_digits({4, 3, 5, 5, 6});

    ASSERT_THAT(words, ElementsAre("gekko", "hello"));
}

TEST_F(T9Fixture, FourSixSixThreeWords)
{
    t9.add_word("gondola");
    t9.add_word("gone");
    t9.add_word("good");
    t9.add_word("goof");
    t9.add_word("home");
    t9.add_word("hone");
    t9.add_word("hood");
    t9.add_word("gekko");
    t9.add_word("hello");

    const auto words = t9.words_from_digits({4, 6, 6, 3});

    const auto words2 = t9.words_from_digits({4, 3, 5, 5, 6});

    //ASSERT_THAT(words, ElementsAre("gekko", "hello"));
}

TEST_F(T9Fixture, NumberFromLetters)
{
    struct NumberLetterPair
    {
        uint8_t number;
        char letter;
    };

    const NumberLetterPair valid_pairs[] = {
        {2, 'a'}, {2, 'b'}, {2, 'c'},
        {3, 'd'}, {3, 'e'}, {3, 'f'},
        {4, 'g'}, {4, 'h'}, {4, 'i'},
        {5, 'j'}, {5, 'k'}, {5, 'l'},
        {6, 'm'}, {6, 'n'}, {6, 'o'},
        {7, 'p'}, {7, 'q'}, {7, 'r'}, {7, 's'},
        {8, 't'}, {8, 'u'}, {8, 'v'},
        {9, 'w'}, {9, 'x'}, {9, 'y'}, {9, 'z'}};

    for (const auto pair : valid_pairs) {
        ASSERT_THAT(t9.number_from_letter(pair.letter), Eq(pair.number));
    }
}