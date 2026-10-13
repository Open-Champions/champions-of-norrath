// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "init.cpp"
#include <string>

static std::string trace;
static int node_result, leaf_result;
extern "C" {
int Init__t7VIArray1Z11VIWorldNodei(unsigned char* a, int n) { trace += "node:" + std::to_string(reinterpret_cast<unsigned long>(a) % 1000) + "," + std::to_string(n) + ";"; return node_result; }
int Init__t7VIArray1Z11VIWorldLeafi(unsigned char* a, int n) { trace += "leaf:" + std::to_string(reinterpret_cast<unsigned long>(a) % 1000) + "," + std::to_string(n) + ";"; return leaf_result; }
}

int main() {
    unsigned char* tree = reinterpret_cast<unsigned char*>(0x10000);
    const std::string base = std::to_string(0x10000 % 1000);
    const std::string leaf = std::to_string((0x10000 + 8) % 1000);
    node_result = 0; leaf_result = 0; trace.clear();
    if (Init__11VIWorldTreeii(tree, 11, 22) != 0 || trace != "node:" + base + ",11;leaf:" + leaf + ",22;") return 1;
    node_result = 5; leaf_result = 9; trace.clear();
    if (Init__11VIWorldTreeii(tree, 3, 4) != 0) return 2;  // positive results count as success and the result is 0
    node_result = -3; trace.clear();
    if (Init__11VIWorldTreeii(tree, 3, 4) != -1 || trace != "node:" + base + ",3;") return 3;
    node_result = 0; leaf_result = -2; trace.clear();
    if (Init__11VIWorldTreeii(tree, 3, 4) != -1 || trace != "node:" + base + ",3;leaf:" + leaf + ",4;") return 4;
    return 0;
}
