
#include <gtest/gtest.h>
#include "Conveyor/StaticMultiplexBelt.hpp"

class Operator1
{
public:
    template<int N>
    int executeForward(int z)
    {
        return z + N;
    }

    template<int N>
    int executeBackward(int z)
    {
        return z - N;
    }
};

class Operator2
{
public:
    template<int N>
    std::string executeForward(int z)
    {
        return std::to_string(z);
    }

    template<int>
    int executeBackward(std::string s)
    {
        return std::stoi(s);
    }
};

TEST(StaticMultiplexBelt, Basic)
{
    Conveyor::StaticMultiplexBelt<
        Operator1,
        Operator1,
        Operator1,
        Operator2
    > belt;

    ASSERT_EQ(belt.numberOfOperators(), 4);

    auto answerForward = belt.executeForward<2>(2);

    ASSERT_EQ(answerForward, "8");

    auto answerBackward = belt.executeBackward<2>(answerForward);

    ASSERT_EQ(answerBackward, 2);

    auto anotherAnswerForward = belt.executeForward<4>(2);

    ASSERT_EQ(anotherAnswerForward, "14");

    auto anotherAnswerBackward = belt.executeBackward<4>(anotherAnswerForward);

    ASSERT_EQ(anotherAnswerBackward, 2);
}

TEST(StaticMultiplexBelt, EmbeddedBelt)
{
    Conveyor::StaticMultiplexBelt<
        Operator1,
        Conveyor::StaticMultiplexBelt<
            Operator1,
            Operator1,
            Conveyor::StaticMultiplexBelt<
                Operator1,
                Operator1,
                Operator1
            >
        >,
        Operator2
    > embeddedBelt;

    auto answerForward = embeddedBelt.executeForward<2>(2);

    ASSERT_EQ(answerForward, "14");

    auto answerBackward = embeddedBelt.executeBackward<2>(answerForward);

    ASSERT_EQ(answerBackward, 2);
}