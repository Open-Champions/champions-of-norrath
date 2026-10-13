// Host contract for routeClear__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>
int nodecount = 9;
extern "C" {
alignas(8) unsigned char norrath_route_nodes_bytes[32768];
alignas(8) unsigned char norrath_route_cache_bytes[256];
unsigned char norrath_route_node_used_bytes[4] = {5, 0, 0, 0};
unsigned char norrath_route_free_root_bytes[4] = {7, 0, 0, 0};
void routeClear__Fv();
}
static short h(const unsigned char* p) { short v; std::memcpy(&v, p, 2); return v; }
static int w(const unsigned char* p) { int v; std::memcpy(&v, p, 4); return v; }
int main() {
    std::memset(norrath_route_nodes_bytes, 0x11, sizeof norrath_route_nodes_bytes);
    std::memset(norrath_route_cache_bytes, 0x22, sizeof norrath_route_cache_bytes);
    routeClear__Fv();
    if (nodecount != 0 || w(norrath_route_node_used_bytes) != 0 || w(norrath_route_free_root_bytes) != 0) return 1;
    for (int i = 0; i != 512; ++i) {
        const unsigned char* n = norrath_route_nodes_bytes + 64 * i;
        if (h(n + 56) != (i == 511 ? -1 : i + 1) || h(n + 58) != -2 || w(n + 60) != -1) return 2;
        for (int j = 0; j != 8; ++j) if (h(n + 36 + 2 * j) != -1) return 3;
        if (n[0] != 0x11 || n[35] != 0x11 || n[52] != 0x11 || n[55] != 0x11) return 4;  // other bytes untouched
    }
    for (int j = 0; j != 8; ++j) {
        const unsigned char* c = norrath_route_cache_bytes + 32 * j;
        if (w(c + 24) != -2 || c[4] != 0x22 || c[31] != 0x22) return 5;
    }
    std::printf("route clear contract passed\n");
    return 0;
}
