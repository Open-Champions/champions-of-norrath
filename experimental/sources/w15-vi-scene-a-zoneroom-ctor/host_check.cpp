// Host check only; not a build input.
#include "ctor.cpp"
static char trace[256]; static int tl;
static unsigned char* theRoom;
extern "C" void __t7VIArray1Zi(unsigned char* a) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "arr:%ld;", static_cast<long>(a - theRoom)); }
static int clearResult;
extern "C" int Clear__10VIZoneRoom(unsigned char* r) {
    tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "clear:%d;w116:%d;", r == theRoom, *reinterpret_cast<int*>(r + 116));
    return clearResult;
}
int main() {
    alignas(8) unsigned char room[160];
    theRoom = room;
    const int results[] = {0, -1, 33};
    for (int cr : results) {
        for (int i = 0; i < 160; ++i) room[i] = static_cast<unsigned char>(0xa5 ^ i);
        unsigned char before[160];
        for (int i = 0; i < 160; ++i) before[i] = room[i];
        clearResult = cr; tl = 0; trace[0] = 0;
        if (__10VIZoneRoom(room) != cr) return 1;
        if (__builtin_strcmp(trace, "arr:60;arr:68;clear:1;w116:0;") != 0) return 2;
        for (int i = 0; i < 160; ++i) {
            bool zeroed = (i >= 4 && i < 16) || (i >= 16 && i < 28) || (i >= 116 && i < 120);
            if (zeroed ? room[i] != 0 : room[i] != before[i]) return 3;
        }
    }
    return 0;
}
