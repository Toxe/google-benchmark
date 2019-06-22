#include <benchmark/benchmark.h>
#include <boost/regex.hpp>
#include <iostream>
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
    "[001F86EA | 2009-11-02 16:05:50] (193.98.108.243:1789, 10994) /cmd.php [co_doc.details] RQST END   [normal]    84 ms"};

const char* regex1 = R"(\[(.+) \| ([^\]]+)\] \((.+, )?(\d+)\) (.+) \[(.+)\] RQST END   \[(.+)\] *(\d+) ms)";
const char* regex2 = R"(\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)";

const std::regex std_regex_re1{regex1};
const std::regex std_regex_re2{regex2};
const boost::regex boost_regex_re1{regex1};
const boost::regex boost_regex_re2{regex2};

static void BM_OneLine_StdRegex(benchmark::State& state, const std::regex& re)
{
    std::smatch m;

    for (auto _ : state)
        std::regex_match(one_line, m, re);
}

static void BM_AllLines_StdRegex(benchmark::State& state, const std::regex& re)
{
    std::smatch m;

    for (auto _ : state)
        for (auto line : all_lines)
            std::regex_match(line, m, re);
}

static void BM_OneLine_BoostRegex(benchmark::State& state, const boost::regex& re)
{
    boost::smatch m;

    for (auto _ : state)
        boost::regex_match(one_line, m, re);
}

static void BM_AllLines_BoostRegex(benchmark::State& state, const boost::regex& re)
{
    boost::smatch m;

    for (auto _ : state)
        for (auto line : all_lines)
            boost::regex_match(line, m, re);
}

BENCHMARK_CAPTURE(BM_OneLine_StdRegex, 1, std_regex_re1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_StdRegex, 2, std_regex_re2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_StdRegex, 1, std_regex_re1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_StdRegex, 2, std_regex_re2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_BoostRegex, 1, boost_regex_re1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_BoostRegex, 2, boost_regex_re2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_BoostRegex, 1, boost_regex_re1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_BoostRegex, 2, boost_regex_re2)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
