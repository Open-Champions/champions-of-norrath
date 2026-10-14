// Host check only; not a build input.
#include "init.cpp"
#include <cstring>
#include <initializer_list>
static int clr, ini, clrRet, iniRet; static void *clrSeen, *iniSeen;
extern "C" int Clear__9VICSprite(void* o) { ++clr; clrSeen = o; return clrRet; }
extern "C" int Init__9VIHSprite(void* o) { ++ini; iniSeen = o; return iniRet; }
int main() {
    alignas(8) unsigned char s[512];
    for (int cr : {0, -1}) for (int ir : {0, -2}) for (int flag : {0, 1, 5}) {
        std::memset(s, 0x5a, sizeof s); clr = ini = 0; clrRet = cr; iniRet = ir;
        unsigned before = 0x5a5a5a5au;
        int r = Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi(s, 11, 22, 33, -3.5f, flag);
        bool fail = cr < 0 || ir < 0;
        if (r != (fail ? -1 : 0) || clr != 1 || clrSeen != s || ini != (cr < 0 ? 0 : 1) || (ini && iniSeen != s)) return 1;
        unsigned fl; int sk, ra, sx; float sc;
        std::memcpy(&fl, s + 400, 4); std::memcpy(&sk, s + 404, 4); std::memcpy(&ra, s + 408, 4); std::memcpy(&sx, s + 412, 4); std::memcpy(&sc, s + 208, 4);
        if (fail) { if (fl != before || sk != 0x5a5a5a5a || sc == -3.5f) return 2; continue; }
        if (fl != (flag ? (before | 1u) : (before & ~1u)) || sk != 11 || ra != 22 || sx != 33 || sc != -3.5f) return 3;
    }
    return 0;
}
