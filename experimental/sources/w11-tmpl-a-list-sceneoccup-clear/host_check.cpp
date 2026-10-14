#include "clear.cpp"
#include <cstring>
#include <vector>
static unsigned nodes[4][48];
static std::vector<int> trace;
static int idx(void* p) { for (int i = 0; i < 4; ++i) if (p == nodes[i]) return i; return -1; }
extern "C" void __builtin_delete(void* p) { trace.push_back(2000); trace.push_back(idx(p)); }
int main() {
    unsigned list[4];
    auto u = [](unsigned* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); };
    for (int n = 0; n <= 3; ++n) {
        std::memset(nodes, 0x5a, sizeof nodes);
        std::memset(list, 0x77, sizeof list);
        trace.clear();
        list[0] = n; list[1] = n ? u(nodes[0]) : 0;
        for (int i = 0; i < n; ++i) nodes[i][6] = (i + 1 < n) ? u(nodes[i + 1]) : 0;
        if (Clear__t6VIList1ZQ27VIScene12VISceneOccup(list) != 0) return 1;
        if (list[0] != 0 || list[1] != 0 || list[2] != 0 || list[3] != 0x77777777u) return 2;
        std::vector<int> want;
        for (int i = 0; i < n; ++i) {
            
            want.push_back(2000); want.push_back(i);
        }
        if (trace != want) return 3;
    }
    return 0;
}
