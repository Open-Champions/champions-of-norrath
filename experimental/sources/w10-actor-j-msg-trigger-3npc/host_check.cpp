// Host contract for msg_trigger__3NPCi; not an EE input. Callees are stubs that log.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
static long long chunksResult = 0x1122334455667788ll;
extern "C" {
signed char cutMode;
char levelName[400];
long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int mask) { calls += "chunks(" + std::to_string(reinterpret_cast<unsigned long>(m) & 0xfff) + "," + std::to_string(mask) + ");"; return chunksResult; }
void* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + n + ");"; return reinterpret_cast<void*>(0x777); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float t, int f) {
    calls += "anim(" + std::to_string(reinterpret_cast<unsigned long>(h)) + "," + std::to_string(t) + "," + std::to_string(f) + ");";
}
int msg_trigger__8Creaturei(void*, int v) { calls += "base" + std::to_string(v) + ";"; return 0; }
}
#include "msg_trigger.cpp"
int main() {
    alignas(16) static unsigned char object[1280];
    int bad = 0;
    struct Case { int cut; const char* name; const char* level; int arg; const char* expect; long long chunks; int w244, w1136; };
    const Case cases[] = {
        {1, "sylea", "hate3", 5, "chunks(1056,2);", chunksResult, 2, 0},
        {1, "SyLeA", "", 0, "chunks(1056,0);", chunksResult, 0, 0},
        {0, "sylea", "hate3", 1, "base1;", 0, 0, 0},
        {0, "firlow", "hate3", 1, "lump(idle1.anm);anim(1911,0.100000,3);", 0, 0, 1},
        {1, "FIRLOW", "HATE3", 0, "lump(idle1.anm);anim(1911,0.100000,3);", 0, 0, 1},
        {1, "firlow", "hate2", 0, "base0;", 0, 0, 0},
        {1, "other", "hate3", 0, "base0;", 0, 0, 0},
    };
    for (const Case& c : cases) {
        std::memset(object, 0, sizeof object); std::memset(levelName, 0, sizeof levelName);
        unsigned long model = 1056; std::memcpy(object + 1056, &model, sizeof model);
        std::strcpy(reinterpret_cast<char*>(object + 1068), c.name);
        std::strcpy(levelName + 81, c.level);
        cutMode = static_cast<signed char>(c.cut); calls.clear();
        msg_trigger__3NPCi(object, c.arg);
        long long got; std::memcpy(&got, object + 552, 8);
        int w244, w1136; std::memcpy(&w244, object + 244, 4); std::memcpy(&w1136, object + 1136, 4);
        if (calls != c.expect || got != c.chunks || w244 != c.w244 || w1136 != c.w1136) { std::printf("fail: %s\n  %s\n", c.name, calls.c_str()); bad = 1; }
    }
    return bad;
}
