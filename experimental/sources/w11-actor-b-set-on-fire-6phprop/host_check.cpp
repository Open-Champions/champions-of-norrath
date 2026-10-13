// Host contract for setOnFire__6PhPropi; not an EE input. Includes the source.
#include "set_on_fire.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    for (int v : {0, 1, 2, 255, 256, 257, -1, 0x12345678}) {
        unsigned char object[600];
        std::memset(object, 0x5a, sizeof object);
        setOnFire__6PhPropi(object, v);
        if (object[570] != static_cast<unsigned char>(v)) return 1;
        for (int i = 0; i < 600; ++i) if (i != 570 && object[i] != 0x5a) return 2;
    }
    return 0;
}
