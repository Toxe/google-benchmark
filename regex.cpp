#define OVECCOUNT 30
#define PCRE2_CODE_UNIT_WIDTH 8

#include <benchmark/benchmark.h>
#include <boost/regex.hpp>
#include <fstream>
#include <iostream>
#include <pcre.h>
#include <pcre2.h>
#include <re2/re2.h>
#include <regex>

const std::string one_line{"[00180D0F | 2009-09-15 09:34:48] (127.0.0.1:39170, 879) /cmd.php [co_search.browse] RQST END   [normal]   799 ms"};
const std::vector<std::string> all_lines{
    "[05821BE4 | 2019-05-13 12:28:56] (13036) http://test.site/projects/cmd.php [co_project.view] RQST START",
    "[05821BE4 | 2019-05-13 12:28:56] (13036) http://test.site/projects/cmd.php [co_project.view] RQST END   [normal]   402 ms",
    "[05671FA1 | 2019-04-17 14:08:03] (62931) http://test.site/projects/cmd.php [co_project.view] RQST START",
    "[05671FA1 | 2019-04-17 14:08:03] (62931) http://test.site/projects/cmd.php [co_project.view] RQST END   [normal]   237 ms",
    "[05822AE4 | 2019-06-17 06:59:03] (15828) http://test.site/cmd.php [co_project.dialog_doc_details] RQST START",
    "[05822AE4 | 2019-06-17 06:59:03] (15828) http://test.site/cmd.php [co_project.dialog_doc_details] RQST END   [normal]   318 ms",
    "[00180D0F | 2009-09-15 09:34:47] (127.0.0.1:39170, 879) /cmd.php [co_search.browse] RQST START",
    "[00180D0F | 2009-09-15 09:34:48] (127.0.0.1:39170, 879) /cmd.php [co_search.browse] RQST END   [normal]   799 ms",
    "[00180D0D | 2009-09-15 09:34:19] (127.0.0.1:39169, 23727) /browse/ RQST START",
    "[00180D0D | 2009-09-15 09:34:19] (127.0.0.1:39169, 23727) /browse/ RQST END   [normal]    35 ms",
    "[001F86EA | 2009-11-02 16:05:50] (127.0.0.1:1789, 10994) /cmd.php [co_doc.details] RQST START",
    "[001F86EA | 2009-11-02 16:05:50] (127.0.0.1:1789, 10994) /cmd.php [co_doc.details] RQST END   [normal]    84 ms"};

const char* regex1 = R"(\[(.+) \| ([^\]]+)\] \((.+, )?(\d+)\) (.+) \[(.+)\] RQST END   \[(.+)\] *(\d+) ms)";
const char* regex2 = R"(\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)";
const char* regex3 = R"(^\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)";

std::size_t check_std_regex(const std::string& line, const std::regex& re, std::smatch& m)
{
    std::size_t length = 0;

    if (std::regex_match(line, m, re))
        if (m.size() > 1)
            for (int i = 1; i < static_cast<int>(m.size()); ++i)
                length += static_cast<std::size_t>(m[static_cast<std::size_t>(i)].length());

    return length;
}

std::size_t check_boost_regex(const std::string& line, const boost::regex& re, boost::smatch& m)
{
    std::size_t length = 0;

    if (boost::regex_match(line, m, re))
        if (m.size() > 1)
            for (int i = 1; i < static_cast<int>(m.size()); ++i)
                length += static_cast<std::size_t>(m[i].length());

    return length;
}

std::size_t check_re2(const std::string& line, const re2::RE2& re, const std::vector<RE2::Arg*>& arguments_ptrs, const std::vector<std::string>& results, std::size_t args_count)
{
    std::size_t length = 0;

    if (RE2::FullMatchN(line, re, arguments_ptrs.data(), static_cast<int>(args_count)))
        for (std::size_t i = 0; i < args_count; ++i)
            length += results[i].length();

    return length;
}

