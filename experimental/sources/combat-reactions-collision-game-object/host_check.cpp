#include "collision.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[64];
    unsigned char other[64];
    unsigned char before[64];
    std::memset(object, 0x5a, sizeof object);
    std::memset(other, 0xa5, sizeof other);
    std::memcpy(before, object, sizeof object);
    msg_collision__10GameObjectP10GameObject(object, other);
    if (std::memcmp(before, object, sizeof object) != 0) { std::fprintf(stderr, "fail: object changed\n"); return 1; }
    for (unsigned i = 0; i < sizeof other; ++i) if (other[i] != 0xa5) { std::fprintf(stderr, "fail: other changed\n"); return 1; }
    return 0;
}
