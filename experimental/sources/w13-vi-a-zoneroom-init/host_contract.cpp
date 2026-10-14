#include "init.cpp"
#include <string.h>
#include <string>

static std::string tr; static int calc_ret, alloc_ret; static void* rm;
extern "C" int CalcDataSize__10VIZoneRoom(void* r) { tr += "C;"; rm = r; return calc_ret; }
extern "C" int Allocate__10VIZoneRoom(void* r) { tr += "A;"; rm = r; return alloc_ret; }

int main() {
    alignas(4) unsigned char r[160]; float box[6] = {1, 2, 3, 4, 5, 6};
    auto fresh = [&] { memset(r, 0x44, sizeof r); *(unsigned*)r = 0x10; *(int*)(r + 92) = 1000; *(int*)(r + 96) = 2000; *(int*)(r + 116) = 7; tr.clear(); };
    fresh(); calc_ret = -1; alloc_ret = 0;
    if (Init__10VIZoneRoomiiUiUiR6VIBBox(r, 11, 22, 33, 44, box) != -1 || tr != "C;" || rm != r) return 1;
    if (*(float*)(r + 4) != 1 || *(float*)(r + 8) != 2 || *(float*)(r + 12) != 3 || *(float*)(r + 16) != 4 || *(float*)(r + 20) != 5 || *(float*)(r + 24) != 6) return 2;
    if (*(unsigned*)r != 0x12 || *(int*)(r + 28) != -1 || *(int*)(r + 40) != 33 || *(int*)(r + 48) != 44) return 3;
    if (*(int*)(r + 76) != 0 || *(int*)(r + 80) != 11 || *(int*)(r + 84) != 0 || *(int*)(r + 88) != 22) return 4;
    if (*(int*)(r + 92) != 1000 || *(int*)(r + 96) != 2000) return 5;
    fresh(); calc_ret = 5; alloc_ret = -3;
    if (Init__10VIZoneRoomiiUiUiR6VIBBox(r, 11, 22, 33, 44, box) != -1 || tr != "C;A;") return 6;
    if (*(int*)(r + 92) != 1000) return 7;
    fresh(); alloc_ret = 0;
    if (Init__10VIZoneRoomiiUiUiR6VIBBox(r, 11, 22, 33, 44, box) != 0 || tr != "C;A;") return 8;
    if (*(int*)(r + 92) != 1007 || *(int*)(r + 96) != 2007) return 9;
    unsigned char e[160]; memset(e, 0x44, sizeof e);
    auto put = [&](int off, unsigned v) { memcpy(e + off, &v, 4); };
    put(0, 0x12); put(28, 0xffffffffu); put(40, 33); put(48, 44); put(76, 0); put(80, 11); put(84, 0); put(88, 22);
    put(92, 1007); put(96, 2007); put(116, 7);
    float f[6] = {1, 2, 3, 4, 5, 6}; memcpy(e + 4, f, 12); memcpy(e + 16, f + 3, 12);
    put(92, 1007); put(96, 2007);
    if (memcmp(r, e, 160) != 0) return 10;
    return 0;
}
