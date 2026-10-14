#include "read_string_body.cpp"

#include <cstdio>
#include <cstring>

static int reads, seeks, last_read, last_origin;
static long long last_offset;
extern "C" int Read__6VIFilePvi(unsigned char*, void* buffer, int count) {
    ++reads;
    last_read = count;
    std::memset(buffer, 'x', count);
    return count;
}
extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(unsigned char*, long long offset, int origin) {
    ++seeks;
    last_offset = offset;
    last_origin = origin;
    return 0;
}

int main() {
    alignas(8) unsigned char f[640] = {};
    char buf[32];
    std::memset(buf, 'q', sizeof buf);
    *reinterpret_cast<int*>(f + 232) = 1;
    int* remaining = reinterpret_cast<int*>(f + 32 + 264);
    *remaining = 100;
    if (ReadStringBody__9VIObjFilePcUsUs(f, buf, 4, 10) != -1 || reads != 0) return 1;
    *reinterpret_cast<int*>(f + 584) = 1;
    *remaining = 3;
    if (ReadStringBody__9VIObjFilePcUsUs(f, buf, 4, 10) != -1 || reads != 0 || *remaining != 3) return 2;
    // Fits: capacity - 1 >= length.
    *remaining = 10;
    if (ReadStringBody__9VIObjFilePcUsUs(f, buf, 4, 5) != 0 || last_read != 4 || seeks != 0) return 3;
    if (buf[4] != 0 || buf[3] != 'x' || buf[5] != 'q' || *remaining != 6) return 4;
    // Does not fit.
    std::memset(buf, 'q', sizeof buf);
    *remaining = 20;
    if (ReadStringBody__9VIObjFilePcUsUs(f, buf, 9, 5) != 0 || last_read != 4 || seeks != 1) return 5;
    if (last_offset != 5 || last_origin != 0 || buf[4] != 0 || buf[3] != 'x' || buf[5] != 'q' || *remaining != 11) return 6;
    // Empty string.
    std::memset(buf, 'q', sizeof buf);
    if (ReadStringBody__9VIObjFilePcUsUs(f, buf, 0, 8) != 0 || buf[0] != 0 || buf[1] != 'q' || *remaining != 11) return 7;
    std::puts("objfile read-string-body passed");
    return 0;
}
