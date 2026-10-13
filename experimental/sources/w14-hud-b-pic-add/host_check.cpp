// Host check for picAdd__4itemPc. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
#include "pic_add.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
alignas(8) unsigned char _4item$picList[64 * 192];
signed char _4item$picsToLoad;
int drawFrameNm;
}
static int kickCalls;
static int kickFreesRecord = -1;  // record index to free on the call, or -1 to free none
static int kickResult;
static std::string trace;
extern "C" int picKickOneOut__4itemv() {
    ++kickCalls;
    trace += "kick;";
    if (kickFreesRecord >= 0) _4item$picList[kickFreesRecord * 192] = 0;
    return kickResult;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static void reset(int used) {
    std::memset(_4item$picList, 0x33, sizeof _4item$picList);
    for (int i = 0; i < 64; ++i) {
        if (i < used) std::snprintf(reinterpret_cast<char*>(_4item$picList + 192 * i), 20, "old%d", i);
        else _4item$picList[192 * i] = 0;
    }
    _4item$picsToLoad = 0;
    drawFrameNm = 4242;
    kickCalls = 0; kickFreesRecord = -1; kickResult = 0; trace.clear();
}
static short halfAt(int i) { short v; std::memcpy(&v, _4item$picList + 192 * i + 64, 2); return v; }
static int wordAt(int i) { int v; std::memcpy(&v, _4item$picList + 192 * i + 124, 4); return v; }

int main() {
    char name[] = "newpic.tex";
    // empty list: record 0
    reset(0);
    picAdd__4itemPc(name);
    CHECK(std::strcmp(reinterpret_cast<char*>(_4item$picList), "newpic.tex") == 0);
    CHECK(halfAt(0) == -1 && wordAt(0) == 4242 && _4item$picsToLoad == 1 && kickCalls == 0);
    CHECK(_4item$picList[192] == 0 && _4item$picList[66] == 0x33);
    // first free record in the middle wins; earlier records are untouched
    reset(10);
    picAdd__4itemPc(name);
    CHECK(std::strcmp(reinterpret_cast<char*>(_4item$picList + 192 * 10), "newpic.tex") == 0);
    CHECK(halfAt(10) == -1 && wordAt(10) == 4242 && std::strcmp(reinterpret_cast<char*>(_4item$picList + 192 * 9), "old9") == 0);
    CHECK(halfAt(9) == 0x3333 && wordAt(11) == 0x33333333);
    // a hole below the used prefix is taken first
    reset(30);
    _4item$picList[192 * 4] = 0;
    picAdd__4itemPc(name);
    CHECK(std::strcmp(reinterpret_cast<char*>(_4item$picList + 192 * 4), "newpic.tex") == 0 && halfAt(4) == -1);
    // last record free
    reset(63);
    picAdd__4itemPc(name);
    CHECK(std::strcmp(reinterpret_cast<char*>(_4item$picList + 192 * 63), "newpic.tex") == 0 && kickCalls == 0);
    // full list, kick returns zero: nothing changes
    reset(64);
    unsigned char before[sizeof _4item$picList];
    std::memcpy(before, _4item$picList, sizeof before);
    picAdd__4itemPc(name);
    CHECK(kickCalls == 1 && std::memcmp(before, _4item$picList, sizeof before) == 0 && _4item$picsToLoad == 0);
    // full list, kick frees record 20 and returns nonzero: rescanned and added there
    reset(64);
    kickFreesRecord = 20; kickResult = 1;
    picAdd__4itemPc(name);
    CHECK(kickCalls == 1 && std::strcmp(reinterpret_cast<char*>(_4item$picList + 192 * 20), "newpic.tex") == 0);
    CHECK(halfAt(20) == -1 && wordAt(20) == 4242 && _4item$picsToLoad == 1);
    // nonzero result without freeing anything: the kick is repeated until it reports zero
    reset(64);
    kickResult = 5;
    kickResult = 0;
    kickFreesRecord = -1;
    picAdd__4itemPc(name);
    CHECK(kickCalls == 1);
    std::puts("ok");
    return 0;
}
