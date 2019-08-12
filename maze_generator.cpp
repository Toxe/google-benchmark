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

struct Coordinates_v2 {
    int x, y;
};

enum class Directions_v3 {
    North = 0,
    East,
    South,
    West
};

enum class WallFlags_v3 {
    North = 0b0001,
    East  = 0b0010,
    South = 0b0100,
    West  = 0b1000
};

class Maze_v2 {
public:
    Maze_v2(const int width, const int height) : width_{width}, height_{height}, nodes_(width * height) {}

    int width() const { return width_; }
    int height() const { return height_; }

    Node_v1* node(const Coordinates_v2 coords) { return &nodes_[coords.y * width_ + coords.x]; }
    bool valid_coords(const Coordinates_v2 coords) const { return coords.x >= 0 && coords.y >= 0 && coords.x < width_ && coords.y < height_; }

private:
    int width_;
    int height_;
    std::vector<Node_v1> nodes_;
};

class Maze_v3 {
public:
    Maze_v3(const int width, const int height)
        : width_{width},
          height_{height},
          nodes_(width * height, static_cast<unsigned char>(WallFlags_v3::North) | static_cast<unsigned char>(WallFlags_v3::East) | static_cast<unsigned char>(WallFlags_v3::South) | static_cast<unsigned char>(WallFlags_v3::West)) {}

    int width() const { return width_; }
    int height() const { return height_; }

    bool valid_coords(const Coordinates_v2 coords) const { return coords.x >= 0 && coords.y >= 0 && coords.x < width_ && coords.y < height_; }

    bool node_visited(const Coordinates_v2 coords) const { return nodes_[coords.y * width_ + coords.x] & 0b10000; }
    void set_node_visited(const Coordinates_v2 coords) { nodes_[coords.y * width_ + coords.x] |= 0b10000; }

    bool has_wall(const Coordinates_v2 coords, WallFlags_v3 wall) const { return nodes_[coords.y * width_ + coords.x] & static_cast<unsigned char>(wall); }

    void clear_walls(const Coordinates_v2 orig, const Coordinates_v2 dest, WallFlags_v3 orig_wall, WallFlags_v3 dest_wall) {
        nodes_[orig.y * width_ + orig.x] &= ~(static_cast<unsigned char>(orig_wall));
        nodes_[dest.y * width_ + dest.x] &= ~(static_cast<unsigned char>(dest_wall));
    }

private:
    int width_;
    int height_;
    std::vector<unsigned char> nodes_;
};

class Maze_v4 {
    using Node = unsigned char;

public:
    enum class Directions {
        North = 0,
        East,
        South,
        West
    };

    enum class WallFlags {
        North = 0b0001,
        East  = 0b0010,
        South = 0b0100,
        West  = 0b1000
    };

    struct Coordinates {
        int x, y;
    };

    Maze_v4(const int width, const int height)
        : width_{width},
          height_{height},
          nodes_(width * height, static_cast<Node>(WallFlags::North) | static_cast<Node>(WallFlags::East) | static_cast<Node>(WallFlags::South) | static_cast<Node>(WallFlags::West)) {}

    int width() const { return width_; }
    int height() const { return height_; }

    bool valid_coords(const Coordinates coords) const { return coords.x >= 0 && coords.y >= 0 && coords.x < width_ && coords.y < height_; }

    Coordinates coords_in_direction(const Coordinates coords, const Directions dir) {
        const Coordinates offset{direction_coords_offset_[static_cast<int>(dir)]};
        return Coordinates{coords.x + offset.x, coords.y + offset.y};
    }

    Node& node(const Coordinates coords) { return nodes_[coords.y * width_ + coords.x]; };
    bool node_visited(const Coordinates coords) { return node(coords) & 0b10000; }
    void set_node_visited(const Coordinates coords) { node(coords) |= 0b10000; }

    bool has_wall(const Coordinates coords, WallFlags wall) { return node(coords) & static_cast<Node>(wall); }

