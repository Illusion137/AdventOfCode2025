#include "../utils.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <vector>

#define SAMPLE_1 40
#define SAMPLE_2 25272

struct Circuit;

struct JunctionBox {
    int32_t x;
    int32_t y;
    int32_t z;
    Circuit *circuit;
    double distance_from(const JunctionBox &box2) const{
        return std::sqrt(
            std::pow(box2.x - this->x, 2) +
            std::pow(box2.y - this->y, 2) +
            std::pow(box2.z - this->z, 2)
        );
    }
};

struct Circuit
{
private:
	std::vector<JunctionBox*> data;
public:
    Circuit(JunctionBox *box1, JunctionBox *box2){
        box1->circuit = this;
        box2->circuit = this;
        this->data = {box1, box2};
    }

    void emplace(JunctionBox *other){
        other->circuit = this;
        this->data.emplace_back(other);
    }

    JunctionBox* get(size_t index){
        return this->data.at(index);
    }
    
    void merge_with(Circuit *other){
        for(auto *box: other->data){
            if (box->circuit == this) continue;
            box->circuit = this;
            this->data.push_back(box);
        }
        other->data.clear();
    }

    size_t size() const{
        return this->data.size();
    }
};

std::vector<JunctionBox> get_junction_boxes(const std::string& path){
    const auto lines = read_file_lines(path);
    std::vector<JunctionBox> junction_boxes;
    junction_boxes.reserve(lines.size());
    for(const auto &line: lines){
        const auto split = split_string(line, ",");
        junction_boxes.emplace_back(std::atoi(split[0].c_str()), std::atoi(split[1].c_str()), std::atoi(split[2].c_str()), nullptr);
    }
    return junction_boxes;
}

typedef std::tuple<double, JunctionBox*, JunctionBox*> Edge;
std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> get_edge_min_heap(std::vector<JunctionBox> &junction_boxes){
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> edge_min_heap;
    for(uint32_t i = 0; i < junction_boxes.size(); i++){
        for(uint32_t j = i + 1; j < junction_boxes.size(); j++){
            edge_min_heap.emplace(junction_boxes[i].distance_from(junction_boxes[j]), &junction_boxes[i], &junction_boxes[j]);
        }
    }
    return edge_min_heap;
}

uint64_t sol1(const std::string& path, uint32_t connections_length){
    auto junction_boxes = get_junction_boxes(path);
    auto edge_min_heap = get_edge_min_heap(junction_boxes);

    std::vector<std::unique_ptr<Circuit>> circuits;
    circuits.reserve(1000);
    uint32_t i = 0;
    while(!edge_min_heap.empty() && i < connections_length){
        const auto top = edge_min_heap.top();
        edge_min_heap.pop();

        if(std::get<1>(top)->circuit && std::get<2>(top)->circuit){
            if(std::get<1>(top)->circuit != std::get<2>(top)->circuit){
                std::get<1>(top)->circuit->merge_with(std::get<2>(top)->circuit);
            }
        }
        else if(std::get<1>(top)->circuit){
            std::get<1>(top)->circuit->emplace(std::get<2>(top));
        }
        else if(std::get<2>(top)->circuit){
            std::get<2>(top)->circuit->emplace(std::get<1>(top));
        }
        else {
            circuits.emplace_back(std::make_unique<Circuit>(std::get<1>(top), std::get<2>(top)));
        }
        i++;
    }

    std::sort(circuits.begin(), circuits.end(), [](auto &circuit1, auto &circuit2){return circuit1->size() > circuit2->size();});
    return circuits[0]->size() * circuits[1]->size() * circuits[2]->size();
}

uint64_t sol2(const std::string& path){
    auto junction_boxes = get_junction_boxes(path);
    auto edge_min_heap = get_edge_min_heap(junction_boxes);

    std::vector<std::unique_ptr<Circuit>> circuits;
    circuits.reserve(10000);
    uint32_t box_count = junction_boxes.size();
    while(!edge_min_heap.empty()){
        const auto top = edge_min_heap.top();
        edge_min_heap.pop();

        if(std::get<1>(top)->circuit && std::get<2>(top)->circuit){
            if(std::get<1>(top)->circuit != std::get<2>(top)->circuit){
                std::get<1>(top)->circuit->merge_with(std::get<2>(top)->circuit);
                box_count--;
            }
        }
        else if(std::get<1>(top)->circuit){
            std::get<1>(top)->circuit->emplace(std::get<2>(top));
            box_count--;
        }
        else if(std::get<2>(top)->circuit){
            std::get<2>(top)->circuit->emplace(std::get<1>(top));
            box_count--;
        }
        else {
            circuits.emplace_back(std::make_unique<Circuit>(std::get<1>(top), std::get<2>(top)));
            box_count--;
        }
        if(box_count == 1) {
            return uint64_t(std::get<1>(top)->x) * uint64_t(std::get<2>(top)->x);
        }
    }
    return 0;
}

int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE, 10);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    assert(sol1_sample_result, SAMPLE_1);

    const auto sol1_real_result = sol1(INPUT_FILE, 1000);
    std::println("Solution1-Real  : {}", sol1_real_result);
    std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    assert(sol2_sample_result, SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}