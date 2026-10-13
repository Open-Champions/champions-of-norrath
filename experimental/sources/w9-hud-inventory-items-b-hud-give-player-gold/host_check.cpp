#include <cmath>
struct GoldPoint3 { float x, y, z; };
struct GoldNumberEntry { float x, y, z, driftX, driftY, driftZ; int frame, amount; unsigned char player, flag, pad[2]; };
extern "C" {
GoldNumberEntry goldNumber[8];
unsigned char player[23984 * 4];
unsigned char worldViewMatrix[64];
int frameNm;
int eRandState;
int playerGoldx[4] = {50, 590, 50, 590};
int playerGoldy[4] = {200, 200, 250, 250};
void hudGivePlayerGold__FiG6Point3i(int, GoldPoint3, int);
}

static float wReturn;
static GoldPoint3 outValue, seenIn;
static int calls;
static void* seenMatrix;
extern "C" float matTransform__FP6Point3P8Matrix44T0(GoldPoint3* out, void* m, GoldPoint3* in) {
    ++calls; seenMatrix = m; seenIn = *in; *out = outValue; return wReturn;
}

static void reset() { for (auto& e : goldNumber) e = GoldNumberEntry{}; calls = 0; }

int main() {
    // a free entry is filled; the flag stays 0 when the point is in view
    reset(); frameNm = 1234; eRandState = 1;
    wReturn = 100.0f; outValue = {1000.0f, 200.0f, 50.0f};
    for (int i = 0; i < 3; ++i) goldNumber[i].amount = 9;
    hudGivePlayerGold__FiG6Point3i(1, GoldPoint3{4, 5, 6}, 77);
    if (calls != 1 || seenMatrix != worldViewMatrix || seenIn.x != 4 || seenIn.y != 5 || seenIn.z != 6) return 1;
    GoldNumberEntry& e = goldNumber[3];
    if (e.amount != 77 || e.player != 1 || e.frame != 1234 || e.flag != 0) return 2;
    // x = 1000/100*0.5 + 320 = 325, y = 200/100 + 240 = 242, z zeroed
    if (e.x != 325.0f || e.y != 242.0f || e.z != 0.0f) return 3;
    // drift: direction to the player-1 target (590, 200), rotated a quarter turn, scaled by a random value
    unsigned state = 1u * 0x19660du + 0x3c6ef35fu;
    if ((unsigned)eRandState != state) return 4;
    float r = ((float)(state >> 16) / 65536.0f - 0.5f) * 9.0f;
    float dx = 590.0f - 325.0f, dy = 200.0f - 242.0f, s = 1.0f / std::sqrt(dx * dx + dy * dy);
    if (std::fabs(e.driftX - dy * s * r) > 1e-3f || std::fabs(e.driftY - (-dx * s * r)) > 1e-3f || e.driftZ != 0.0f) return 5;
    // neighbours untouched
    if (goldNumber[2].amount != 9 || goldNumber[4].amount != 0 || goldNumber[4].frame != 0) return 6;
    // flag: too close, too far to the side, too far up
    struct Case { float w, x, y; int flag; } cases[] = {
        {19.0f, 0, 0, 1}, {20.0f, 0, 0, 0}, {10.0f + 20.0f, 700.0f * 30.0f + 1000, 0, 1}, {30.0f, -(700.0f * 30.0f + 1000), 0, 1},
        {30.0f, 0, 300.0f * 30.0f + 1000, 1}, {30.0f, 0, -(300.0f * 30.0f + 1000), 1}, {30.0f, 700.0f * 30.0f - 10, 300.0f * 30.0f - 10, 0},
    };
    for (auto& c : cases) {
        reset(); wReturn = c.w; outValue = {c.x, c.y, 0};
        hudGivePlayerGold__FiG6Point3i(0, GoldPoint3{0, 0, 0}, 1);
        if (goldNumber[0].flag != c.flag) return 10;
    }
    // all eight in use: the amount goes to the player's record word at +232 and nothing else changes
    reset(); for (auto& g : goldNumber) g.amount = 3;
    *reinterpret_cast<int*>(player + 2 * 23984 + 232) = 100;
    int before = eRandState;
    hudGivePlayerGold__FiG6Point3i(2, GoldPoint3{0, 0, 0}, 50);
    if (*reinterpret_cast<int*>(player + 2 * 23984 + 232) != 150 || calls != 0 || eRandState != before) return 20;
    for (auto& g : goldNumber) if (g.amount != 3 || g.frame != 0) return 21;
    // the last entry is usable
    reset(); for (int i = 0; i < 7; ++i) goldNumber[i].amount = 3;
    wReturn = 50.0f; outValue = {0, 0, 0};
    hudGivePlayerGold__FiG6Point3i(0, GoldPoint3{0, 0, 0}, 8);
    if (goldNumber[7].amount != 8 || calls != 1) return 22;
    return 0;
}
