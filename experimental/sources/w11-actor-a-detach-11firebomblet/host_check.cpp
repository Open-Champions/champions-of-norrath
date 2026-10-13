// Host contract for detach__11FireBomblet; not an EE input. Returns non-zero when wrong.
#include "detach.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    alignas(16) static unsigned char object[512];
    int bad = 0;
    for (unsigned v : {0u, 1u, 0xdeadbeefu}) {
        std::memset(object, 0x5a, sizeof object);
        std::memcpy(object + 172, &v, 4);
        detach__11FireBomblet(object);
        unsigned w; std::memcpy(&w, object + 172, 4);
        if (w != 0) bad = 1;
        for (int i = 0; i < 512; ++i)
            if ((i < 172 || i >= 176) && v != 0 && object[i] != 0x5a) bad = 1;
    }
    return bad;
}
