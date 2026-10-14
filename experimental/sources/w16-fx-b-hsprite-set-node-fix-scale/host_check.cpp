// Host check only; not a build input.
#include "set_node_fix_scale.cpp"
#include <cstring>
int main() {
    alignas(8) static char nodes[256 * 4]; std::memset(nodes, 0, sizeof nodes);
    alignas(8) char obj[128]; std::memset(obj, 0, sizeof obj);
    *(char**)(obj + 72) = nodes;
    float v[3] = {1.5f, -2.0f, 3.25f};
    if (SetNodeFixScale__9VIHSpriteiRC7VIVect3(obj, 2, v) != 0) return 1;
    if (std::memcmp(nodes + 512 + 112, v, 12) != 0) return 2;
    for (int i = 0; i < 256 * 4; i++) if ((i < 624 || i >= 636) && nodes[i] != 0) return 3;
    return 0;
}
