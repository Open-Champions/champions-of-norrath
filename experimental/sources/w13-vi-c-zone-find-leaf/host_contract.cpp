// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "zone-find-leaf.cpp"
#include <cstring>

struct Node { float plane[4]; unsigned front, back; };

int main() {
    // Node 0 splits on x (front: x >= 2), node 1 splits on y (front: y >= 0).
    Node nodes[2] = {
        {{1.0f, 0.0f, 0.0f, -2.0f}, 1u, 0x40000000u | 7u},
        {{0.0f, 1.0f, 0.0f, 0.0f}, 0x40000000u | 11u, 0x40000000u | 13u},
    };
    alignas(16) unsigned char zone[96] = {};
    const Node* base = nodes;
    std::memcpy(zone + 72, &base, sizeof base);
    float p1[3] = {1.0f, 5.0f, 0.0f};
    if (FindLeaf__C6VIZoneRC7VIVect3(zone, p1) != 0u) return 1;
    int count = 2;
    std::memcpy(zone + 68, &count, 4);
    if (FindLeaf__C6VIZoneRC7VIVect3(zone, p1) != 7u) return 2;
    float p2[3] = {3.0f, 1.0f, 0.0f};
    if (FindLeaf__C6VIZoneRC7VIVect3(zone, p2) != 11u) return 3;
    float p3[3] = {2.0f, -1.0f, 9.0f};
    if (FindLeaf__C6VIZoneRC7VIVect3(zone, p3) != 13u) return 4;
    std::memset(zone + 68, 0, 4);
    if (FindLeaf__C6VIZoneRC7VIVect3(zone, p3) != 0u) return 5;
    return 0;
}
