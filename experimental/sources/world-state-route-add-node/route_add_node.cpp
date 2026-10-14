#include "route_add_node.hpp"
// Observed routeAddNode at 0x002bfcb8, 572 bytes. A free-root of -1 first
// retires the cluster-cache entry whose frame stamp is furthest behind
// drawFrameNm. The retired node's successor becomes the free root, then the
// same call allocates that node and replaces its Z with height plus 36.
namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned read_half_raw(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8);
}
void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
int signed_half(unsigned value) {
    return value < 0x8000u ? int(value) : int(value) - 65536;
}
int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? int(value) : -1 - int(0xffffffffu - value);
}
unsigned char* node_at(unsigned index) {
    return norrath_route_nodes_bytes + index * 64u;
}
}
extern "C" void norrath_route_add_node(const unsigned char* point) {
    unsigned char saved[12];
    for (unsigned i = 0; i != 12u; ++i) saved[i] = point[i];
    while (signed_word(read_word(norrath_route_free_root_bytes)) == -1) {
        int best = -1;
        int chosen = -1;
        for (int index = 0; index != 8; ++index) {
            unsigned char* slot = norrath_route_cache_bytes + unsigned(index) * 32u + 4u;
            if (signed_word(read_word(slot + 20)) == -2) continue;
            const unsigned age = read_word(norrath_route_draw_frame_bytes) - read_word(slot);
            if (best < signed_word(age)) {
                best = signed_word(age);
                chosen = index;
            }
        }
        // A failed search leaves chosen at -1. The original then addresses
        // clustercache-32. That predecessor is outside this object.
        unsigned char* entry = norrath_route_cache_bytes + unsigned(chosen) * 32u;
        const unsigned retired = read_word(entry);
        unsigned char* retired_node = node_at(retired);
        const int link = signed_half(read_half_raw(retired_node + 58));
        if (link != -2) {
            const int count = signed_half(read_half_raw(retired_node + 52));
            const unsigned previous = read_half_raw(norrath_route_free_root_bytes);
            const unsigned used = read_word(norrath_route_node_used_bytes) - 1u;
            write_half(retired_node + 58, 0xfffeu);
            write_half(retired_node + 56, previous);
            write_word(norrath_route_node_used_bytes, used);
            write_half(retired_node + 52, 0u);
            write_word(norrath_route_free_root_bytes, retired);
            if (count > 0) {
                unsigned char* cursor = retired_node + 12;
                int remaining = count;
                do {
                    const unsigned referenced = read_half_raw(cursor) & 0x7fffu;
                    unsigned char* referenced_node = node_at(referenced);
                    write_half(referenced_node + 36 + 2 * link, 0xffffu);
                    const int referenced_link = signed_half(read_half_raw(referenced_node + 58));
                    cursor += 2;
                    if (referenced_link == link) norrath_route_unroll_vnode(int(referenced));
                    remaining -= 1;
                } while (remaining != 0);
            }
        }
        write_word(entry + 24, 0xfffffffeu);
    }
    const unsigned taken = read_word(norrath_route_free_root_bytes);
    unsigned char* node = node_at(taken);
    for (unsigned i = 0; i != 12u; ++i) node[i] = saved[i];
    const int successor = signed_half(read_half_raw(node + 56));
    write_word(norrath_route_node_used_bytes, read_word(norrath_route_node_used_bytes) + 1u);
    write_half(node + 58, 0xffffu);
    write_word(norrath_route_free_root_bytes, static_cast<unsigned>(successor));
    write_half(node + 52, 0u);
    unsigned char* world = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(
        read_word(norrath_route_world_word_bytes)));
    const float height = norrath_world_find_height(world, saved, 6.0f);
    const float lifted = height + 36.0f;
    const unsigned char* lifted_bytes = reinterpret_cast<const unsigned char*>(&lifted);
    node[8] = lifted_bytes[0];
    node[9] = lifted_bytes[1];
    node[10] = lifted_bytes[2];
    node[11] = lifted_bytes[3];
}
