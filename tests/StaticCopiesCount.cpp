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
    explicit CopyMoveCounter(int i)
    {
        ++constructed;
    }

    CopyMoveCounter()
    {
        ++constructed;
    }

    ~CopyMoveCounter()
    {
        ++destructed;
    }

    CopyMoveCounter(CopyMoveCounter&& rhs) noexcept
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

    CopyMoveCounter& operator=(CopyMoveCounter&& rhs) noexcept
    {
        ++moves;

        return (*this);
    }

    int execute(int x)
    {
        return x;
    }

};

template<typename T>
class PlaceholderOperator
{
public:
    T execute(T arg)
    {
        return arg;
    }
};


template<int Count, typename Operator, typename... Operators>
struct Creator
{
    using type = typename Creator<Count - 1, Operator, Operators..., Operator>::type;
};

template<typename Operator, typename... Operators>
struct Creator<0, Operator, Operators...>
{
    using type = Conveyor::StaticBelt<Operators...>;
};

TEST(StaticConveyor, ConstructorOperatorCopyMoveCount)
{
    constructed = 0;
    destructed = 0;
    copies = 0;
    moves = 0;

    Creator<4, CopyMoveCounter>::type belt(1, 2, 3, 4);

    TestCout() << " Number of operators: " << belt.numberOfOperators();
    TestCout() << " Constructed: " << constructed;
    TestCout() << " Destructed : " << destructed;
    TestCout() << " Moves      : " << moves;
    TestCout() << " Copies     : " << copies;

    ASSERT_EQ(constructed, belt.numberOfOperators());
    ASSERT_EQ(destructed,  0);
    ASSERT_EQ(moves,       0);
    ASSERT_EQ(copies,      0);
}

TEST(StaticConveyor, DefaultConstructorOperatorCopyMoveCount)
{
    constructed = 0;
    destructed = 0;
    copies = 0;
    moves = 0;

    Creator<4, CopyMoveCounter>::type belt;

    TestCout() << " Number of operators: " << belt.numberOfOperators();
    TestCout() << " Constructed: " << constructed;
    TestCout() << " Destructed : " << destructed;
    TestCout() << " Moves      : " << moves;
    TestCout() << " Copies     : " << copies;
}

TEST(StaticConveyor, NoArgsOperatorCopyMoveCount)
{
    constructed = 0;
    destructed = 0;
    copies = 0;
    moves = 0;

    auto belt = Creator<4, CopyMoveCounter>::type(
        CopyMoveCounter(),
        CopyMoveCounter(),
        CopyMoveCounter(),
        CopyMoveCounter()
    );

    TestCout() << " Number of operators: " << belt.numberOfOperators();
    TestCout() << " Constructed: " << constructed;
    TestCout() << " Destructed : " << destructed;
    TestCout() << " Moves      : " << moves;
    TestCout() << " Copies     : " << copies;
}

TEST(StaticConveyor, ArgumentCopiesMovesCount)
{
    constructed = 0;
    destructed = 0;
    copies = 0;
    moves = 0;

    Creator<128, PlaceholderOperator<CopyMoveCounter>>::type belt;

    auto copy = CopyMoveCounter();

    belt.execute(std::move(copy));

    TestCout() << " Number of operators    : " << belt.numberOfOperators();
    TestCout() << " Constructed            : " << constructed;
    TestCout() << " Destructed (no * 2 + 2): " << destructed;
    TestCout() << " Moves      (no * 2 + 2): " << moves;
    TestCout() << " Copies                 : " << copies;

    ASSERT_EQ(constructed, 1);
    ASSERT_EQ(destructed, belt.numberOfOperators() * 2 + 2);
    ASSERT_EQ(moves, belt.numberOfOperators() * 2 + 2);
    ASSERT_EQ(copies, 0);

    constructed = 0;
    moves = 0;
    copies = 0;
    destructed = 0;
}