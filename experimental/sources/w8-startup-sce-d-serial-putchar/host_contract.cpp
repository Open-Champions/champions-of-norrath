#include "serial_putchar.cpp"

#include <cstdio>

static int log[8];
static int n;
extern "C" int kputchar(int c) {
    if (n < 8) log[n] = c;
    n += 1;
    return 100 + c;
}

int main() {
    if (serialPutchar('A') != 100 + 'A' || n != 1 || log[0] != 'A') return 1;
    n = 0;
    if (serialPutchar(10) != 110 || n != 2 || log[0] != 13 || log[1] != 10) return 2;
    n = 0;
    if (serialPutchar(13) != 113 || n != 1 || log[0] != 13) return 3;
    n = 0;
    if (serialPutchar(0) != 100 || n != 1) return 4;
    std::puts("serial putchar contract passed");
    return 0;
}
