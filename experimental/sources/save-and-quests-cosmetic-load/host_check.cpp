#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> trace;
static char names[4][4] = {"grp", "a", "b", "c"};
static char heads[3];
extern "C" {
const char* objectSaveGetFromStringPool__Fi(int i) { trace.push_back("pool" + std::to_string(i)); return names[i - 20]; }
void* lumpFindResource__FPcT0(char* a, char* b) {
    trace.push_back(std::string("lump:") + a + "," + b);
    return &heads[trace.back() == "lump:grp,a" ? 0 : trace.back() == "lump:grp,b" ? 1 : 2];
}
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int c) {
    trace.push_back(std::string("add") + (h == &heads[2] ? "H2" : "H?") + "," + std::to_string(c));
    if (s == nullptr) trace.push_back("nullstate");
}
void animSetToEnd__FP14AnimationState(void*) { trace.push_back("end"); }
void objectAddToSlowRunList__FP10GameObject(void*) { trace.push_back("slow"); }
}
#include "cosmetic_load.cpp"
using sq::read_word;
using sq::write_word;
static unsigned char object[160];
static unsigned char buf[48];
static void setup(unsigned flags, int h1, int h2) {
    std::memset(object, 0, sizeof object);
    std::memset(buf, 0, sizeof buf);
    trace.clear();
    for (int i = 0; i < 4; ++i) write_word(buf + i * 4, 20u + i);
    write_word(buf + 16, 0x01020304u);
    write_word(buf + 20, flags);
    write_word(buf + 24, 0x55667788u);
    buf[28] = (unsigned char)h1; buf[29] = (unsigned char)(h1 >> 8);
    buf[30] = (unsigned char)h2; buf[31] = (unsigned char)(h2 >> 8);
}
int main() {
    setup(0u, -3, 1234);
    int n = msg_load__16CosmeticPropAnimPc(object, (char*)buf);
    if (n != 32) return 1;
    const char* p = (const char*)sq::pointer(read_word(object + 76));
    if (std::strcmp(p, "grp") != 0) return 2;
    if (read_word(object + 80) != sq::pointer_bits(names[1]) || read_word(object + 84) != sq::pointer_bits(names[2]) ||
        read_word(object + 88) != sq::pointer_bits(names[3])) return 3;
    if (read_word(object + 12) != 0x01020304u || read_word(object + 148) != 0u || read_word(object + 144) != 0x55667788u) return 4;
    if (sq::read_single(object + 32) != -3.0f || sq::read_single(object + 28) != 1234.0f) return 5;
    if (object[41] != 0xfd || object[40] != (1234 & 0xff)) return 6;
    if (read_word(object + 92) != sq::pointer_bits(&heads[0]) || read_word(object + 96) != sq::pointer_bits(&heads[1])) return 7;
    // flags 0: add with count 1, no end, then the slow list
    std::string t;
    for (auto& s : trace) t += s + " ";
    if (t != "pool20 pool21 pool22 pool23 lump:grp,a lump:grp,b lump:grp,c addH2,1 slow ") return 8;
    // bit 1: count 4; bit 0: set to end between add and the run list
    setup(3u, 0, 0);
    msg_load__16CosmeticPropAnimPc(object, (char*)buf);
    t.clear();
    for (auto& s : trace) t += s + " ";
    if (t != "pool20 pool21 pool22 pool23 lump:grp,a lump:grp,b lump:grp,c addH2,4 end slow ") return 9;
    setup(1u, 0, 0);
    msg_load__16CosmeticPropAnimPc(object, (char*)buf);
    t.clear();
    for (auto& s : trace) t += s + " ";
    if (t != "pool20 pool21 pool22 pool23 lump:grp,a lump:grp,b lump:grp,c addH2,1 end slow ") return 10;
    // bit 1 alone keeps count 4 without the end call
    setup(2u, 0, 0);
    msg_load__16CosmeticPropAnimPc(object, (char*)buf);
    t.clear();
    for (auto& s : trace) t += s + " ";
    if (t.find("addH2,4 slow") == std::string::npos) return 11;
    std::puts("ok");
    return 0;
}