std::size_t check_pcre(const std::string& line, const pcre* re, const pcre_extra* sd)
{
    std::size_t length = 0;
    int ovector[OVECCOUNT];

    const int rc = pcre_exec(re, sd, line.c_str(), static_cast<int>(line.size()), 0, 0, ovector, OVECCOUNT);

    if (rc > 1) {
        for (int i = 1; i < rc; ++i) {
            const char* substring_start = line.c_str() + ovector[2*i];
            const int substring_length = ovector[2*i + 1] - ovector[2*i];
            const std::string_view s{substring_start, static_cast<std::string_view::size_type>(substring_length)};
            length += s.size();
        }
    }

    return length;
}

std::size_t check_pcre_jit(const std::string& line, const pcre* re, const pcre_extra* sd, pcre_jit_stack* jit_stack)
{
    std::size_t length = 0;
    int ovector[OVECCOUNT];

    const int rc = pcre_jit_exec(re, sd, line.c_str(), static_cast<int>(line.size()), 0, 0, ovector, OVECCOUNT, jit_stack);

    if (rc > 1) {
        for (int i = 1; i < rc; ++i) {
            const char* substring_start = line.c_str() + ovector[2*i];
            const int substring_length = ovector[2*i + 1] - ovector[2*i];
            const std::string_view s{substring_start, static_cast<std::string_view::size_type>(substring_length)};
            length += s.size();
        }
    }

    return length;
}

std::size_t check_pcre2(const std::string& line, const pcre2_code* re, pcre2_match_data* match_data)
{
    std::size_t length = 0;
    const PCRE2_SPTR subject = reinterpret_cast<PCRE2_SPTR>(line.c_str());

    const int rc = pcre2_match(re, subject, line.size(), 0, 0, match_data, nullptr);

    if (rc > 1) {
        const PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);

        for (int i = 1; i < rc; ++i) {
            const PCRE2_SPTR substring_start = subject + ovector[2*i];
            const int substring_length = static_cast<int>(ovector[2*i + 1] - ovector[2*i]);
            const std::string_view s{reinterpret_cast<const char*>(substring_start), static_cast<std::string_view::size_type>(substring_length)};
            length += s.size();
        }
    }

    return length;
}

std::size_t check_pcre2_jit(const std::string& line, const pcre2_code* re, pcre2_match_data* match_data, pcre2_match_context* mcontext)
{
    std::size_t length = 0;
    const PCRE2_SPTR subject = reinterpret_cast<PCRE2_SPTR>(line.c_str());

    const int rc = pcre2_jit_match(re, subject, line.size(), 0, 0, match_data, mcontext);

    if (rc > 1) {
        const PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);

        for (int i = 1; i < rc; ++i) {
            const PCRE2_SPTR substring_start = subject + ovector[2*i];
            const int substring_length = static_cast<int>(ovector[2*i + 1] - ovector[2*i]);
            const std::string_view s{reinterpret_cast<const char*>(substring_start), static_cast<std::string_view::size_type>(substring_length)};
            length += s.size();
        }
    }

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

static void BM_OneLine_StdRegex(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const std::regex re{pattern};
    std::smatch m;

    for (auto _ : state)
        length += check_std_regex(one_line, re, m);

    state.counters["length"] = length;
}

static void BM_AllLines_StdRegex(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const std::regex re{pattern};
    std::smatch m;

    for (auto _ : state)
        for (auto line : all_lines)
            length += check_std_regex(line, re, m);

    state.counters["length"] = length;
}

static void BM_Logfile_StdRegex(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const std::regex re{pattern};
    std::smatch m;

    auto lines = load_logfile();

    for (auto _ : state)
        for (auto line : lines)
            length += check_std_regex(line, re, m);

    state.counters["length"] = length;
}

static void BM_OneLine_BoostRegex(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const boost::regex re{pattern};
    boost::smatch m;

    for (auto _ : state)
        length += check_boost_regex(one_line, re, m);

    state.counters["length"] = length;
}

