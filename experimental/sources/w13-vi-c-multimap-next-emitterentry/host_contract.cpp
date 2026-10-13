// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "multimap-next-emitterentry.cpp"
#include <cstring>
#include <vector>

struct Entry { int key, parent, left, right; };

int main() {
    // A search tree over keys inserted in a fixed scrambled order; entry 0 is the null node.
    const int order[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 65, 90, 5};
    const int n = sizeof order / sizeof order[0];
    std::vector<Entry> e(n + 1);
    std::vector<unsigned char> table((n + 1) * 112, 0xcc);
    e[0] = Entry{0, 0, 0, 0};
    for (int i = 1; i <= n; ++i) {
        e[i] = Entry{order[i - 1], 0, 0, 0};
        if (i == 1) continue;
        int at = 1;
        for (;;) {
            int& slot = e[i].key < e[at].key ? e[at].left : e[at].right;
            if (slot == 0) { slot = i; e[i].parent = at; break; }
            at = slot;
        }
    }
    for (int i = 0; i <= n; ++i) {
        std::memcpy(&table[i * 112 + 4], &e[i].parent, 4);
        std::memcpy(&table[i * 112 + 8], &e[i].left, 4);
        std::memcpy(&table[i * 112 + 12], &e[i].right, 4);
    }
    alignas(16) unsigned char map[32] = {};
    const unsigned char* base = table.data();
    std::memcpy(map + 8, &base, sizeof base);
    // In-order walk from the smallest key must produce keys in increasing order and end at zero.
    int node = 1;
    while (e[node].left) node = e[node].left;
    int last = -1, count = 0;
    while (node) {
        if (e[node].key <= last) return 1;
        last = e[node].key;
        ++count;
        node = Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(map, node);
    }
    if (count != n) return 2;
    return 0;
}
