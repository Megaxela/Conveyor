#include <gtest/gtest.h>
#include <Conveyor/StaticBelt.hpp>
#include <memory>

#define MYLIB_RESOLVE_OVERLOAD(...) \
	[](auto&&...args)->decltype(auto){return __VA_ARGS__(std::forward<decltype(args)>(args)...);}

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

//TEST(StaticConveyor, EmbeddedBelt)
//{
//    Conveyor::StaticBelt<
//        Conveyor::StaticBelt<
//            SomeAction1,
//            SomeAction1
//        >
//    > belt;
//
//    belt.execute(12);
//
//    ASSERT_EQ(belt.execute(12), 5 * 5 + 12);
//}

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