static void BM_AllLines_BoostRegex(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const boost::regex re{pattern};
    boost::smatch m;

    for (auto _ : state)
        for (auto line : all_lines)
            length += check_boost_regex(line, re, m);

    state.counters["length"] = length;
}

static void BM_Logfile_BoostRegex(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const boost::regex re{pattern};
    boost::smatch m;

    auto lines = load_logfile();

    for (auto _ : state)
        for (auto line : lines)
            length += check_boost_regex(line, re, m);

    state.counters["length"] = length;
}

static void BM_OneLine_RE2(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const re2::RE2 re{pattern};

    if (!re.ok())
        throw std::runtime_error{"RE2 compilation error"};

    std::size_t args_count = static_cast<std::size_t>(re.NumberOfCapturingGroups());

    std::vector<RE2::Arg> arguments(args_count);
    std::vector<RE2::Arg*> arguments_ptrs(args_count);
    std::vector<std::string> results(args_count);

    for (std::size_t i = 0; i < args_count; ++i) {
        arguments[i] = &results[i];
        arguments_ptrs[i] = &arguments[i];
    }

    for (auto _ : state)
        length += check_re2(one_line, re, arguments_ptrs, results, args_count);

    state.counters["length"] = length;
}

static void BM_AllLines_RE2(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const re2::RE2 re{pattern};

    if (!re.ok())
        throw std::runtime_error{"RE2 compilation error"};

    std::size_t args_count = static_cast<std::size_t>(re.NumberOfCapturingGroups());

    std::vector<RE2::Arg> arguments(args_count);
    std::vector<RE2::Arg*> arguments_ptrs(args_count);
    std::vector<std::string> results(args_count);

    for (std::size_t i = 0; i < args_count; ++i) {
        arguments[i] = &results[i];
        arguments_ptrs[i] = &arguments[i];
    }

    for (auto _ : state)
        for (auto line : all_lines)
            length += check_re2(line, re, arguments_ptrs, results, args_count);

    state.counters["length"] = length;
}

static void BM_Logfile_RE2(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;
    const re2::RE2 re{pattern};

    if (!re.ok())
        throw std::runtime_error{"RE2 compilation error"};

    std::size_t args_count = static_cast<std::size_t>(re.NumberOfCapturingGroups());

    std::vector<RE2::Arg> arguments(args_count);
    std::vector<RE2::Arg*> arguments_ptrs(args_count);
    std::vector<std::string> results(args_count);

    for (std::size_t i = 0; i < args_count; ++i) {
        arguments[i] = &results[i];
        arguments_ptrs[i] = &arguments[i];
    }

    auto lines = load_logfile();

    for (auto _ : state)
        for (auto line : lines)
            length += check_re2(line, re, arguments_ptrs, results, args_count);

    state.counters["length"] = length;
}

std::tuple<pcre*, pcre_extra*> init_pcre(const char* pattern)
{
    const char* error;
    int erroroffset;

    pcre* re = pcre_compile(pattern, 0, &error, &erroroffset, nullptr);

    if (!re)
        throw std::runtime_error{"PCRE compilation failed"};

    pcre_extra* sd = pcre_study(re, 0, &error);

    if (!sd && error)
        throw std::runtime_error{"PCRE study error"};

    return {re, sd};
}

std::tuple<pcre*, pcre_extra*, pcre_jit_stack*> init_pcre_jit(const char* pattern)
{
    const char* error;
    int erroroffset;

    pcre* re = pcre_compile(pattern, 0, &error, &erroroffset, nullptr);

    if (!re)
        throw std::runtime_error{"PCRE compilation failed"};

    pcre_extra* sd = pcre_study(re, PCRE_STUDY_JIT_COMPILE, &error);

    if (!sd && error)
        throw std::runtime_error{"PCRE study error"};

    pcre_jit_stack* jit_stack = pcre_jit_stack_alloc(32*1024, 512*1024);

    if (!jit_stack)
        throw std::runtime_error{"PCRE JIT stack alloc error"};

    pcre_assign_jit_stack(sd, nullptr, jit_stack);

    return {re, sd, jit_stack};
}