    void clear_walls(const Coordinates orig, const Coordinates dest, Directions dir) {
        const WallFlags orig_wall = wall_in_direction_[static_cast<int>(dir)];
        const WallFlags dest_wall = wall_in_direction_[static_cast<int>(opposite_direction_[static_cast<int>(dir)])];
        node(orig) &= ~(static_cast<Node>(orig_wall));
        node(dest) &= ~(static_cast<Node>(dest_wall));
    }

private:
    const int width_;
    const int height_;
    std::vector<Node> nodes_;

    const WallFlags wall_in_direction_[4] = { WallFlags::North, WallFlags::East, WallFlags::South, WallFlags::West };
    const Directions opposite_direction_[4] = { Directions::South, Directions::West, Directions::North, Directions::East };
    const Coordinates direction_coords_offset_[4] = { Coordinates{0, -1}, Coordinates{1, 0}, Coordinates{0, 1}, Coordinates{-1, 0} };
};

class Maze_v5 {
    using Node = unsigned char;

public:
    enum class Directions {
        North = 0,
        East,
        South,
        West
    };

    enum class WallFlags {
        North = 0b0001,
        East  = 0b0010,
        South = 0b0100,
        West  = 0b1000
    };

    struct Coordinates {
        int x, y;
    };

    Maze_v5(const int width, const int height)
        : width_{width},
          height_{height},
          nodes_(width * height, static_cast<Node>(WallFlags::North) | static_cast<Node>(WallFlags::East) | static_cast<Node>(WallFlags::South) | static_cast<Node>(WallFlags::West)),
          random_device_(),
          random_generator_(random_device_()) {}

    int width() const { return width_; }
    int height() const { return height_; }

    bool valid_coords(const Coordinates coords) const { return coords.x >= 0 && coords.y >= 0 && coords.x < width_ && coords.y < height_; }

    Coordinates coords_in_direction(const Coordinates coords, const Directions dir) {
        const Coordinates offset{direction_coords_offset_[static_cast<int>(dir)]};
        return Coordinates{coords.x + offset.x, coords.y + offset.y};
    }

    Node& node(const Coordinates coords) { return nodes_[coords.y * width_ + coords.x]; };
    bool node_visited(const Coordinates coords) { return node(coords) & 0b10000; }
    void set_node_visited(const Coordinates coords) { node(coords) |= 0b10000; }

    bool has_wall(const Coordinates coords, WallFlags wall) { return node(coords) & static_cast<Node>(wall); }

    void clear_walls(const Coordinates orig, const Coordinates dest, Directions dir) {
        const WallFlags orig_wall = wall_in_direction_[static_cast<int>(dir)];
        const WallFlags dest_wall = wall_in_direction_[static_cast<int>(opposite_direction_[static_cast<int>(dir)])];
        node(orig) &= ~(static_cast<Node>(orig_wall));
        node(dest) &= ~(static_cast<Node>(dest_wall));
    }

    std::vector<Directions> random_directions() {
        std::vector<Directions> directions{Directions::North, Directions::East, Directions::South, Directions::West};
        std::shuffle(directions.begin(), directions.end(), random_generator_);
        return directions;
    }

private:
    const int width_;
    const int height_;
    std::vector<Node> nodes_;

    std::random_device random_device_;
    std::mt19937 random_generator_;

    const WallFlags wall_in_direction_[4] = { WallFlags::North, WallFlags::East, WallFlags::South, WallFlags::West };
    const Directions opposite_direction_[4] = { Directions::South, Directions::West, Directions::North, Directions::East };
    const Coordinates direction_coords_offset_[4] = { Coordinates{0, -1}, Coordinates{1, 0}, Coordinates{0, 1}, Coordinates{-1, 0} };
};

class Maze_v6 {
    using Node = unsigned char;

public:
    enum class Directions {
        North = 0,
        East,
        South,
        West
    };

    enum class WallFlags {
        North = 0b0001,
        East  = 0b0010,
        South = 0b0100,
        West  = 0b1000
    };

    struct Coordinates {
        int x, y;
    };

