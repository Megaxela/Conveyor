//
// Created by megaxela on 1/11/18.
//

#pragma once

#include <benchmark/benchmark.h>
#include <functional>
#include <iostream>

#define BENCHMARK_TEMPLATE_RANGE(x)               \
template<int N>                                        \
struct x##_DEDUCTOR                                    \
{                                                      \
    void operator()(::benchmark::State& state)         \
    {                                                  \
        x<N>(state);                                 \
    }                                                  \
};                                                     \
BENCHMARK_PRIVATE_DECLARE(x) =                         \
    ((TemplateFunctionBenchmark<x##_DEDUCTOR> *) (::benchmark::internal::RegisterBenchmarkInternal( \
        new TemplateFunctionBenchmark<x##_DEDUCTOR>(#x))))

template<template <int N> typename Func>
class TemplateFunctionBenchmark : public ::benchmark::internal::Benchmark
{
    using FuncMap = std::map<int, std::function<void(::benchmark::State&)>>;

public:
    explicit TemplateFunctionBenchmark(const char* name) :
        Benchmark(name),
        _name(name)
    {

    }

    template<int Start, int Limit, int Step=8>
    TemplateFunctionBenchmark* TemplateDenseRange()
    {
        DenseRange(Start, Limit, Step);

        Filler<Start, Limit, Step, true>{}(_tests);

        return this;
    };

    template<int Start, int Limit, int Step=8>
    TemplateFunctionBenchmark* TemplateRange()
    {
        RangeMultiplier(Step);
        Range(Start, Limit);

        Filler<Start, Limit, Step, false>{}(_tests);

        return this;
    };

    void Run(benchmark::State& state) override
    {
        if (_tests.empty())
        {
            std::cerr << "No tests was generated for test \"" << _name << "\"." << std::endl;
            return;
        }

        auto range = state.range(0);

        auto searchResult = _tests.find(range);
        if (searchResult == _tests.end())
        {
            std::cerr << "Can't find test with spec for \"" << range << "\"." << std::endl;
            return;
        }

        searchResult->second(state);
    }

private:

    template<
        int Current,
        int Limit,
        int Step,
        bool Dense
    >
    struct Filler
    {
        static constexpr int Next()
        {
            if (Dense)
            {
                if (Current + Step > Limit)
                {
                    return Limit;
                }

                return Current + Step;
            }
            else
            {
                if (Current * Step > Limit)
                {
                    return Limit;
                }

                return Current * Step;
            }
        }

        void operator()(FuncMap& map)
        {
            map[Current] = Func<Current>();

            Filler<
                Next(),
                Limit,
                Step,
                Dense
            >{}(map);
        }
    };

    template<
        int Limit,
        int Step,
        bool Dense
    >
    struct Filler<Limit, Limit, Step, Dense>
    {
        void operator()(FuncMap& map)
        {
            map[Limit] = Func<Limit>();
        }
    };

    FuncMap _tests;
    const char* _name;
};