static void BM_OneLine_PCRE(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [re, sd] = init_pcre(pattern);

    for (auto _ : state)
        length += check_pcre(one_line, re, sd);

    pcre_free_study(sd);
    pcre_free(re);

    state.counters["length"] = length;
}

static void BM_AllLines_PCRE(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [re, sd] = init_pcre(pattern);

    for (auto _ : state)
        for (auto line : all_lines)
            length += check_pcre(line, re, sd);

    pcre_free_study(sd);
    pcre_free(re);

    state.counters["length"] = length;
}

static void BM_Logfile_PCRE(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto lines = load_logfile();
    auto [re, sd] = init_pcre(pattern);

    for (auto _ : state)
        for (auto line : lines)
            length += check_pcre(line, re, sd);

    pcre_free_study(sd);
    pcre_free(re);

    state.counters["length"] = length;
}

static void BM_OneLine_PCRE_JIT(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [re, sd, jit_stack] = init_pcre_jit(pattern);

    for (auto _ : state)
        length += check_pcre_jit(one_line, re, sd, jit_stack);

    pcre_jit_stack_free(jit_stack);
    pcre_free_study(sd);
    pcre_free(re);

    state.counters["length"] = length;
}

static void BM_AllLines_PCRE_JIT(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [re, sd, jit_stack] = init_pcre_jit(pattern);

    for (auto _ : state)
        for (auto line : all_lines)
            length += check_pcre_jit(line, re, sd, jit_stack);

    pcre_jit_stack_free(jit_stack);
    pcre_free_study(sd);
    pcre_free(re);

    state.counters["length"] = length;
}

static void BM_Logfile_PCRE_JIT(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto lines = load_logfile();
    auto [re, sd, jit_stack] = init_pcre_jit(pattern);

    for (auto _ : state)
        for (auto line : lines)
            length += check_pcre_jit(line, re, sd, jit_stack);

    pcre_jit_stack_free(jit_stack);
    pcre_free_study(sd);
    pcre_free(re);

    state.counters["length"] = length;
}

std::tuple<pcre2_code*, pcre2_match_data*> init_pcre2(const char* pattern)
{
    int errorcode;
    PCRE2_SIZE erroroffset;

    pcre2_code* re = pcre2_compile(reinterpret_cast<PCRE2_SPTR>(pattern), PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroroffset, nullptr);

    if (!re)
        throw std::runtime_error{"PCRE2 compilation failed"};

    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, nullptr);

    if (!match_data)
        throw std::runtime_error{"PCRE2 unable to create match data"};

    return {re, match_data};
}

std::tuple<pcre2_code*, pcre2_match_context*, pcre2_jit_stack*, pcre2_match_data*> init_pcre2_jit(const char* pattern)
{
    int errorcode;
    PCRE2_SIZE erroroffset;

    pcre2_code* re = pcre2_compile(reinterpret_cast<PCRE2_SPTR>(pattern), PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroroffset, nullptr);

    if (!re)
        throw std::runtime_error{"PCRE2 compilation failed"};

    if (pcre2_jit_compile(re, PCRE2_JIT_COMPLETE) < 0)
        throw std::runtime_error{"PCRE2 JIT compile error"};

    pcre2_match_context* mcontext = pcre2_match_context_create(nullptr);

    if (!mcontext)
        throw std::runtime_error{"PCRE2 unable to create match context"};

    pcre2_jit_stack* jit_stack = pcre2_jit_stack_create(32*1024, 512*1024, nullptr);

    if (!jit_stack)
        throw std::runtime_error{"PCRE2 unable to create JIT stack"};

    pcre2_jit_stack_assign(mcontext, nullptr, jit_stack);

    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, nullptr);

    if (!match_data)
        throw std::runtime_error{"PCRE2 unable to create match data"};

    return {re, mcontext, jit_stack, match_data};
}

