#include "undo_browser.cpp"

#include <cstdio>

QuickLoadStampBytes undoQuickLoadStamp, quickLoadStamp;

int main() {
    for (int i = 0; i < 64; ++i) {
        undoQuickLoadStamp.bytes[i] = (unsigned char)(i * 7 + 1);
        quickLoadStamp.bytes[i] = (unsigned char)(255 - i);
    }
    undoBrowser__Fv();
    for (int i = 0; i < 64; ++i) {
        if (quickLoadStamp.bytes[i] != (unsigned char)(i * 7 + 1)) { std::printf("FAIL byte %d\n", i); return 1; }
        if (undoQuickLoadStamp.bytes[i] != (unsigned char)(i * 7 + 1)) { std::printf("FAIL source %d\n", i); return 1; }
    }
    std::puts("ok");
    return 0;
}
