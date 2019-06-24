#include <benchmark/benchmark.h>
#include <fstream>
#include <iostream>
#include <hs/hs.h>

const std::string one_line{"[00180D0F | 2009-09-15 09:34:48] (193.98.108.243:39170, 879) /cmd.php [co_search.browse] RQST END   [normal]   799 ms"};
const std::vector<std::string> all_lines{
    "[05821BE4 | 2019-05-13 12:28:56] (13036) http://contiview.site/projects/cmd.php [co_project.view] RQST START",
    "[05821BE4 | 2019-05-13 12:28:56] (13036) http://contiview.site/projects/cmd.php [co_project.view] RQST END   [normal]   402 ms",
    "[05671FA1 | 2019-04-17 14:08:03] (62931) http://contiview.site/projects/cmd.php [co_project.view] RQST START",
    "[05671FA1 | 2019-04-17 14:08:03] (62931) http://contiview.site/projects/cmd.php [co_project.view] RQST END   [normal]   237 ms",
    "[05822AE4 | 2019-06-17 06:59:03] (15828) http://contiview.site/cmd.php [co_project.dialog_doc_details] RQST START",
    "[05822AE4 | 2019-06-17 06:59:03] (15828) http://contiview.site/cmd.php [co_project.dialog_doc_details] RQST END   [normal]   318 ms",
    "[00180D0F | 2009-09-15 09:34:47] (193.98.108.243:39170, 879) /cmd.php [co_search.browse] RQST START",
    "[00180D0F | 2009-09-15 09:34:48] (193.98.108.243:39170, 879) /cmd.php [co_search.browse] RQST END   [normal]   799 ms",
    "[00180D0D | 2009-09-15 09:34:19] (193.98.108.243:39169, 23727) /browse/ RQST START",
    "[00180D0D | 2009-09-15 09:34:19] (193.98.108.243:39169, 23727) /browse/ RQST END   [normal]    35 ms",
    "[001F86EA | 2009-11-02 16:05:50] (193.98.108.243:1789, 10994) /cmd.php [co_doc.details] RQST START",
    "[001F86EA | 2009-11-02 16:05:50] (193.98.108.243:1789, 10994) /cmd.php [co_doc.details] RQST END   [normal]    84 ms"};

const char* regex1 = R"(\[(.+) \| ([^\]]+)\] \((.+, )?(\d+)\) (.+) \[(.+)\] RQST END   \[(.+)\] *(\d+) ms)";
const char* regex2 = R"(\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)";

static int match_found_handler(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void* ctx)
{
    std::size_t* p = (std::size_t*) ctx;
    *p = (to - from);
    return 0;
}

std::size_t check_hyperscan(const std::string& line, const hs_database_t* database, hs_scratch_t* scratch)
{
    std::size_t length = 0;

    if (hs_scan(database, line.c_str(), line.length(), 0, scratch, match_found_handler, &length) != HS_SUCCESS)
        throw std::runtime_error{"Hyperscan scan error"};

    return length;
}

std::vector<std::string> load_logfile()
{
    std::ifstream in("../logfile.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(in, line))
        lines.push_back(line);

    return lines;
}

std::tuple<hs_database_t*, hs_scratch_t*> init_hyperscan(const char* pattern)
{
    hs_database_t* database;
    hs_scratch_t* scratch = nullptr;
    hs_compile_error_t* compile_err;

    if (hs_compile(pattern, HS_FLAG_DOTALL | HS_FLAG_SINGLEMATCH, HS_MODE_BLOCK, nullptr, &database, &compile_err) != HS_SUCCESS)
        throw std::runtime_error{"Hyperscan unable to compile pattern"};

    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS)
        throw std::runtime_error{"Hyperscan unable to allocate scratch space"};

    return std::make_tuple(database, scratch);
}

static void BM_OneLine_Hyperscan(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [database, scratch] = init_hyperscan(pattern);

    for (auto _ : state)
        length += check_hyperscan(one_line, database, scratch);

    hs_free_scratch(scratch);
    hs_free_database(database);

    state.counters["length"] = length;
}

static void BM_AllLines_Hyperscan(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [database, scratch] = init_hyperscan(pattern);

    for (auto _ : state)
        for (auto line : all_lines)
            length += check_hyperscan(line, database, scratch);

    hs_free_scratch(scratch);
    hs_free_database(database);

    state.counters["length"] = length;
}

static void BM_Logfile_Hyperscan(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto lines = load_logfile();
    auto [database, scratch] = init_hyperscan(pattern);

    for (auto _ : state)
        for (auto line : lines)
            length += check_hyperscan(line, database, scratch);

    hs_free_scratch(scratch);
    hs_free_database(database);

    state.counters["length"] = length;
}

BENCHMARK_CAPTURE(BM_OneLine_Hyperscan, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_Hyperscan, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_Hyperscan, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_Hyperscan, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_Hyperscan, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_Hyperscan, 2, regex2)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