static void BM_OneLine_PCRE2(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [re, match_data] = init_pcre2(pattern);

    for (auto _ : state)
        length += check_pcre2(one_line, re, match_data);

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    state.counters["length"] = length;
}

static void BM_AllLines_PCRE2(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [re, match_data] = init_pcre2(pattern);

    for (auto _ : state)
        for (auto line : all_lines)
            length += check_pcre2(line, re, match_data);

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    state.counters["length"] = length;
}

static void BM_Logfile_PCRE2(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto lines = load_logfile();
    auto [re, match_data] = init_pcre2(pattern);

    for (auto _ : state)
        for (auto line : lines)
            length += check_pcre2(line, re, match_data);

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    state.counters["length"] = length;
}

static void BM_OneLine_PCRE2_JIT(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [re, mcontext, jit_stack, match_data] = init_pcre2_jit(pattern);

    for (auto _ : state)
        length += check_pcre2_jit(one_line, re, match_data, mcontext);

    pcre2_match_data_free(match_data);
    pcre2_jit_stack_free(jit_stack);
    pcre2_match_context_free(mcontext);
    pcre2_code_free(re);

    state.counters["length"] = length;
}

static void BM_AllLines_PCRE2_JIT(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto [re, mcontext, jit_stack, match_data] = init_pcre2_jit(pattern);

    for (auto _ : state)
        for (auto line : all_lines)
            length += check_pcre2_jit(line, re, match_data, mcontext);

    pcre2_match_data_free(match_data);
    pcre2_jit_stack_free(jit_stack);
    pcre2_match_context_free(mcontext);
    pcre2_code_free(re);

    state.counters["length"] = length;
}

static void BM_Logfile_PCRE2_JIT(benchmark::State& state, const char* pattern)
{
    std::size_t length = 0;

    auto lines = load_logfile();
    auto [re, mcontext, jit_stack, match_data] = init_pcre2_jit(pattern);

    if (!match_data)
        throw std::runtime_error{"PCRE2 unable to create match data"};

    for (auto _ : state)
        for (auto line : lines)
            length += check_pcre2_jit(line, re, match_data, mcontext);

    pcre2_match_data_free(match_data);
    pcre2_jit_stack_free(jit_stack);
    pcre2_match_context_free(mcontext);
    pcre2_code_free(re);

    state.counters["length"] = length;
}

BENCHMARK_CAPTURE(BM_OneLine_BoostRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_BoostRegex, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_BoostRegex, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE2, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE2, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE2, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE2_JIT, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE2_JIT, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE2_JIT, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE_JIT, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE_JIT, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_PCRE_JIT, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_RE2, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_RE2, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_RE2, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_StdRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_StdRegex, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_OneLine_StdRegex, 3, regex3)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BM_AllLines_BoostRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_BoostRegex, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_BoostRegex, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE2, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE2, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE2, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE2_JIT, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE2_JIT, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE2_JIT, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE_JIT, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE_JIT, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_PCRE_JIT, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_RE2, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_RE2, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_RE2, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_StdRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_StdRegex, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_AllLines_StdRegex, 3, regex3)->Unit(benchmark::kMicrosecond);

BENCHMARK_CAPTURE(BM_Logfile_BoostRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_BoostRegex, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_BoostRegex, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE2, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE2, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE2, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE2_JIT, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE2_JIT, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE2_JIT, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE_JIT, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE_JIT, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_PCRE_JIT, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_RE2, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_RE2, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_RE2, 3, regex3)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_StdRegex, 1, regex1)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_StdRegex, 2, regex2)->Unit(benchmark::kMicrosecond);
BENCHMARK_CAPTURE(BM_Logfile_StdRegex, 3, regex3)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
