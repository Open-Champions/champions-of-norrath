#include "cd_start_stream.cpp"
#include <cstdio>
#include <cstring>
signed char streaming, powerOffMode;
char* streamOutput;
int streamSizeLeft;
char CDmode;
static int lookup_result, start_calls, syncs, pushes, pops, prints, fail_count;
static int start_sector[8];
static char* start_mode[8];
static unsigned push_color;
int cdDirectoryLookup__FPcPiT1i(char*, int* sector, int* size, int mode) {
    if (mode != 0 || streamOutput != nullptr || streamSizeLeft != 0) return 0;
    *sector = 1000;
    *size = 55;
    return lookup_result;
}
int scePrintf(const char* format, ...) { ++prints; return std::strcmp(format, "'%s' not found\n"); }
void enginePushBorderColor__FUi(unsigned color) { ++pushes; push_color = color; if (streaming != 1) fail_count += 100; }
void enginePopBorderColor__Fv() { ++pops; }
void engineSyncVBlank__Fv() { ++syncs; }
int sceCdStStart(int sector, char* mode) {
    start_sector[start_calls] = sector;
    start_mode[start_calls] = mode;
    return ++start_calls > fail_count;
}
int main() {
    char name[] = "file";
    streamOutput = reinterpret_cast<char*>(1);
    streamSizeLeft = 9;
    lookup_result = 0;
    cdStartStream__FPci(name, 0);
    if (prints != 1 || pushes != 0 || start_calls != 0 || streaming != 0) return 1;
    const int offsets[] = {0, 2047, 2048, 4096, -1, -2048, 0x12345678};
    for (int retries = 0; retries < 4; ++retries) {
        for (int offset : offsets) {
            lookup_result = 1;
            start_calls = syncs = pushes = pops = 0;
            fail_count = retries;
            streaming = 0;
            cdStartStream__FPci(name, offset);
            if (start_calls != retries + 1 || syncs != retries || pushes != 1 || pops != 1) return 2;
            if (push_color != 0x00ffffffu || streaming != 1) return 3;
            for (int i = 0; i <= retries; ++i)
                if (start_sector[i] != 1000 + (offset >> 11) || start_mode[i] != &CDmode) return 4;
        }
    }
    std::puts("cd_start_stream passed");
    return 0;
}
