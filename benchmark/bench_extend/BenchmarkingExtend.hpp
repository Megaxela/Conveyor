#pragma once

#include <benchmark/benchmark.h>

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