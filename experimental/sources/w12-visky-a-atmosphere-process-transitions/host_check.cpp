// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// Material, sprite and sound calls are recording stubs. Handles and palette entries are 32-bit words
// mapped onto an arena (base 0x100000); expected traces are written out from the notes at the top of
// process_transitions.cpp.
#define NORRATH_HOST_ARENA
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "process_transitions.cpp"

static const unsigned SELF = 0x100, SCENE = 0x2000, ENT = 0x4000;  // arena offsets
static std::string trace;
static int pal_entries[16];  // palette size by sprite index
static int play_counter;
static std::string S(long v) { return std::to_string(v); }
static std::string Fs(float f) { char b[40]; std::snprintf(b, sizeof b, "%.9g", f); return b; }

extern "C" {
void ClearAllSplashes__12VIAtmosphere(unsigned char*) { trace += "clr;"; }
void ResetLightning__12VIAtmosphere(unsigned char*) { trace += "reset;"; }
unsigned Sprite__7VIScenei(unsigned char*, int index) { trace += "spr" + S(index) + ";"; return 0x1000 + static_cast<unsigned>(index); }
unsigned GetMaterialPal__C14VISimpleSprite(unsigned sprite) { trace += "pal" + S(sprite - 0x1000) + ";"; return sprite + 1; }
int GetMaterialPalSize__8VIRasteri(unsigned, unsigned pal) { trace += "size;"; return pal_entries[pal - 0x1001]; }
unsigned GetMaterialPalEntry__8VIRasterii(unsigned, unsigned, int index) { trace += "ent" + S(index) + ";"; return arena_base + ENT + 64 * static_cast<unsigned>(index); }
void GetLayerColor__C10VIMateriali(unsigned char* out, unsigned material, int layer) {
    trace += "get" + S((material - arena_base - ENT) / 64) + "." + S(layer) + ";";
    out[0] = static_cast<unsigned char>(layer + 1); out[1] = 2; out[2] = 3; out[3] = 0x77;
}
void SetLayerColor__10VIMaterialiRC9VIColor32(unsigned material, int layer, const unsigned char* c) {
    trace += "set" + S((material - arena_base - ENT) / 64) + "." + S(layer) + "=" + S(c[0]) + "," + S(c[1]) + "," + S(c[2]) + "," + S(c[3]) + ";";
}
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(unsigned m, int layer, unsigned tex, unsigned, unsigned) { trace += "tex" + S((m - arena_base - ENT) / 64) + "." + S(layer) + "=" + S(tex) + ";"; }
void SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(unsigned, int n, unsigned, unsigned) { trace += "num" + S(n) + ";"; }
void CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(unsigned, int to, int from, unsigned, unsigned) { trace += "copy" + S(to) + "<" + S(from) + ";"; }
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(unsigned, int layer, int mode) { trace += "blend" + S(layer) + "," + S(mode) + ";"; }
int Play__13VISoundDeviceii12VISOUNDCLASS(unsigned, int sound, int volume, int cls) {
    ++play_counter; trace += "play" + S(sound) + "," + S(volume) + "," + S(cls) + "=" + S(500 + play_counter) + ";"; return 500 + play_counter;
}
void SetVolume__13VISoundDeviceif(unsigned, int h, float v) { trace += "vol" + S(h) + "=" + Fs(v) + ";"; }
void SetPan__13VISoundDeviceif(unsigned, int h, float v) { trace += "pan" + S(h) + "=" + Fs(v) + ";"; }
void SetPitch__13VISoundDeviceii(unsigned, int h, int p) { trace += "pitch" + S(h) + "=" + S(p) + ";"; }
void Stop__13VISoundDevicei(unsigned, int h) { trace += "stop" + S(h) + ";"; }
}

