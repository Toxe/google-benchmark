#include <benchmark/benchmark.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::vector<std::string> test_strings_splitted{
    "2020-03-17 16:58:06", "1000001", "3118", "1022", "192.168.152.50", "project.view", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:58:08", "1000002", "2640", "1137", "192.168.195.114", "collection.view_archive", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:58:58", "1000003", "111", "1121", "192.168.228.209", "user.delete", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:59:02", "1000004", "135", "1060", "192.168.194.102", "doc.update", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:59:08", "1000005", "2356", "1109", "192.168.174.108", "project.view", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"};

const std::vector<std::string> test_strings_one_line{
    "2020-03-17 16:58:06\t1000001\t3118\t1022\t192.168.152.50\tproject.view\tMozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:58:08\t1000002\t2640\t1137\t192.168.195.114\tcollection.view_archive\tMozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:58:58\t1000003\t111\t1121\t192.168.228.209\tuser.delete\tMozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:59:02\t1000004\t135\t1060\t192.168.194.102\tdoc.update\tMozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36",
    "2020-03-17 16:59:08\t1000005\t2356\t1109\t192.168.174.108\tproject.view\tMozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"};

std::vector<std::vector<std::string>> test_string_sets{
    {"2020-03-17 16:58:06", "1000001", "3118", "1022", "192.168.152.50", "project.view", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"},
    {"2020-03-17 16:58:08", "1000002", "2640", "1137", "192.168.195.114", "collection.view_archive", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"},
    {"2020-03-17 16:58:58", "1000003", "111", "1121", "192.168.228.209", "user.delete", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"},
    {"2020-03-17 16:59:02", "1000004", "135", "1060", "192.168.194.102", "doc.update", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"},
    {"2020-03-17 16:59:08", "1000005", "2356", "1109", "192.168.174.108", "project.view", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36"}};

struct Dataset_v02_Position {
    Dataset_v02_Position(std::string::size_type start, std::string::size_type end) : start_{start}, end_{end} {}
    std::string::size_type start_;
    std::string::size_type end_;
};

struct Dataset_v02 {
    Dataset_v02(const std::string& line, char delim) : line_{line} {
        std::string::size_type start_pos = 0;
        std::string::size_type pos;

        while ((pos = line.find(delim, start_pos)) != std::string::npos) {
            positions.emplace_back(start_pos, pos);
            start_pos = pos + 1;
        }

        if (start_pos < line.size())
            positions.emplace_back(start_pos, line.size());
    }

    const std::string line_;
    std::vector<Dataset_v02_Position> positions;
};

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

std::string concat_strings_with_dataset_v02(const std::vector<Dataset_v02>& datasets, const std::string& delim)
{
    std::string big;

    for (std::size_t i = 0; i < datasets.size(); ++i) {
        const auto& ds{datasets[i]};

        for (const auto& pos : ds.positions) {
            big += std::string{ds.line_, pos.start_, pos.end_ - pos.start_};
        }

        if (i < (datasets.size() - 1))
            big += delim;
    }

    return big;
}

static void BM_ConcatStrings(benchmark::State& state)
{
    for (auto _ : state) {
        auto s{concat_strings(test_strings_splitted)};
        benchmark::DoNotOptimize(s);
    }
}

static void BM_ConcatStrings_with_Stringstream(benchmark::State& state)
{
    for (auto _ : state) {
        auto s{concat_strings_with_stringstream(test_strings_splitted)};
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

static void BM_ConcatStrings_with_Dataset_v02(benchmark::State& state)
{
    std::vector<Dataset_v02> datasets;

    for (const auto& line : test_strings_one_line)
        datasets.emplace_back(line, '\t');

    for (auto _ : state) {
        auto s{concat_strings_with_dataset_v02(datasets, ",")};
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK(BM_ConcatStrings);
BENCHMARK(BM_ConcatStrings_with_Stringstream);

BENCHMARK(BM_ConcatStringSets);
BENCHMARK(BM_ConcatStringSets_with_Stringstream);

BENCHMARK(BM_ConcatStrings_with_Dataset_v02);

BENCHMARK_MAIN();
