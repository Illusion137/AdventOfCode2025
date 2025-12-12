#include "../utils.hpp"
#include <cstdint>
#include <memory>
#include <unordered_map>

#define SAMPLE_1 5
#define SAMPLE_2 2

#define YOU_SERVER 0
#define OUT_SERVER 1

struct RawServer {
    uint64_t id;
    std::vector<uint64_t> attached_ids;
};

struct Server {
    uint64_t id;
    std::vector<std::shared_ptr<Server>> attached_servers;
};

typedef std::shared_ptr<Server> ServerRack;

constexpr uint64_t parse_id(const std::string &strid){
    if(strid == "you") return YOU_SERVER;
    if(strid == "out") return OUT_SERVER;
    return strid[0] + (strid[1] << 16) + (strid[2] << 8);
}
static constinit uint64_t dac_id = parse_id("dac");
static constinit uint64_t fft_id = parse_id("fft");

ServerRack get_server_rack(const std::string &path, const std::string initial_pos = "you"){
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
        raw_servers.emplace_back(parse_id(id_string), std::move(attached_servers));
    }

    std::shared_ptr<Server> out_server = std::make_shared<Server>(OUT_SERVER);
    std::unordered_map<uint64_t, std::shared_ptr<Server>> server_id_map;
    std::vector<std::shared_ptr<Server>> all_servers;

    server_id_map.insert({OUT_SERVER, out_server});
    all_servers.emplace_back(out_server);

    for(const auto &raw_server: raw_servers){
        auto server = std::make_shared<Server>(raw_server.id);
        all_servers.emplace_back(server);
        server_id_map.insert({raw_server.id, server});
    }
    for(const auto &raw_server: raw_servers){
        auto server = server_id_map[raw_server.id];
        for(const auto &attached_id: raw_server.attached_ids){
            server->attached_servers.emplace_back(server_id_map[attached_id]);
        }
    }
    const auto initial_id = parse_id(initial_pos);
    for(const auto &server: all_servers){
        if(server->id == initial_id) return server;
    }

    return nullptr;
}

uint32_t sol1_dp(const std::shared_ptr<Server> &server_ptr){
    if(server_ptr->id == OUT_SERVER) return 1;
    uint32_t sum = 0;
    for(uint32_t i = 0; i < server_ptr->attached_servers.size(); i++){
        sum += sol1_dp(server_ptr->attached_servers[i]);
    }
    return sum;
}

uint64_t sol1(const std::string &path){
    const auto server_rack = get_server_rack(path);
    return sol1_dp(server_rack);
}

std::unordered_map<__uint128_t, uint64_t> sol2_cache;
uint64_t sol2_dp(const std::shared_ptr<Server> &server_ptr, bool found_dac, bool found_fft){
    const __uint128_t cache_key = ((__uint128_t)server_ptr.get() << 64)| ((__uint128_t)found_dac << 32) | (__uint128_t)found_fft;
    if(sol2_cache.contains(cache_key)) return sol2_cache[cache_key];
    if(server_ptr->id == OUT_SERVER && found_dac && found_fft) return 1;
    else if(server_ptr->id == OUT_SERVER) return 0;
    uint64_t sum = 0;
    for(uint64_t i = 0; i < server_ptr->attached_servers.size(); i++){
        sum += sol2_dp(server_ptr->attached_servers[i], 
            found_dac || server_ptr->attached_servers[i]->id == dac_id, 
            found_fft || server_ptr->attached_servers[i]->id == fft_id);
    }
    sol2_cache.insert({cache_key, sum});
    return sum;
}

uint64_t sol2(const std::string &path){
    const auto server_rack = get_server_rack(path, "svr");
    sol2_cache.clear();
    return sol2_dp(server_rack, false, false);
}

int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    sassert(sol1_sample_result, SAMPLE_1);

    const auto sol1_real_result = sol1(INPUT_FILE);
    std::println("Solution1-Real  : {}", sol1_real_result);
    std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE2);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    sassert(sol2_sample_result, SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}