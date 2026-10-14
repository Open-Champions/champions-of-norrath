// Host check only; not a build input.
#include "add_segment.cpp"
#include <cstring>
#include <string>
alignas(8) static unsigned char trail[128];
alignas(8) static unsigned char nodeBlock[96];
static int emptyReturns;
static std::string trace;
extern "C" void* RemHead__7VILList(void* list) {
    trace += "R" + std::to_string(static_cast<unsigned char*>(list) - trail) + ";";
    if (emptyReturns > 0) { --emptyReturns; return nullptr; }
    return nodeBlock;
}
extern "C" void AddHead__7VILListP7VILNode(void* list, void* node) {
    trace += "A" + std::to_string(static_cast<unsigned char*>(list) - trail) + (node == nodeBlock ? "n;" : "?;");
}
extern "C" void ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(void* self, unsigned segment) {
    trace += "C" + std::to_string(segment) + (self == trail ? "t;" : "?;");
}
int main() {
    unsigned a[3] = {1, 2, 3}, b[3] = {4, 5, 6}, c[4] = {7, 8, 9, 10}, d[4] = {11, 12, 13, 14};
    std::memset(nodeBlock, 0xee, sizeof nodeBlock);
    *reinterpret_cast<unsigned*>(trail + 36) = 777;
    emptyReturns = 2;
    if (AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3(trail, a, b, c, d) != 0) return 1;
    if (trace != "R40;C777t;R40;C777t;R40;A28n;") return 2;
    const unsigned* w = reinterpret_cast<const unsigned*>(nodeBlock);
    for (int i = 0; i < 3; ++i) if (w[4 + i] != a[i] || w[7 + i] != b[i]) return 3;
    for (int i = 0; i < 4; ++i) if (w[10 + i] != c[i] || w[14 + i] != d[i]) return 4;
    if (w[18] != 0 || w[0] != 0xeeeeeeeeu || w[3] != 0xeeeeeeeeu || w[19] != 0xeeeeeeeeu) return 5;
    return 0;
}
