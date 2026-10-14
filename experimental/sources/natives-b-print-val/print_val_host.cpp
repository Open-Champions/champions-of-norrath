#include <cstdio>
extern "C" int amxPrintVal__FP5__amxPi(void*, int*);

int main() {
    int cells[] = {4, 99};
    if (amxPrintVal__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (amxPrintVal__FP5__amxPi(cells, cells) != 0) return 2;
    if (cells[1] != 99) return 3;
    std::puts("amxPrintVal passed");
    return 0;
}
