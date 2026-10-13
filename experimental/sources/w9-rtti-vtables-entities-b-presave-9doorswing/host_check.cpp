#include <cstdio>
#include <cstring>

#include "presave.cpp"

int main() {
    alignas(8) unsigned char object[128];
    std::memset(object, 0x5a, sizeof object);
    unsigned char copy[128];
    std::memcpy(copy, object, sizeof object);
    msg_presave__9DoorSwing(object);
    msg_presave__9DoorSwing(nullptr);
    if (std::memcmp(copy, object, sizeof object) != 0) {
        std::fprintf(stderr, "fail: object changed\n");
        return 1;
    }
    return 0;
}
