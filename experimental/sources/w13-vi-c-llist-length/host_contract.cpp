// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "llist-length.cpp"

int main() {
    void* nodes[5][2];
    for (int n = 0; n <= 5; ++n) {
        for (int i = 0; i < 5; ++i) { nodes[i][0] = (i + 1 < n) ? static_cast<void*>(nodes[i + 1]) : 0; nodes[i][1] = 0; }
        void* header[2] = {n ? static_cast<void*>(nodes[0]) : 0, 0};
        void* list[1] = {header};
        if (Length__C7VILList(list) != n) return 1 + n;
    }
    return 0;
}
