// Host contract for Innoruuk msg_draw; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_H_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10h_host_base;
void* w10h_host_fn(unsigned) { return nullptr; }
#include "msg_draw.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static std::string calls; static void* sRecord = reinterpret_cast<void*>(0x5150);
static void *dSelf, *dModel, *dTexture, *dSecond; static unsigned char rPoint[12]; static void* aSet;
static void *tModel, *tTexture, *tBase, *tRecord; static int tMode, tA, tB; static unsigned tFlags;
extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* x) { calls += "draw;"; dSelf = s; dModel = m; dTexture = t; dSecond = x; return sRecord; }
void modelSetRelightAttachment__FG6Point3(const void* p) { calls += "relight;"; std::memcpy(rPoint, p, 12); }
void lightSetAmbientOffset__FPs(short* p) { calls += "ambient;"; aSet = p; }
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int mode, void* b, void* r, unsigned f, int a, int bb) {
    calls += "attach;"; tModel = m; tTexture = t; tMode = mode; tBase = b; tRecord = r; tFlags = f; tA = a; tB = bb; }
void lightResetOptionalParams__Fv() { calls += "lreset;"; }
void modelResetOptionalParams__Fv() { calls += "mreset;"; }
}
int main() {
    int bad = 0;
    w10h_host_base = arena;
    const unsigned gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    for (unsigned gate : gates) {
        unsigned char* object = arena + 0x1000;
        std::memset(arena, 0x33, sizeof arena); calls.clear();
        unsigned block = 0x200; std::memcpy(object + 36, &block, 4);
        unsigned words[] = {0x400, 0x500, 0x600, 0x700, 0x800};  // m+12, m+20, m+172, m+180 and a spare
        std::memcpy(arena + block + 12, &words[0], 4); std::memcpy(arena + block + 20, &words[1], 4);
        std::memcpy(arena + block + 172, &words[2], 4); std::memcpy(arena + block + 180, &words[3], 4);
        for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x60 + i);
        std::memcpy(object + 960, &gate, 4);
        msg_draw__8Innoruuk(object);
        if (dSelf != object || dModel != arena + words[0] || dTexture != arena + words[1] || dSecond != nullptr) { std::puts("fail: creature draw arguments"); bad = 1; }
        if (gate == 0) {
            if (calls != "draw;") { std::printf("fail: gate zero calls %s\n", calls.c_str()); bad = 1; }
            continue;
        }
        if (calls != "draw;relight;ambient;attach;lreset;mreset;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (std::memcmp(rPoint, object + 16, 12) != 0 || aSet != object + 1168) { std::puts("fail: relight point or ambient pointer"); bad = 1; }
        if (tModel != arena + words[2] || tTexture != arena + words[3] || tMode != 0 || tBase != arena + words[0] || tRecord != sRecord || tFlags != 0xffffffffu || tA != -1 || tB != 0) { std::puts("fail: attachment arguments"); bad = 1; }
    }
    if (!bad) std::puts("innoruuk msg_draw ok");
    return bad;
}
