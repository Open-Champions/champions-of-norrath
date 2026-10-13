// Observed routeClear__Fv at 0x002bfbd8, 220 bytes. Resets the route node pool and the
// cluster cache. nodecount and nodeused are set to 0. Of the 512 nodes (64 bytes each, at
// the object recorded as norrath_route_nodes_bytes), every node gets the word at +60 set to
// -1, the halfwords at +36, +38, ... +50 (eight of them) set to -1, the halfword at +58 set
// to -2, and the halfword at +56 set to the next node's index, except node 511 whose +56
// halfword is -1. That chains the nodes into a free list. The word at offset 24 of each of
// the eight 32-byte cache slots (norrath_route_cache_bytes) is set to -2, and freeroot is
// set to 0. Nothing is returned.
// Inferred: -2 marks a free slot (routeAddNode tests a cache slot word against -2) and the +56
// halfword is the free-list link. Unknown: what the +60 word and node bytes 0..35 hold.
extern int nodecount;
extern "C" unsigned char norrath_route_nodes_bytes[];
extern "C" unsigned char norrath_route_cache_bytes[];
extern "C" unsigned char norrath_route_node_used_bytes[];
extern "C" unsigned char norrath_route_free_root_bytes[];

extern "C" void routeClear__Fv() {
    nodecount = 0;
    *reinterpret_cast<int*>(norrath_route_node_used_bytes) = 0;
    for (int i = 0; i < 512; ++i) {
        unsigned char* node = norrath_route_nodes_bytes + i * 64;
        *reinterpret_cast<int*>(node + 60) = -1;
        *reinterpret_cast<short*>(node + 56) = (i == 511) ? -1 : static_cast<short>(i + 1);
        *reinterpret_cast<short*>(node + 58) = -2;
        for (int j = 7; j >= 0; --j) *reinterpret_cast<short*>(node + 36 + 2 * j) = -1;
    }
    for (int j = 7; j >= 0; --j) *reinterpret_cast<int*>(norrath_route_cache_bytes + 32 * j + 24) = -2;
    *reinterpret_cast<int*>(norrath_route_free_root_bytes) = 0;
}
