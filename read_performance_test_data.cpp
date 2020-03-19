#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

const std::string filename{"performance_test_data.txt"};

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

struct Dataset_v01 {
    Dataset_v01(std::ifstream& in) {
        if (!std::getline(in, s1, '\t')) throw std::runtime_error{"misformed input data"};
        if (!std::getline(in, s2, '\t')) throw std::runtime_error{"misformed input data"};
        if (!std::getline(in, s3, '\t')) throw std::runtime_error{"misformed input data"};
        if (!std::getline(in, s4, '\t')) throw std::runtime_error{"misformed input data"};
        if (!std::getline(in, s5, '\t')) throw std::runtime_error{"misformed input data"};
        if (!std::getline(in, s6, '\t')) throw std::runtime_error{"misformed input data"};
        if (!std::getline(in, s7))       throw std::runtime_error{"misformed input data"};
    }

    std::string s1;
    std::string s2;
    std::string s3;
    std::string s4;
    std::string s5;
    std::string s6;
    std::string s7;
};

std::vector<std::vector<std::string>> read_v01(const std::string& filename)
{
    std::ifstream in{filename};
    std::vector<std::vector<std::string>> datasets;

    while (in) {
        std::string s1;  if (!std::getline(in, s1, '\t')) break;
        std::string s2;  if (!std::getline(in, s2, '\t')) break;
        std::string s3;  if (!std::getline(in, s3, '\t')) break;
        std::string s4;  if (!std::getline(in, s4, '\t')) break;
        std::string s5;  if (!std::getline(in, s5, '\t')) break;
        std::string s6;  if (!std::getline(in, s6, '\t')) break;
        std::string s7;  if (!std::getline(in, s7))       break;

        std::vector<std::string> ds{s1, s2, s3, s4, s5, s6, s7};
        datasets.push_back(ds);
    }

    return datasets;
}

std::vector<Dataset_v01> read_dataset_v01(const std::string& filename)
{
    std::ifstream in{filename};
    std::vector<Dataset_v01> datasets;

    while (in) {
        try {
            Dataset_v01 ds{in};
            datasets.push_back(ds);
        } catch (std::runtime_error& e) {
            break;
        }
    }

    return datasets;
}

std::vector<Dataset_v01> read_dataset_v01_emplace_back(const std::string& filename)
{
    std::ifstream in{filename};
    std::vector<Dataset_v01> datasets;

    while (in) {
        try {
            datasets.emplace_back(in);
        } catch (std::runtime_error& e) {
            break;
        }
    }

    return datasets;
}

std::vector<std::vector<std::string>> read_v02_split_string(const std::string& filename)
{
    std::ifstream in{filename};
    std::vector<std::vector<std::string>> line_values;
    std::string line;

    while (std::getline(in, line)) {
        const std::vector<std::string> tokens{split_string_emplace_back(line, '\t')};

        if (tokens.size() != 7)
            break;

        line_values.push_back(tokens);
    }

    return line_values;
}

static void BM_Read_v01(benchmark::State& state)
{
    for (auto _ : state) {
        auto datasets{read_v01(filename)};
        benchmark::DoNotOptimize(datasets);
    }
}

static void BM_Read_Dataset_v01(benchmark::State& state)
{
    for (auto _ : state) {
        auto datasets{read_dataset_v01(filename)};
        benchmark::DoNotOptimize(datasets);
    }
}

static void BM_Read_Dataset_v01_emplace_back(benchmark::State& state)
{
    for (auto _ : state) {
        auto datasets{read_dataset_v01_emplace_back(filename)};
        benchmark::DoNotOptimize(datasets);
    }
}

static void BM_Read_v02_split_string(benchmark::State& state)
{
    for (auto _ : state) {
        auto line_values{read_v02_split_string(filename)};
        benchmark::DoNotOptimize(line_values);
    }
}

BENCHMARK(BM_Read_v01);
BENCHMARK(BM_Read_Dataset_v01);
BENCHMARK(BM_Read_Dataset_v01_emplace_back);
BENCHMARK(BM_Read_v02_split_string);

BENCHMARK_MAIN();
