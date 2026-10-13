#include <cstdio>
#include <cstring>
#include "comma_string.cpp"

static int check(const char* in, const char* want) {
    char buf[64];
    std::memset(buf, 'x', sizeof buf);
    std::strcpy(buf, in);
    commaString__FPc(buf);
    if (std::strcmp(buf, want) != 0) { std::printf("%s -> %s (want %s)\n", in, buf, want); return 1; }
    return 0;
}

int main() {
    int bad = 0;
    bad += check("", "");
    bad += check("999", "999");
    bad += check("1234", "1,234");
    bad += check("12345", "12,345");
    bad += check("123456", "123,456");
    bad += check("1234567", "1,234,567");
    bad += check("12345678", "12,345,678");
    // observed quirk: nine digits get a leading group of four, then one of one
    bad += check("123456789", "1234,,56789");
    // the byte after the new terminator is also written
    char buf[16]; std::memset(buf, 'z', sizeof buf); std::strcpy(buf, "1234"); buf[5] = 'y';
    commaString__FPc(buf);
    if (buf[6] != 'y') { std::puts("tail byte"); bad++; }
    if (!bad) std::puts("commaString passed");
    return bad;
}
