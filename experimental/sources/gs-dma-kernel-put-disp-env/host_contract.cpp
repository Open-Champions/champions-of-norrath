#define NORRATH_HOST_CONTRACT 1
#include "put_disp_env.cpp"

#include <cstdio>
#include <cstring>

static unsigned char param[16];
static unsigned addresses[8];
static unsigned long long values[8];
static int stores;

extern "C" unsigned char* sceGsGetGParam() { return param; }
extern "C" void norrath_host_gs_store(unsigned address, unsigned long long value) {
    addresses[stores] = address;
    values[stores] = value;
    stores += 1;
}

static void set_qword(unsigned char* env, int offset, unsigned long long value) {
    for (int byte = 0; byte < 8; ++byte)
        env[offset + byte] = static_cast<unsigned char>(value >> (8 * byte));
}

int main() {
    unsigned char env[40];
    std::memset(env, 0, sizeof env);
    set_qword(env, 0, 0x11ull);
    set_qword(env, 8, 0x22ull);
    set_qword(env, 16, 0x33ull);
    set_qword(env, 24, 0x44ull);
    set_qword(env, 32, 0x55ull);
    std::memset(param, 0, sizeof param);
    param[6] = 3;
    stores = 0;
    sceGsPutDispEnv(env);
    if (stores != 5) return 1;
    const unsigned other[5] = {0x12000000u, 0x12000020u, 0x12000090u, 0x120000a0u, 0x120000e0u};
    const unsigned long long other_values[5] = {0x11ull, 0x22ull, 0x33ull, 0x44ull, 0x55ull};
    for (int index = 0; index < 5; ++index) {
        if (addresses[index] != other[index] || values[index] != other_values[index]) return 2;
    }
    param[6] = 1;
    stores = 0;
    sceGsPutDispEnv(env);
    if (stores != 4) return 3;
    const unsigned direct[4] = {0x12000000u, 0x12000070u, 0x12000080u, 0x120000c0u};
    const unsigned long long direct_values[4] = {0x11ull, 0x33ull, 0x44ull, 0x55ull};
    for (int index = 0; index < 4; ++index) {
        if (addresses[index] != direct[index] || values[index] != direct_values[index]) return 4;
    }
    std::puts("put display contract passed");
    return 0;
}
