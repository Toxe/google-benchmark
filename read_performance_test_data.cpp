#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

const std::string filename{"performance_test_data.txt"};

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

std::vector<Dataset_v01> read_v02(const std::string& filename)
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

std::vector<Dataset_v01> read_v03(const std::string& filename)
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

static void BM_Read_v01(benchmark::State& state)
{
    for (auto _ : state) {
        auto datasets{read_v01(filename)};
        benchmark::DoNotOptimize(datasets);
    }
}

static void BM_Read_v02(benchmark::State& state)
{
    for (auto _ : state) {
        auto datasets{read_v02(filename)};
        benchmark::DoNotOptimize(datasets);
    }
}

static void BM_Read_v03(benchmark::State& state)
{
    for (auto _ : state) {
        auto datasets{read_v03(filename)};
        benchmark::DoNotOptimize(datasets);
    }
}

BENCHMARK(BM_Read_v01);
BENCHMARK(BM_Read_v02);
BENCHMARK(BM_Read_v03);

BENCHMARK_MAIN();
