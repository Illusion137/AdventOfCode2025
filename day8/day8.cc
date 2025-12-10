#include "../utils.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iterator>
#include <list>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#define SAMPLE_1 40
#define SAMPLE_2 -1

class Circuit;

struct JunctionBox {
    int32_t x;
    int32_t y;
    int32_t z;
    Circuit* circuit;
    double distance_from(const JunctionBox &box2) const{
        return std::sqrt(
            std::pow(box2.x - this->x, 2) +
            std::pow(box2.y - this->y, 2) +
            std::pow(box2.z - this->z, 2)
        );
    }
};

class Circuit
{
	private:
	std::list<JunctionBox> members;

	public:
	void mergeInto(Circuit*){
        while (members.size() > 0)
        {
            JunctionBox to_add = members.front();
            toAdd->circuit = other;
            other->members.append(toAdd);
        }

    }
	int getSize(){

    }
	void addMember(JunctionBox* b){

    }
	friend class JunctionBox;
};

template<>
struct std::formatter<JunctionBox> : std::formatter<std::string> {
    template<typename FormatContext>
    auto format(const JunctionBox& s, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "Box({}, {}, {})", s.x, s.y, s.z);
    }
};
std::vector<JunctionBox> get_junction_boxes(const std::string& path){
    const auto lines = read_file_lines(path);
    std::vector<JunctionBox> junction_boxes;
    junction_boxes.reserve(lines.size());
    for(const auto &line: lines){
        const auto split = split_string(line, ",");
        junction_boxes.emplace_back(std::atoi(split[0].c_str()), std::atoi(split[1].c_str()), std::atoi(split[2].c_str()));
    }
    return junction_boxes;
}

std::vector<std::vector<double>> get_distance_map(const std::vector<JunctionBox> &junction_boxes){
    std::vector<std::vector<double>> distance_map;
    distance_map.resize(junction_boxes.size());
    for(uint32_t i = 0; i < junction_boxes.size(); i++){
        distance_map[i].resize(junction_boxes.size());
        for(uint32_t j = 0; j < junction_boxes.size(); j++){
            if(i == j) distance_map[i][j] = std::numeric_limits<double>::max(); 
            else distance_map[i][j] = junction_boxes[i].distance_from(junction_boxes[j]);
        }
    }
    return distance_map;
}
uint32_t get_min_index(const std::vector<double> &row){
    uint32_t min_index = -1;
    double min_value = std::numeric_limits<double>::max();
    for(uint32_t i = 0; i < row.size(); i++){
        if(row[i] < min_value){
            min_value = row[i];
            min_index = i;
        }
    }    
    return min_index;
}
std::vector<uint32_t> get_min_index_list(const std::vector<std::vector<double>> &distance_map){
    std::vector<uint32_t> list;
    list.reserve(distance_map.size());
    for(const auto &distances: distance_map){
        list.emplace_back(get_min_index(distances));
    }
    return list;
}


// void rec(const std::vector<uint32_t>& min_index_list, std::unordered_set<uint32_t> &used_indexes, uint32_t initial_index, std::unordered_set<uint32_t> *output_set = nullptr){
//     if(output_set != nullptr) {
//         if(output_set->contains(min_index_list[initial_index])) return;
//         output_set->insert(min_index_list[initial_index]);
//         used_indexes.insert(min_index_list[initial_index]);
//         rec(min_index_list, used_indexes, initial_index, output_set);
//     }
//     output_set = new std::unordered_set<uint32_t>{};
//     output_set->insert(initial_index);
    
// }

void shift_up(std::vector<std::pair<JunctionBox*, JunctionBox*>*> &arr, uint32_t idx, uint32_t size)
{
	int max = size-1;
	delete arr[max];
	arr[max] = NULL;
	for (int i=max; i>idx&&i>0; i--)
	{
		arr[i] = arr[i-1];
	}
	arr[idx] = NULL;
}


int32_t add_pair_to_connections(std::vector<std::pair<JunctionBox*, JunctionBox*>*> &connections, std::pair<JunctionBox*, JunctionBox*> *add_pair, uint32_t connection_length){
    for(uint32_t i = 0; i < connection_length; i++){
        auto &pair = connections[i];
        if(pair == nullptr){
            pair = add_pair;
            return i;
        }
        if(add_pair->first->distance_from(*add_pair->second) < pair->first->distance_from(*pair->second)){
            shift_up(connections, i, connection_length);
            pair = add_pair;
            return i;
        }
    }
    return -1;
} 


