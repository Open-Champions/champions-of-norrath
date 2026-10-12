#include <initializer_list>
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include "route_unroll_vnode.cpp"
#include "../level-anim-load-common/host_check.hpp"
unsigned char norrath_route_nodes_bytes[32768];
unsigned char norrath_route_node_used_bytes[4];
unsigned char norrath_route_free_root_bytes[4];
static unsigned char want[32768];
static void node_set(unsigned char* base, int n, int link, int count, std::initializer_list<unsigned> children) {
    unsigned char* p = base + n * 64;
    sh_(p + 58, link); sh_(p + 52, count);
    int i = 0; for (unsigned c : children) sh_(p + 12 + 2 * i++, c);
}
int main() {
    // Already unlinked: no stores at all.
    for (unsigned i = 0; i != 32768; ++i) norrath_route_nodes_bytes[i] = want[i] = (i * 13) & 0xff;
    node_set(norrath_route_nodes_bytes, 3, -2, 2, {4, 5}); node_set(want, 3, -2, 2, {4, 5});
    sw_(norrath_route_node_used_bytes, 10); sw_(norrath_route_free_root_bytes, 0x00070006);
    unsigned char used_want[4], root_want[4]; sw_(used_want, 10); sw_(root_want, 0x00070006);
    norrath_route_unroll_vnode(3);
    if (same("idle", norrath_route_nodes_bytes, want, 32768) || same("idle used", norrath_route_node_used_bytes, used_want, 4) ||
        same("idle root", norrath_route_free_root_bytes, root_want, 4)) return 1;
    // Node 3 (link 9) refers to nodes 4 (link 9, recursed), 5 (link 2, not recursed) and
    // 6 (link 9 but with the top bit set on the reference, still index 6).
    for (unsigned i = 0; i != 32768; ++i) norrath_route_nodes_bytes[i] = want[i] = (i * 13) & 0xff;
    node_set(norrath_route_nodes_bytes, 3, 9, 3, {4, 5, 0x8006});
    node_set(norrath_route_nodes_bytes, 4, 9, 1, {5});
    node_set(norrath_route_nodes_bytes, 5, 2, 0, {});
    node_set(norrath_route_nodes_bytes, 6, 9, 0, {});
    std::memcpy(want, norrath_route_nodes_bytes, 32768);
    sw_(norrath_route_node_used_bytes, 10); sw_(norrath_route_free_root_bytes, 0x00070006);
    // Expected, by hand: retire 3 (free root 6 -> half 0x0006... low half of the old root),
    // then reference 4 gets -1 at 36+18 and recurses; 4 retires, stores -1 into 5 at 36+18,
    // 5's link (2) differs. Reference 5 of node 3 then gets -1 at 36+18; link 2 != 9.
    // Reference 6 gets -1 at 36+18 and, link 9 == 9, is retired as well.
    sh_(want + 3 * 64 + 56, 6); sh_(want + 3 * 64 + 58, 0xfffe); sh_(want + 3 * 64 + 52, 0);
    sh_(want + 4 * 64 + 36 + 18, 0xffff);
    sh_(want + 4 * 64 + 56, 3); sh_(want + 4 * 64 + 58, 0xfffe); sh_(want + 4 * 64 + 52, 0);   // old root is node 3
    sh_(want + 5 * 64 + 36 + 18, 0xffff);
    sh_(want + 6 * 64 + 36 + 18, 0xffff);
    sh_(want + 6 * 64 + 56, 4); sh_(want + 6 * 64 + 58, 0xfffe); sh_(want + 6 * 64 + 52, 0);   // old root is node 4
    sw_(used_want, 7); sw_(root_want, 6);
    norrath_route_unroll_vnode(3);
    if (same("tree", norrath_route_nodes_bytes, want, 32768)) return 2;
    if (same("tree used", norrath_route_node_used_bytes, used_want, 4)) return 3;
    if (same("tree root", norrath_route_free_root_bytes, root_want, 4)) return 4;
    // Count zero or negative retires the node and visits no child.
    for (unsigned i = 0; i != 32768; ++i) norrath_route_nodes_bytes[i] = want[i] = (i * 5) & 0xff;
    node_set(norrath_route_nodes_bytes, 8, 1, -4, {9, 10}); std::memcpy(want, norrath_route_nodes_bytes, 32768);
    sw_(norrath_route_node_used_bytes, 1); sw_(norrath_route_free_root_bytes, 0xffffffff);
    sh_(want + 8 * 64 + 56, 0xffff); sh_(want + 8 * 64 + 58, 0xfffe); sh_(want + 8 * 64 + 52, 0);
    sw_(used_want, 0); sw_(root_want, 8);
    norrath_route_unroll_vnode(8);
    if (same("negative count", norrath_route_nodes_bytes, want, 32768) || same("used", norrath_route_node_used_bytes, used_want, 4) ||
        same("root", norrath_route_free_root_bytes, root_want, 4)) return 5;
    return 0;
}
