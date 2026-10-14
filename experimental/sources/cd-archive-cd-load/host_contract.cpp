#include <cstdio>
#include <cstdlib>
#include <cstring>
extern "C" unsigned char cdBusy = 0;
extern "C" unsigned cdResult = 0;
extern "C" unsigned char command[16] = {};
extern "C" const unsigned char norrath_archive_load_failure_format_view[] = "'%s' not found\n";
static int waits = 0;
static int lookups = 0;
static int lookup_mode = -1;
static int prints = 0;
static const char* printed_name = nullptr;
static int issues = 0;
static int processes = 0;
static int syncs = 0;
static int pushes = 0;
static int pops = 0;
static unsigned stored_address = 99;
static unsigned stored_value = 99;
static int stores = 0;
static int clear_busy_on_process = 0;
extern "C" void norrath_host_store_absolute(unsigned address, unsigned value) {
    stored_address = address;
    stored_value = value;
    stores += 1;
}
extern "C" void cdWait__Fv() { waits += 1; }
extern "C" int cdDirectoryLookup__FPcPiT1i(char*, int* sector, int* bytes, int mode) {
    lookups += 1;
    lookup_mode = mode;
    *sector = 40;
    *bytes = 3000;
    return lookups == 1 ? 0 : 1;
}
extern "C" int scePrintf(const char* format, ...) {
    prints += 1;
    printed_name = format;
    return 0;
}
extern "C" void issueCommand__Fv() {
    issues += 1;
    if (command[0] != 1) std::_Exit(50);
}
extern "C" void cdProcess__Fv() {
    processes += 1;
    if (clear_busy_on_process != 0) cdBusy = 0;
}
extern "C" void enginePushBorderColor__FUi(unsigned color) {
    pushes += 1;
    if (color != 0x00ffffffu) std::_Exit(51);
}
extern "C" void engineSyncVBlank__Fv() { syncs += 1; }
extern "C" void enginePopBorderColor__Fv() { pops += 1; }
extern "C" void cdLoad__FPcPvii(char*, void*, int, int);
int main() {
    unsigned char destination[4] = {};
    cdLoad__FPcPvii(const_cast<char*>("missing.bin"), destination, 0, 0);
    if (waits != 1 || lookups != 1 || lookup_mode != 0 || prints != 1) return 1;
    if (std::strcmp(printed_name, "'%s' not found\n") != 0) return 2;
    if (stores != 1 || stored_address != 0u || stored_value != 2u) return 3;
    if (issues != 0 || cdBusy != 0 || pushes != 1 || processes != 0 || pops != 1) return 4;
    clear_busy_on_process = 1;
    cdLoad__FPcPvii(const_cast<char*>("srs.bin"), destination, 0, 2048);
    if (issues != 1 || processes != 1 || syncs != 1 || cdBusy != 0) return 5;
    unsigned sector = 0;
    unsigned blocks = 0;
    unsigned dest = 0;
    std::memcpy(&sector, command + 4, 4);
    std::memcpy(&blocks, command + 8, 4);
    std::memcpy(&dest, command + 12, 4);
    if (sector != 41u) return 6;
    if (blocks != ((3000u + 2047u) >> 11)) return 7;
    if (dest != static_cast<unsigned>(reinterpret_cast<unsigned long>(destination))) return 8;
    cdLoad__FPcPvii(const_cast<char*>("srs.bin"), destination, 100, -2048);
    std::memcpy(&sector, command + 4, 4);
    std::memcpy(&blocks, command + 8, 4);
    if (sector != 39u) return 9;
    if (blocks != ((100u + 2047u) >> 11)) return 10;
    if (pushes != 3 || pops != 3) return 11;
    std::puts("cd load contract passed");
    return 0;
}