uint64_t sol1(const std::string& path, uint32_t connections_length){
    auto junction_boxes = get_junction_boxes(path);
    std::vector<std::pair<JunctionBox*, JunctionBox*>*> connections(connections_length, nullptr);

    for(uint32_t i = 0; i < connections_length; i++){
        for(uint32_t j = i + 1; j < connections_length; j++){
            std::pair<JunctionBox*, JunctionBox*> *pair = 
                new std::pair<JunctionBox*, JunctionBox*>{&(junction_boxes[i]), &(junction_boxes[j])};
            const uint32_t index = add_pair_to_connections(connections, pair, connections_length);
			if(index == -1)
			{
				delete pair;
				pair = NULL;
			}
        }
    }
    return 0;

    std::list<Circuit> circuits;
	for (int i=0; i<1000; i++)
	{
		JunctionBox* jb1 = connections[i]->x;
		JunctionBox* jb2 = connections[i]->y;

		if (jb1->getCircuit() == NULL)
		{
			if (jb2->getCircuit() == NULL)
			{
				Circuit* c = new Circuit();
				jb1->setCircuit(c);
				jb2->setCircuit(c);
				c->addMember(jb1);
				c->addMember(jb2);
				circuits.append(c);
			}
			else
			{
				jb1->setCircuit(jb2->getCircuit());
				jb2->getCircuit()->addMember(jb1);
			}
		}
		else
		{
			if (jb2->getCircuit() == NULL)
			{
				jb2->setCircuit(jb1->getCircuit());
				jb1->getCircuit()->addMember(jb2);
			}
			else if (jb2->getCircuit() != jb1->getCircuit())
			{
				Circuit* c = jb2->getCircuit();
				c->mergeInto(jb1->getCircuit());
			}
			// if jb2->getCircuit() == jb1->getCircuit() then do nothing
		}
	}

	Circuit* topThree[3];
	for (int i=0; i<3; i++)
	{
		topThree[i] = NULL;
	}
	while (circuits.length() > 0)
	{
		Circuit* c = circuits.pop();
		int idx = addCircuitSorted(&topThree, c, 3);
		if (idx == -1)
		{
			delete c;
			c = NULL;
		}
	}

	int mulResult = 1;
	for (int i=0; i<3; i++)
	{
		std::cout << topThree[i]->getSize() << std::endl;
		mulResult *= topThree[i]->getSize();
	}

    return mulResult;

    
    // const auto distance_map = get_distance_map(junction_boxes);

    // std::println("{}", get_min_index_list(distance_map));

    // std::vector<std::unordered_set<uint32_t>> group_indexes;

    // std::unordered_map<uint32_t, uint32_t> box_index_to_group_index;

    // uint32_t count = 0;
    // for(uint32_t i = 0; i < distance_map.size() && count < 10; i++){
    //     // std::println("{} -> {}", i, min_index);
    //     const uint32_t min_index = get_min_index(distance_map[i]);
    //     if(box_index_to_group_index.contains(i)) {
    //         if(box_index_to_group_index.contains(min_index)) {
    //             const auto to_delete_index = box_index_to_group_index[min_index];
    //             if(to_delete_index == box_index_to_group_index[i]) continue;
    //             for(const auto &min_index_group_value: group_indexes[to_delete_index]){
    //                 group_indexes[box_index_to_group_index[i]].insert(min_index_group_value);
    //                 count++;
    //                 box_index_to_group_index[min_index_group_value] = box_index_to_group_index[i];
    //             }
    //             group_indexes.erase(group_indexes.begin() + to_delete_index);
    //         }
    //         else {
    //             group_indexes[box_index_to_group_index[min_index]].insert(min_index);
    //             count++;
    //         }
    //     }
    //     else {
    //         if(box_index_to_group_index.contains(min_index)) {
    //             group_indexes[box_index_to_group_index[min_index]].insert(i);
    //             count++;
    //             box_index_to_group_index.insert({i, box_index_to_group_index[min_index]});
    //         }
    //         else{
    //             group_indexes.push_back({i, min_index});
    //             box_index_to_group_index.insert({i, group_indexes.size() - 1});
    //             box_index_to_group_index.insert({min_index, group_indexes.size() - 1});
    //             count++;
    //             count++;
    //         }
    //     }
    // }

    // std::sort(group_indexes.begin(), group_indexes.end(), [](auto &group1, auto &group2){return group1.size() > group2.size();});
    // std::println("{}", group_indexes);

    // return group_indexes[0].size() * group_indexes[1].size() * group_indexes[2].size();
}

uint64_t sol2(const std::string& path){
    uint64_t sum = 0;
    return sum;
}

int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE, 10);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    assert(sol1_sample_result == SAMPLE_1);

    // const auto sol1_real_result = sol1(INPUT_FILE, 1000);
    // std::println("Solution1-Real  : {}", sol1_real_result);
    // std::println("");

    // const auto sol2_sample_result = sol2(SAMPLE_FILE);
    // std::println("Solution2-Sample: {}", sol2_sample_result);
    // assert(sol2_sample_result == SAMPLE_2);

    // const auto sol2_real_result = sol2(INPUT_FILE);
    // std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}