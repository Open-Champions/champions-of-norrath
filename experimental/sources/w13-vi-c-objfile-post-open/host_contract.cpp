// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "objfile-post-open.cpp"
#include <cstring>

static int log_[32], n;
static long long args_[32];
static long long tell_value;
static int magic_to_set;
static unsigned char* file;
static void put(int off, int v) { std::memcpy(file + off, &v, 4); }
static int get(int off) { int v; std::memcpy(&v, file + off, 4); return v; }
extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(void*, long long off, int origin) { args_[n] = off * 10 + origin; log_[n++] = 1; return 0; }
extern "C" long long Tell__6VIFile(void*) { log_[n++] = 2; return tell_value; }
extern "C" int WriteFileHeader__9VIObjFilei(void*, int a) { log_[n++] = 3; return 0; }
extern "C" int ReadFileHeader__9VIObjFilei(void*, int a) { log_[n++] = 4; put(200, magic_to_set); return 0; }
extern "C" int AbortOpen__9VIObjFile(void*) { log_[n++] = 5; return 0; }

int main() {
    alignas(8) unsigned char buf[640];
    file = buf;
    // create
    std::memset(buf, 0x77, sizeof buf); put(192, 2); n = 0;
    if (PostOpen__9VIObjFile(buf) != 0 || n != 1 || log_[0] != 3) return 1;
    long long v; std::memcpy(&v, buf + 216, 8); if (v != 32) return 2;
    std::memcpy(&v, buf + 224, 8); if (v != -1) return 3;
    if (get(200) != 0x4f424a46 || get(204) || get(208) || get(212)) return 4;
    if (get(564) != 1 || get(568) != 0 || get(580) || get(584) || get(588)) return 5;
    // too short
    std::memset(buf, 0x77, sizeof buf); put(192, 0); n = 0; tell_value = 31;
    if (PostOpen__9VIObjFile(buf) != -1 || n != 3 || log_[2] != 5) return 6;
    if (get(564) != 0x77777777) return 7;
    // bad magic
    tell_value = 32; magic_to_set = 1; n = 0;
    if (PostOpen__9VIObjFile(buf) != -2 || n != 4 || log_[2] != 4 || log_[3] != 5) return 8;
    // mode 1: seek to end again
    std::memset(buf, 0x77, sizeof buf); put(192, 1); magic_to_set = 0x4f424a46; n = 0;
    if (PostOpen__9VIObjFile(buf) != 0 || n != 4 || log_[3] != 1 || args_[3] != 2) return 9;
    if (get(564) != 1 || get(568) != 0) return 10;
    // mode 0, cursor already at start: no extra seek
    std::memset(buf, 0x77, sizeof buf); put(192, 0); tell_value = 100; n = 0;
    long long start = 100; std::memcpy(buf + 216, &start, 8);
    if (PostOpen__9VIObjFile(buf) != 0 || n != 4) return 11;
    if (get(564) != 0 || get(568) != 1) return 12;
    // mode 0, cursor elsewhere: relative seek by the start value
    std::memset(buf, 0x77, sizeof buf); put(192, 0); tell_value = 100; n = 0;
    start = 64; std::memcpy(buf + 216, &start, 8);
    if (PostOpen__9VIObjFile(buf) != 0 || n != 5 || log_[4] != 1 || args_[4] != 641) return 13;
    return 0;
}
