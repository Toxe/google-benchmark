#include <algorithm>
#include <benchmark/benchmark.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

class Fibonacci {
public:
    int current() { return curr_; }
    int next();
    void print();

private:
    int n_ = 1;
    int prev_ = 0;
    int curr_ = 1;
};

int Fibonacci::next()
{
    int next = prev_ + curr_;
    prev_ = curr_;
    curr_ = next;
    ++n_;
    return curr_;
}

void Fibonacci::print()
{
    std::cout << "F(" << n_ << ") = " << curr_ << std::endl;
}

template <typename T>
std::vector<T> prime_factorization(const T number)
{
    std::vector<T> factors;
    T n = number;
    T f = 2;
    const T root = std::sqrtf(n);

    while (n > 1) {
        if (n % f == 0) {
            n = n / f;
            factors.push_back(f);
        } else {
            if (++f > root) {
                factors.push_back(n); // prime
                break;
            }
        }
    }

    return factors;
}

template <typename T>
bool is_palindrome_number(const T number)
{
    const std::string s{std::to_string(number)};
    auto right = s.cend() - 1;

    for (auto left = s.cbegin(); left < right; ++left, --right)
        if (*left != *right)
            return false;

    return true;
}

template <typename T>
bool is_prime(T number)
{
    if (number < 2)
        return false;

    const T root = std::sqrt(number);

    for (T i = 2; i <= root; ++i)
        if (number % i == 0)
            return false;

    return true;
}

int nth_prime(int n)
{
    int number = 1;

    while (n > 0) {
        ++number;

        if (is_prime(number))
            --n;
    }

    return number;
}

long long calc_adjacent_digits_product(std::string::const_iterator begin, std::string::const_iterator end)
{
    long long prod = 1;

    for (auto p = begin; p < end; ++p)
        prod *= *p - 48;

    return prod;
}

int euler001()
{
    int sum = 0;

    for (int i = 0; i < 1000; ++i)
        if (i % 3 == 0 || i % 5 == 0)
            sum += i;

    return sum;
}

int euler002()
{
    Fibonacci fib;
    int sum = 0;

    for (; fib.current() < 4000000; fib.next())
        if (fib.current() % 2 == 0)
            sum += fib.current();

    return sum;
}

template <typename T>
T euler003(T number)
{
    const auto factors = prime_factorization(number);
    return factors.back();
}

int euler004()
{
    int max = 0;

    for (int i = 999; i >= 100; --i) {
        for (int j = i; j >= 100; --j) {
            const int n = i * j;

            if (n > max)
                if (is_palindrome_number(n))
                    max = n;
        }
    }

    return max;
}

int euler005()
{
    int n = 1;

    for (int i = 2; i <= 20; ++i)
        n = std::lcm(n, i);

    return n;
}

int euler006()
{
    int sum = 0;
    int sum_of_squares = 0;

    for (int i = 1; i <= 100; ++i) {
        sum += i;
        sum_of_squares += i * i;
    }

    return sum * sum - sum_of_squares;
}

long long euler008(const std::string& big_number)
{
    std::string::const_iterator begin = big_number.begin();
    std::string::const_iterator end = begin + 13;
    long long max = 0;

    while (end <= big_number.end()) {
        auto prod = calc_adjacent_digits_product(begin, end);

        if (prod > max)
            max = prod;

        ++begin;
        ++end;
    }

    return max;
}

int euler009()
{
    for (int a = 1; a <= (1000/3); ++a) {
        for (int b = a + 1; b <= (999 - a) / 2; ++b) {
            int c = 1000 - (a + b);

            if ((a * a) + (b * b) == (c * c))
                return a * b * c;
        }
    }

    return 0;
}

static void BM_Euler001(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler001());
}

static void BM_Euler002(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler002());
}

static void BM_Euler003_Int(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler003((int) state.range(0)));
}

static void BM_Euler003_LongLong(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler003((long long) state.range(0)));
}

static void BM_Euler004(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler004());
}

static void BM_Euler004_IsPalindromeNumber_Int(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(is_palindrome_number((int) state.range(0)));
}

