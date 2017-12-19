#include <benchmark/benchmark.h>
#include <functional>
#include <Conveyor/Belt.hpp>
#include <Conveyor/StaticBelt.hpp>
#include <gtest/gtest.h>
#include "StaticOperatorType.hpp"

static const int count = 200;

using InternalType = std::string;

static void StringForming(benchmark::State& state)
{
    for (auto _ : state)
    {
        InternalType s = "INITIAL";
        for (int i = 0; i < count; ++i)
        {
            s = std::move(s + "TEST");
        }
    }
}

BENCHMARK(StringForming);

static void FunctionalCreationSpeed(benchmark::State& state)
{
    for (auto _ : state)
    {
        // Initializing
        std::vector<std::function<InternalType(InternalType)>> content;

        for (int i = 0; i < count; ++i)
        {
            content.emplace_back(
                [](const InternalType& a) -> InternalType
                {
                    return std::move(a + "TEST");
                }
            );
        }
    }
}

BENCHMARK(FunctionalCreationSpeed);

class TestOperator : public Conveyor::Operator
{
public:
    std::any execute(const std::any &arg) override
    {
        return std::move(std::any_cast<InternalType>(arg) + "TEST");
    }
};

static void DynamicCreationSpeed(benchmark::State& state)
{
    for (auto _ : state)
    {
        // Initializing
        Conveyor::Belt dynamicBelt;

        for (int i = 0; i < count; ++i)
        {
            dynamicBelt.addOperator(std::make_shared<TestOperator>());
        }
    }
}

BENCHMARK(DynamicCreationSpeed);

static void StaticCreationSpeed(benchmark::State& state)
{
    for (auto _ : state)
    {
        // Initializing
        HugeBeltCreator<TestOperator>::Belt belt;
    }
}

BENCHMARK(StaticCreationSpeed);

static void FunctionalWorkSpeed(benchmark::State& state)
{
    // Initializing
    std::vector<std::function<InternalType(InternalType)>> functionalBelt;

    for (int i = 0; i < count; ++i)
    {
        functionalBelt.emplace_back(
            [](const InternalType& a)
            {
                return std::move(a + "TEST");
            }
        );
    }

    for (auto _ : state)
    {
        InternalType argument = "INITIAL";

        for (auto&& el : functionalBelt)
        {
            argument = el(argument);
        }
    }
}

BENCHMARK(FunctionalWorkSpeed);

static void DynamicWorkSpeed(benchmark::State& state)
{
    // Initializing
    Conveyor::Belt dynamicBelt;

    for (int i = 0; i < count; ++i)
    {
        dynamicBelt.addOperator(std::make_shared<TestOperator>());
    }

    for (auto _ : state)
    {
        dynamicBelt.execute(InternalType("INITIAL"));
    }
}

BENCHMARK(DynamicWorkSpeed);

class TestStaticOperator
{
public:
    InternalType execute(const InternalType& arg)
    {
        return std::move(arg + "TEMP");
    }
};

static void StaticWorkSpeed(benchmark::State& state)
{
    // Initializing
    HugeBeltCreator<TestStaticOperator>::Belt belt;

    for (auto _ : state)
    {
        belt.execute("INITIAL");
    }
}

BENCHMARK(StaticWorkSpeed);

BENCHMARK_MAIN();