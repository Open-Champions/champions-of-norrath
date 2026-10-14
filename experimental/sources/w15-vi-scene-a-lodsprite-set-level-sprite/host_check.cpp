// Host check only; not a build input.
#include "set.cpp"
static char trace[256]; static int tl;
static void put(const char* tag, int v) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "%s:%d;", tag, v); }
static unsigned char* theScene;
extern "C" int ReleaseSprite__7VIScenei(unsigned char* s, int i) { put("rel", i); put("sc", s == theScene); return 5; }
extern "C" int ShareSprite__7VIScenei(unsigned char* s, int i) { put("share", i); put("sc", s == theScene); return 7; }
int main() {
    alignas(8) unsigned char lod[256];
    unsigned char scene[8];
    theScene = scene;
    const int olds[] = {-1, 0, 9, 123456};
    for (int level = 0; level < 4; ++level) {
        for (int old : olds) {
            for (int i = 0; i < 256; ++i) lod[i] = static_cast<unsigned char>(i);
            unsigned char before[256];
            *reinterpret_cast<int*>(lod + 76 + 8 * level) = old;
            for (int i = 0; i < 256; ++i) before[i] = lod[i];
            tl = 0; trace[0] = 0;
            int r = SetLevelSprite__11VILODSpriteiiR7VIScene(lod, level, 42 + level, scene);
            if (r != 0) return 1;
            char expect[256]; int el = 0;
            if (old != -1) el += __builtin_snprintf(expect + el, sizeof expect - el, "rel:%d;sc:1;", old);
            el += __builtin_snprintf(expect + el, sizeof expect - el, "share:%d;sc:1;", 42 + level);
            if (__builtin_strcmp(expect, trace) != 0) return 2;
            for (int i = 0; i < 256; ++i) {
                int expectByte = before[i];
                if (i >= 76 + 8 * level && i < 80 + 8 * level) expectByte = (42 + level) >> (8 * (i - 76 - 8 * level)) & 0xff;
                if (lod[i] != expectByte) return 3;
            }
        }
    }
    return 0;
}