static void BM_Euler004_IsPalindromeNumber_LongLong(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(is_palindrome_number((long long) state.range(0)));
}

static void BM_Euler005(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler005());
}

static void BM_Euler006(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler006());
}

static void BM_Euler007_IsPrime_Int(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(is_prime((int) state.range(0)));
}

static void BM_Euler007_IsPrime_LongLong(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(is_prime((long long) state.range(0)));
}

static void BM_Euler007_NthPrime(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(nth_prime(state.range(0)));
}

static void BM_Euler008(benchmark::State& state)
{
    const std::string big_number{
        "73167176531330624919225119674426574742355349194934"
        "96983520312774506326239578318016984801869478851843"
        "85861560789112949495459501737958331952853208805511"
        "12540698747158523863050715693290963295227443043557"
        "66896648950445244523161731856403098711121722383113"
        "62229893423380308135336276614282806444486645238749"
        "30358907296290491560440772390713810515859307960866"
        "70172427121883998797908792274921901699720888093776"
        "65727333001053367881220235421809751254540594752243"
        "52584907711670556013604839586446706324415722155397"
        "53697817977846174064955149290862569321978468622482"
        "83972241375657056057490261407972968652414535100474"
        "82166370484403199890008895243450658541227588666881"
        "16427171479924442928230863465674813919123162824586"
        "17866458359124566529476545682848912883142607690042"
        "24219022671055626321111109370544217506941658960408"
        "07198403850962455444362981230987879927244284909188"
        "84580156166097919133875499200524063689912560717606"
        "05886116467109405077541002256983155200055935729725"
        "71636269561882670428252483600823257530420752963450"};

    for (auto _ : state)
        benchmark::DoNotOptimize(euler008(big_number));
}

static void BM_Euler009(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler009());
}

BENCHMARK(BM_Euler001);
BENCHMARK(BM_Euler002);

BENCHMARK(BM_Euler003_Int)->Arg(3);
BENCHMARK(BM_Euler003_Int)->Arg(4);
BENCHMARK(BM_Euler003_Int)->Arg(7);
BENCHMARK(BM_Euler003_Int)->Arg(8);
BENCHMARK(BM_Euler003_Int)->Arg(23);
BENCHMARK(BM_Euler003_Int)->Arg(28);
BENCHMARK(BM_Euler003_Int)->Arg(1200);
BENCHMARK(BM_Euler003_Int)->Arg(13195);
BENCHMARK(BM_Euler003_Int)->Arg(6535781);
BENCHMARK(BM_Euler003_Int)->Arg(6535783);
BENCHMARK(BM_Euler003_Int)->Arg(6535787);
BENCHMARK(BM_Euler003_LongLong)->Arg(600851475143);

BENCHMARK(BM_Euler004);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(3);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(30);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(33);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(300);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(303);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(330);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(333);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(9009);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(9019);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(1234221);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(1234321);
BENCHMARK(BM_Euler004_IsPalindromeNumber_Int)->Arg(12344321);
BENCHMARK(BM_Euler004_IsPalindromeNumber_LongLong)->Arg(12345677654321);
BENCHMARK(BM_Euler004_IsPalindromeNumber_LongLong)->Arg(12345676654321);

BENCHMARK(BM_Euler005);
BENCHMARK(BM_Euler006);

BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(2);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(8);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(11);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(47);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(49);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(251);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(4815);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(4817);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(16123);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(16127);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(1046527);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(1046529);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(16769021);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(16769023);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(27644437);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(27644439);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(84657327);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(84657329);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(479001593);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(479001597);
BENCHMARK(BM_Euler007_IsPrime_Int)->Arg(479001599);
BENCHMARK(BM_Euler007_IsPrime_LongLong)->Arg(999999000001);
BENCHMARK(BM_Euler007_IsPrime_LongLong)->Arg(999999000003);
BENCHMARK(BM_Euler007_NthPrime)->Arg(6);
BENCHMARK(BM_Euler007_NthPrime)->Arg(100);
BENCHMARK(BM_Euler007_NthPrime)->Arg(10001);

BENCHMARK(BM_Euler008);
BENCHMARK(BM_Euler009);

BENCHMARK_MAIN();
