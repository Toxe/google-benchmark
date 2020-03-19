#include <benchmark/benchmark.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::vector<std::string> test_strings{
    "2020-03-17 16:58:06", "1000001", "3118", "1022", "192.168.152.50", "project.view", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:58:08", "1000002", "2640", "1137", "192.168.195.114", "collection.view_archive", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:58:58", "1000003", "111", "1121", "192.168.228.209", "user.delete", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:59:02", "1000004", "135", "1060", "192.168.194.102", "doc.update", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:59:08", "1000005", "2356", "1109", "192.168.174.108", "project.view", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"};

std::vector<std::vector<std::string>> test_string_sets{
    {"2020-03-17 16:58:06", "1000001", "3118", "1022", "192.168.152.50", "project.view", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"},
    {"2020-03-17 16:58:08", "1000002", "2640", "1137", "192.168.195.114", "collection.view_archive", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"},
    {"2020-03-17 16:58:58", "1000003", "111", "1121", "192.168.228.209", "user.delete", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"},
    {"2020-03-17 16:59:02", "1000004", "135", "1060", "192.168.194.102", "doc.update", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"},
    {"2020-03-17 16:59:08", "1000005", "2356", "1109", "192.168.174.108", "project.view", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"}};

std::string concat_strings(const std::vector<std::string>& strings)
{
    std::string big;

    for (const auto& s : strings)
        big += s;

    return big;
}

std::string concat_strings_with_stringstream(const std::vector<std::string>& strings)
{
    std::stringstream big;

    for (const auto& s : strings)
        big << s;

    return big.str();
}

std::string concat_string_sets(const std::vector<std::vector<std::string>>& string_sets, const std::string& delim)
{
    std::string big;

    for (std::size_t i = 0; i < string_sets.size(); ++i) {
        for (const auto& s : string_sets[i])
            big += s;

        if (i < (string_sets.size() - 1))
            big += delim;
    }

    return big;
}

std::string concat_string_sets_with_stringstream(const std::vector<std::vector<std::string>>& string_sets, const std::string& delim)
{
    std::stringstream big;

    for (std::size_t i = 0; i < string_sets.size(); ++i) {
        for (const auto& s : string_sets[i])
            big << s;

        if (i < (string_sets.size() - 1))
            big << delim;
    }

    return big.str();
}

static void BM_ConcatStrings(benchmark::State& state)
{
    for (auto _ : state) {
        auto s{concat_strings(test_strings)};
        benchmark::DoNotOptimize(s);
    }
}

static void BM_ConcatStrings_with_Stringstream(benchmark::State& state)
{
    for (auto _ : state) {
        auto s{concat_strings_with_stringstream(test_strings)};
        benchmark::DoNotOptimize(s);
    }
}

static void BM_ConcatStringSets(benchmark::State& state)
{
    for (auto _ : state) {
        auto s{concat_string_sets(test_string_sets, ",")};
        benchmark::DoNotOptimize(s);
    }
}

static void BM_ConcatStringSets_with_Stringstream(benchmark::State& state)
{
    for (auto _ : state) {
        auto s{concat_string_sets_with_stringstream(test_string_sets, ",")};
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK(BM_ConcatStrings);
BENCHMARK(BM_ConcatStrings_with_Stringstream);

BENCHMARK(BM_ConcatStringSets);
BENCHMARK(BM_ConcatStringSets_with_Stringstream);

BENCHMARK_MAIN();
