#include <cstdio>
#include <cstring>
#include "game_object_save.cpp"
int main() {
    unsigned char object[16];
    char buffer[8];
    std::memset(object, 0x33, sizeof object);
    std::memset(buffer, 0x44, sizeof buffer);
    if (msg_save__10GameObjectPc(object, buffer) != 0) return 1;
    for (unsigned char b : object) if (b != 0x33) return 2;
    for (char b : buffer) if (b != 0x44) return 3;
    if (msg_save__10GameObjectPc(nullptr, nullptr) != 0) return 4;
    std::puts("ok");
    return 0;
}
