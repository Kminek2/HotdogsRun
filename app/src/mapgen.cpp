#include "mapgen.h"

#include <queue>
#include <stack>
#include <random>
#include <algorithm>
#include <iterator>

#include <iostream>

const std::array<glm::vec2, 8> MapGen::neighbor_map = { {
    {-1, 1}, {0, 1}, {1, 1},
    {-1, 0},         {1, 0},
    {-1,-1}, {0,-1}, {1,-1}
} };

std::vector<glm::vec2> MapGen::Bresenham(glm::vec2 start, glm::vec2 end) {
    int dx = start[0] - end[0];
    int dy = start[1] - end[1];
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;
    int y = end[0];
   
    
        

    std::vector<glm::vec2> result;

    for (int x = start[0]; x < end[0]; x++) {
        result.emplace_back(x,y);
        std::cout << 'x:' << x << ' y:' << y << std::endl;

        if (D > 0) {
            y += 1;
            D = D +(2 * (dy-dx));
        }
        else D = D + 2 * dy;
    }
    return result;
}

bool dfs(glm::vec2& start, uint16_t n, std::vector<glm::vec2>& points, std::set<glm::vec2>& taken, std::mt19937& gen) {
    if (n == 0) return true;

    std::array<glm::vec2, 8> n_map = MapGen::neighbor_map;
    std::shuffle(std::begin(n_map), std::end(n_map), gen);

    points.push_back(start);
    taken.insert(start);

    for (glm::vec2 neighbor : n_map) {
        glm::vec2 cur = start + neighbor;

        if (!taken.count(cur)) 
            if (dfs(cur, n - 1, points, taken, gen))
                return true;
    }

    points.pop_back();
    taken.erase(taken.find(start));

    return false;
}

std::vector<glm::vec2> MapGen::generateMap(uint16_t len, size_t seed) {
    // -- init --
    assert(len >= 3);

    if (seed == static_cast<size_t>(-1)) {
        std::random_device rd;
        seed = rd();
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 7);

    std::vector<glm::vec2> points = { {0, 0} };
    points.reserve(2 * len);

    std::set<glm::vec2> taken = { {0, 0} };
    glm::vec2 current = { 1, 0 }; // will be inserted in the dfs

    // -- dfs --
    std::cout << (int)dfs(current, len - 1, points, taken, gen) << '\n';

    // -- bfs --

    points.shrink_to_fit();
    return points;
}

void MapGen::spreadMapPoints(std::vector<glm::vec2>& points, float spread) {
	std::transform(points.begin(), points.end(), points.begin(), [&spread](glm::vec2& a) { return a * spread; });
}
void MapGen::offsetMapPoints(std::vector<glm::vec2>& points, float offset) {
	std::transform(points.begin(), points.end(), points.begin(), [&offset](glm::vec2& a) { return a + glm::vec2(offset); }); 
}