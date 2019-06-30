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
std::vector<T> integer_factorization(const T number)
{
    std::vector<T> factors;

    for (T i = 1; i <= number; ++i)
        if (number % i == 0)
            factors.push_back(i);

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

int nth_triangular_number(int n)
{
    return (n * (n + 1)) / 2;
}

int calc_grid_product(const char* p, int increment)
{
    int prod = 1;

    for (int i = 0; i < 4; ++i, p += increment)
        prod *= *p;

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

long long euler010()
{
    long long sum = 0;

    for (int i = 1; i < 2000000; ++i)
        if (is_prime(i))
            sum += i;

    return sum;
}

int euler011()
{
    const char grid[20][20] = {
        { 8,  2, 22, 97, 38, 15,  0, 40,  0, 75,  4,  5,  7, 78, 52, 12, 50, 77, 91,  8},
        {49, 49, 99, 40, 17, 81, 18, 57, 60, 87, 17, 40, 98, 43, 69, 48,  4, 56, 62,  0},
        {81, 49, 31, 73, 55, 79, 14, 29, 93, 71, 40, 67, 53, 88, 30,  3, 49, 13, 36, 65},
        {52, 70, 95, 23,  4, 60, 11, 42, 69, 24, 68, 56,  1, 32, 56, 71, 37,  2, 36, 91},
        {22, 31, 16, 71, 51, 67, 63, 89, 41, 92, 36, 54, 22, 40, 40, 28, 66, 33, 13, 80},
        {24, 47, 32, 60, 99,  3, 45,  2, 44, 75, 33, 53, 78, 36, 84, 20, 35, 17, 12, 50},
        {32, 98, 81, 28, 64, 23, 67, 10, 26, 38, 40, 67, 59, 54, 70, 66, 18, 38, 64, 70},
        {67, 26, 20, 68,  2, 62, 12, 20, 95, 63, 94, 39, 63,  8, 40, 91, 66, 49, 94, 21},
        {24, 55, 58,  5, 66, 73, 99, 26, 97, 17, 78, 78, 96, 83, 14, 88, 34, 89, 63, 72},
        {21, 36, 23,  9, 75,  0, 76, 44, 20, 45, 35, 14,  0, 61, 33, 97, 34, 31, 33, 95},
        {78, 17, 53, 28, 22, 75, 31, 67, 15, 94,  3, 80,  4, 62, 16, 14,  9, 53, 56, 92},
        {16, 39,  5, 42, 96, 35, 31, 47, 55, 58, 88, 24,  0, 17, 54, 24, 36, 29, 85, 57},
        {86, 56,  0, 48, 35, 71, 89,  7,  5, 44, 44, 37, 44, 60, 21, 58, 51, 54, 17, 58},
        {19, 80, 81, 68,  5, 94, 47, 69, 28, 73, 92, 13, 86, 52, 17, 77,  4, 89, 55, 40},
        { 4, 52,  8, 83, 97, 35, 99, 16,  7, 97, 57, 32, 16, 26, 26, 79, 33, 27, 98, 66},
        {88, 36, 68, 87, 57, 62, 20, 72,  3, 46, 33, 67, 46, 55, 12, 32, 63, 93, 53, 69},
        { 4, 42, 16, 73, 38, 25, 39, 11, 24, 94, 72, 18,  8, 46, 29, 32, 40, 62, 76, 36},
        {20, 69, 36, 41, 72, 30, 23, 88, 34, 62, 99, 69, 82, 67, 59, 85, 74,  4, 36, 16},
        {20, 73, 35, 29, 78, 31, 90,  1, 74, 31, 49, 71, 48, 86, 81, 16, 23, 57,  5, 54},
        { 1, 70, 54, 71, 83, 51, 54, 69, 16, 92, 33, 48, 61, 43, 52,  1, 89, 19, 67, 48}};

    int max = 0;

    for (int row = 0; row < 20; ++row) {
        for (int col = 0; col < 20; ++col) {
            if (col < 17) {
                max = std::max(calc_grid_product(&grid[row][col], 1), max);
            }

            if (row < 17) {
                max = std::max(calc_grid_product(&grid[row][col], 20), max);

                if (col >= 3)
                    max = std::max(calc_grid_product(&grid[row][col], 19), max);

                if (col < 17)
                    max = std::max(calc_grid_product(&grid[row][col], 21), max);
            }
        }
    }

    return max;
}

int euler012(std::size_t min_factors)
{
    std::size_t max = 0;
    int n = 1;

    while (true) {
        int triangular_number = nth_triangular_number(n);
        auto factors{integer_factorization(triangular_number)};

        if ((max = std::max(max, factors.size())) >= min_factors)
            return triangular_number;

        ++n;
    }

    return 0;
}

std::string euler013()
{
    const std::vector<std::string> numbers{
        "37107287533902102798797998220837590246510135740250",
        "46376937677490009712648124896970078050417018260538",
        "74324986199524741059474233309513058123726617309629",
        "91942213363574161572522430563301811072406154908250",
        "23067588207539346171171980310421047513778063246676",
        "89261670696623633820136378418383684178734361726757",
        "28112879812849979408065481931592621691275889832738",
        "44274228917432520321923589422876796487670272189318",
        "47451445736001306439091167216856844588711603153276",
        "70386486105843025439939619828917593665686757934951",
        "62176457141856560629502157223196586755079324193331",
        "64906352462741904929101432445813822663347944758178",
        "92575867718337217661963751590579239728245598838407",
        "58203565325359399008402633568948830189458628227828",
        "80181199384826282014278194139940567587151170094390",
        "35398664372827112653829987240784473053190104293586",
        "86515506006295864861532075273371959191420517255829",
        "71693888707715466499115593487603532921714970056938",
        "54370070576826684624621495650076471787294438377604",
        "53282654108756828443191190634694037855217779295145",
        "36123272525000296071075082563815656710885258350721",
        "45876576172410976447339110607218265236877223636045",
        "17423706905851860660448207621209813287860733969412",
        "81142660418086830619328460811191061556940512689692",
        "51934325451728388641918047049293215058642563049483",
        "62467221648435076201727918039944693004732956340691",
        "15732444386908125794514089057706229429197107928209",
        "55037687525678773091862540744969844508330393682126",
        "18336384825330154686196124348767681297534375946515",
        "80386287592878490201521685554828717201219257766954",
        "78182833757993103614740356856449095527097864797581",
        "16726320100436897842553539920931837441497806860984",
        "48403098129077791799088218795327364475675590848030",
        "87086987551392711854517078544161852424320693150332",
        "59959406895756536782107074926966537676326235447210",
        "69793950679652694742597709739166693763042633987085",
        "41052684708299085211399427365734116182760315001271",
        "65378607361501080857009149939512557028198746004375",
        "35829035317434717326932123578154982629742552737307",
        "94953759765105305946966067683156574377167401875275",
        "88902802571733229619176668713819931811048770190271",
        "25267680276078003013678680992525463401061632866526",
        "36270218540497705585629946580636237993140746255962",
        "24074486908231174977792365466257246923322810917141",
        "91430288197103288597806669760892938638285025333403",
        "34413065578016127815921815005561868836468420090470",
        "23053081172816430487623791969842487255036638784583",
        "11487696932154902810424020138335124462181441773470",
        "63783299490636259666498587618221225225512486764533",
        "67720186971698544312419572409913959008952310058822",
        "95548255300263520781532296796249481641953868218774",
        "76085327132285723110424803456124867697064507995236",
        "37774242535411291684276865538926205024910326572967",
        "23701913275725675285653248258265463092207058596522",
        "29798860272258331913126375147341994889534765745501",
        "18495701454879288984856827726077713721403798879715",
        "38298203783031473527721580348144513491373226651381",
        "34829543829199918180278916522431027392251122869539",
        "40957953066405232632538044100059654939159879593635",
        "29746152185502371307642255121183693803580388584903",
        "41698116222072977186158236678424689157993532961922",
        "62467957194401269043877107275048102390895523597457",
        "23189706772547915061505504953922979530901129967519",
        "86188088225875314529584099251203829009407770775672",
        "11306739708304724483816533873502340845647058077308",
        "82959174767140363198008187129011875491310547126581",
        "97623331044818386269515456334926366572897563400500",
        "42846280183517070527831839425882145521227251250327",
        "55121603546981200581762165212827652751691296897789",
        "32238195734329339946437501907836945765883352399886",
        "75506164965184775180738168837861091527357929701337",
        "62177842752192623401942399639168044983993173312731",
        "32924185707147349566916674687634660915035914677504",
        "99518671430235219628894890102423325116913619626622",
        "73267460800591547471830798392868535206946944540724",
        "76841822524674417161514036427982273348055556214818",
        "97142617910342598647204516893989422179826088076852",
        "87783646182799346313767754307809363333018982642090",
        "10848802521674670883215120185883543223812876952786",
        "71329612474782464538636993009049310363619763878039",
        "62184073572399794223406235393808339651327408011116",
        "66627891981488087797941876876144230030984490851411",
        "60661826293682836764744779239180335110989069790714",
        "85786944089552990653640447425576083659976645795096",
        "66024396409905389607120198219976047599490197230297",
        "64913982680032973156037120041377903785566085089252",
        "16730939319872750275468906903707539413042652315011",
        "94809377245048795150954100921645863754710598436791",
        "78639167021187492431995700641917969777599028300699",
        "15368713711936614952811305876380278410754449733078",
        "40789923115535562561142322423255033685442488917353",
        "44889911501440648020369068063960672322193204149535",
        "41503128880339536053299340368006977710650566631954",
        "81234880673210146739058568557934581403627822703280",
        "82616570773948327592232845941706525094512325230608",
        "22918802058777319719839450180888072429661980811197",
        "77158542502016545090413245809786882778948721859617",
        "72107838435069186155435662884062257473692284509516",
        "20849603980134001723930671666823555245252804609722",
        "53503534226472524250874054075591789781264330331690"};

    long long sum = 0;

    for (auto s : numbers)
        sum += std::stoll(s.substr(0, 11));

    return std::to_string(sum).substr(0, 10);
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

static void BM_Euler010(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler010());
}

static void BM_Euler011(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler011());
}

static void BM_Euler012(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler012(state.range(0)));
}

static void BM_Euler012_IntegerFactorization(benchmark::State& state)
{
    for (auto _ : state)
        auto factors{integer_factorization((int) state.range(0))};
}

static void BM_Euler013(benchmark::State& state)
{
    for (auto _ : state)
        benchmark::DoNotOptimize(euler013());
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
BENCHMARK(BM_Euler010);
BENCHMARK(BM_Euler011);

BENCHMARK(BM_Euler012)->Arg(50);
BENCHMARK(BM_Euler012)->Arg(100);
BENCHMARK(BM_Euler012)->Arg(200);
BENCHMARK(BM_Euler012_IntegerFactorization)->Arg(1);
BENCHMARK(BM_Euler012_IntegerFactorization)->Arg(2);
BENCHMARK(BM_Euler012_IntegerFactorization)->Arg(28);
BENCHMARK(BM_Euler012_IntegerFactorization)->Arg(224);
BENCHMARK(BM_Euler012_IntegerFactorization)->Arg(2591);
BENCHMARK(BM_Euler012_IntegerFactorization)->Arg(25200);
BENCHMARK(BM_Euler012_IntegerFactorization)->Arg(76576500);

BENCHMARK(BM_Euler013);

BENCHMARK_MAIN();