    Maze_v6(const int width, const int height)
        : width_{width},
          height_{height},
          nodes_(width * height, static_cast<Node>(WallFlags::North) | static_cast<Node>(WallFlags::East) | static_cast<Node>(WallFlags::South) | static_cast<Node>(WallFlags::West)),
          random_device_(),
          random_generator_(random_device_()),
          random_dist_{0, 23} {}

    int width() const { return width_; }
    int height() const { return height_; }

    bool valid_coords(const Coordinates coords) const { return coords.x >= 0 && coords.y >= 0 && coords.x < width_ && coords.y < height_; }

    Coordinates coords_in_direction(const Coordinates coords, const Directions dir) {
        const Coordinates offset{direction_coords_offset_[static_cast<int>(dir)]};
        return Coordinates{coords.x + offset.x, coords.y + offset.y};
    }

    const std::vector<Directions>& random_directions() { return all_possible_random_directions[random_dist_(random_generator_)]; }

    Node& node(const Coordinates coords) { return nodes_[coords.y * width_ + coords.x]; };
    bool node_visited(const Coordinates coords) { return node(coords) & 0b10000; }
    void set_node_visited(const Coordinates coords) { node(coords) |= 0b10000; }

    bool has_wall(const Coordinates coords, WallFlags wall) { return node(coords) & static_cast<Node>(wall); }
    void clear_walls(const Coordinates orig, const Coordinates dest, Directions dir) {
        const WallFlags orig_wall = wall_in_direction_[static_cast<int>(dir)];
        const WallFlags dest_wall = wall_in_direction_[static_cast<int>(opposite_direction_[static_cast<int>(dir)])];
        node(orig) &= ~(static_cast<Node>(orig_wall));
        node(dest) &= ~(static_cast<Node>(dest_wall));
    }

private:
    const int width_;
    const int height_;
    std::vector<Node> nodes_;

    std::random_device random_device_;
    std::mt19937 random_generator_;
    std::uniform_int_distribution<> random_dist_;

    const WallFlags wall_in_direction_[4] = { WallFlags::North, WallFlags::East, WallFlags::South, WallFlags::West };
    const Directions opposite_direction_[4] = { Directions::South, Directions::West, Directions::North, Directions::East };
    const Coordinates direction_coords_offset_[4] = { Coordinates{0, -1}, Coordinates{1, 0}, Coordinates{0, 1}, Coordinates{-1, 0} };
    const std::vector<std::vector<Directions>> all_possible_random_directions = {
        {Directions::North, Directions::East,  Directions::South, Directions::West},
        {Directions::North, Directions::East,  Directions::West,  Directions::South},
        {Directions::North, Directions::South, Directions::East,  Directions::West},
        {Directions::North, Directions::South, Directions::West,  Directions::East},
        {Directions::North, Directions::West,  Directions::East,  Directions::South},
        {Directions::North, Directions::West,  Directions::South, Directions::East},
        {Directions::East,  Directions::North, Directions::South, Directions::West},
        {Directions::East,  Directions::North, Directions::West,  Directions::South},
        {Directions::East,  Directions::South, Directions::North, Directions::West},
        {Directions::East,  Directions::South, Directions::West,  Directions::North},
        {Directions::East,  Directions::West,  Directions::North, Directions::South},
        {Directions::East,  Directions::West,  Directions::South, Directions::North},
        {Directions::South, Directions::North, Directions::East,  Directions::West},
        {Directions::South, Directions::North, Directions::West,  Directions::East},
        {Directions::South, Directions::East,  Directions::North, Directions::West},
        {Directions::South, Directions::East,  Directions::West,  Directions::North},
        {Directions::South, Directions::West,  Directions::North, Directions::East},
        {Directions::South, Directions::West,  Directions::East,  Directions::North},
        {Directions::West,  Directions::North, Directions::East,  Directions::South},
        {Directions::West,  Directions::North, Directions::South, Directions::East},
        {Directions::West,  Directions::East,  Directions::North, Directions::South},
        {Directions::West,  Directions::East,  Directions::South, Directions::North},
        {Directions::West,  Directions::South, Directions::North, Directions::East},
        {Directions::West,  Directions::South, Directions::East,  Directions::North}};
};