static void sw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned rw(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void sf(unsigned off, float f) { std::memcpy(arena + off, &f, 4); }
static float rf(unsigned off) { float f; std::memcpy(&f, arena + off, 4); return f; }
static float bf(unsigned u) { float f; std::memcpy(&f, &u, 4); return f; }

// Sprites: 5, 6, 7 are the faded ones (one, two and one palette entries), 3 is the sky material.
// Palette entry i has i == 0 ? two : one layers unless set otherwise.
static void arm(float t, float dt, int a, int b, int c, int d) {
    std::memset(arena, 0, sizeof arena);
    trace.clear(); play_counter = 0;
    for (int i = 0; i < 16; ++i) pal_entries[i] = 0;
    pal_entries[5] = 1; pal_entries[6] = 2; pal_entries[7] = 1; pal_entries[3] = 1;
    for (int i = 0; i < 4; ++i) sw(ENT + 64 * i + 8, i == 0 ? 2 : 1);
    sw(SCENE + 19352, 0x7000); sw(SCENE + 19356, 0x7100); sw(SCENE + 19360, 0x7200);
    sf(SELF + 1544, t); sf(SELF + 1192, dt);
    sw(SELF + 1516, a); sw(SELF + 1520, b); sw(SELF + 1532, c); sw(SELF + 1536, d);
    sw(SELF + 1248, 3); sw(SELF + 1260, 5); sw(SELF + 1264, 6); sw(SELF + 1268, 7);
    sw(SELF + 1528, 0x3333);
    sf(SELF + 1564, 1.0f);
    sw(SELF + 4496, 0xffffffffu); sw(SELF + 4500, 0xffffffffu);
    sw(SELF + 4488, 41); sw(SELF + 4492, 42);
    sf(SELF + 128, 0.25f);
}
static void go(float hour, float g) { ProcessTransitions__12VIAtmosphereR7VISceneff(arena + SELF, arena + SCENE, hour, g); }
// Expected trace of one sprite fade: sprite index, entries and the alpha byte.
static std::string fade(int sprite, int entries, int alpha) {
    std::string s = "spr" + S(sprite) + ";pal" + S(sprite) + ";size;";
    for (int e = 0; e < entries; ++e) {
        s += "ent" + S(e) + ";";
        const int layers = e == 0 ? 2 : 1;
        for (int l = 0; l < layers; ++l) s += "get" + S(e) + "." + S(l) + ";set" + S(e) + "." + S(l) + "=" + S(l + 1) + ",2,3," + S(alpha) + ";";
    }
    return s;
}
static float expect_starfade(float h) {
    if (h < 6.0f) return 1.0f - h * bf(0x3e2aaaaa);
    if (18.0f < h) return (h - 18.0f) * bf(0x3e2aaaaa);
    return 0.0f;
}

int main() {
    // 1. t == 0 with W[1516] set: both fades zero; with W[1516] clear: star fade and F[1472] = 1.0. The
    //    timers are zeroed at the end because the intensity is 0.
    arm(0.0f, 0.1f, 1, 0, 0, 0);
    sf(SELF + 4504, 3.0f); sf(SELF + 4508, 3.0f); sf(SELF + 4512, 3.0f);
    go(3.0f, 1.0f);
    if (!trace.empty() || rf(SELF + 1472) != 0.0f || rf(SELF + 1564) != 0.0f) return 1;
    if (rf(SELF + 4504) != 0.0f || rf(SELF + 4508) != 0.0f || rf(SELF + 4512) != 0.0f) return 2;
    arm(0.0f, 0.1f, 0, 0, 0, 0); go(3.0f, 1.0f);
    if (rf(SELF + 1564) != expect_starfade(3.0f) || rf(SELF + 1472) != 1.0f) return 3;
    arm(0.0f, 0.1f, 0, 0, 0, 0); go(12.0f, 1.0f);
    if (rf(SELF + 1564) != 0.0f) return 4;
    arm(0.0f, 0.1f, 0, 0, 0, 0); go(21.0f, 1.0f);
    if (rf(SELF + 1564) != (21.0f - 18.0f) * bf(0x3e2aaaaa)) return 5;
    arm(0.0f, 0.1f, 0, 0, 0, 0); go(6.0f, 1.0f);
    if (rf(SELF + 1564) != 0.0f) return 6;  // 6.0 is not below 6.0 and not above 18.0
    arm(0.0f, 0.1f, 0, 0, 0, 0); go(18.0f, 1.0f);
    if (rf(SELF + 1564) != 0.0f) return 7;

    // 2. Fade alphas by state pair. t = 0.25 (dt 0): (int)(0.25 * 255) = 63.
    struct Case { int a, b, alpha; bool scaled_by_inverse; bool scaled_by_t; bool zeroed; } cases[] = {
        {0, 1, 255 - 63, true, false, false}, {0, 0, 255, false, false, false}, {0, 2, 255, false, false, false},
        {1, 0, 63, false, true, false}, {1, 1, 0, false, false, true}, {1, 2, 0, false, false, true},
        {2, 0, 255, false, false, false}, {2, 1, 0, false, false, true}, {3, 3, 0, false, false, true}};
    for (const Case& cs : cases) {
        arm(0.25f, 0.0f, cs.a, cs.b, 0, 0); go(12.0f, 1.0f);
        const int second = 255 - ((255 - cs.alpha) << 1) < 0 ? 0 : 255 - ((255 - cs.alpha) << 1);
        // F[1564] starts at the star fade of hour 12 (0.0): scaling it leaves 0.0, so check the flag word
        // and F[1472] instead, and the call order.
        std::string want = fade(5, 1, cs.alpha) + fade(6, 2, second) + fade(7, 1, cs.alpha);
        if (trace.substr(0, want.size()) != want) return 8;
        if (rw(SELF + 1476) != (cs.alpha != 0 ? 1u : 0u)) return 9;
        if (rf(SELF + 1472) != static_cast<float>(cs.alpha) * bf(0x3b808080)) return 10;
        if (rf(SELF + 1544) != 0.25f) return 11;
    }
    // 3. The star fade scales: hour 0 gives F[1564] = 1; pair (0,1) multiplies by 1 - t, pair (1,0) by t.
    arm(0.25f, 0.0f, 0, 1, 0, 0); go(0.0f, 1.0f);
    if (rf(SELF + 1564) != 1.0f * (1.0f - 0.25f)) return 12;
    arm(0.25f, 0.0f, 1, 0, 0, 0); go(0.0f, 1.0f);
    if (rf(SELF + 1564) != 1.0f * 0.25f) return 13;
    arm(0.25f, 0.0f, 1, 1, 0, 0); go(0.0f, 1.0f);
    if (rf(SELF + 1564) != 0.0f) return 14;
    arm(0.25f, 0.0f, 0, 0, 0, 0); go(0.0f, 1.0f);
    if (rf(SELF + 1564) != 1.0f) return 15;
    // 4. t advances by dt * 0x3d088888 and is capped at 1.0.
    arm(0.25f, 3.0f, 0, 0, 0, 0); go(12.0f, 1.0f);
    if (rf(SELF + 1544) != 0.25f + 3.0f * bf(0x3d088888) && rf(SELF + 1544) != 0.0f) return 16;  // reaches 1.0 and rolls over to the next state
    arm(0.25f, 1.0f, 0, 0, 0, 0); go(12.0f, 1.0f);
    if (rf(SELF + 1544) != 0.25f + 1.0f * bf(0x3d088888)) return 17;
    // 5. A full frame with weather indices (3, 5): classes (1, 2) give F[128] = t; the target is
    //    lerp(levels[3], levels[5], t) = 1.0 + (0.5 - 1.0) * 0.25; the intensity was 0, so splashes are cleared;
    //    both rain sounds start; the sky material is set up for t < 0.5; the lightning timer expires.
    arm(0.25f, 0.0f, 0, 0, 3, 5);
    sf(SELF + 4504, 0.0f);
    go(12.0f, 0.5f);
    {
        const float t = 0.25f, target = 1.0f + (0.5f - 1.0f) * t;
        std::string want = fade(5, 1, 255) + fade(6, 2, 255) + fade(7, 1, 255);
        want += "clr;play41,127,0=501;reset;pan501=-1;play41,127,0=502;pan502=1;pitch502=4176;";
        const float vol = (target * 0.5f) * ((1.0f - t) * 0.5f);
        want += "vol501=" + Fs(vol) + ";vol502=" + Fs(vol) + ";";
        want += "spr3;pal3;ent0;blend0,3;num3;copy1<2;copy0<1;tex0.1=" + S(0x3333) + ";";
        want += "get0.1;set0.1=2,2,3,127;get0.2;set0.2=3,2,3,32;";
        const float lv = (((1.0f - 0.0f) * 0.5f) * ((1.0f - t) * target)) * bf(0x3f666666);
        want += "play42,127,0=503;vol503=" + Fs(lv) + ";pan503=-1;play42,127,0=504;vol504=" + Fs(lv) + ";pan504=1;pitch504=2496;reset;";
        want += "vol501=" + Fs(vol) + ";vol502=" + Fs(vol) + ";";
        if (trace != want) { std::printf("%s\n%s\n", trace.c_str(), want.c_str()); return 18; }
        if (rf(SELF + 128) != t || rf(SELF + 72) != target) return 19;
        if (rw(SELF + 4496) != 501 || rw(SELF + 4500) != 502) return 20;
    }
    // 6. Weather classes: F[128] by (class of W[1532], class of W[1536]). Index 1..3 is class 1, 4.. class 2.
    struct Wind { int c, d; float want_f128; } winds[] = {
        {1, 4, 0.25f}, {4, 1, 0.75f}, {1, 1, 0.0f}, {1, 0, 0.0f}, {0, 1, 0.0f}, {4, 4, 1.0f}, {4, 0, 1.0f}, {0, 4, 1.0f}, {0, 0, 1.0f}, {3, 4, 0.25f}, {6, 3, 0.75f}, {-2, 2, 0.0f}};
    for (const Wind& w : winds) {
        arm(0.25f, 0.0f, 0, 0, w.c, w.d);
        sw(SELF + 4496, 9); sw(SELF + 4500, 9);  // sounds already running
        go(12.0f, 1.0f);
        if (w.c <= 0 && w.d <= 0) continue;
        if (rf(SELF + 128) != w.want_f128) return 21;
    }
    // 7. Without weather (both indices <= 0): intensity 0, running sounds are stopped and reset to -1;
    //    a handle of 0 is left alone, a handle of -1 is also "stopped" (only 0 is skipped).
    arm(0.25f, 0.0f, 0, 0, 0, -3);
    sw(SELF + 4496, 9); sw(SELF + 4500, 0); sf(SELF + 72, 0.7f);
    go(12.0f, 1.0f);
    {
        std::string want = fade(5, 1, 255) + fade(6, 2, 255) + fade(7, 1, 255) + "stop9;";
        if (trace.substr(0, want.size()) != want || trace.find("stop0") != std::string::npos) return 22;
        if (rw(SELF + 4496) != 0xffffffffu || rw(SELF + 4500) != 0u || rf(SELF + 72) != 0.0f) return 23;
    }
    // 8. Sky material for 0.5 <= t < 1.0: the sprites 5..7 have no palette entries here, so only the size
    //    queries show. Entry 0 with one layer gets two layers; alpha (int)(((t - 0.5) * 2) * 64) = 32.
    const std::string nofade = "spr5;pal5;size;spr6;pal6;size;spr7;pal7;size;";
    arm(0.75f, 0.0f, 0, 0, 0, 0);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sw(ENT + 8, 1);
    go(12.0f, 1.0f);
    if (trace != nofade + "spr3;pal3;ent0;copy1<0;copy2<1;num2;tex0.1=" + S(0x3333) + ";get0.1;set0.1=2,2,3,32;") { std::printf("%s\n", trace.c_str()); return 24; }
    //    With two layers already present the setup calls are skipped.
    arm(0.75f, 0.0f, 0, 0, 0, 0);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    go(12.0f, 1.0f);
    if (trace != nofade + "spr3;pal3;ent0;get0.1;set0.1=2,2,3,32;") return 25;
    //    First half with three layers present: the blend mode is set, the setup is skipped.
    arm(0.25f, 0.0f, 0, 0, 0, 0);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sw(ENT + 8, 3);
    go(12.0f, 1.0f);
    if (trace != nofade + "spr3;pal3;ent0;blend0,3;get0.1;set0.1=2,2,3,127;get0.2;set0.2=3,2,3,32;") return 26;
    //    With one layer present the first-half setup runs.
    arm(0.25f, 0.0f, 0, 0, 0, 0);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sw(ENT + 8, 1);
    go(12.0f, 1.0f);
    if (trace != nofade + "spr3;pal3;ent0;blend0,3;num3;copy1<2;copy0<1;tex0.1=" + S(0x3333) + ";get0.1;set0.1=2,2,3,127;get0.2;set0.2=3,2,3,32;") return 27;
    // 9. t reaches 1.0 (0.98 + 1.0 * 0.0333 capped): layer textures and alphas, then the state advance.
    struct Adv { int b, next_weather, cur, nxt, flag, held, sky; unsigned want_1520, want_1552; float want_t; unsigned want_1548, want_1528, want_1536; };
    // Fixed words: W[1528] = 0x3333, W[1232] = 1001, W[1236] = 1002, W[1240] = 1003, W[1244] = 1004.
    struct { Adv a; } rows[] = {
        // nxt == cur: the first rule is skipped (W[1548] still takes cur).
        {{1, 4, 2, 2, 1, 4, 3, 1, 2, 0.0f, 2, 0x3333, 4}},
        // flag clear: nothing more than the copy.
        {{1, 4, 2, 3, 0, 4, 3, 1, 2, 0.0f, 2, 0x3333, 4}},
        // nxt == 0, weather <= 0: W[1520] = 0, W[1528] = W[1232], t = 1e-5.
        {{1, 0, 2, 0, 1, 0, 3, 0, 0, bf(0x3727c5ac), 2, 1001, 0}},
        // nxt == 0, weather > 0: t restored to 0, W[1548] = 0.
        {{1, 4, 2, 0, 1, 4, 3, 1, 0, 0.0f, 0, 0x3333, 4}},
        // nxt == 1, weather <= 0: W[1528] = W[1236].
        {{1, 0, 2, 1, 1, 0, 3, 0, 1, bf(0x3727c5ac), 2, 1002, 0}},
        // nxt == 1, weather > 0: W[1548] = 1.
        {{1, 4, 2, 1, 1, 4, 3, 1, 1, 0.0f, 1, 0x3333, 4}},
        // nxt == 3, weather <= 0: W[1520] = 1, W[1528] = W[1240].
        {{0, 0, 2, 3, 1, 0, 3, 1, 3, bf(0x3727c5ac), 2, 1003, 0}},
        // nxt == 3, weather > 0: W[1548] = 3.
        {{1, 5, 2, 3, 1, 5, 3, 1, 3, 0.0f, 3, 0x3333, 5}},
    };
    for (const auto& row : rows) {
        const Adv& r = row.a;
        arm(0.98f, 1.0f, 0, r.b, 0, r.next_weather);
        pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
        sw(SELF + 4496, 0); sw(SELF + 4500, 0);
        sw(SELF + 1232, 1001); sw(SELF + 1236, 1002); sw(SELF + 1240, 1003); sw(SELF + 1244, 1004);
        sw(SELF + 1552, r.cur); sw(SELF + 1556, r.nxt); sw(SELF + 1228, r.flag); sw(SELF + 1540, r.held); sw(SELF + 1548, 9);
        go(12.0f, 1.0f);
        if (trace.find("spr3;pal3;ent0;tex0.0=13107;tex0.1=13107;get0.0;set0.0=1,2,3,255;get0.1;set0.1=2,2,3,64;") == std::string::npos) return 28;
        if (rw(SELF + 1516) != static_cast<unsigned>(r.b) || rw(SELF + 1524) != 0x3333u) return 29;
        if (rw(SELF + 1532) != static_cast<unsigned>(r.next_weather)) return 30;
        if (rw(SELF + 1520) != r.want_1520) return 31;
        if (rw(SELF + 1552) != r.want_1552) return 32;
        if (rf(SELF + 1544) != r.want_t) return 33;
        if (rw(SELF + 1548) != r.want_1548) return 34;
        if (rw(SELF + 1528) != r.want_1528) return 35;
        (void)r.want_1536;
    }
    //    Second rule: W[1540] differs from the adopted weather index, the flag is set and t is 0.
    arm(0.98f, 1.0f, 0, 1, 0, 2);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sw(SELF + 1232, 1001); sw(SELF + 1236, 1002); sw(SELF + 1240, 1003);
    sw(SELF + 1552, 4); sw(SELF + 1556, 4); sw(SELF + 1228, 1); sw(SELF + 1540, 6); sw(SELF + 1548, 5);
    go(12.0f, 1.0f);
    if (rw(SELF + 1536) != 6u || rw(SELF + 1520) != 1u || rw(SELF + 1528) != 1003u || rf(SELF + 1544) != bf(0x3727c5ac) || rw(SELF + 1540) != 6u) return 36;
    //    With W[1540] == 0 the sky state W[1548] picks the handle: 2 -> W[1520] = 1, W[1232 + 4 * 2] = W[1240].
    arm(0.98f, 1.0f, 0, 1, 0, 2);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sw(SELF + 1232, 1001); sw(SELF + 1236, 1002); sw(SELF + 1240, 1003);
    sw(SELF + 1552, 2); sw(SELF + 1556, 2); sw(SELF + 1228, 1); sw(SELF + 1540, 0); sw(SELF + 1548, 9);
    go(12.0f, 1.0f);
    if (rw(SELF + 1520) != 1u || rw(SELF + 1528) != 1003u || rw(SELF + 1536) != 0u) return 37;  // W[1548] = W[1552] = 2: W[1520] = (2 == 2)
    arm(0.98f, 1.0f, 0, 1, 0, 2);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sw(SELF + 1232, 1001); sw(SELF + 1236, 1002); sw(SELF + 1240, 1003);
    sw(SELF + 1552, 1); sw(SELF + 1556, 1); sw(SELF + 1228, 1); sw(SELF + 1540, 0); sw(SELF + 1548, 9);
    go(12.0f, 1.0f);
    if (rw(SELF + 1536) != 0u || rw(SELF + 1520) != 0u || rw(SELF + 1528) != 1002u || rf(SELF + 1544) != bf(0x3727c5ac)) return 38;
    // 10. Lightning timer. Weather indices (1, 1) give intensity 0.15 and F[128] = 0.
    auto lightning = [&](float timer, float other, float dt) {
        arm(0.25f, dt, 0, 0, 1, 1);
        pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
        sw(SELF + 4496, 7); sw(SELF + 4500, 8);
        sf(SELF + 4504, timer); sf(SELF + 4508, other); sf(SELF + 4512, 0.75f); sf(SELF + 4520, 0.5f);
        go(12.0f, 2.0f);
    };
    lightning(2.0f, 1.0f, 0.5f);
    if (rf(SELF + 4504) != 1.5f || rf(SELF + 4508) != 1.0f || rf(SELF + 4512) != 0.75f || trace.find("play42") != std::string::npos) return 39;
    lightning(0.5f, 1.0f, 0.5f);
    {
        if (rf(SELF + 4504) != 0.0f || rf(SELF + 4508) != 0.0f || rf(SELF + 4512) != bf(0x3727c5ac)) return 40;
        const float intensity = 0.15f;  // lerp(levels[1], levels[1], t)
        const float lv = (((1.0f - 0.5f) * 2.0f) * ((1.0f - 0.0f) * bf(0x3e199999))) * bf(0x3f666666);
        (void)intensity;
        if (trace.find("play42,127,0=501;vol501=" + Fs(lv) + ";pan501=-1;play42,127,0=502;vol502=" + Fs(lv) + ";pan502=1;pitch502=2496;reset;") == std::string::npos) { std::printf("%s\n", trace.c_str()); return 41; }
    }
    lightning(0.25f, 1.0f, 0.5f);  // 0.25 < 1.0: the second timer resets, then 0.25 - 0.5 < 0 plays the sound
    if (rf(SELF + 4504) != -0.25f || rf(SELF + 4508) != 0.0f || trace.find("play42") == std::string::npos) return 42;
    lightning(1.0f, 1.0f, 0.5f);  // not below: nothing reset
    if (rf(SELF + 4508) != 1.0f || rf(SELF + 4512) != 0.75f) return 43;
    // 11. F[128] at 1.0 skips the lightning block and the volume refresh: weather (0, 4) has class (0, 2).
    arm(0.25f, 0.5f, 0, 0, 0, 4);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 5); sw(SELF + 4500, 6); sf(SELF + 4504, 0.1f);
    go(12.0f, 1.0f);
    if (rf(SELF + 128) != 1.0f || rf(SELF + 4504) != 0.1f || trace.find("play42") != std::string::npos) return 44;
    // The second sound handle is only refreshed when it is nonzero; a zero first handle is skipped.
    arm(0.25f, 0.5f, 0, 0, 1, 1);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sf(SELF + 4504, 9.0f);
    go(12.0f, 1.0f);
    if (trace.find("vol") != std::string::npos) return 45;
    // 12. Boundaries and exact traces. t = 0.5 exactly takes the second variant (two layers), t = 1.0 takes only
    //     the final block; the first-variant alphas use 255 and 64 exactly (t = 0.3125 gives 159 and 24).
    arm(0.5f, 0.0f, 0, 0, 0, 0);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0); sw(ENT + 8, 1);
    go(12.0f, 1.0f);
    if (trace != nofade + "spr3;pal3;ent0;copy1<0;copy2<1;num2;tex0.1=" + S(0x3333) + ";get0.1;set0.1=2,2,3,0;") return 46;
    arm(0.3125f, 0.0f, 0, 0, 0, 0);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0); sw(ENT + 8, 3);
    go(12.0f, 1.0f);
    if (trace != nofade + "spr3;pal3;ent0;blend0,3;get0.1;set0.1=2,2,3,159;get0.2;set0.2=3,2,3,24;") return 47;
    arm(0.98f, 1.0f, 0, 1, 0, 0);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sw(SELF + 1552, 2); sw(SELF + 1556, 2);
    go(12.0f, 1.0f);
    if (trace != nofade + "spr3;pal3;ent0;tex0.0=13107;tex0.1=13107;get0.0;set0.0=1,2,3,255;get0.1;set0.1=2,2,3,64;") return 48;
    //     A first rule that sets t = 1e-5 keeps the second rule from adopting W[1540].
    arm(0.98f, 1.0f, 0, 1, 0, 0);
    pal_entries[5] = pal_entries[6] = pal_entries[7] = 0;
    sw(SELF + 4496, 0); sw(SELF + 4500, 0);
    sw(SELF + 1232, 1001); sw(SELF + 1552, 3); sw(SELF + 1556, 0); sw(SELF + 1228, 1); sw(SELF + 1540, 6);
    go(12.0f, 1.0f);
    if (rw(SELF + 1536) != 0u || rw(SELF + 1520) != 0u || rw(SELF + 1528) != 1001u || rf(SELF + 1544) != bf(0x3727c5ac) || rw(SELF + 1552) != 0u) return 49;
    return 0;
}
