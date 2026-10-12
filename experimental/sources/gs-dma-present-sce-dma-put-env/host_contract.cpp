#define NORRATH_HOST_CONTRACT 1
#include "sce_dma_put_env.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" unsigned char sceDmaCurrentEnv[80] = {};
static unsigned ctrl;
static std::vector<std::string> loads;
static std::vector<std::pair<unsigned, unsigned>> stores;

extern "C" unsigned norrath_host_dma_env_load32(unsigned address) {
    loads.push_back(address == 0x1000e000u ? "e000" :
                    address == 0x1000e020u ? "e020" :
                    address == 0x1000e030u ? "e030" :
                    address == 0x1000e040u ? "e040" :
                    address == 0x1000e050u ? "e050" : "?");
    return address == 0x1000e000u ? ctrl : 0x111u;
}
extern "C" void norrath_host_dma_env_store32(unsigned address, unsigned value) {
    stores.push_back({address, value});
}

static void reset(unsigned control) {
    ctrl = control;
    loads.clear();
    stores.clear();
    std::memset(sceDmaCurrentEnv, 0x5a, sizeof sceDmaCurrentEnv);
}

static void fill(unsigned char* env) {
    std::memset(env, 0, 20);
    env[0] = 0;
    env[1] = 1;
    env[2] = 2;
    env[3] = 1;
    env[4] = 0x11;
    env[5] = 0x11;
    env[6] = 0x22;
    env[7] = 0x22;
    env[8] = 0x33;
    env[9] = 0x33;
    env[10] = 0x44;
    env[11] = 0x44;
    env[12] = 0x55;
    env[13] = 0x55;
    env[14] = 0x55;
    env[15] = 0x55;
    env[16] = 0x66;
    env[17] = 0x66;
    env[18] = 0x66;
    env[19] = 0x66;
}

int main() {
    unsigned char env[20];
    fill(env);
    reset(0xffffffffu);
    if (sceDmaPutEnv(env) != 0) return 1;
    if (loads.size() != 5 || loads[0] != "e000" || loads[4] != "e040") return 2;
    if (stores.size() != 5) return 3;
    if (stores[0].first != 0x1000e000u || stores[0].second != 0xfffffc4fu) return 4;
    if (stores[1].second != 0x11112222u || stores[2].second != 0x44443333u) return 5;
    if (stores[3].first != 0x1000e050u || stores[3].second != 0x55555555u) return 6;
    if (stores[4].first != 0x1000e040u || stores[4].second != 0x66666666u) return 7;
    if (std::memcmp(sceDmaCurrentEnv, env, 20) != 0 || sceDmaCurrentEnv[20] != 0x5a) return 8;

    env[3] = 0;
    reset(0xffffffffu);
    if (sceDmaPutEnv(env) != 0 || stores[0].second != 0xffffff4du) return 9;

    fill(env);
    env[0] = 10;
    reset(1u);
    if (sceDmaPutEnv(env) != -1 || !stores.empty() || loads.size() != 5) return 10;
    env[0] = 0;
    env[1] = 10;
    if (sceDmaPutEnv(env) != -2) return 11;
    env[1] = 0;
    env[2] = 10;
    if (sceDmaPutEnv(env) != -3) return 12;
    env[2] = 0;
    env[3] = 7;
    if (sceDmaPutEnv(env) != -4) return 13;
    std::puts("dma put env contract passed");
    return 0;
}
