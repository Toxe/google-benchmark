#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <vector>

const std::string test_string{"2020-03-18 19:24:40\t1000001\t99\t1182\t192.168.25.78\tuser.delete\tMozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"};

std::vector<std::string> split_string(const std::string& s, const char delim)
{
    std::vector<std::string> tokens;
    std::string::size_type start_pos = 0;
    std::string::size_type pos;

    while ((pos = s.find(delim, start_pos)) != std::string::npos) {
        const std::string sub{s.substr(start_pos, pos - start_pos)};
        tokens.push_back(sub);
        start_pos = pos + 1;
    }

    if (start_pos < s.size()) {
        const std::string sub{s.substr(start_pos)};
        tokens.push_back(sub);
    }

    return tokens;
}

std::vector<std::string> split_string_emplace_back(const std::string& s, const char delim)
{
    std::vector<std::string> tokens;
    std::string::size_type start_pos = 0;
    std::string::size_type pos;

    while ((pos = s.find(delim, start_pos)) != std::string::npos) {
        tokens.emplace_back(s.substr(start_pos, pos - start_pos));
        start_pos = pos + 1;
    }

    if (start_pos < s.size())
        tokens.emplace_back(s.substr(start_pos));

    return tokens;
}

static void BM_SplitString(benchmark::State& state)
{
    for (auto _ : state) {
        auto tokens{split_string(test_string, '\t')};
        benchmark::DoNotOptimize(tokens);
    }
}

static void BM_SplitString_emplace_back(benchmark::State& state)
{
    for (auto _ : state) {
        auto tokens{split_string_emplace_back(test_string, '\t')};
        benchmark::DoNotOptimize(tokens);
    }
}

BENCHMARK(BM_SplitString);
BENCHMARK(BM_SplitString_emplace_back);

BENCHMARK_MAIN();