class Maze_v7 {
public:
    using Node = unsigned char;

    enum class Directions { North = 0, East, South, West };
    enum class WallFlags { North = 0b0001, East = 0b0010, South = 0b0100, West = 0b1000 };

    struct Coordinates { int x, y; };

    Maze_v7(const int width, const int height)
        : width_{width},
          height_{height},
          nodes_(width * height, static_cast<Node>(WallFlags::North) | static_cast<Node>(WallFlags::East) | static_cast<Node>(WallFlags::South) | static_cast<Node>(WallFlags::West)),
          random_device_(),
          random_generator_(random_device_()),
          random_dist_{0, 23} {}

    int width() const { return width_; }
    int height() const { return height_; }

    bool valid_coords(const Coordinates coords) const { return coords.x >= 0 && coords.y >= 0 && coords.x < width_ && coords.y < height_; }

    Coordinates coords_in_direction(const Coordinates coords, const Directions dir) {
        const Coordinates offset{direction_coords_offset_[static_cast<int>(dir)]};
        return Coordinates{coords.x + offset.x, coords.y + offset.y};
    }

    const std::vector<Directions>& random_directions() { return all_possible_random_directions[random_dist_(random_generator_)]; }

    Node& node(const Coordinates coords) { return nodes_[coords.y * width_ + coords.x]; };
    bool node_visited(const Coordinates coords) { return node(coords) & 0b10000; }
    void set_node_visited(const Coordinates coords) { node(coords) |= 0b10000; }

    bool has_wall(const Coordinates coords, WallFlags wall) { return node(coords) & static_cast<Node>(wall); }
    void clear_walls(const Coordinates orig, const Coordinates dest, Directions dir) {
        const WallFlags orig_wall = wall_in_direction_[static_cast<int>(dir)];
        const WallFlags dest_wall = wall_in_direction_[static_cast<int>(opposite_direction_[static_cast<int>(dir)])];
        node(orig) &= ~(static_cast<Node>(orig_wall));
        node(dest) &= ~(static_cast<Node>(dest_wall));
    }

private:
    const int width_;
    const int height_;
    std::vector<Node> nodes_;

    std::random_device random_device_;
    std::mt19937 random_generator_;
    std::uniform_int_distribution<> random_dist_;

