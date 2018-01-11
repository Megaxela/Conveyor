//
// Created by megaxela on 1/10/18.
//

#pragma once

#include <benchmark/benchmark.h>

//template <class ...Types>
//void BenchmarkClassnameTemplateRange() {
//    swallow(benchmark::RegisterBenchmark(NAME(Types), &BM_template<Types>)...);
//}
//
//template <int ...Values>
//void BenchmarkIntTemplateRange() {
//    swallow(benchmark::RegisterBenchmark(NAME(Values), &BM_template<Values>)...);
//}

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