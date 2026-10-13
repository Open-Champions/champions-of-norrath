// Host contract for getFreeFrameState__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
// Random rings of 2 to 6 nodes are built in the arena and compared with an independent
// model that keeps the ring as a vector of node numbers.
#define NORRATH_HOST_ARENA
#include <vector>
#include <cstdlib>
#include "../w10-world-b-common/host_arena.hpp"
#include "../w10-world-b-common/host_check.hpp"
unsigned char* fsList;
unsigned char* fsListBack;
#include "get_free_frame_state.cpp"
static const unsigned NODE = 0x100, OWNER = 0x2000;
static unsigned char* node(int i) { return arena_at(0x400 + NODE * i); }
static unsigned char* owner(int i) { return arena_at(OWNER + 0x100 * i); }
int main() {
    std::srand(7);
    int busy_runs = 0, moved_total = 0;
    for (int round = 0; round != 3000; ++round) {
        const int n = 2 + std::rand() % 5;
        std::memset(arena_at(0), 0xa5, 0x4000);
        std::vector<int> ring;            // front .. last
        for (int i = 0; i != n; ++i) ring.push_back(i);
        for (int i = n - 1; i > 0; --i) std::swap(ring[i], ring[std::rand() % (i + 1)]);
        std::vector<int> has_owner(n), busy(n);
        for (int i = 0; i != n; ++i) {
            has_owner[i] = std::rand() % 4 != 0;
            busy[i] = has_owner[i] && std::rand() % 3 != 0;
            sw_(node(i) + 8, 0x1111 + i);
            sw_(node(i) + 20, has_owner[i] ? norrath_host_address(owner(i)) : 0);
            sw_(node(i) + 32, 0x32); sw_(node(i) + 36, 0x36);
            sw_(owner(i) + 4, 0xabc);
            unsigned flags = busy[i] ? (std::rand() % 2 ? 0x4000 : 0x0800) : (std::rand() & 0xb7ff);
            if (busy[i]) flags |= std::rand() % 2 ? 0x00ff : 0;
            sh_(owner(i) + 28, flags);
            busy[i] = has_owner[i] && (flags & 0x4800) != 0;
        }
        for (int k = 0; k != n; ++k) {
            sw_(node(ring[k]) + 12, k ? norrath_host_address(node(ring[k - 1])) : 0);
            sw_(node(ring[k]) + 16, k + 1 != n ? norrath_host_address(node(ring[k + 1])) : 0);
        }
        // Skip rings the image would never leave: every node busy (the front would cycle
        // forever), or a lone busy front with no other node to move.
        bool all_busy = true; for (int i = 0; i != n; ++i) if (!busy[i]) all_busy = false;
        if (all_busy) continue;
        fsListBack = node(ring.front()); fsList = node(ring.back());
        // Model.
        std::vector<int> m = ring;
        int moved = 0;
        while (has_owner[m.front()] && busy[m.front()]) { m.push_back(m.front()); m.erase(m.begin()); ++moved; }
        int chosen = m.front();
        m.erase(m.begin()); m.push_back(chosen);
        moved_total += moved; busy_runs += moved != 0;
        unsigned char* got = getFreeFrameState__Fv();
        if (got != node(chosen)) { std::printf("round %d: wrong node\n", round); return 1; }
        // The list from fsListBack, following +16, equals the model; +12 links match.
        unsigned char* p = fsListBack; unsigned char* prev = nullptr;
        for (size_t k = 0; k != m.size(); ++k) {
            if (p != node(m[k])) { std::printf("round %d: order at %zu\n", round, k); return 2; }
            if (hw(p + 12) != (prev ? norrath_host_address(prev) : 0)) { std::printf("round %d: prev at %zu\n", round, k); return 3; }
            prev = p; p = norrath_wb::load_pointer(p + 16);
        }
        if (p != nullptr || fsList != node(m.back())) { std::printf("round %d: tail\n", round); return 4; }
        if (hw(node(chosen) + 16) != 0 || hw(node(chosen) + 20) != 0 || hw(node(chosen) + 8) != 1000000u ||
            hw(node(chosen) + 32) != 0 || hw(node(chosen) + 36) != 0) { std::printf("round %d: fields\n", round); return 5; }
        // Only the chosen node's owner lost its +4 word.
        for (int i = 0; i != n; ++i) {
            const unsigned want = (i == chosen && has_owner[i]) ? 0 : 0xabc;
            if (hw(owner(i) + 4) != want) { std::printf("round %d: owner %d\n", round, i); return 6; }
            if (i != chosen && hw(node(i) + 8) != 0x1111u + i) return 7;
        }
    }
    if (busy_runs < 100 || moved_total < 100) return 8;
    std::printf("getFreeFrameState check passed (%d rings moved nodes)\n", busy_runs);
    return 0;
}
