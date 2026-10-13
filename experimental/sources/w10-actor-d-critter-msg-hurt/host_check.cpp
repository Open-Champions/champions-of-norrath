// Host contract for msg_hurt__7CritterfP10GameObjecti; not an EE input. Includes the source.
#include "msg_hurt.cpp"
#include <cstdio>
#include <cstring>
int main() {
    alignas(16) static unsigned char object[1100], before[1100];
    std::memset(object, 0x5a, sizeof object); std::memcpy(before, object, sizeof object);
    msg_hurt__7CritterfP10GameObjecti(object, 3.5f, object + 8, 7);
    if (std::memcmp(object, before, sizeof object) != 0) { std::puts("fail: wrote the object"); return 1; }
    return 0;
}
