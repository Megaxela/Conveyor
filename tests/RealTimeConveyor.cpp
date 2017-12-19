#include <gtest/gtest.h>
#include <Conveyor/Belt.hpp>
#include <Conveyor/FuncOperator.hpp>

class AddOperation : public Conveyor::Operator
{
public:
    std::any execute(const std::any& arg) override
    {
        return std::any_cast<int>(arg) + 5;
    }
};

TEST(RealTimeConveyor, MainFunctional)
{
    Conveyor::Belt belt;

    int count = 1000;

    for (int i = 0; i < count; ++i)
    {
        belt.addOperator(std::make_shared<AddOperation>());
    }

    auto result = std::any_cast<int>(belt.execute(10));

    ASSERT_EQ(result, 5 * count + 10);
}

TEST(RealTimeConveyor, FuncOperator)
{
    Conveyor::Belt belt;

    int count = 1000;

    for (int i = 0; i < count; ++i)
    {
        belt.addOperator(std::make_shared<Conveyor::FuncOperator>(
            [](const std::any& arg) -> std::any
            {
                return std::any_cast<int>(arg) + 5;
            }
        ));
    }

    auto result = std::any_cast<int>(belt.execute(10));

    ASSERT_EQ(result, 5 * count + 10);
}