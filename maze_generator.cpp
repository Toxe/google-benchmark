#include <algorithm>
#include <benchmark/benchmark.h>
#include <random>
#include <vector>

struct Node_v1 {
    bool visited = false;
    bool has_north_wall = true;
    bool has_east_wall = true;
    bool has_south_wall = true;
    bool has_west_wall = true;
};

void coord_in_direction_v1(const int x, const int y, const int dir, int* nx, int* ny)
{
    *nx = x;
    *ny = y;

    switch (dir) {
    case 0:
        *nx = x;
        *ny = y - 1;
        break;
    case 1:
        *nx = x + 1;
        *ny = y;
        break;
    case 2:
        *nx = x;
        *ny = y + 1;
        break;
    case 3:
        *nx = x - 1;
        *ny = y;
        break;
    }
}

Node_v1* node_at_v1(Node_v1* maze, const int num_cols, const int x, const int y)
{
    return &maze[y * num_cols + x];
}

void visit_v1(Node_v1* maze, const int num_rows, const int num_cols, const int x, const int y)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> directions{0, 1, 2, 3};
    std::shuffle(directions.begin(), directions.end(), g);

    Node_v1* current_node = node_at_v1(maze, num_cols, x, y);

    current_node->visited = true;

    for (auto dir : directions) {
        int nx, ny;
        coord_in_direction_v1(x, y, dir, &nx, &ny);

        if (nx >= 0 && ny >= 0 && nx < num_cols && ny < num_rows) {
            Node_v1* next_node = node_at_v1(maze, num_cols, nx, ny);

            if (!next_node->visited) {
                switch (dir) {
                case 0:
                    current_node->has_north_wall = false;
                    next_node->has_south_wall = false;
                    break;
                case 1:
                    current_node->has_east_wall = false;
                    next_node->has_west_wall = false;
                    break;
                case 2:
                    current_node->has_south_wall = false;
                    next_node->has_north_wall = false;
                    break;
                case 3:
                    current_node->has_west_wall = false;
                    next_node->has_east_wall = false;
                    break;
                }

                visit_v1(maze, num_rows, num_cols, nx, ny);
            }

        }
    }
}

static void BM_Visit_v1(benchmark::State& state)
{
    constexpr int num_rows = 15;
    constexpr int num_cols = 15;

    for (auto _ : state) {
        Node_v1 maze[num_rows * num_cols];
        visit_v1(maze, num_rows, num_cols, 0, 0);
        benchmark::DoNotOptimize(maze);
    }
}

BENCHMARK(BM_Visit_v1);

BENCHMARK_MAIN();
