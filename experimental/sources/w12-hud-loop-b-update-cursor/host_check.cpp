// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// evalHermite is stubbed as the identity on its first argument; getMoveDir returns a preset direction.
#include "update_cursor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
signed char multiHelpMessageOn[4];
int sellMode[4];
signed char helpMessageOn;
alignas(8) unsigned char netLocalPad[4 * 88 + 16];
}
static int moveDir, localAnswer, dirCalls;
static std::string trace;
extern "C" {
int getMoveDir__FiiiP12screenStruct(int p, int r, int c, int*) {
    ++dirCalls;
    trace += "dir" + std::to_string(p) + "," + std::to_string(r) + "," + std::to_string(c) + ";";
    return moveDir;
}
float evalHermite__Ffffff(float t, float a, float b, float c, float d) {
    if (a != 0.0f || b != 1.0f || c != 0.0f || d != 0.0f) trace += "hermite-args?;";
    return t;
}
int netPlayerIsLocal__Fi(int p) { trace += "local" + std::to_string(p) + ";"; return localAnswer; }
void* lumpFindResource__FPcT0(char* l, char* n) { trace += std::string("find:") + l + "," + n + ";"; return &trace; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* o, float v) {
    trace += std::string("seq") + (s == &trace ? "" : "?") + (o ? "?" : "") + std::to_string(v) + ";";
}
}
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}
struct Screen { int w[24]; };
static std::string state(const Screen& s) {
    float t; std::memcpy(&t, &s.w[15], 4);
    return std::to_string(s.w[5]) + "," + std::to_string(s.w[6]) + "," + std::to_string(s.w[7]) + "," + std::to_string(s.w[8]) + "," +
           std::to_string(s.w[9]) + "," + std::to_string(t > 0.0f ? 1 : 0);
}
static Screen make(int col, int row, float timer) {
    Screen s; std::memset(&s, 0, sizeof s);
    s.w[6] = col; s.w[7] = row; std::memcpy(&s.w[15], &timer, 4);
    return s;
}
static void reset() {
    std::memset(multiHelpMessageOn, 0, sizeof multiHelpMessageOn); std::memset(sellMode, 0, sizeof sellMode);
    helpMessageOn = 0; std::memset(netLocalPad, 0xff, sizeof netLocalPad); trace.clear(); dirCalls = 0; localAnswer = 1;
}

int main() {
    Screen s;
    // Early exits leave everything alone and never ask for a direction.
    reset(); multiHelpMessageOn[2] = 1; s = make(0, 0, 0); moveDir = 1;
    updateCursor__FiiiiiP12screenStructi(2, 3, 3, 40, 50, s.w, 1); expect("multi help", std::to_string(dirCalls) + state(s), "00,0,0,0,0,0");
    reset(); sellMode[1] = 1; updateCursor__FiiiiiP12screenStructi(1, 3, 3, 40, 50, s.w, 1); expect("sell mode", std::to_string(dirCalls), "0");
    reset(); helpMessageOn = 1; updateCursor__FiiiiiP12screenStructi(0, 3, 3, 40, 50, s.w, 1); expect("help message", std::to_string(dirCalls), "0");
    // Flag 0 clears the four pad bits of this player only.
    reset(); moveDir = 0; s = make(0, 0, 0);
    updateCursor__FiiiiiP12screenStructi(1, 3, 3, 40, 50, s.w, 0);
    unsigned word, other;
    std::memcpy(&word, netLocalPad + 8 + 88, 4); std::memcpy(&other, netLocalPad + 8, 4);
    expect("pad bits", std::to_string(word == (0xffffffffu & ~0xf00000u)) + std::to_string(other == 0xffffffffu), "11");
    reset(); updateCursor__FiiiiiP12screenStructi(1, 3, 3, 40, 50, s.w, 1);
    std::memcpy(&word, netLocalPad + 8 + 88, 4); expect("flag set keeps bits", std::to_string(word == 0xffffffffu), "1");
    expect("trace", trace, "dir1,3,3;");
    // Right, mid-slide: column 2, width 50, timer 0 -> offset 100 + (int)((0.0909 + 0.01) * 50) = 105.
    reset(); moveDir = 1; s = make(2, 0, 0.0f);
    updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 1);
    expect("right mid", state(s), "1,2,0,0,105,1"); expect("right trace", trace, "dir0,3,4;");
    // Right, finishing: timer 0.95 -> offset 152 >= 150: column 3, offset 150, direction and timer cleared, sound played.
    reset(); s = make(2, 0, 0.95f);
    updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 1);
    expect("right done", state(s), "0,3,0,0,150,0");
    expect("right done trace", trace, "dir0,3,4;local0;find:fx.lmp,game_butn_browse.anm;seq1.000000;");
    // Same without the flag, and with the flag but a non-local player: no sound / no lookup.
    reset(); s = make(2, 0, 0.95f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("no flag trace", trace, "dir0,3,4;");
    reset(); localAnswer = 0; s = make(2, 0, 0.95f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 1); expect("remote trace", trace, "dir0,3,4;local0;");
    // Left: column 2 timer 0 -> offset 100 - 5 = 95 (inside); timer 0.95 -> 100 - 52 = 48 (done, column 1, offset 50).
    reset(); moveDir = -1; s = make(2, 0, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("left mid", state(s), "-1,2,0,0,95,1");
    reset(); s = make(2, 0, 0.95f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("left done", state(s), "0,1,0,0,50,0");
    // Left at column -1 is blocked; right at the last column (columns - 1 = 3 < 4) is blocked; with rows zero no clamping.
    reset(); s = make(-1, 0, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("left blocked", state(s), "0,-1,0,0,0,0");
    reset(); moveDir = 1; s = make(4, 0, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("right blocked", state(s), "0,4,0,0,0,0");
    reset(); s = make(3, 0, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("right last column ok", state(s), "1,3,0,0,155,1");
    reset(); s = make(4, 0, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 0, 4, 40, 50, s.w, 0); expect("rows zero no clamp", state(s), "1,4,0,0,205,1");
    // Down (4): row 1, height 40, timer 0 -> offset 40 + (int)(0.1009 * 40) = 44 (inside); finishing row 2, offset 80.
    reset(); moveDir = 4; s = make(0, 1, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("down mid", state(s), "4,0,1,44,0,1");
    reset(); s = make(0, 1, 0.95f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("down done", state(s), "0,0,2,80,0,0");
    // Up (2): row 1 -> row 0, offset 0; row 0 is blocked.
    reset(); moveDir = 2; s = make(0, 1, 0.95f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("up done", state(s), "0,0,0,0,0,0");
    reset(); s = make(0, 0, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("up blocked", state(s), "0,0,0,0,0,0");
    // Down is blocked when rows <= row (3 rows, row 3); moves with row height zero are ignored.
    reset(); moveDir = 4; s = make(0, 3, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("down blocked", state(s), "0,0,3,0,0,0");
    reset(); s = make(0, 1, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 0, 50, s.w, 0); expect("zero row height", state(s), "4,0,1,0,0,0");
    // Another direction value is stored but does nothing.
    reset(); moveDir = 3; s = make(0, 1, 0.0f); updateCursor__FiiiiiP12screenStructi(0, 3, 4, 40, 50, s.w, 0); expect("other direction", state(s), "3,0,1,0,0,0");
    std::printf(bad ? "FAIL\n" : "ok: update cursor\n");
    return bad;
}
