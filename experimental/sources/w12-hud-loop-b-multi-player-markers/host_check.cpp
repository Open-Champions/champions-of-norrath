// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// isin/icos are stubbed to return angle / 100 and angle / 1000, so each sprite's offset shows its angle.
#include "multi_player_markers.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int nmPlayers, gameMultiPlayerMarkersOn, frameNm;
void* haloTexture5 = reinterpret_cast<void*>(0x1357);
alignas(8) unsigned char player[4 * 23984];
}
static int customAnswer, dlgAnswer;
struct Sprite { float x, y, z, size, depth; int a, b, c, alpha; void* tex; int flags, mode; };
static std::vector<Sprite> sprites;
static std::vector<int> isinArgs, icosArgs;
extern "C" {
int netGetCameraCustomControl__Fi(int) { return customAnswer; }
int DLG_Active__Fv() { return dlgAnswer; }
float isin__Fi(int a) { isinArgs.push_back(a); return a / 100.0f; }
float icos__Fi(int a) { icosArgs.push_back(a); return a / 1000.0f; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float size, float depth, int a, int b, int c, int alpha, void* tex, short flags, int mode) {
    sprites.push_back({p.x, p.y, p.z, size, depth, a, b, c, alpha, tex, flags, mode});
}
}
alignas(8) static unsigned char object[1400];
static int bad;
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); bad = 1; } }
static void setInt(unsigned char* p, int off, int v) { std::memcpy(p + off, &v, 4); }
static void setFloat(unsigned char* p, int off, float v) { std::memcpy(p + off, &v, 4); }
static void reset(int idx) {
    std::memset(object, 0, sizeof object);
    std::memset(player, 0, sizeof player);
    setInt(object, 960, idx); setFloat(object, 16, 1.0f); setFloat(object, 20, 2.0f); setFloat(object, 24, 3.0f);
    setFloat(player + idx * 23984, 172, 90.0f); setFloat(player + idx * 23984, 176, 100.0f);
    nmPlayers = 2; gameMultiPlayerMarkersOn = 1; frameNm = 0; customAnswer = 0; dlgAnswer = 0;
    sprites.clear(); isinArgs.clear(); icosArgs.clear();
}
static size_t run() { maybeDrawMultiPlayerMarkers__6Player(object); return sprites.size(); }

int main() {
    reset(1); check(run() == 45, "baseline draws 45 sprites");
    reset(1); setInt(object, 1328, 1); check(run() == 0, "word 1328");
    reset(1); nmPlayers = 1; check(run() == 0, "single player");
    reset(1); gameMultiPlayerMarkersOn = 0; check(run() == 0, "markers off");
    reset(1); customAnswer = 1; check(run() == 0, "camera control without bit");
    reset(1); customAnswer = 1; setInt(object, 12, 0x400000); check(run() == 45, "camera control with bit");
    reset(1); dlgAnswer = 1; check(run() == 0, "dialog");
    reset(1); setFloat(player + 23984, 172, 0.0f); check(run() == 0, "zero health");
    reset(1); setFloat(player + 23984, 172, -5.0f); check(run() == 0, "negative health");
    // Wave: idx 1, frameNm 70000 -> 70000 % 65536 = 4464; argument (500 + 4464) * 562; alpha from wave 2789368 / 100.
    reset(1); frameNm = 70000; run();
    check(isinArgs.size() == 46 && isinArgs[0] == (500 + 4464) * 562, "wave argument");
    const float wave = isinArgs[0] / 100.0f;
    check(sprites[0].alpha == static_cast<int>(wave * 14.0f + 57.0f), "alpha");
    // Negative frame: C remainder keeps the sign.
    reset(2); frameNm = -3; run();
    check(isinArgs[0] == (1000 - 3) * 562, "negative frame argument");
    // Sprites: angle for s = 0, 8, ... is (short)(int)(s * 182.0444); position = pos + radius * (isin, icos).
    reset(0); frameNm = 0; run();
    const float radius = isinArgs[0] / 100.0f + 20.0f;
    check(sprites.size() == 45 && isinArgs.size() == 46 && icosArgs.size() == 45, "call counts");
    for (int i = 0; i < 45; ++i) {
        const int angle = static_cast<short>(static_cast<int>(static_cast<float>(i * 8) * 182.04443359375f));
        check(isinArgs[1 + i] == angle && icosArgs[i] == angle, "angle");
        check(sprites[i].x == 1.0f + radius * (angle / 100.0f) && sprites[i].y == 2.0f + radius * (angle / 1000.0f) && sprites[i].z == 3.0f, "position");
        check(sprites[i].size == 6.0f && sprites[i].depth == 1822.0f && sprites[i].tex == haloTexture5 && sprites[i].flags == 0 && sprites[i].mode == 0, "constants");
    }
    // Colour by player index.
    const int colours[4][3] = {{182, 47, 58}, {24, 217, 30}, {216, 219, 33}, {71, 61, 209}};
    for (int idx = 0; idx < 4; ++idx) {
        reset(idx); run();
        check(sprites[0].a == colours[idx][0] && sprites[0].b == colours[idx][1] && sprites[0].c == colours[idx][2], "colour");
    }
    // Low health ratio flickers black on frames with bit 2 set only.
    reset(3); setFloat(player + 3 * 23984, 172, 30.0f); frameNm = 2; run();
    check(sprites[0].a == 0 && sprites[0].b == 0 && sprites[0].c == 0, "flicker black");
    reset(3); setFloat(player + 3 * 23984, 172, 30.0f); frameNm = 1; run();
    check(sprites[0].a == 71, "flicker off frame");
    reset(3); setFloat(player + 3 * 23984, 172, 34.0f); frameNm = 2; run();
    check(sprites[0].a == 71, "ratio above threshold");
    std::printf(bad ? "FAIL\n" : "ok: multi player markers\n");
    return bad;
}
