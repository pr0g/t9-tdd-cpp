#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include <vector>

using namespace testing;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/// 1 ...  2 abc 3 def 
/// 4 ghi  5 jkl 6 mno
/// 7 pqrs 8 tuv 9 wxyz

class T9
{
public:
    std::vector<std::string> words_from_digits(
        const std::vector<size_t> digits)
    {
        const auto words = [&digits](){
            if (digits[0] == 2)
            {
                return std::vector<std::string>{ "a", "b", "c" };
            }
            else if (digits[0] == 3)
            {
                return std::vector<std::string>{ "d", "e", "f" };
            }

            return std::vector<std::string>{};
        }();

        return words;
    }
private:

};

class T9Fixture : public Test
{
public:
    T9 t9;
};

TEST_F(T9Fixture, SingleDigitTwoGivesABC)
{
    const auto words = t9.words_from_digits( { 2 } );
    ASSERT_THAT(words, ElementsAre( "a", "b", "c" ));
}

TEST_F(T9Fixture, SingleDigitThreeGivesDEF)
{
    const auto words = t9.words_from_digits( { 3 } );
    ASSERT_THAT(words, ElementsAre( "d", "e", "f" ));
}

TEST_F(T9Fixture, DoubleDigitTwoGivesAAABBACBACA)
{
    const auto words = t9.words_from_digits( { 2, 2 } );
    ASSERT_THAT(words, ElementsAre( "aa", "ab", "ba", "ca" ));
}