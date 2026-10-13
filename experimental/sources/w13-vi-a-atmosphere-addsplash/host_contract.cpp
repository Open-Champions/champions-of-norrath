#include "add_splash.cpp"
#include <string.h>
#include <string>

static std::string tr; static int roll; static unsigned char* nodep;
static unsigned char* base;
extern "C" void* RemHead__7VILList(void* l) { tr += "H" + std::to_string((unsigned char*)l - base) + ";"; return nodep; }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { tr += "T" + std::to_string((unsigned char*)l - base) + ",ok;"; if (n != nodep) tr += "BADNODE"; }
extern "C" int rand(void) { tr += "r;"; return roll; }
static float fb(unsigned b) { float f; memcpy(&f, &b, 4); return f; }

int main() {
    alignas(4) static unsigned char at[200]; alignas(4) static unsigned char nd[64];
    base = at; float p[3] = {1.5f, -2.f, 9.25f};
    memset(at, 0x31, sizeof at); memset(nd, 0x42, sizeof nd); nodep = 0;
    AddSplash__12VIAtmosphereR7VIVect3(at, p);
    if (tr != "H36;") return 1;
    for (int i = 0; i < 200; ++i) if (at[i] != 0x31) return 2;
    for (int r = 0; r < 256; ++r) {
        tr.clear(); nodep = nd; roll = r; *(int*)(at + 32) = 4; memset(nd, 0x42, sizeof nd);
        AddSplash__12VIAtmosphereR7VIVect3(at, p);
        if (tr != "H36;r;T20,ok;") return 3;
        if (*(int*)(at + 32) != 5) return 4;
        if (*(float*)(nd + 16) != 1.5f || *(float*)(nd + 20) != -2.f || *(float*)(nd + 24) != 9.25f || *(int*)(nd + 32) != 0) return 5;
        float u = (float)((131 * r) & 0xff) * fb(0x3b808080);
        if (*(float*)(nd + 28) != u * fb(0x3e4ccccc) + fb(0x3e90624d)) return 6;
        for (int i = 0; i < 64; ++i) if (!(i >= 16 && i < 36) && nd[i] != 0x42) return 7;
    }
    return 0;
}
