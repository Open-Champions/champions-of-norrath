#include "get_dialog_size.cpp"
#include <cstdio>
#include <cstring>
static unsigned char table[80 * 3];
static unsigned char* found = nullptr;
static char* seen = nullptr;
extern "C" unsigned char* soundFindDialog__FPc(char* name) { seen = name; return found; }
int main() {
    char name[] = "x";
    if (soundGetDialogSize__FPc(name) != 0 || seen != name) return 1;
    int w0 = 1000, w1 = 1640, w2 = 99;
    std::memcpy(table + 64, &w0, 4);
    std::memcpy(table + 80 + 64, &w1, 4);
    std::memcpy(table + 160 + 64, &w2, 4);
    found = table;
    if (soundGetDialogSize__FPc(name) != 640) return 2;
    found = table + 80;
    if (soundGetDialogSize__FPc(name) != 99 - 1640) return 3;
    std::puts("get_dialog_size passed");
    return 0;
}
