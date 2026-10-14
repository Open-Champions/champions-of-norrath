// Host contract for stun__6Playeri; not an EE input. Includes the source.
#include "stun.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    unsigned char object[64];
    std::memset(object, 0x3c, sizeof object);
    for (int v : {0, 1, -1, 100}) stun__6Playeri(object, v);
    for (unsigned char b : object) if (b != 0x3c) return 1;
    return 0;
}
