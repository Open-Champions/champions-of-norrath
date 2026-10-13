// Host contract for GothTrap msg_run; not an EE input. The object must come back unchanged.
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[64], copy[64];
    std::memset(object, 0x5a, sizeof object);
    std::memcpy(copy, object, sizeof object);
    msg_run__8GothTrap(object);
    if (std::memcmp(object, copy, sizeof object) != 0) { std::fputs("fail: object changed\n", stderr); return 1; }
    std::puts("gothtrap msg_run ok");
    return 0;
}
