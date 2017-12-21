#include <gtest/gtest.h>
#include <Conveyor/StaticBelt.hpp>
#include "TestingExtend.hpp"

static int constructed = 0;
static int destructed = 0;

static int copies = 0;
static int moves = 0;

class CopyMoveCounter
{
public:
    CopyMoveCounter()
    {
        ++constructed;
    }

    ~CopyMoveCounter()
    {
        ++destructed;
    }

    CopyMoveCounter(const CopyMoveCounter&& rhs) noexcept
    {
        ++moves;
    }

    CopyMoveCounter(const CopyMoveCounter& rhs)
    {
        ++copies;
    }

    CopyMoveCounter& operator=(const CopyMoveCounter& rhs)
    {
        ++copies;

        return (*this);
    }

    CopyMoveCounter& operator=(const CopyMoveCounter&& rhs) noexcept
    {
        ++copies;

        return (*this);
    }

};

template<typename T>
class PlaceholderOperator
{
public:
    T execute(const T& arg)
    {
        return std::move(arg);
    }
};

TEST(StaticConveyor, CopiesMovesCount)
{
    Conveyor::StaticBelt<
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>,
        PlaceholderOperator<CopyMoveCounter>
    > belt;

    belt.execute(CopyMoveCounter());

    TestCout() << " Number of operators: " << belt.numberOfOperators();
    TestCout() << " Constructed: " << constructed;
    TestCout() << " Destructed : " << destructed;
    TestCout() << " Moves      : " << moves;
    TestCout() << " Copies     : " << copies;

    ASSERT_EQ(constructed, 1);
    ASSERT_EQ(moves, belt.numberOfOperators());
    ASSERT_EQ(copies, 0);

    constructed = 0;
    moves = 0;
    copies = 0;
}