#include "dir_entry_is_quick_load.cpp"

#include <cstdio>
#include <cstring>

unsigned char quickLoadStamp[64];

int main() {
    unsigned char e[64];
    for (int i = 0; i < 64; ++i) quickLoadStamp[i] = static_cast<unsigned char>(i * 7 + 1);
    std::strcpy(reinterpret_cast<char*>(quickLoadStamp + 32), "BASLUS-20565quick");
    std::memcpy(e, quickLoadStamp, 64);
    if (dirEntryIsQuickLoad__FP14sceMcTblGetDir(e) != 1) return 1;
    e[0] ^= 0xff;  // byte 0 is not compared
    if (dirEntryIsQuickLoad__FP14sceMcTblGetDir(e) != 1) return 2;
    e[20] ^= 0xff;  // neither is anything between 8 and 31
    if (dirEntryIsQuickLoad__FP14sceMcTblGetDir(e) != 1) return 3;
    for (int b = 1; b < 8; ++b) {
        e[b] ^= 1;
        if (dirEntryIsQuickLoad__FP14sceMcTblGetDir(e) != 0) return 10 + b;
        e[b] ^= 1;
    }
    e[40] ^= 1;
    if (dirEntryIsQuickLoad__FP14sceMcTblGetDir(e) != 0) return 20;
    std::puts("dir entry is quick load passed");
    return 0;
}
