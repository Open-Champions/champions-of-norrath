// Host contract for eventCollisionGround__11LavaTractor. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" void eventCollisionGround__11LavaTractor(void* object);
int main() {
    unsigned char object[64], before[64];
    for (int i = 0; i < 64; ++i) object[i] = before[i] = static_cast<unsigned char>(i * 5 + 3);
    eventCollisionGround__11LavaTractor(object);
    if (std::memcmp(object, before, 64) != 0) { std::fprintf(stderr, "fail: object changed\n"); return 1; }
    return 0;
}
