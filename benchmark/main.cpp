#include <benchmark/benchmark.h>
#include <functional>
#include <Conveyor/Belt.hpp>
#include <Conveyor/StaticBelt.hpp>
#include <gtest/gtest.h>
#include "bench_extend/BenchmarkingExtend.hpp"
#include "bench_extend/TemplateFunctionBenchmark.hpp"

static const int count = 200;

using InternalType = uint64_t;
//constexpr const char* ADDABLE = "TEST";
//constexpr const char* INITIAL = "HELLO";
#define ADDABLE uint64_t(0x00FF00FF00FF00FF)
#define INITIAL uint64_t(0x0000FF0000FF0000)

class TestOperator : public Conveyor::Operator
{
public:
    std::any execute(const std::any &arg) override
    {
        return (std::any_cast<InternalType>(arg) + ADDABLE);
    }
};

class TestStaticOperator
{
public:
    InternalType execute(const InternalType& arg)
    {
        auto answer = (arg + ADDABLE);
        benchmark::DoNotOptimize(answer);
        return answer;
    }
};

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
                    return a + ADDABLE;
                }
            );
        }

        benchmark::DoNotOptimize(content);
    }
}

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

        benchmark::DoNotOptimize(dynamicBelt);
    }
}

static void StaticCreationSpeed(benchmark::State& state)
{
    for (auto _ : state)
    {
        // Initializing
        typename Creator<count, TestStaticOperator>::type belt;

        benchmark::DoNotOptimize(belt);
    }
}

static void FunctionExecution(benchmark::State& state)
{
    // Initializing
    std::vector<std::function<InternalType(InternalType)>> content;

    for (int i = 0; i < state.range(0); ++i)
    {
        content.emplace_back(
            [](const InternalType& a) -> InternalType
            {
                return a + ADDABLE;
            }
        );
    }

    for (auto _ : state)
    {
        InternalType tmp = INITIAL;

        for (auto&& func : content)
        {
            tmp = func(tmp);
        }

        benchmark::DoNotOptimize(tmp);
    }

    state.SetComplexityN(state.range(0));
}

static void DynamicExecution(benchmark::State& state)
{
    // Initializing

    Conveyor::Belt dynamicBelt;

    for (int i = 0; i < state.range(0); ++i)
    {
        dynamicBelt.addOperator(std::make_shared<TestOperator>());
    }

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(dynamicBelt.execute(INITIAL));
    }

    state.SetComplexityN(state.range(0));
}

template<int N>
static void StaticExecution(benchmark::State& state)
{
    typename Creator<N, TestStaticOperator>::type staticBelt;

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(staticBelt.execute(INITIAL));
    }

    state.SetComplexityN(N);
}

constexpr int StartExecutionRange = 5;
// Can't make more operators in StaticConveyor
// because it's very memory consumpting
constexpr int FinishExecutionRange = 250;
constexpr int ExecutionRangeStep = 10;

BENCHMARK(FunctionalCreationSpeed);
BENCHMARK(DynamicCreationSpeed);
BENCHMARK(StaticCreationSpeed);

BENCHMARK(FunctionExecution)
    ->DenseRange(
        StartExecutionRange,
        FinishExecutionRange,
        ExecutionRangeStep
    )->Complexity();

BENCHMARK(DynamicExecution)
    ->DenseRange(
        StartExecutionRange,
        FinishExecutionRange,
        ExecutionRangeStep
    )->Complexity();

// Can't do significant more
BENCHMARK_TEMPLATE_RANGE(StaticExecution)
    ->TemplateDenseRange<
        StartExecutionRange,
        FinishExecutionRange,
        ExecutionRangeStep
    >()->Complexity();

BENCHMARK_MAIN();