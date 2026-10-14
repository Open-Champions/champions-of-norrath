#include "route_add_node.hpp"
#include <cstdio>
#include <cstring>
unsigned char norrath_route_nodes_bytes[32768];
unsigned char norrath_route_cache_bytes[256];
unsigned char norrath_route_node_used_bytes[4];
unsigned char norrath_route_free_root_bytes[4];
unsigned char norrath_route_world_word_bytes[4];
unsigned char norrath_route_draw_frame_bytes[4];
static int unrolled[8];
static int unroll_count;
static unsigned char seen_world[4];
static unsigned char seen_point[12];
static float seen_radius;
extern "C" void norrath_route_unroll_vnode(int index) {
    unrolled[unroll_count++] = index;
}
extern "C" float norrath_world_find_height(unsigned char* world, const unsigned char* point, float radius) {
    std::memcpy(seen_world, &world, 4);
    std::memcpy(seen_point, point, 12);
    seen_radius = radius;
    return 10.0f;
}
static void put_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned get_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void put_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
static unsigned get_half(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8);
}
static void put_float(unsigned char* p, float value) {
    unsigned bits = 0;
    std::memcpy(&bits, &value, 4);
    put_word(p, bits);
}
static int fail(int code) { std::printf("route add node failed %d\n", code); return code; }
int main() {
    unsigned char world_object[4] = {};
    put_word(norrath_route_world_word_bytes, static_cast<unsigned>(reinterpret_cast<unsigned long>(world_object)));
    put_word(norrath_route_node_used_bytes, 4);
    put_word(norrath_route_free_root_bytes, 2);
    put_half(norrath_route_nodes_bytes + 2 * 64 + 56, 5);
    unsigned char point[12];
    put_float(point, 1.0f);
    put_float(point + 4, 2.0f);
    put_float(point + 8, 3.0f);
    norrath_route_add_node(point);
    if (get_word(norrath_route_free_root_bytes) != 5) return fail(1);
    if (get_word(norrath_route_node_used_bytes) != 5) return fail(2);
    if (std::memcmp(norrath_route_nodes_bytes + 128, point, 8) != 0) return fail(3);
    float lifted = 0;
    std::memcpy(&lifted, norrath_route_nodes_bytes + 136, 4);
    if (lifted != 46.0f) return fail(4);
    if (get_half(norrath_route_nodes_bytes + 128 + 52) != 0) return fail(5);
    if (get_half(norrath_route_nodes_bytes + 128 + 58) != 0xffffu) return fail(6);
    if (seen_radius != 6.0f || std::memcmp(seen_point, point, 12) != 0) return fail(7);
    if (unroll_count != 0) return fail(8);

    std::memset(norrath_route_nodes_bytes, 0, sizeof norrath_route_nodes_bytes);
    std::memset(norrath_route_cache_bytes, 0, sizeof norrath_route_cache_bytes);
    put_word(norrath_route_draw_frame_bytes, 100);
    put_word(norrath_route_free_root_bytes, 0xffffffffu);
    put_word(norrath_route_node_used_bytes, 5);
    put_word(norrath_route_cache_bytes + 0, 1);
    put_word(norrath_route_cache_bytes + 4, 90);
    put_word(norrath_route_cache_bytes + 32, 2);
    put_word(norrath_route_cache_bytes + 36, 0);
    put_word(norrath_route_cache_bytes + 32 + 24, 0xfffffffeu);
    put_word(norrath_route_cache_bytes + 64, 3);
    put_word(norrath_route_cache_bytes + 68, 40);
    for (int index = 3; index != 8; ++index)
        put_word(norrath_route_cache_bytes + unsigned(index) * 32u + 24u, 0xfffffffeu);
    put_half(norrath_route_nodes_bytes + 3 * 64 + 58, 4);
    put_half(norrath_route_nodes_bytes + 3 * 64 + 52, 2);
    put_half(norrath_route_nodes_bytes + 3 * 64 + 12, 8);
    put_half(norrath_route_nodes_bytes + 3 * 64 + 14, 6);
    put_half(norrath_route_nodes_bytes + 8 * 64 + 58, 4);
    put_half(norrath_route_nodes_bytes + 6 * 64 + 58, 0);
    norrath_route_add_node(point);
    if (get_word(norrath_route_cache_bytes + 24) != 0u) return fail(9);
    if (get_word(norrath_route_cache_bytes + 64 + 24) != 0xfffffffeu) return fail(10);
    if (get_word(norrath_route_cache_bytes + 32 + 24) != 0xfffffffeu) return fail(11);
    if (get_half(norrath_route_nodes_bytes + 8 * 64 + 44) != 0xffffu) return fail(12);
    if (get_half(norrath_route_nodes_bytes + 6 * 64 + 44) != 0xffffu) return fail(13);
    if (unroll_count != 1 || unrolled[0] != 8) return fail(14);
    if (get_word(norrath_route_node_used_bytes) != 5) return fail(15);
    if (get_word(norrath_route_free_root_bytes) != 0xffffffffu) return fail(16);
    lifted = 0;
    std::memcpy(&lifted, norrath_route_nodes_bytes + 3 * 64 + 8, 4);
    if (lifted != 46.0f) return fail(17);
    std::puts("route add node passed");
    return 0;
}
