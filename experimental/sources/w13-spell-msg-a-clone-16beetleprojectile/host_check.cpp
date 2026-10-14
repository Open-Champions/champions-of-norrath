// Host contract for clone__16beetleProjectile; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_10GameObject[8], vt_10Projectile[8], vt_13MissileWeapon[8], vt_16beetleProjectile[8];
static unsigned char arena[512] __attribute__((aligned(16)));
static unsigned char src[512] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[16];
static int callCount;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4);
    std::memset(d, 0, 4);
}
static unsigned lightWordsAtCall[2];
void lightAdd__FP5Light(void* light) {
    calls += "light" + std::to_string(static_cast<unsigned char*>(light) - arena) + ";";
    std::memcpy(lightWordsAtCall, light, 8);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

static int run(unsigned w0, unsigned w4) {
    int bad = 0;
    for (int i = 0; i < 512; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    std::memcpy(src + 368, &w0, 4);
    std::memcpy(src + 372, &w4, 4);
    std::memset(arena, 0xa5, sizeof arena);
    calls.clear(); callCount = 0; lightWordsAtCall[0] = lightWordsAtCall[1] = 7;
    void* r = clone__16beetleProjectile(src);
    bool light = w0 != 0 || w4 != 0;
    std::string want = "alloc460;lh0<0;lh4<4;lh8<8;lh76<76;lh92<92;lh116<116;lh140<140;lh164<164;lh188<188;lh292<292;";
    if (light) want += "light368;";
    want += "lh452<452;";
    if (r != arena) { std::puts("fail: returns the new object"); bad = 1; }
    if (calls != want) { std::puts(calls.c_str()); std::puts("fail: call order"); bad = 1; }
    const unsigned char* expect[] = {vt_10GameObject, vt_10GameObject, vt_10GameObject, vt_10Projectile,
        vt_13MissileWeapon, vt_13MissileWeapon, vt_13MissileWeapon, vt_13MissileWeapon, vt_13MissileWeapon,
        vt_13MissileWeapon, vt_16beetleProjectile};
    for (int i = 0; i < 11; ++i)
        if (wordAt72AtCall[i] != lo(expect[i])) { std::printf("fail: table word at list head call %d\n", i); bad = 1; }
    unsigned w72; std::memcpy(&w72, arena + 72, 4);
    if (w72 != lo(vt_16beetleProjectile)) { std::puts("fail: final table pointer"); bad = 1; }
    if (light && (lightWordsAtCall[0] != 0 || lightWordsAtCall[1] != 0)) { std::puts("fail: light words not cleared before the call"); bad = 1; }
    static bool copied[512], head[512];
    std::memset(copied, 0, sizeof copied); std::memset(head, 0, sizeof head);
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{80,80},{84,91},{96,107},{108,115},{120,131},{132,139},{144,155},
        {156,163},{168,179},{180,187},{192,203},{204,205},{208,248},{250,254},{256,291},{296,427},{428,451},{456,459}};
    for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
    const unsigned heads[] = {0, 4, 8, 76, 92, 116, 140, 164, 188, 292, 452};
    for (unsigned h : heads) for (unsigned i = h; i < h + 4; ++i) head[i] = true;
    for (unsigned i = 0; i < 460; ++i) {
        bool cleared = light && i >= 368 && i < 376;
        if (i >= 72 && i < 76) continue;
        if (cleared) { if (arena[i] != 0) { std::printf("fail: byte %u not cleared\n", i); bad = 1; break; } continue; }
        if (head[i]) { if (arena[i] != 0) { std::printf("fail: head byte %u not zeroed\n", i); bad = 1; break; } continue; }
        if (copied[i] && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
        if (!copied[i] && arena[i] != 0xa5) { std::printf("fail: byte %u written but not in a copy range\n", i); bad = 1; break; }
    }
    return bad;
}

int main() {
    int bad = 0;
    failNext = true; calls.clear();
    if (clone__16beetleProjectile(src) != nullptr || calls != "alloc460;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false;
    bad |= run(5, 6); bad |= run(0, 6); bad |= run(5, 0); bad |= run(0, 0);
    return bad;
}
