#include <benchmark/benchmark.h>
#include <chrono>
#include <thread>

static void BM_StringCreation(benchmark::State& state)
{
    for (auto _ : state)
        std::string empty_string;
}

static void BM_StringCopy(benchmark::State& state)
{
    std::string x = "hello";

    for (auto _ : state)
        std::string copy(x);
}

static void BM_memcpy(benchmark::State& state)
{
    char* src = new char[state.range(0)];
    char* dst = new char[state.range(0)];

    memset(src, 'x', state.range(0));

    for (auto _ : state)
        benchmark::DoNotOptimize(memcpy(dst, src, state.range(0)));

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));

    delete[] src;
    delete[] dst;
}

static void BM_StringCompare(benchmark::State& state)
{
    std::string s1(state.range(0), '-');
    std::string s2(state.range(0), '-');

    for (auto _ : state)
        benchmark::DoNotOptimize(s1.compare(s2));

    state.SetComplexityN(state.range(0));
}

static void BM_DoNotOptimizeTest(benchmark::State& state)
{
    for (auto _ : state) {
        int x = 0;
        for (int i = 0; i < 64; ++i) {
            benchmark::DoNotOptimize(x += i);
        }
    }
}

static void BM_vector_push_back(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(1);
        benchmark::DoNotOptimize(v.data()); // Allow v.data() to be clobbered.
        v.push_back(42);
        benchmark::ClobberMemory(); // Force 42 to be written to memory.
    }
}

static void BM_Milliseconds(benchmark::State& state)
{
    for (auto _ : state)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

BENCHMARK(BM_StringCopy);
BENCHMARK(BM_StringCreation);
BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(1 << 13);
BENCHMARK(BM_StringCompare)->RangeMultiplier(2)->Range(1 << 10, 1 << 14)->Complexity(benchmark::oN);
BENCHMARK(BM_DoNotOptimizeTest);
BENCHMARK(BM_vector_push_back);
BENCHMARK(BM_Milliseconds)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
