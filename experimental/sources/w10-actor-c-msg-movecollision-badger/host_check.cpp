#include <cstdio>
#include <cstring>
extern "C" void msg_moveCollision__6BadgerPP10GameObject(void*, void**);
int main() {
    unsigned char object[600];
    unsigned char copy[600];
    void* slots[4] = {object, object + 1, nullptr, object + 3};
    void* slot_copy[4];
    std::memset(object, 0x6b, sizeof object);
    std::memcpy(copy, object, sizeof object);
    std::memcpy(slot_copy, slots, sizeof slots);
    msg_moveCollision__6BadgerPP10GameObject(object, slots);
    if (std::memcmp(copy, object, sizeof object) != 0 || std::memcmp(slot_copy, slots, sizeof slots) != 0) {
        std::fprintf(stderr, "fail: state changed\n");
        return 1;
    }
    return 0;
}
