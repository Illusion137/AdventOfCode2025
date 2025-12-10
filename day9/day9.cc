#include "../utils.hpp"
#include <algorithm>
#include <cstdint>
#include <vector>

#define SAMPLE_1 50
#define SAMPLE_2 24

struct Point {
    int32_t x;
    int32_t y;
    inline int64_t get_area(const Point &other_point) const{
        const int64_t length = std::abs(other_point.x - this->x) + 1;
        const int64_t height = std::abs(other_point.y - this->y) + 1;
        return length * height;
    }
    inline bool intersects_line(const Point &p1a, const Point &p1b, const Point &p2a, const Point &p2b) const{
        return p2a.x < p1b.x && p2b.x > p1a.x && p2a.y < p1b.y && p2b.y > p1a.y;
    }
    inline Point min(const Point &other_point) const{
        return {std::min(this->x, other_point.x), std::min(this->y, other_point.y)};
    }
    inline Point max(const Point &other_point) const{
        return {std::max(this->x, other_point.x), std::max(this->y, other_point.y)};
    }
};

std::vector<Point> get_points(const std::string &path){
    const auto lines = read_file_lines(path);
    std::vector<Point> points;
    points.reserve(lines.size());
    for(const auto &line: lines){
        const auto split = split_string(line, ",");
        points.emplace_back(std::atoi(split[0].c_str()), std::atoi(split[1].c_str()));
    }
    return points;
}

std::vector<std::tuple<int64_t, Point, Point>> get_areas(const std::vector<Point> &points){
    std::vector<std::tuple<int64_t, Point, Point>> areas;
    areas.reserve((points.size() - 1) * points.size());
    for(uint32_t i = 0; i < points.size(); i++){
        for(uint32_t j = i + 1; j < points.size(); j++){
            areas.push_back({points[i].get_area(points[j]), points[i].min(points[j]), points[i].max(points[j])});
            std::ranges::push_heap(areas, {}, [](auto &&area) { return std::get<0>(area); });
        }
    }
    return areas;
}

uint64_t sol1(const std::string& path){
    const auto points = get_points(path);
    const auto areas = get_areas(points);
    return std::get<0>(areas[0]);
}

uint64_t sol2(const std::string& path){
    const auto points = get_points(path);
    auto areas = get_areas(points);

    while (true) {
        const auto [area, area_point1, area_point2] = areas[0];
        bool exit = true;
        for (uint32_t i = 0; i < points.size(); i++) {
            auto point1 = points[i];
            auto point2 = points[(i + 1) % points.size()];
            std::tie(point1, point2) = std::tuple{point1.min(point2), point1.max(point2)};
            if (area_point1.intersects_line(area_point1, area_point2, point1, point2)){
                std::ranges::pop_heap(areas, {}, [](auto &&area) { return std::get<0>(area); });
                areas.pop_back();
                exit = false;
                break;
            }
        }
        if(exit){
            return area;
        }
    }
    return 0;
}

int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    assert(sol1_sample_result == SAMPLE_1);

    const auto sol1_real_result = sol1(INPUT_FILE);
    std::println("Solution1-Real  : {}", sol1_real_result);
    std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    assert(sol2_sample_result == SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}