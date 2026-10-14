#include <cstdio>
#include <cstring>
#include "st_read.cpp"
extern "C" int SCE_CD_debug = 0;
extern "C" int stm_status = 1;
extern "C" unsigned char dum_mode[40] = {};
static unsigned script[16]; static int script_len, script_pos, stream_calls, delays, writebacks, prints, bad;
static unsigned counts[16]; static unsigned char* dests[16]; static unsigned char* wb_addr; static int wb_bytes;
extern "C" unsigned sceCdStream(unsigned argument, unsigned count, void* buffer, unsigned command, const unsigned char* mode) {
    if (argument != 0 || command != 2 || mode != dum_mode) bad = 1;
    counts[stream_calls] = count; dests[stream_calls] = static_cast<unsigned char*>(buffer);
    ++stream_calls;
    return script_pos < script_len ? script[script_pos++] : 0;
}
extern "C" void sceSifWriteBackDCache(void* address, int bytes) { ++writebacks; wb_addr = static_cast<unsigned char*>(address); wb_bytes = bytes; }
extern "C" void DelayThread(int us) { if (us != 600) bad = 1; ++delays; }
extern "C" int scePrintf(const char*, ...) { ++prints; return 0; }
static void reset() { script_len = script_pos = stream_calls = delays = writebacks = prints = bad = 0; }
int main() {
    static unsigned char dest[16 << 11];
    int error = -1;
    // Idle stream: nothing happens, error pointer untouched.
    stm_status = 0; reset();
    if (sceCdStRead(4, dest, 0, &error) != 0 || error != -1 || stream_calls != 0 || writebacks != 0) return 1;
    stm_status = 1;
    // Mode 0: one call, result split into count and error.
    reset(); script[0] = 0x00050003u; script_len = 1;
    if (sceCdStRead(4, dest, 0, &error) != 3 || error != 5 || stream_calls != 1 || counts[0] != 4 || dests[0] != dest) return 2;
    if (writebacks != 1 || wb_addr != dest || wb_bytes != (4 << 11) || bad) return 3;
    // Mode 1, all blocks at once.
    reset(); script[0] = 4; script_len = 1; error = -1;
    if (sceCdStRead(4, dest, 1, &error) != 4 || error != 0 || stream_calls != 1) return 4;
    // Mode 1, partial reads advance the destination and the remaining count.
    reset(); script[0] = 1; script[1] = 2; script[2] = 3; script_len = 3;
    if (sceCdStRead(6, dest, 1, &error) != 6 || stream_calls != 3 || error != 0) return 5;
    if (counts[0] != 6 || counts[1] != 5 || counts[2] != 3 || dests[1] != dest + 2048 || dests[2] != dest + 3 * 2048) return 6;
    // Mode 1, zero blocks without an error waits 600 us and retries.
    reset(); script[0] = 0; script[1] = 0; script[2] = 2; script_len = 3;
    if (sceCdStRead(2, dest, 1, &error) != 2 || delays != 2 || stream_calls != 3 || bad) return 7;
    // Error with progress continues; the last error is reported. Error with no progress stops.
    reset(); script[0] = 0x00070001u; script[1] = 0x00080000u; script_len = 2;
    if (sceCdStRead(4, dest, 1, &error) != 1 || error != 8 || stream_calls != 2 || delays != 0) return 8;
    reset(); script[0] = 0x00070001u; script[1] = 0x00000003u; script_len = 2;
    if (sceCdStRead(4, dest, 2, &error) != 4 || error != 7 || stream_calls != 2) return 9;
    // Debug output: entry, one error line, ended; none when off.
    reset(); SCE_CD_debug = 1; script[0] = 0x00070001u; script[1] = 0x00000003u; script_len = 2;
    sceCdStRead(4, dest, 1, &error);
    if (prints != 3) return 10;
    reset(); SCE_CD_debug = 0; script[0] = 4; script_len = 1;
    sceCdStRead(4, dest, 1, &error);
    if (prints != 0) return 11;
    std::puts("sceCdStRead contract passed");
    return 0;
}