    const WallFlags wall_in_direction_[4] = { WallFlags::North, WallFlags::East, WallFlags::South, WallFlags::West };
    const Directions opposite_direction_[4] = { Directions::South, Directions::West, Directions::North, Directions::East };
    const Coordinates direction_coords_offset_[4] = { Coordinates{0, -1}, Coordinates{1, 0}, Coordinates{0, 1}, Coordinates{-1, 0} };
    const std::vector<std::vector<Directions>> all_possible_random_directions = {
        {Directions::North, Directions::East,  Directions::South, Directions::West},
        {Directions::North, Directions::East,  Directions::West,  Directions::South},
        {Directions::North, Directions::South, Directions::East,  Directions::West},
        {Directions::North, Directions::South, Directions::West,  Directions::East},
        {Directions::North, Directions::West,  Directions::East,  Directions::South},
        {Directions::North, Directions::West,  Directions::South, Directions::East},
        {Directions::East,  Directions::North, Directions::South, Directions::West},
        {Directions::East,  Directions::North, Directions::West,  Directions::South},
        {Directions::East,  Directions::South, Directions::North, Directions::West},
        {Directions::East,  Directions::South, Directions::West,  Directions::North},
        {Directions::East,  Directions::West,  Directions::North, Directions::South},
        {Directions::East,  Directions::West,  Directions::South, Directions::North},
        {Directions::South, Directions::North, Directions::East,  Directions::West},
        {Directions::South, Directions::North, Directions::West,  Directions::East},
        {Directions::South, Directions::East,  Directions::North, Directions::West},
        {Directions::South, Directions::East,  Directions::West,  Directions::North},
        {Directions::South, Directions::West,  Directions::North, Directions::East},
        {Directions::South, Directions::West,  Directions::East,  Directions::North},
        {Directions::West,  Directions::North, Directions::East,  Directions::South},
        {Directions::West,  Directions::North, Directions::South, Directions::East},
        {Directions::West,  Directions::East,  Directions::North, Directions::South},
        {Directions::West,  Directions::East,  Directions::South, Directions::North},
        {Directions::West,  Directions::South, Directions::North, Directions::East},
        {Directions::West,  Directions::South, Directions::East,  Directions::North}};
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

Coordinates_v2 coords_in_direction_v2(const Coordinates_v2 coords, const int dir)
{
    Coordinates_v2 new_coords{coords};

    switch (dir) {
    case 0:
        new_coords.y = coords.y - 1;
        break;
    case 1:
        new_coords.x = coords.x + 1;
        break;
    case 2:
        new_coords.y = coords.y + 1;
        break;
    case 3:
        new_coords.x = coords.x - 1;
        break;
    }

    return new_coords;
}

Coordinates_v2 coords_in_direction_v3(const Coordinates_v2 coords, const Directions_v3 dir)
{
    Coordinates_v2 new_coords{coords};

    switch (dir) {
    case Directions_v3::North:
        new_coords.y = coords.y - 1;
        break;
    case Directions_v3::East:
        new_coords.x = coords.x + 1;
        break;
    case Directions_v3::South:
        new_coords.y = coords.y + 1;
        break;
    case Directions_v3::West:
        new_coords.x = coords.x - 1;
        break;
    }

    return new_coords;
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

void visit_v2(Maze_v2& maze, const Coordinates_v2 coords)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> directions{0, 1, 2, 3};
    std::shuffle(directions.begin(), directions.end(), g);

    Node_v1* current_node = maze.node(coords);

    current_node->visited = true;

    for (const auto dir : directions) {
        Coordinates_v2 new_coords{coords_in_direction_v2(coords, dir)};

        if (maze.valid_coords(new_coords)) {
            Node_v1* next_node = maze.node(new_coords);

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

                visit_v2(maze, new_coords);
            }

        }
    }
}

void visit_v3(Maze_v3& maze, const Coordinates_v2 coords)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<Directions_v3> Directions_v3{Directions_v3::North, Directions_v3::East, Directions_v3::South, Directions_v3::West};
    std::shuffle(Directions_v3.begin(), Directions_v3.end(), g);

    maze.set_node_visited(coords);

    for (const auto dir : Directions_v3) {
        Coordinates_v2 new_coords{coords_in_direction_v3(coords, dir)};

        if (maze.valid_coords(new_coords)) {
            if (!maze.node_visited(new_coords)) {
                switch (dir) {
                case Directions_v3::North:
                    maze.clear_walls(coords, new_coords, WallFlags_v3::North, WallFlags_v3::South);
                    break;
                case Directions_v3::East:
                    maze.clear_walls(coords, new_coords, WallFlags_v3::East, WallFlags_v3::West);
                    break;
                case Directions_v3::South:
                    maze.clear_walls(coords, new_coords, WallFlags_v3::South, WallFlags_v3::North);
                    break;
                case Directions_v3::West:
                    maze.clear_walls(coords, new_coords, WallFlags_v3::West, WallFlags_v3::East);
                    break;
                }

                visit_v3(maze, new_coords);
            }
        }
    }
}

void visit_v4(Maze_v4& maze, const Maze_v4::Coordinates coords)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<Maze_v4::Directions> directions{Maze_v4::Directions::North, Maze_v4::Directions::East, Maze_v4::Directions::South, Maze_v4::Directions::West};
    std::shuffle(directions.begin(), directions.end(), g);

    maze.set_node_visited(coords);

    for (const auto dir : directions) {
        Maze_v4::Coordinates new_coords{maze.coords_in_direction(coords, dir)};

        if (maze.valid_coords(new_coords)) {
            if (!maze.node_visited(new_coords)) {
                maze.clear_walls(coords, new_coords, dir);
                visit_v4(maze, new_coords);
            }
        }
    }
}

