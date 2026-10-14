#include <cstdio>
#include <cstdlib>
extern "C" int SCE_CD_debug = 0;
extern "C" int stm_status = 1;
extern "C" unsigned char dum_mode[4] = {};
static int flushes;
static int flush_bytes;
static int delays;
static int calls;
static unsigned script[4];
static unsigned seen_count[4];
static void* seen_buffer[4];
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    if (address == nullptr) std::_Exit(30);
    ++flushes;
    flush_bytes = bytes;
}
extern "C" void DelayThread(int microseconds) {
    if (microseconds != 600) std::_Exit(31);
    ++delays;
}
extern "C" int norrath_sce_printf(const char*, ...) { return 0; }
extern "C" unsigned sceCdStream(int argument, int count, void* buffer, int command, void* mode) {
    if (argument != 0 || command != 2 || mode != dum_mode) std::_Exit(32);
    seen_count[calls] = static_cast<unsigned>(count);
    seen_buffer[calls] = buffer;
    return script[calls++];
}
extern "C" int norrath_archive_cd_stream_read(unsigned, void*, int, int*);
int main() {
    unsigned char buffer[8192] = {};
    int error = 99;
    stm_status = 0;
    if (norrath_archive_cd_stream_read(2, buffer, 0, &error) != 0 || error != 99 || flushes != 0) return 1;
    stm_status = 1;
    script[0] = (0x3u << 16) | 2u;
    if (norrath_archive_cd_stream_read(2, buffer, 0, &error) != 2 || error != 3) return 2;
    if (flushes != 1 || flush_bytes != 4096 || calls != 1 || seen_count[0] != 2u) return 3;
    calls = 0;
    delays = 0;
    error = 7;
    script[0] = 0;
    script[1] = 2u;
    if (norrath_archive_cd_stream_read(2, buffer, 1, &error) != 2 || error != 0 || delays != 1) return 4;
    if (seen_count[0] != 2u || seen_buffer[0] != buffer || seen_count[1] != 2u) return 5;
    calls = 0;
    delays = 0;
    script[0] = 1u;
    script[1] = 1u;
    if (norrath_archive_cd_stream_read(2, buffer, 1, &error) != 2 || error != 0 || delays != 0) return 6;
    if (seen_buffer[1] != buffer + 2048 || seen_count[1] != 1u) return 7;
    calls = 0;
    script[0] = 0x11u << 16;
    if (norrath_archive_cd_stream_read(4, buffer, 1, &error) != 0 || error != 0x11 || delays != 0) return 8;
    std::puts("stream read contract passed");
    return 0;
}
