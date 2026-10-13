// Host contract for msg_save__7CritterPc; not an EE input. Includes the source.
#include "msg_save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    alignas(16) static unsigned char object[1100], before[1100];
    alignas(16) static char buffer[64], bufBefore[64];
    std::memset(object, 0x5a, sizeof object); std::memcpy(before, object, sizeof object);
    std::memset(buffer, 0x33, sizeof buffer); std::memcpy(bufBefore, buffer, sizeof buffer);
    int r = msg_save__7CritterPc(object, buffer);
    if (r != 0 || std::memcmp(object, before, sizeof object) != 0 || std::memcmp(buffer, bufBefore, sizeof buffer) != 0) {
        std::puts("fail"); return 1;
    }
    return 0;
}
