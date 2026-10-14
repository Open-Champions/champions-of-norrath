// Host check for getPic__4itemiPQ24item11description. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
#include "get_pic.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
alignas(8) unsigned char _4item$picList[64 * 192];
int drawFrameNm;
}
static std::string trace;
static char wanted[64];
static int fallbackToken;
extern "C" void getPicName__4itemPciPQ24item11description(char* out, int n, void* description) {
    char b[48];
    std::snprintf(b, sizeof b, "name %d %d;", n, description != nullptr);
    trace += b;
    std::strcpy(out, wanted);
}
extern "C" void picAdd__4itemPc(char* name) { trace += std::string("add ") + name + ";"; }
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    trace += std::string("find ") + lump + " " + name + ";";
    return &fallbackToken;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static void reset() {
    std::memset(_4item$picList, 0, sizeof _4item$picList);
    trace.clear();
    drawFrameNm = 1000;
    for (int i = 0; i < 64; ++i) {
        std::snprintf(reinterpret_cast<char*>(_4item$picList + 192 * i), 40, "pic%d.tex", i);
        short slot = 5;
        std::memcpy(_4item$picList + 192 * i + 64, &slot, 2);
        int stamp = 7;
        std::memcpy(_4item$picList + 192 * i + 124, &stamp, 4);
    }
}
static int stampOf(int i) { int v; std::memcpy(&v, _4item$picList + 192 * i + 124, 4); return v; }

int main() {
    int item = 0;
    // found: stamp updated, result is the record's data at +128
    reset();
    std::strcpy(wanted, "pic9.tex");
    void* r = getPic__4itemiPQ24item11description(6, &item);
    CHECK(r == _4item$picList + 192 * 9 + 128);
    CHECK(trace == "name 6 1;");
    CHECK(stampOf(9) == 1000 && stampOf(8) == 7 && stampOf(10) == 7);
    // the last record is reachable, and the first of equal names wins
    reset();
    std::strcpy(wanted, "pic63.tex");
    r = getPic__4itemiPQ24item11description(0, 0);
    CHECK(r == _4item$picList + 192 * 63 + 128 && trace == "name 0 0;" && stampOf(63) == 1000);
    reset();
    std::strcpy(reinterpret_cast<char*>(_4item$picList + 192 * 30), "dup");
    std::strcpy(reinterpret_cast<char*>(_4item$picList + 192 * 31), "dup");
    std::strcpy(wanted, "dup");
    r = getPic__4itemiPQ24item11description(1, &item);
    CHECK(r == _4item$picList + 192 * 30 + 128 && stampOf(30) == 1000 && stampOf(31) == 7);
    // a negative first slot (not ready): fallback texture, stamp untouched
    reset();
    short neg = -1;
    std::memcpy(_4item$picList + 192 * 4 + 64, &neg, 2);
    std::strcpy(wanted, "pic4.tex");
    r = getPic__4itemiPQ24item11description(2, &item);
    CHECK(r == &fallbackToken && trace == "name 2 1;find equipmnt.lmp pic_dagger1.tex;" && stampOf(4) == 7);
    // zero slot counts as ready
    reset();
    short zero = 0;
    std::memcpy(_4item$picList + 192 * 4 + 64, &zero, 2);
    std::strcpy(wanted, "pic4.tex");
    r = getPic__4itemiPQ24item11description(2, &item);
    CHECK(r == _4item$picList + 192 * 4 + 128 && stampOf(4) == 1000);
    // not found: picAdd is asked to add it and the fallback texture is returned
    reset();
    std::strcpy(wanted, "missing.tex");
    r = getPic__4itemiPQ24item11description(3, &item);
    CHECK(r == &fallbackToken);
    CHECK(trace == "name 3 1;add missing.tex;find equipmnt.lmp pic_dagger1.tex;");
    for (int i = 0; i < 64; ++i) CHECK(stampOf(i) == 7);
    // a name matching only a prefix of a record name is not found
    reset();
    std::strcpy(wanted, "pic1");
    r = getPic__4itemiPQ24item11description(3, &item);
    CHECK(r == &fallbackToken && trace.find("add pic1;") != std::string::npos);
    std::puts("get pic passed");
    return 0;
}
