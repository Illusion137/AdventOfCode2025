#include "../utils.hpp"

#define SAMPLE_1 5
#define SAMPLE_2 -1

#define YOU_SERVER 0
#define OUT_SERVER 1

struct RawServer {
    uint64_t id;
    std::vector<uint64_t> attached_ids;
};

struct Server {
    uint64_t id;
    std::vector<Server*> attached_servers;
};

typedef Server* ServerRack;

uint64_t parse_id(const std::string &strid){
    if(strid == "you") return YOU_SERVER;
    if(strid == "out") return OUT_SERVER;
    return strid[0] + (strid[1] << 16) + (strid[2] << 8);
}

ServerRack get_server_rack(const std::string &path){
    const auto lines = read_file_lines(path);
    std::vector<RawServer> raw_servers;
    raw_servers.reserve(lines.size());
    for(const auto &line: lines){
        const auto line_split = split_string(line, ": ");
        const auto id_string = line_split[0];
        const auto attached_ids_string = line_split[1];
        const auto attached_ids_split = split_string(attached_ids_string, " ");
        std::vector<uint64_t> attached_servers;
        attached_servers.reserve(attached_ids_split.size());
        for(const auto &attached_ids_split_string: attached_ids_split){
            attached_servers.emplace_back(parse_id(attached_ids_split_string));
        }
        raw_servers.emplace_back(parse_id(line), std::move(attached_servers));
    }    

    return;
}

uint64_t sol1(const std::string &path){
    const auto server_rack = get_server_rack(path);
    uint64_t sum = 0;
    return sum;
}

uint64_t sol2(const std::string &path){
    uint64_t sum = 0;
    return sum;
}

int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    sassert(sol1_sample_result, SAMPLE_1);

    const auto sol1_real_result = sol1(INPUT_FILE);
    std::println("Solution1-Real  : {}", sol1_real_result);
    std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    sassert(sol2_sample_result, SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}