void visit_v5(Maze_v5& maze, const Maze_v5::Coordinates coords)
{
    maze.set_node_visited(coords);

    for (const auto dir : maze.random_directions()) {
        Maze_v5::Coordinates new_coords{maze.coords_in_direction(coords, dir)};

        if (maze.valid_coords(new_coords)) {
            if (!maze.node_visited(new_coords)) {
                maze.clear_walls(coords, new_coords, dir);
                visit_v5(maze, new_coords);
            }
        }
    }
}

void visit_v6(Maze_v6& maze, const Maze_v6::Coordinates coords)
{
    maze.set_node_visited(coords);

    for (const auto dir : maze.random_directions()) {
        Maze_v6::Coordinates new_coords{maze.coords_in_direction(coords, dir)};

        if (maze.valid_coords(new_coords)) {
            if (!maze.node_visited(new_coords)) {
                maze.clear_walls(coords, new_coords, dir);
                visit_v6(maze, new_coords);
            }
        }
    }
}

void visit_v7(Maze_v7& maze, const Maze_v7::Coordinates coords)
{
    maze.set_node_visited(coords);

    for (const auto dir : maze.random_directions()) {
        Maze_v7::Coordinates next_coords{maze.coords_in_direction(coords, dir)};

        if (maze.valid_coords(next_coords) && !maze.node_visited(next_coords)) {
            maze.clear_walls(coords, next_coords, dir);
            visit_v7(maze, next_coords);
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

static void BM_Visit_v2(benchmark::State& state)
{
    for (auto _ : state) {
        Maze_v2 maze(static_cast<int>(state.range(0)), static_cast<int>(state.range(0)));
        visit_v2(maze, {0, 0});
        benchmark::DoNotOptimize(maze);
    }
}

static void BM_Visit_v3(benchmark::State& state)
{
    for (auto _ : state) {
        Maze_v3 maze(static_cast<int>(state.range(0)), static_cast<int>(state.range(0)));
        visit_v3(maze, {0, 0});
        benchmark::DoNotOptimize(maze);
    }
}

static void BM_Visit_v4(benchmark::State& state)
{
    for (auto _ : state) {
        Maze_v4 maze(static_cast<int>(state.range(0)), static_cast<int>(state.range(0)));
        visit_v4(maze, {0, 0});
        benchmark::DoNotOptimize(maze);
    }
}

static void BM_Visit_v5(benchmark::State& state)
{
    for (auto _ : state) {
        Maze_v5 maze(static_cast<int>(state.range(0)), static_cast<int>(state.range(0)));
        visit_v5(maze, {0, 0});
        benchmark::DoNotOptimize(maze);
    }
}

static void BM_Visit_v6(benchmark::State& state)
{
    for (auto _ : state) {
        Maze_v6 maze(static_cast<int>(state.range(0)), static_cast<int>(state.range(0)));
        visit_v6(maze, {0, 0});
        benchmark::DoNotOptimize(maze);
    }
}

static void BM_Visit_v7(benchmark::State& state)
{
    for (auto _ : state) {
        Maze_v7 maze(static_cast<int>(state.range(0)), static_cast<int>(state.range(0)));
        visit_v7(maze, {0, 0});
        benchmark::DoNotOptimize(maze);
    }
}

BENCHMARK(BM_Visit_v1)->Arg(15)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v2)->Arg(15)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v3)->Arg(15)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v4)->Arg(15)->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_Visit_v5)->Arg(15)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v5)->Arg(25)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v5)->Arg(50)->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_Visit_v6)->Arg(15)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v6)->Arg(25)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v6)->Arg(50)->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_Visit_v7)->Arg(15)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v7)->Arg(25)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Visit_v7)->Arg(50)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
