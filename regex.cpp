#include <benchmark/benchmark.h>
#include <regex>

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
    "[001F86EA | 2009-11-02 16:05:50] (193.98.108.243:1789, 10994) /cmd.php [co_doc.details] RQST END   [normal]    84 ms"
};

const std::regex std_regex_re1{R"(\[(.+) \| ([^\]]+)\] \((.+, )?(\d+)\) (.+) \[(.+)\] RQST END   \[(.+)\] *(\d+) ms)"};
const std::regex std_regex_re2{R"(\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)"};

static void BM_OneLine_Match_StdString_Regex1(benchmark::State& state)
{
    std::smatch m;
    int found = 0;

    for (auto _ : state)
        if (std::regex_match(one_line, m, std_regex_re1))
            ++found;

    state.counters["found"] = found;
}

static void BM_OneLine_Match_StdString_Regex2(benchmark::State& state)
{
    std::smatch m;
    int found = 0;

    for (auto _ : state)
        if (std::regex_match(one_line, m, std_regex_re2))
            ++found;

    state.counters["found"] = found;
}

static void BM_AllLines_Match_StdString_Regex1(benchmark::State& state)
{
    std::smatch m;
    int found = 0;

    for (auto _ : state)
        for (auto line : all_lines)
            if (std::regex_match(line, m, std_regex_re1))
                ++found;

    state.counters["found"] = found;
}

static void BM_AllLines_Match_StdString_Regex2(benchmark::State& state)
{
    std::smatch m;
    int found = 0;

    for (auto _ : state)
        for (auto line : all_lines)
            if (std::regex_match(line, m, std_regex_re2))
                ++found;

    state.counters["found"] = found;
}

BENCHMARK(BM_OneLine_Match_StdString_Regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_OneLine_Match_StdString_Regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AllLines_Match_StdString_Regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_AllLines_Match_StdString_Regex2)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();