#include "deci2_putchar.cpp"

#include <cstdio>
#include <cstring>

static char flushed[8][160];
static int flushes;

extern "C" void kputs(const char* text) {
    std::strcpy(flushed[flushes++], text);
}

int main() {
    deci2Putchar('a');
    deci2Putchar('b');
    if (flushes != 0 || count != 2) return 1;
    deci2Putchar('\n');
    if (flushes != 1 || std::strcmp(flushed[0], "ab\n") != 0 || count != 0) return 2;

    // A newline alone flushes just itself.
    deci2Putchar('\n');
    if (flushes != 2 || std::strcmp(flushed[1], "\n") != 0 || count != 0) return 3;

    // 126 characters fill the buffer without a flush; the 127th triggers one.
    for (int i = 0; i < 126; ++i) deci2Putchar('x');
    if (flushes != 2 || count != 126) return 4;
    deci2Putchar('y');
    if (flushes != 3 || count != 1) return 5;
    if (std::strlen(flushed[2]) != 126) return 6;
    for (int i = 0; i < 126; ++i)
        if (flushed[2][i] != 'x') return 7;
    if (linebuf[0] != 'y') return 8;

    // A newline arriving when the buffer is full flushes twice: the full buffer,
    // then the newline on its own.
    count = 0;
    flushes = 0;
    for (int i = 0; i < 126; ++i) deci2Putchar('z');
    deci2Putchar('\n');
    if (flushes != 2 || count != 0 || std::strcmp(flushed[1], "\n") != 0) return 9;
    std::puts("deci2Putchar passed");
    return 0;
}
