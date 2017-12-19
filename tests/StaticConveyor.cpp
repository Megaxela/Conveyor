#include <gtest/gtest.h>
#include <Conveyor/StaticBelt.hpp>
#include <memory>

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