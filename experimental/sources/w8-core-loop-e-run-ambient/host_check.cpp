// Host check for runAmbient__Fv. Not an EE plan input.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#define sprintf norrath_host_sprintf
#include "run_ambient.cpp"
#undef sprintf

extern "C" {
unsigned char stream[1356];
alignas(8) unsigned char ambientGroup[680 * 4];
int nmAmbientGroups = 1;
unsigned eRandState = 1;
float soundMusicVolume = 0.25f;
float streamVolume[3] = {0, 0, 0};
char engineCurrentLevelName[80] = "lvl";
}
static std::string trace;
static int status2 = 0;
static float scale_value = 0.5f;
static unsigned char dialog_store[200];
static bool have_dialog = true;
static std::string found_name;

extern "C" int soundGetStreamPlayStatus__Fi(int s) { trace += "G" + std::to_string(s) + ";"; return status2; }
extern "C" float animGetVolumeScale__Fii(int a, int b) { trace += "V" + std::to_string(a) + "," + std::to_string(b) + ";"; return scale_value; }
extern "C" void soundUpdateAdjustments__Fv() { trace += "U;"; }
extern "C" unsigned char* soundFindDialog__FPc(char* n) { found_name = n; trace += std::string("D[") + n + "];"; return have_dialog ? dialog_store : nullptr; }
extern "C" int norrath_host_sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f);
    std::vsnprintf(d, 80, f, ap);
    va_end(ap);
    trace += std::string("F[") + d + "];";
    return 0;
}
extern "C" void soundStartStream__FiPciiiiii(int s, char* n, int v, int a, int b, int c, int d, int e) {
    trace += "S" + std::to_string(s) + "," + n + "," + std::to_string(v) + "," + std::to_string(a) + "," + std::to_string(b)
        + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + ";";
}
static void w32(unsigned char* p, int v) { for (int i = 0; i < 4; ++i) p[i] = (unsigned char)((unsigned)v >> (8 * i)); }
static int next_state(unsigned s) { return (int)(s * 1664525u + 1013904223u); }

static void reset(int count) {
    trace.clear();
    std::memset(stream, 0, sizeof stream);
    std::memset(ambientGroup, 0, sizeof ambientGroup);
    nmAmbientGroups = 1; eRandState = 1; status2 = 0; streamVolume[2] = 0.0f;
    unsigned char* g = ambientGroup;
    w32(g, 2); w32(g + 4, 6); w32(g + 8, count); w32(g + 12, 0); w32(g + 16, 7); w32(g + 20, 9);
    for (int i = 0; i < count; ++i) { std::snprintf((char*)g + 24 + i * 41, 40, "entry%d", i); g[64 + i * 41] = 0; }
    std::memset(dialog_store, 0, sizeof dialog_store);
    w32(dialog_store + 64, 100); w32(dialog_store + 144, 160); w32(dialog_store + 68, 5); dialog_store[72] = 0xfe; dialog_store[73] = 0xff;
    w32(dialog_store + 76, 77);
}

