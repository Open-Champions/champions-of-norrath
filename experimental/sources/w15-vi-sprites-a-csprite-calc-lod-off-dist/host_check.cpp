// Host check only; not a build input.
#include "calc_lod_off_dist.cpp"
#include <cstring>
#include <initializer_list>
static int types[2]; static int typeCalls, spriteCalls; static int idsSeen[3]; static unsigned char record[128];
extern "C" int GetSpriteType__7VIScenei(void*, int id) { idsSeen[typeCalls] = id; return types[typeCalls++ < 1 ? 0 : 1]; }
extern "C" unsigned char* Sprite__7VIScenei(void*, int id) { ++spriteCalls; idsSeen[2] = id; return record; }
int main() {
    alignas(8) unsigned char s[4096];
    const int cases[][2] = {{7, 0}, {3, 3}, {0, 3}, {0, 0}, {7, 7}, {3, 7}, {1, 2}};
    for (int id : {-1, 5, 0}) for (auto& c : cases) {
        std::memset(s, 0x11, sizeof s); std::memset(record, 0, sizeof record);
        float v = 9.5f; std::memcpy(record + 48, &v, 4);
        std::memcpy(s + 3856, &id, 4); types[0] = c[0]; types[1] = c[1]; typeCalls = spriteCalls = 0;
        if (CalcLODOffDist__9VICSpriteR7VIScene(s, s) != 0) return 1;
        bool copy = id != -1 && (c[0] == 7 || c[1] == 3);
        int wantTypeCalls = id == -1 ? 0 : (c[0] == 7 ? 1 : 2);
        if (typeCalls != wantTypeCalls || spriteCalls != (copy ? 1 : 0)) return 2;
        float got; std::memcpy(&got, s + 48, 4);
        unsigned raw; std::memcpy(&raw, s + 48, 4);
        if (copy ? got != 9.5f : raw != 0x11111111u) return 3;
        if (copy && idsSeen[2] != id) return 4;
    }
    return 0;
}
