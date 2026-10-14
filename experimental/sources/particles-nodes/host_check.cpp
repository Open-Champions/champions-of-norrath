// Host contract for nodes. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char nodes[6096 * 20];
int main() {
    if (sizeof nodes != 121920u) return 1;
    for (unsigned i = 0; i < sizeof nodes; ++i) {
        if (nodes[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "nodes", i);
            return 1;
        }
    }
    nodes[0] = 1;
    if (nodes[0] != 1) return 1;
    return 0;
}
