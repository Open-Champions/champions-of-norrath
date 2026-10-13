#include <stdint.h>
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<uintptr_t>(word))
#include "buffer_flush.cpp"
#include <string.h>
#include <string>

static std::string tr; static int seek_ret, write_ret;
extern "C" int sceLseek(int fd, int off, int wh) { tr += "S" + std::to_string(fd) + "," + std::to_string(off) + "," + std::to_string(wh) + ";"; return seek_ret; }
extern "C" int sceWrite(int fd, const void* p, int n) { tr += "W" + std::to_string(fd) + "," + std::to_string((uintptr_t)p) + "," + std::to_string(n) + ";"; return write_ret; }

#define Q(off) (*(long long*)(f + (off)))
int main() {
    alignas(8) unsigned char f[256];
    auto fresh = [&] {
        memset(f, 0x77, sizeof f); tr.clear();
        *(int*)f = 5; Q(24) = 1000; Q(136) = 900; Q(144) = 100; Q(152) = 140; Q(176) = 1; *(int*)(f + 188) = 0x4000;
    };
    fresh(); Q(176) = 0;
    if (BufferFlush__6VIFile(f) != 0 || !tr.empty() || Q(152) != 140) return 1;
    fresh(); Q(152) = 100;
    if (BufferFlush__6VIFile(f) != 0 || !tr.empty() || Q(144) != 100) return 2;
    // position already at the target: no seek
    fresh(); write_ret = 40;
    if (BufferFlush__6VIFile(f) != 0 || tr != "W5,16484,40;") return 3;
    if (Q(24) != 1040 || Q(144) != 0 || Q(152) != 0) return 4;
    // position elsewhere: seek then write
    fresh(); Q(24) = 7; seek_ret = 1000; write_ret = 40;
    if (BufferFlush__6VIFile(f) != 0 || tr != "S5,1000,0;W5,16484,40;") return 5;
    if (Q(24) != 1040) return 6;
    // short seek result: stored, error, no write
    fresh(); Q(24) = 7; seek_ret = -1;
    if (BufferFlush__6VIFile(f) != -1 || tr != "S5,1000,0;" || Q(24) != -1 || Q(152) != 140) return 7;
    // short write: position advances, dirty range kept
    fresh(); write_ret = 10;
    if (BufferFlush__6VIFile(f) != -1 || Q(24) != 1010 || Q(144) != 100 || Q(152) != 140) return 8;
    fresh(); write_ret = -1;
    if (BufferFlush__6VIFile(f) != -1 || Q(24) != 999) return 9;
    // only the low word of the dirty start is added to the buffer address
    fresh(); Q(144) = 0x100000000ll + 100; Q(152) = 0x100000000ll + 140; Q(136) = 900 - 0x100000000ll; Q(24) = 1000; write_ret = 40;
    if (BufferFlush__6VIFile(f) != 0 || tr != "W5,16484,40;") return 10;
    return 0;
}
