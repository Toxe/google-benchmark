#include <algorithm>
#include <benchmark/benchmark.h>
#include <cmath>
#include <iostream>
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

BENCHMARK_MAIN();
