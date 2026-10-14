// Host check for runMusic__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define memset norrath_host_memset
#define strcmp norrath_host_strcmp
#define strstr norrath_host_strstr
#include "run_music.cpp"
#undef memset
#undef strcmp
#undef strstr

extern "C" {
signed char cutMode = 0;
int musicTimer = 100;
unsigned eRandState = 7;
int frameNm = 0;
int drawFrameNm = 0;
char levelName[400];
short currScorePos = 0;
short startScorePos = 0;
short numEnvironments = 0;
float currActivityScore = 0.0f;
int lastMusicState = 0;
int timeLastTransition = 0;
float soundMusicVolume = 1.0f;
signed char norrath_music_activity_ring_view[32];
}

static std::string trace;
static bool zero_on_memset = true;
static int flag_value = 0;                 // soundAllFlags returns this ...
static int flag_late = 0;                  // ... except from the ninth call on, which return this
static int flag_calls = 0;
static bool late_enabled = false;   // the late value only applies inside the state tests
static int stream_status = 0;
static char dir_store[16 * 80], gdir_store[16 * 80];
static bool have_dir = true, have_gdir = true;

extern "C" void* norrath_host_memset(void* d, int v, unsigned n) {
    trace += "M" + std::to_string(v) + "," + std::to_string(n) + (d == norrath_music_activity_ring_view ? "" : "?") + ";";
    if (zero_on_memset) std::memset(d, v, n);
    return d;
}
extern "C" int norrath_host_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
extern "C" char* norrath_host_strstr(const char* a, const char* b) { return const_cast<char*>(std::strstr(a, b)); }
extern "C" int soundAllFlags__Fi(int i) {
    trace += "F" + std::to_string(i) + ";";
    return (late_enabled && flag_calls++ >= 8) ? flag_late : flag_value;
}
extern "C" int soundGetStreamPlayStatus__Fi(int i) { trace += "P" + std::to_string(i) + ";"; return stream_status; }
static bool blank_first_dir = false;   // the equal-position start before the state step sees an empty directory
static char blank_dir[16 * 80];
extern "C" char* soundGetDialogDir__Fv() {
    trace += "D;";
    if (blank_first_dir) { blank_first_dir = false; return blank_dir; }
    return have_dir ? dir_store : nullptr;
}
extern "C" char* soundGetGlobalDialogDir__Fv() { trace += "G;"; return have_gdir ? gdir_store : nullptr; }
extern "C" void soundStartMusic__FPci(char* n, int mode) { trace += std::string("SM[") + n + "," + std::to_string(mode) + "];"; }
extern "C" void soundStartGlobalMusic__FPci(char* n, int mode) { trace += std::string("SG[") + n + "," + std::to_string(mode) + "];"; }
extern "C" void soundStartDialogStream__Fiii(int s, int i, int m) { trace += "SD[" + std::to_string(s) + "," + std::to_string(i) + "," + std::to_string(m) + "];"; }

static void setDir(char* store, std::vector<std::string> names) {
    std::memset(store, 0, 16 * 80);
    for (size_t i = 0; i < names.size(); ++i) std::strcpy(store + i * 80, names[i].c_str());
}
static void reset() {
    trace.clear(); cutMode = 0; musicTimer = 100; eRandState = 7; frameNm = 30; drawFrameNm = 1;
    std::strcpy(levelName + 81, "plains"); currScorePos = 0; startScorePos = 0; numEnvironments = 0;
    currActivityScore = 0.0f; lastMusicState = 0; timeLastTransition = 0; soundMusicVolume = 1.0f;
    std::memset(norrath_music_activity_ring_view, 0, 32);
    zero_on_memset = true; flag_value = 0; flag_late = 0; flag_calls = 0; late_enabled = false; blank_first_dir = false; stream_status = 0; have_dir = have_gdir = true;
    setDir(dir_store, {"xx_amb_1", "plain"});
    setDir(gdir_store, {"mx_boss_g1", "other", "mx_boss_g2", "mx_boss_g3"});
}
static unsigned lcg(unsigned s) { return s * 1664525u + 1013904223u; }

