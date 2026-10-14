#include "read_begin_at.cpp"

#include <cstdio>

static int seeks, begins, seek_index_seen, begin_result;
static long long seek_offset;
static int seek_origin;
static unsigned short *seen_t, *seen_v;
static unsigned long long* seen_position;
static unsigned* seen_size;
static unsigned char* seek_file;

extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(unsigned char* file, long long offset, int origin) {
    ++seeks;
    seek_file = file;
    seek_offset = offset;
    seek_origin = origin;
    seek_index_seen = *reinterpret_cast<int*>(file + 232);
    return 0;
}
extern "C" int ReadBegin__9VIObjFileRUsT1RUlRUi(unsigned char*, unsigned short* t, unsigned short* v, unsigned long long* position, unsigned* size) {
    ++begins;
    seen_t = t; seen_v = v; seen_position = position; seen_size = size;
    return begin_result;
}

int main() {
    alignas(8) unsigned char f[640] = {};
    unsigned short t = 0, v = 0;
    *reinterpret_cast<int*>(f + 232) = 4;
    if (ReadBegin__9VIObjFileUlRUsT2(f, 100, &t, &v) != -1 || seeks != 0) return 1;
    *reinterpret_cast<int*>(f + 568) = 1;
    *reinterpret_cast<int*>(f + 572) = 1;
    if (ReadBegin__9VIObjFileUlRUsT2(f, 100, &t, &v) != -1 || seeks != 0) return 2;
    *reinterpret_cast<int*>(f + 572) = 0;
    *reinterpret_cast<int*>(f + 576) = 1;
    if (ReadBegin__9VIObjFileUlRUsT2(f, 100, &t, &v) != -1 || seeks != 0 || *reinterpret_cast<int*>(f + 232) != 4) return 3;
    *reinterpret_cast<int*>(f + 576) = 0;
    begin_result = 7;
    if (ReadBegin__9VIObjFileUlRUsT2(f, 0x123456789ull, &t, &v) != 0) return 4;
    if (seeks != 1 || seek_offset != 0x123456789ll || seek_origin != 1 || seek_file != f) return 5;
    if (*reinterpret_cast<int*>(f + 232) != -1 || begins != 1 || seek_index_seen != -1) return 6;
    if (seen_t != &t || seen_v != &v || seen_position == nullptr || seen_size == nullptr) return 7;
    begin_result = -2;
    if (ReadBegin__9VIObjFileUlRUsT2(f, 5, &t, &v) != -1) return 8;
    begin_result = 0;
    if (ReadBegin__9VIObjFileUlRUsT2(f, 5, &t, &v) != 0) return 9;
    std::puts("objfile read-begin-at passed");
    return 0;
}
