// Host contract for GetRomName. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// sceOpen, sceRead and sceClose are scripted. The contract records the path, flag and size, that a failed
// open reads and closes nothing, that a non-empty gRomName makes no call at all, and the returned address.
#include <cstdio>
#include <cstring>
extern "C" { char gRomName[15]; }
static int open_result, opens, reads, closes, last_flags, last_size, last_fd_read, last_fd_close;
static char last_path[32];
extern "C" int sceOpen(const char* p, int f) { ++opens; std::strcpy(last_path, p); last_flags = f; return open_result; }
extern "C" int sceRead(int fd, void* b, int n) { ++reads; last_fd_read = fd; last_size = n; std::memcpy(b, "ABCDTEFGHIJKLMN", n); return n; }
extern "C" int sceClose(int fd) { ++closes; last_fd_close = fd; return 0; }
#include "get_rom_name.cpp"
int main() {
    int bad = 0;
    open_result = 7;
    if (GetRomName() != gRomName) { std::puts("return"); ++bad; }
    if (opens != 1 || reads != 1 || closes != 1 || std::strcmp(last_path, "rom0:ROMVER") || last_flags != 1 ||
        last_size != 14 || last_fd_read != 7 || last_fd_close != 7 || std::memcmp(gRomName, "ABCDTEFGHIJKLM", 14) || gRomName[14] != 0) {
        std::puts("first call"); ++bad;
    }
    if (GetRomName() != gRomName || opens != 1 || reads != 1 || closes != 1) { std::puts("second call must do nothing"); ++bad; }
    std::memset(gRomName, 0, sizeof gRomName); opens = reads = closes = 0; open_result = -1;
    if (GetRomName() != gRomName || opens != 1 || reads != 0 || closes != 0 || gRomName[0] != 0) { std::puts("failed open"); ++bad; }
    open_result = 0;
    GetRomName();
    if (reads != 1 || closes != 1) { std::puts("descriptor 0 is valid"); ++bad; }
    return bad;
}
