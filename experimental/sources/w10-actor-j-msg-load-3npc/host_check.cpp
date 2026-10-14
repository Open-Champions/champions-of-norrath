// Host contract for msg_load__3NPCPc; not an EE input. Callees are stubs that log.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
static unsigned long findResult;
static const char* texts[] = {"lumpA", "modelB", "texC"};
extern "C" {
char playedStinger;
char* objectSaveGetFromStringPool__Fi(int i) { calls += "pool" + std::to_string(i) + ";"; return const_cast<char*>(texts[i - 10]); }
void* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return reinterpret_cast<void*>(0x100 + std::strlen(n)); }
void animInitState__FP14AnimationState(void*) { calls += "init;"; }
void* objectFindAnimation__FP10GameObjectPc(void*, char* n) { calls += std::string("find(") + n + ");"; return reinterpret_cast<void*>(findResult); }
void animAdd__FP14AnimationStateP15AnimationHeaderi(void*, void* h, int f) { calls += "add(" + std::to_string(reinterpret_cast<unsigned long>(h)) + "," + std::to_string(f) + ");"; }
void animRandomizeTime__FP14AnimationState(void*) { calls += "rand;"; }
long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int mask) { calls += "chunks(" + std::to_string(reinterpret_cast<unsigned long>(m)) + "," + std::to_string(mask) + ");"; return 0x1122334455667788ll; }
}
#include "msg_load.cpp"
int main() {
    alignas(16) static unsigned char o[1280];
    alignas(16) static int buf[8];
    int bad = 0;
    for (unsigned long found : {0ul, 0x900ul}) {
        std::memset(o, 0xaa, sizeof o); playedStinger = 1; calls.clear(); findResult = found;
        float f = 33.5f;
        buf[0] = 10; buf[1] = 11; buf[2] = 12; buf[3] = 0x12345; buf[4] = 0x80; std::memcpy(buf + 5, &f, 4);
        int n = msg_load__3NPCPc(o, reinterpret_cast<char*>(buf));
        short h; int w; float g; long long chunks; unsigned m, t;
        std::memcpy(&h, o + 1066, 2); std::memcpy(&w, o + 244, 4); std::memcpy(&g, o + 176, 4); std::memcpy(&chunks, o + 552, 8);
        std::memcpy(&m, o + 1056, sizeof m); std::memcpy(&t, o + 1060, sizeof t);
        std::string expect = "pool10;pool11;pool12;lump(lumpA,modelB);lump(lumpA,texC);init;find(idle);";
        if (found) expect += "add(2304,1);rand;";
        expect += "chunks(" + std::to_string(0x100 + 6) + ",128);";
        if (n != 24 || calls != expect || h != 0x2345 || w != 0x80 || g != f || chunks != 0x1122334455667788ll || m != 0x106 || t != 0x104 ||
            playedStinger != 0 || std::strcmp(reinterpret_cast<char*>(o + 960), "lumpA") || std::strcmp(reinterpret_cast<char*>(o + 992), "modelB") ||
            std::strcmp(reinterpret_cast<char*>(o + 1024), "texC")) { std::printf("fail %d %s\n", n, calls.c_str()); bad = 1; }
    }
    return bad;
}
