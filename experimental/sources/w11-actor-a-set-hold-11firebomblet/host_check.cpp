// Host contract for setHold__11FireBombleti; not an EE input. Returns non-zero when wrong.
#include "set_hold.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    alignas(16) static unsigned char object[512];
    int bad = 0;
    for (int v : {0, 1, 0x7f, 0x80, 0xff, 0x1234, -1}) {
        std::memset(object, 0x5a, sizeof object);
        setHold__11FireBombleti(object, v);
        if (object[156] != static_cast<unsigned char>(v)) bad = 1;
        for (int i = 0; i < 512; ++i)
            if (i != 156 && object[i] != 0x5a) bad = 1;
    }
    return bad;
}
