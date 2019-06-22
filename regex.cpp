#define OVECCOUNT 30
#define PCRE2_CODE_UNIT_WIDTH 8

#include <benchmark/benchmark.h>
#include <boost/regex.hpp>
#include <iostream>
#include <pcre.h>
#include <pcre2.h>
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

static void BM_OneLine_StdRegex(benchmark::State& state, const char* pattern)
{
    const std::regex re{pattern};
    std::smatch m;

    for (auto _ : state)
        std::regex_match(one_line, m, re);
}

static void BM_AllLines_StdRegex(benchmark::State& state, const char* pattern)
{
    const std::regex re{pattern};
    std::smatch m;

    for (auto _ : state)
        for (auto line : all_lines)
            std::regex_match(line, m, re);
}

static void BM_OneLine_BoostRegex(benchmark::State& state, const char* pattern)
{
    const boost::regex re{pattern};
    boost::smatch m;

    for (auto _ : state)
        boost::regex_match(one_line, m, re);
}

static void BM_AllLines_BoostRegex(benchmark::State& state, const char* pattern)
{
    const boost::regex re{pattern};
    boost::smatch m;

    for (auto _ : state)
        for (auto line : all_lines)
            boost::regex_match(line, m, re);
}

static void BM_OneLine_PCRE(benchmark::State& state, const char* pattern)
{
    const char* error;
    int erroroffset;
    int ovector[OVECCOUNT];

    pcre* re = pcre_compile(pattern, 0, &error, &erroroffset, nullptr);

    if (!re)
        throw std::runtime_error{"PCRE compilation failed"};

    pcre_extra* sd = pcre_study(re, 0, &error);

    if (!sd && error)
        throw std::runtime_error{"PCRE study error"};

    for (auto _ : state)
        pcre_exec(re, sd, one_line.c_str(), one_line.size(), 0, 0, ovector, OVECCOUNT);

    pcre_free_study(sd);
    pcre_free(re);
}

static void BM_AllLines_PCRE(benchmark::State& state, const char* pattern)
{
    const char* error;
    int erroroffset;
    int ovector[OVECCOUNT];

    pcre* re = pcre_compile(pattern, 0, &error, &erroroffset, nullptr);

    if (!re)
        throw std::runtime_error{"PCRE compilation failed"};

    pcre_extra* sd = pcre_study(re, 0, &error);

    if (!sd && error)
        throw std::runtime_error{"PCRE study error"};

    for (auto _ : state)
        for (auto line : all_lines)
            pcre_exec(re, sd, line.c_str(), line.size(), 0, 0, ovector, OVECCOUNT);

    pcre_free_study(sd);
    pcre_free(re);
}

static void BM_OneLine_PCRE2(benchmark::State& state, const char* pattern)
{
    int errorcode;
    PCRE2_SIZE erroroffset;

    pcre2_code* re = pcre2_compile((PCRE2_SPTR) pattern, PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroroffset, nullptr);

    if (!re)
        throw std::runtime_error{"PCRE2 compilation failed"};

    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, nullptr);

    if (!match_data)
        throw std::runtime_error{"PCRE2 unable to create match data"};

    for (auto _ : state) {
        const PCRE2_SPTR subject = (PCRE2_SPTR) one_line.c_str();
        pcre2_match(re, subject, one_line.size(), 0, 0, match_data, nullptr);
    }

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
}

static void BM_AllLines_PCRE2(benchmark::State& state, const char* pattern)
{
    int errorcode;
    PCRE2_SIZE erroroffset;

    pcre2_code* re = pcre2_compile((PCRE2_SPTR) pattern, PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroroffset, nullptr);

    if (!re)
        throw std::runtime_error{"PCRE2 compilation failed"};

    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, nullptr);

    if (!match_data)
        throw std::runtime_error{"PCRE2 unable to create match data"};

    for (auto _ : state) {
        for (auto line : all_lines) {
            const PCRE2_SPTR subject = (PCRE2_SPTR) line.c_str();
            pcre2_match(re, subject, line.size(), 0, 0, match_data, nullptr);
        }
    }

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
}

BENCHMARK_CAPTURE(BM_OneLine_StdRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_StdRegex, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_StdRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_StdRegex, 2, regex2)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BM_OneLine_BoostRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_BoostRegex, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_BoostRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_BoostRegex, 2, regex2)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BM_OneLine_PCRE, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE, 2, regex2)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BM_OneLine_PCRE2, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE2, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE2, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE2, 2, regex2)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
