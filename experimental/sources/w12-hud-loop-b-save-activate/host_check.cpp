#include "save_activate.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>

int main() {
    int bad = 0;
    // The word at +204 is set, its neighbours and all other bytes stay as filled.
    alignas(8) unsigned char obj[256];
    for (int fill : {0x00, 0xa5, 0xff}) {
        std::memset(obj, fill, sizeof obj);
        saveActivate__9Savepoint(obj);
        int word;
        std::memcpy(&word, obj + 204, 4);
        bad |= word != 1;
        for (int i = 0; i < 256; ++i)
            if (!(i >= 204 && i < 208) && obj[i] != fill) bad = 1;
    }
    std::printf(bad ? "FAIL\n" : "ok: word at +204 becomes 1\n");
    return bad;
}
