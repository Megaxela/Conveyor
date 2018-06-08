#include <gtest/gtest.h>
#include <Conveyor/StaticBelt.hpp>
#include <memory>

class Arg1
{
public:
    Arg1() :
        value(0)
    {}

    explicit Arg1(int v) :
        value(v)
    {}

    int value;
};

class Arg2
{
public:
    Arg2() :
        value(0)
    {}

    explicit Arg2(int v) :
        value(v)
    {}

    int value;
};

class Arg3
{
public:
    Arg3() :
        value1(0),
        value2(0)
    {}

    Arg3(int v, char b) :
        value1(v),
        value2(b)
    {}

    int value1;
    char value2;
};

class SomeAction1
{
public:
    int execute(int z)
    {
        return z + 5;
    }
};

TEST(StaticConveyor, MainFunctional)
{

    Conveyor::StaticBelt<
        SomeAction1,
        SomeAction1,
        SomeAction1,
        SomeAction1,
        SomeAction1
    > belt;

    ASSERT_EQ(belt.execute(12), 5 * 5 + 12);
}

TEST(StaticConveyor, EmbeddedBelt)
{
    Conveyor::StaticBelt<
        Conveyor::StaticBelt<
            SomeAction1,
            SomeAction1
        >,
        Conveyor::StaticBelt<
            SomeAction1
        >
    > conveyor;

    ASSERT_EQ(conveyor.numberOfOperators(), 2);
    ASSERT_EQ(conveyor.at<0>().numberOfOperators(), 2);
    ASSERT_EQ(conveyor.at<1>().numberOfOperators(), 1);

    ASSERT_EQ(conveyor.execute(12), 5 * 3 + 12);
}

TEST(StaticConveyor, EmbeddedBeltInit)
{
    Conveyor::StaticBelt<
        Conveyor::StaticBelt<
            Arg1,
            Arg2,
            Arg3
        >,
        Arg2,
        Arg3
    > defaultBelt(
        Conveyor::StaticBelt<
            Arg1,
            Arg2,
            Arg3
        >{0, 1, Arg3(2, 3)},
        4,
        Arg3(5, 6)
    );

    ASSERT_EQ(defaultBelt.at<0>().at<0>().value, 0);
    ASSERT_EQ(defaultBelt.at<0>().at<1>().value, 1);
    ASSERT_EQ(defaultBelt.at<0>().at<2>().value1, 2);
    ASSERT_EQ(defaultBelt.at<0>().at<2>().value2, 3);
    ASSERT_EQ(defaultBelt.at<1>().value, 4);
    ASSERT_EQ(defaultBelt.at<2>().value1, 5);
    ASSERT_EQ(defaultBelt.at<2>().value2, 6);
}

class SomeAction2
{
public:
    std::string execute(int z)
    {
        return std::to_string(z);
    }
};

TEST(StaticConveyor, ElementAccess)
{
    Conveyor::StaticBelt<
        SomeAction1,
        SomeAction2
    > belt;

    int         firstResult  = belt.at<0>().execute(5);
    std::string secondResult = belt.at<1>().execute(firstResult);

    ASSERT_EQ(secondResult, "10");
}

TEST(StaticConveyor, DefaultConstructors)
{
    Conveyor::StaticBelt<
        Arg1,
        Arg2,
        Arg3
    > defaultBelt;

    ASSERT_EQ(defaultBelt.at<0>().value, 0);
    ASSERT_EQ(defaultBelt.at<1>().value, 0);
    ASSERT_EQ(defaultBelt.at<2>().value1, 0);
    ASSERT_EQ(defaultBelt.at<2>().value2, 0);
}

TEST(StaticConveyor, InitConstructors)
{
    Conveyor::StaticBelt<
        Arg1,
        Arg2,
        Arg3
    > defaultBelt(1, 2, Arg3(3, 4));

    ASSERT_EQ(defaultBelt.at<0>().value, 1);
    ASSERT_EQ(defaultBelt.at<1>().value, 2);
    ASSERT_EQ(defaultBelt.at<2>().value1, 3);
    ASSERT_EQ(defaultBelt.at<2>().value2, 4);
}