int main() {
    // Stream status bit blocks everything and touches nothing.
    reset(3); status2 = 0x20;
    runAmbient__Fv();
    if (trace != "G2;" || word_at(ambientGroup + 12) != 0) return 1;
    // Busy flags on stream records 2 or 0 (directly or through an entry) block.
    reset(3); w32(stream + 904 + 20, 1); w32(stream + 904 + 32, 0x20);
    runAmbient__Fv(); if (trace != "G2;") return 2;
    reset(3); w32(stream + 904 + 20, 0); w32(stream + 904 + 32, 0x20);   // flag word is ignored while +20 is zero
    runAmbient__Fv(); if (trace == "G2;") return 3;
    reset(3); w32(stream + 20, 1); w32(stream + 32, 0x20);
    runAmbient__Fv(); if (trace != "G2;") return 4;
    reset(3); w32(stream + 448, 2); w32(stream + 108 + 48, 0x20);
    runAmbient__Fv(); if (trace != "G2;") return 5;
    reset(3); w32(stream + 904 + 448, 1);
    runAmbient__Fv(); if (trace != "G2;") return 6;

    // Countdown still positive: only decremented.
    reset(3); w32(ambientGroup + 12, 5);
    runAmbient__Fv();
    if (word_at(ambientGroup + 12) != 4 || trace != "G2;" || eRandState != 1) return 7;
    // Zero countdown starts a sound: first LCG step sets the timer, second the choice.
    reset(3);
    runAmbient__Fv();
    {
        const unsigned s1 = 1u * 1664525u + 1013904223u;
        const unsigned s2 = s1 * 1664525u + 1013904223u;
        const int timer = (int)((2.0f + (float)(s1 >> 16) * 1.52587890625e-05f * 4.0f) * 60.0f);
        if (word_at(ambientGroup + 12) != timer) return 8;
        if (eRandState != s2) return 9;
        const int pick = (int)((s2 >> 16) % 3u);   // all three free; pick-th free entry among the first two else the last
        const int chosen = pick > 1 ? 2 : pick;
        char want[64];
        std::snprintf(want, sizeof want, "G2;V7,9;U;D[entry%d];F[lvl.vat];S2,lvl.vat,128,100,60,5,-2,77;", chosen);
        if (trace != want) { std::fprintf(stderr, "got %s\nwant %s\n", trace.c_str(), want); return 10; }
        if (ambientGroup[64 + chosen * 41] != 1) return 11;
        for (int i = 0; i < 3; ++i) if (i != chosen && ambientGroup[64 + i * 41] != 0) return 12;
        if (streamVolume[2] != 0.5f * 0.5f) return 13;
    }
    // Volume clamps: scale above 1 limits to 1, a negative scale to 0; an unchanged volume skips the update.
    reset(3); scale_value = 4.0f; soundMusicVolume = 0.9f;
    runAmbient__Fv();
    if (streamVolume[2] != 1.0f || trace.find("U;") == std::string::npos) return 14;
    reset(3); scale_value = -1.0f; streamVolume[2] = 0.0f;
    runAmbient__Fv();
    if (trace.find("U;") != std::string::npos) return 15;
    scale_value = 0.5f; soundMusicVolume = 0.25f;

    // Equal dialog words only look the dialog up; a missing dialog still marks the entry.
    reset(3); w32(dialog_store + 144, 100);
    runAmbient__Fv();
    if (trace.find("F[") != std::string::npos || trace.find("S2,") != std::string::npos) return 16;
    reset(3); have_dialog = false;
    runAmbient__Fv();
    have_dialog = true;
    if (trace.find("D[") == std::string::npos || trace.find("F[") != std::string::npos) return 17;
    {
        int flagged = 0;
        for (int i = 0; i < 3; ++i) flagged += ambientGroup[64 + i * 41];
        if (flagged != 1) return 18;
    }

    // No free entries: only the random state step and the timer.
    reset(2); ambientGroup[64] = 1; ambientGroup[64 + 41] = 1;
    runAmbient__Fv();
    if (trace != "G2;" || eRandState != (unsigned)(1u * 1664525u + 1013904223u)) return 19;

    // Fewer than two free entries clears all flags before marking the chosen one.
    reset(3); ambientGroup[64] = 1; ambientGroup[64 + 41] = 1;   // only entry 2 is free
    runAmbient__Fv();
    if (ambientGroup[64] != 0 || ambientGroup[64 + 41] != 0 || ambientGroup[64 + 82] != 1) return 20;
    // Two free entries keep the other flags: the marked entry stays marked.
    reset(4); ambientGroup[64 + 41 * 3] = 1;   // entries 0..2 free (three), mark one more
    ambientGroup[64 + 41 * 2] = 1;             // now two free: 0 and 1
    runAmbient__Fv();
    if (ambientGroup[64 + 41 * 3] != 1 || ambientGroup[64 + 41 * 2] != 1) return 23;
    // A countdown of one is the last skipped call.
    reset(3); w32(ambientGroup + 12, 1);
    runAmbient__Fv();
    if (word_at(ambientGroup + 12) != 0 || trace != "G2;") return 24;
    // A single-entry group always uses entry 0.
    reset(1);
    runAmbient__Fv();
    if (trace.find("D[entry0]") == std::string::npos) return 21;
    // The second group starts at 680 bytes and the counts are per group.
    reset(2); nmAmbientGroups = 2;
    std::memcpy(ambientGroup + 680, ambientGroup, 680);
    w32(ambientGroup + 12, 3);
    runAmbient__Fv();
    if (word_at(ambientGroup + 12) != 2 || word_at(ambientGroup + 680 + 12) == 0) return 22;
    (void)next_state;
    return 0;
}
