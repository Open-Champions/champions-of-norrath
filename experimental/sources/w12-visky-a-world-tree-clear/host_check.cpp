// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "clear.cpp"
#include <string>

static std::string trace;
static int node_result, leaf_result;
extern "C" {
int Clear__t7VIArray1Z11VIWorldNode(unsigned char* a) { trace += "node:" + std::to_string(reinterpret_cast<unsigned long>(a) % 1000) + ";"; return node_result; }
int Clear__t7VIArray1Z11VIWorldLeaf(unsigned char* a) { trace += "leaf:" + std::to_string(reinterpret_cast<unsigned long>(a) % 1000) + ";"; return leaf_result; }
}

int main() {
    unsigned char* tree = reinterpret_cast<unsigned char*>(0x10000);
    const std::string base = std::to_string(0x10000 % 1000);
    const std::string leaf = std::to_string((0x10000 + 8) % 1000);
    node_result = 0; leaf_result = 0; trace.clear();
    if (Clear__11VIWorldTree(tree) != 0 || trace != "node:" + base + ";leaf:" + leaf + ";") return 1;
    node_result = 5; leaf_result = 9; trace.clear();
    if (Clear__11VIWorldTree(tree) != 0) return 2;
    node_result = -3; trace.clear();
    if (Clear__11VIWorldTree(tree) != -1 || trace != "node:" + base + ";") return 3;
    node_result = 0; leaf_result = -2; trace.clear();
    if (Clear__11VIWorldTree(tree) != -1 || trace != "node:" + base + ";leaf:" + leaf + ";") return 4;
    return 0;
}
