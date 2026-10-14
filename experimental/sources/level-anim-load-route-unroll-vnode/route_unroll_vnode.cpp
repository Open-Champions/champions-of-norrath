#include "../level-anim-load-common/mem.hpp"
// Observed routeUnrollVnode__Fi at 0x0031a2d8, 236 bytes (a weak symbol). Route nodes
// are 64 bytes apiece at nodes (0x00438f08). For node n, the signed halfword at +58 is
// its link; -2 means already unlinked and the routine returns with no stores. Otherwise
// the node is retired: halfword +56 takes the low half of the free-root word, +58
// becomes -2, the free-root word becomes n, the used-node word drops by one, and the
// count at +52 (signed halfword, read first) is cleared. For each of count halfwords at
// +12, +14, ...: the low 15 bits name a referenced node, that node's halfword at
// 36 + 2 * link is set to -1, and when the referenced node's own link equals this node's
// link the routine recurses on it.
// Inferred: +12 is a list of neighbouring node indexes with a flag in the top bit; the
// halfwords at +36 are per-link back references.
// Unknown: bounds; the image never checks an index against the 512 nodes, nor does this.
extern "C" {
extern unsigned char norrath_route_nodes_bytes[];
extern unsigned char norrath_route_node_used_bytes[];
extern unsigned char norrath_route_free_root_bytes[];
void norrath_route_unroll_vnode(int index);
}
extern "C" void norrath_route_unroll_vnode(int index) {
    using namespace norrath_lal;
    unsigned char* node = norrath_route_nodes_bytes + index * 64;
    const int link = load_signed_half(node + 58);
    if (link == -2) return;
    const int count = load_signed_half(node + 52);
    store_half(node + 56, load_half(norrath_route_free_root_bytes));
    store_half(node + 58, 0xfffeu);
    store_word(norrath_route_free_root_bytes, static_cast<unsigned>(index));
    store_word(norrath_route_node_used_bytes, load_word(norrath_route_node_used_bytes) - 1u);
    store_half(node + 52, 0);
    for (int remaining = count; remaining > 0; --remaining) {
        const int referenced = static_cast<int>(load_half(node + 12 + 2 * (count - remaining)) & 0x7fffu);
        unsigned char* other = norrath_route_nodes_bytes + referenced * 64;
        store_half(other + 36 + 2 * link, 0xffffu);
        if (load_signed_half(other + 58) == link) norrath_route_unroll_vnode(referenced);
    }
}