int main() {
    // ---- Entry and path selection ----
    reset(); frameNm = 31;
    runMusic__Fv();
    if (trace != "M0,32;") return 1;                     // path C, not a multiple of 30: only the memset
    reset(); cutMode = 1; drawFrameNm = 5; musicTimer = 5;
    runMusic__Fv();
    if (trace != "M0,32;" || musicTimer != 4) return 2;   // path B, timer counts down, then the frame check ends
    reset(); std::strcpy(levelName + 81, "kelsel"); drawFrameNm = 5; musicTimer = 5; frameNm = 31;
    runMusic__Fv();
    if (musicTimer != 4) return 3;
    reset(); std::strcpy(levelName + 81, "kelsel2"); drawFrameNm = 5; musicTimer = 5; frameNm = 31;
    runMusic__Fv();
    if (musicTimer != 4) return 4;
    reset(); std::strcpy(levelName + 81, "kelsel3"); drawFrameNm = 5; musicTimer = 5; frameNm = 31;
    runMusic__Fv();
    if (musicTimer != 5) return 5;                        // other names stay on path C

    // ---- Path B ----
    reset(); cutMode = 1; drawFrameNm = 16; musicTimer = 5;
    runMusic__Fv();
    if (trace != "M0,32;P0;D;SD[0,0,3];") return 6;       // first "amb_" entry
    reset(); cutMode = 1; drawFrameNm = 16; stream_status = 1;
    runMusic__Fv();
    if (trace != "M0,32;P0;") return 7;
    reset(); cutMode = 1; drawFrameNm = 16; have_dir = false;
    runMusic__Fv();
    if (trace != "M0,32;P0;D;") return 8;
    reset(); cutMode = 1; drawFrameNm = 16; setDir(dir_store, {"a", "b"});
    runMusic__Fv();
    if (trace != "M0,32;P0;D;") return 9;
    // Expiry: timer reset from the random state, first start from "_env_" or "mx_theme".
    reset(); cutMode = 1; drawFrameNm = 1; musicTimer = 1;
    setDir(dir_store, {"a", "zz_mx_theme", "b_env_q"});
    runMusic__Fv();
    {
        const unsigned s = lcg(7);
        if (eRandState != s || musicTimer != 14400 + (int)((s >> 16) % 7200u)) return 10;
        if (trace != "M0,32;F0;D;SM[zz_mx_theme,2];") return 11;
    }
    reset(); cutMode = 1; drawFrameNm = 1; musicTimer = 0; flag_value = 0x20;
    runMusic__Fv();
    if (trace != "M0,32;F0;") return 12;
    reset(); cutMode = 1; drawFrameNm = 1; musicTimer = 1; have_dir = false;
    runMusic__Fv();
    if (trace != "M0,32;F0;D;") return 13;
    reset(); cutMode = 1; drawFrameNm = 1; musicTimer = 1; setDir(dir_store, {"x"});
    runMusic__Fv();
    if (trace != "M0,32;F0;D;") return 14;

    // ---- Path C: flag handling (every soundAllFlags call returns one constant) ----
    // A set bit with 0x200 clear: first scan stops at the set bit, 0x200 is read, the rescan stops at it again.
    reset(); frameNm = 60; flag_value = 0x20; currScorePos = 3; norrath_music_activity_ring_view[3] = 5; zero_on_memset = false;
    runMusic__Fv();
    if (trace != "M0,32;F0;F0;F0;") return 15;
    if (norrath_music_activity_ring_view[3] != 0 || currScorePos != 3) return 16;
    // The set bit is the last of the four: each scan reads four times.
    reset(); frameNm = 60; flag_value = 0x100; currScorePos = 2; norrath_music_activity_ring_view[2] = 5; zero_on_memset = false;
    runMusic__Fv();
    if (trace != "M0,32;F0;F0;F0;F0;F0;F0;F0;F0;F0;") return 17;
    if (norrath_music_activity_ring_view[2] != 0) return 18;
    // With 0x200 also set the rescan is skipped and the average runs.
    reset(); frameNm = 60; flag_value = 0x220; currScorePos = 2; startScorePos = 2; numEnvironments = 0; lastMusicState = 99;
    runMusic__Fv();
    if (trace != "M0,32;F0;F0;D;" || currScorePos != 3) return 19;     // equal positions: theme path asks for the directory
    // No bit set: eight reads, then the average.
    reset(); frameNm = 60; flag_value = 0; currScorePos = 2; startScorePos = 2; lastMusicState = 99;
    runMusic__Fv();
    if (trace != "M0,32;F0;F0;F0;F0;F0;F0;F0;F0;D;") return 20;
    // ---- Path C: average ----
    reset(); frameNm = 60; zero_on_memset = false; lastMusicState = 99; startScorePos = 30; currScorePos = 2;
    {
        // Positions 2, 1, 0 and 31 are visited walking backward; the start position 30 itself is not.
        const int values[] = {4, -3, 7, 1};
        const int pos[] = {2, 1, 0, 31};
        for (int i = 0; i < 4; ++i) norrath_music_activity_ring_view[pos[i]] = (signed char)values[i];
        norrath_music_activity_ring_view[30] = 100;
        float sum = 0.0f;
        for (int k = 0; k < 4; ++k) sum = sum + (float)values[k] * ((float)((32 - k) * (32 - k)) * 0.0009765625f);
        const float want = sum / 4.0f;
        runMusic__Fv();
        if (currActivityScore != want) { std::fprintf(stderr, "score %f want %f\n", currActivityScore, want); return 21; }
        if (currScorePos != 3 || startScorePos != 30 || norrath_music_activity_ring_view[3] != 0) return 22;
    }
    // One step back from the start position: a single weighted sample.
    reset(); frameNm = 60; zero_on_memset = false; lastMusicState = 99; startScorePos = 5; currScorePos = 6;
    norrath_music_activity_ring_view[6] = 8;
    runMusic__Fv();
    if (currActivityScore != 8.0f * ((float)(32 * 32) * 0.0009765625f) / 1.0f) return 23;
    // ---- Ring advance ----
    reset(); frameNm = 60; lastMusicState = 99; currScorePos = 31; startScorePos = 5;
    runMusic__Fv();
    if (currScorePos != 0) return 24;
    reset(); frameNm = 60; lastMusicState = 99; currScorePos = 4; startScorePos = 5; zero_on_memset = false;
    norrath_music_activity_ring_view[5] = 9;
    runMusic__Fv();
    if (currScorePos != 5 || startScorePos != 6 || norrath_music_activity_ring_view[5] != 0) return 25;
    reset(); frameNm = 60; lastMusicState = 99; currScorePos = 30; startScorePos = 31;
    runMusic__Fv();
    if (currScorePos != 31 || startScorePos != 0) return 26;
    reset(); frameNm = 60; lastMusicState = 99; currScorePos = 31; startScorePos = 0;
    runMusic__Fv();
    if (currScorePos != 0 || startScorePos != 1) return 27;
    // ---- Equal-position starts ----
    reset(); frameNm = 60; lastMusicState = 99; numEnvironments = 0; setDir(dir_store, {"a", "mx_theme_x", "b"});
    runMusic__Fv();
    if (trace.find("D;SM[mx_theme_x,514];") == std::string::npos || lastMusicState != 0 || timeLastTransition != 60) return 28;
    reset(); frameNm = 60; lastMusicState = 99; numEnvironments = 3; setDir(dir_store, {"mx_env1", "q", "mx_env2", "mx_env3"});
    runMusic__Fv();
    {
        const unsigned st = lcg(7);
        const int n = (int)((st >> 16) % 3u) + 1;
        const char* names[] = {"", "mx_env1", "mx_env2", "mx_env3"};
        std::string want = std::string("D;SM[") + names[n] + ",514];";
        if (trace.find(want) == std::string::npos || lastMusicState != 1 || eRandState != st) return 29;
    }
    reset(); frameNm = 60; lastMusicState = 99; numEnvironments = 2; have_dir = false;
    runMusic__Fv();
    if (eRandState != 7 || lastMusicState != 99) return 30;        // no directory: no random step, no start
    reset(); frameNm = 60; lastMusicState = 99; numEnvironments = 2; setDir(dir_store, {"x"});
    runMusic__Fv();
    if (eRandState != lcg(7) || lastMusicState != 99) return 31;   // empty match: random step but no start

    // ---- State machine. Equal positions keep the score under test; the blank first directory
    // makes the equal-position start a no-op. ----
    auto state = [](int st, float score, int env = 0) {
        reset(); frameNm = 90; lastMusicState = st; currActivityScore = score; currScorePos = 7; startScorePos = 7;
        numEnvironments = (short)env; blank_first_dir = true; timeLastTransition = 0; late_enabled = true;
    };
    // After the blank start: "D;" then the state's own calls. Strip the common prefix for readability.
    auto after = []() {
        const std::string pre = "M0,32;F0;F0;F0;F0;F0;F0;F0;F0;D;";
        return trace.compare(0, pre.size(), pre) == 0 ? trace.substr(pre.size()) : std::string("<prefix:") + trace + ">";
    };
    const float A = 1.742f + 0.001f, B = 1.072f;
    // State 2.
    state(2, 0.2f); setDir(dir_store, {"a", "x_amb_9"});
    runMusic__Fv();
    if (after() != "D;SD[0,1,515];" || lastMusicState != -1 || timeLastTransition != 90) return 40;
    state(2, 0.25f); runMusic__Fv();
    if (after() != "") return 41;
    state(2, 0.3f); runMusic__Fv();
    if (after() != "") return 42;
    state(2, 0.2f); have_dir = false; runMusic__Fv();
    if (after() != "D;" || lastMusicState != 2) return 43;
    state(2, 0.2f); setDir(dir_store, {"a"}); runMusic__Fv();
    if (after() != "D;") return 44;
    // State 1: boss attempt above A, then the ambient start unless 0x20 is set with a volume.
    state(1, 1.0f); setDir(dir_store, {"x_amb_1"}); runMusic__Fv();
    if (after() != "F0;D;SD[0,0,515];") return 45;
    state(1, 1.0f); setDir(dir_store, {"x_amb_1"}); flag_late = 0x20; soundMusicVolume = 0.5f; runMusic__Fv();
    if (trace.find("SD[") != std::string::npos) return 46;
    state(1, 1.0f); setDir(dir_store, {"x_amb_1"}); flag_late = 0x20; soundMusicVolume = 0.0f; runMusic__Fv();
    if (trace.find("SD[0,0,515];") == std::string::npos) return 47;
    // Global boss music: the nth "mx_boss_g" entry, n from the random step.
    state(1, A); setDir(dir_store, {"x_amb_1"}); runMusic__Fv();
    {
        const unsigned st = lcg(7);
        const int n = (int)((st >> 16) & 3u) + 1;
        const char* names[] = {"", "mx_boss_g1", "mx_boss_g2", "mx_boss_g3", ""};
        if (n <= 3) {
            if (trace.find(std::string("G;SG[") + names[n] + ",515];") == std::string::npos) return 48;
        } else {
            if (trace.find("SG[") != std::string::npos) return 49;
        }
    }
    auto bitsF = [](unsigned b) { float f; std::memcpy(&f, &b, 4); return f; };
    const float exactB = bitsF(0x3f893719u), exactA = bitsF(0x3fdef96eu);
    // State 0.
    state(0, 0.5f); setDir(dir_store, {"a", "x_amb_1"}); runMusic__Fv();
    if (after() != "F0;D;SD[0,1,515];" || lastMusicState != -1 || timeLastTransition != 90) return 50;
    state(0, 0.5f); setDir(dir_store, {"a", "x_amb_1"}); flag_late = 0x20; runMusic__Fv();
    if (after() != "F0;" || lastMusicState != 0) return 51;
    state(0, -0.5f); setDir(dir_store, {"a", "x_amb_1"}); runMusic__Fv();
    if (after() != "F0;") return 52;
    state(0, 1.2f); setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (after() != "F0;D;SM[q_mx_theme,514];" || lastMusicState != 0 || timeLastTransition != 90) return 53;
    state(0, exactB); setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (after() != "F0;D;SM[q_mx_theme,514];") return 54;
    state(0, exactA); setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (trace.find("SM[") != std::string::npos) return 55;
    {
        // Environments: the pre-start sees the blank directory, so the state step does the random pick.
        state(0, 1.2f, 3); setDir(dir_store, {"mx_env1", "q", "mx_env2", "mx_env3"}); runMusic__Fv();
        // Two random steps happen: one for the blank pre-start, one for the state step.
        const unsigned s1 = lcg(7), s2 = lcg(s1);
        const int n = (int)((s2 >> 16) % 3u) + 1;
        const char* names[] = {"", "mx_env1", "mx_env2", "mx_env3"};
        if (after() != std::string("F0;D;SM[") + names[n] + ",514];" || lastMusicState != 1 || eRandState != s2) return 56;
    }
    // Boss attempt above A, then the ladder still runs on the same score (not below A, so it stops).
    state(0, 2.0f); setDir(dir_store, {"a", "x_amb_1"}); runMusic__Fv();
    if (trace.find("G;") == std::string::npos || trace.find("SD[") != std::string::npos) return 57;
    // Global directory missing: no random step for the boss pick.
    state(0, 2.0f); have_gdir = false; runMusic__Fv();
    if (eRandState != 7 || trace.find("SG[") != std::string::npos) return 58;
    // State -1.
    state(-1, 0.5f); frameNm = 4020; timeLastTransition = 0; setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (after() != "D;SM[q_mx_theme,514];" || lastMusicState != 0 || timeLastTransition != 4020) return 60;
    state(-1, 0.5f); frameNm = 4020; timeLastTransition = 420; setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (after() != "" || lastMusicState != -1) return 61;
    state(-1, 0.5f); frameNm = 4020; timeLastTransition = 419; setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (lastMusicState != 0) return 62;
    state(-1, -0.5f); frameNm = 4020; timeLastTransition = 0; setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (after() != "" || lastMusicState != -1) return 63;
    state(-1, 1.5f); frameNm = 4020; timeLastTransition = 0; setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (after() != "D;SM[q_mx_theme,514];" || lastMusicState != 0) return 64;
    state(-1, exactA); frameNm = 4020; timeLastTransition = 0; setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (after() != "" ) return 65;
    state(-1, 0.5f); frameNm = 4020; timeLastTransition = 0; have_dir = false; runMusic__Fv();
    if (after() != "D;" || lastMusicState != -1) return 66;
    state(-1, 0.5f); frameNm = 4020; timeLastTransition = 0; setDir(dir_store, {"a"}); runMusic__Fv();
    if (after() != "D;" || lastMusicState != -1) return 67;
    // Unknown states do nothing.
    state(5, 0.5f); runMusic__Fv();
    if (after() != "") return 68;
    // Boss start records state 2 and the time; several seeds choose n from two bits.
    for (unsigned seed = 1; seed < 40; ++seed) {
        state(1, 2.0f); setDir(dir_store, {"a"}); eRandState = seed; runMusic__Fv();
        const unsigned st = lcg(seed);
        const int n = (int)((st >> 16) & 3u) + 1;
        const bool found = n <= 3;
        if (found && (lastMusicState != 2 || timeLastTransition != 90)) return 70;
        if (!found && lastMusicState != 1) return 69;
        if (found && trace.find(std::string("SG[mx_boss_g") + (char)('0' + n) + ",515];") == std::string::npos) return 71;
        if (!found && trace.find("SG[") != std::string::npos) return 72;
    }
    state(1, 2.0f); setDir(dir_store, {"a"}); flag_late = 0x20; soundMusicVolume = 0.5f; eRandState = 1; runMusic__Fv();  // seed 1 picks an entry
    // state 1 with a volume and bit 0x20 returns right after the boss attempt: the boss state is kept.
    if (lastMusicState != 2 || timeLastTransition != 90) return 73;
    // A score of exactly zero is inside the lower band.
    state(0, 0.0f); setDir(dir_store, {"a", "x_amb_1"}); runMusic__Fv();
    if (after() != "F0;D;SD[0,1,515];") return 74;
    state(-1, 0.0f); frameNm = 4020; timeLastTransition = 0; setDir(dir_store, {"a", "q_mx_theme"}); runMusic__Fv();
    if (after() != "D;SM[q_mx_theme,514];") return 75;
    return 0;
}
