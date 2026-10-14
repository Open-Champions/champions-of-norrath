// Host check for norrath_player_proxy_mesh_id. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "proxy_mesh_id.cpp"
#include <cstring>
extern "C" { unsigned char* norrath_player_host_base; }
int main() {
    alignas(16) static unsigned char arena[1024];
    norrath_player_host_base = arena;
    // description at arena+256, record at arena+512
    unsigned char* d = arena + 256;
    unsigned char* r = arena + 512;
    *reinterpret_cast<unsigned*>(d) = 512;
    short bases[] = {0, 7, 105, -3, -32768, 32767};
    for (short base : bases) {
        *reinterpret_cast<short*>(r + 52) = base;
        for (int level = 0; level < 32; ++level) {
            unsigned long long flags = (unsigned long long)level << 11;
            flags |= 0xfffffffffffff7ffull & 0x0ull;  // no other bits
            *reinterpret_cast<unsigned long long*>(d + 8) = flags;
            if (norrath_player_proxy_mesh_id(d) != base) return 1;  // flag clear: level ignored
            *reinterpret_cast<unsigned long long*>(d + 8) = flags | (1ull << 36);
            if (norrath_player_proxy_mesh_id(d) != base + level) return 2;
            // other high bits do not change the result
            *reinterpret_cast<unsigned long long*>(d + 8) = flags | (1ull << 36) | (1ull << 39) | (1ull << 35) | (1ull << 37) | 0x7ffull | (1ull << 16);
            if (norrath_player_proxy_mesh_id(d) != base + level) return 3;
            *reinterpret_cast<unsigned long long*>(d + 8) = flags | (1ull << 39) | (1ull << 35) | (1ull << 37) | 0x7ffull | (1ull << 16);
            if (norrath_player_proxy_mesh_id(d) != base) return 4;
        }
    }
    return 0;
}
