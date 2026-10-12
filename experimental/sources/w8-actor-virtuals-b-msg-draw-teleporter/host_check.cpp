#include <cstdio>
#include <cstring>

#include "msg_draw.cpp"

int main() {
    unsigned char object[512];
    unsigned char before[512];
    std::memset(object, 0x6c, sizeof object);
    std::memcpy(before, object, sizeof object);
    msg_draw__10Teleporter(object);
    if (std::memcmp(object, before, sizeof object) != 0) {
        std::fprintf(stderr, "fail: draw modified the object\n");
        return 1;
    }
    msg_draw__10Teleporter(nullptr);  // the argument is never read
    return 0;
}
