#include "prepare.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

char sProductId[32] = "BASLUS-20470EQONLINE";
unsigned char sIconDefinition[1100];

static std::vector<std::string> calls;
static int status_result, prepared_result, pad_result;
static std::vector<int> sync_results;
static unsigned sync_index;
static const void* write_pointers[8];
static int write_sizes[8];
static unsigned write_count;

extern "C" int VIMemCardStatus__FPi(int* out) { calls.push_back(out ? "status!" : "status"); return status_result; }
extern "C" int VIMemCardIsPrepared__Fv() { calls.push_back("isprep"); return prepared_result; }
extern "C" int VIMemCardPad__Fi(int n) { calls.push_back("pad" + std::to_string(n)); return pad_result; }
extern "C" int printf(const char* f, ...) {
    if (std::strcmp(f, "prepare result: %d\n") != 0) calls.push_back("badprintf");
    calls.push_back("printf");
    return 0;
}
extern "C" int sceMcMkdir(int p, int s, const char* n) { calls.push_back(std::string("mkdir ") + n + (p || s ? "!" : "")); return 0; }
extern "C" int sceMcOpen(int p, int s, const char* n, int m) { calls.push_back(std::string("open ") + n + (p || s || m != 514 ? "!" : "")); return 0; }
extern "C" int sceMcWrite(int fd, const void* b, int n) {
    calls.push_back("write" + std::to_string(fd) + "_" + std::to_string(n));
    if (write_count < 8) { write_pointers[write_count] = b; write_sizes[write_count] = n; ++write_count; }
    if (n == 8) calls.back() += std::string("_") + static_cast<const char*>(b);
    return 0;
}
extern "C" int sceMcFlush(int fd) { calls.push_back("flush" + std::to_string(fd)); return 0; }
extern "C" int sceMcClose(int fd) { calls.push_back("close" + std::to_string(fd)); return 0; }
extern "C" int sceMcSync(int mode, int*, int* result) {
    calls.push_back("sync" + std::to_string(mode));
    if (sync_index < sync_results.size()) *result = sync_results[sync_index];
    ++sync_index;
    return 1;
}

static int run(int status, int prepared, std::vector<int> syncs, int pad, int pad_value = 0) {
    calls.clear();
    status_result = status;
    prepared_result = prepared;
    pad_result = pad_value;
    sync_results = syncs;
    sync_index = 0;
    write_count = 0;
    std::memset(sIconDefinition, 0, sizeof sIconDefinition);
    std::strcpy(reinterpret_cast<char*>(sIconDefinition) + 260, "list.ico");
    unsigned address = 0x00123456;
    std::memcpy(sIconDefinition + 964, &address, 4);
    int size = 12;
    std::memcpy(sIconDefinition + 968, &size, 4);
    return VIMemCardPrepare__Fi(pad);
}

int main() {
    if (run(2, 0, {}, 0) != -1 || calls.size() != 1 || calls[0] != "status") return 1;
    if (run(0, -4, {}, 0) != -1 || calls.back() != "isprep") return 2;
    if (run(0, 1, {}, 0) != 0 || calls.size() != 2) return 3;
    // Full success with prepared == 0: handles 5, 6, 8.
    const std::vector<int> good = {0, 5, 0, 0, 6, 0, 0, 8, 0, 0};
    if (run(0, 0, good, 0) != 0) return 4;
    if (calls[2] != "mkdir /BASLUS-20470EQONLINE" || calls[5] != "open /BASLUS-20470EQONLINE/icon.sys") return 5;
    if (calls[7] != "write5_964" || calls[8] != "flush5") return 6;
    if (calls[12] != "open /BASLUS-20470EQONLINE/list.ico" || calls[14] != "write6_12") return 7;
    if (calls[19] != "open /BASLUS-20470EQONLINE/BASLUS-20470EQONLINE") return 8;
    if (calls[21] != "write8_8_12345678") return 9;
    if (write_pointers[0] != sIconDefinition || reinterpret_cast<__UINTPTR_TYPE__>(write_pointers[1]) != 0x123456) return 10;
    // Result handling after mkdir.
    if (run(0, 0, {-10, 5}, 0) != -1) return 11;
    if (run(0, 0, {-3}, 0) != -2) return 12;
    if (run(0, 0, {-2}, 0) != -1 || run(0, 0, {-5}, 0) != -1) return 13;
    if (run(0, 0, {-4, 5, 0, 0, 6, 0, 0, 8, 0, 0}, 0) != 0) return 14;
    // Open failures.
    if (run(0, 0, {0, -3}, 0) != -2 || run(0, 0, {0, -1}, 0) != -1) return 15;
    if (run(0, 0, {0, 5, -3}, 0) != -2 || run(0, 0, {0, 5, -1}, 0) != -1) return 16;
    if (run(0, 0, {0, 5, 0, -1}, 0) != -1) return 17;
    // Last close negative.
    if (run(0, 0, {0, 5, 0, 0, 6, 0, 0, 8, 0, -1}, 0) != -1) return 18;
    // Padding.
    if (run(0, 0, {0, 5, 0, 0, 6, 0, 0, 8, 0, 0}, 9, -4) != -4) return 19;
    bool padCalled = false;
    for (const std::string& c : calls) if (c == "pad9") padCalled = true;
    if (!padCalled) return 20;
    if (run(0, 0, {0, 5, 0, 0, 6, 0, 0}, -2) != 0 || calls.back() != "sync0") return 21;
    for (const std::string& c : calls) if (c.rfind("pad", 0) == 0 || c.find("BASLUS-20470EQONLINE/BASLUS") != std::string::npos) return 22;
    std::puts("memcard prepare passed");
    return 0;
}
