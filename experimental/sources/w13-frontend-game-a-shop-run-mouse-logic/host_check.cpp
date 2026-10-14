// Host check for shopRunMouseLogic__FiiP12screenStructi. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off -fno-builtin host_check.cpp
#include "shop_run_mouse_logic.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>

extern "C" {
int gameControlPaused;
signed char multiHelpMessageOn[4];
int shopDisableMouse[2];
int hideAbilityAdjustMouse[2];
alignas(8) unsigned char netLocalPad[4 * 88];
alignas(8) unsigned char zeroPad[96];
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

struct Screen {
    int word[32];
    float& f(int i) { return reinterpret_cast<float*>(word)[i]; }
};
static void setPad(unsigned char* pad, float stickX, float stickY) {
    std::memcpy(pad + 20, &stickX, 4);
    std::memcpy(pad + 24, &stickY, 4);
}
static const float k1 = 5.59495163f;  // 0x40b309d8

static void reset(Screen& s) {
    std::memset(&s, 0, sizeof s);
    gameControlPaused = 0;
    std::memset(multiHelpMessageOn, 0, sizeof multiHelpMessageOn);
    shopDisableMouse[0] = shopDisableMouse[1] = 0;
    hideAbilityAdjustMouse[0] = hideAbilityAdjustMouse[1] = 0;
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    std::memset(zeroPad, 0, sizeof zeroPad);
}

int main() {
    Screen s;
    // the stick moves the position by k1 * stick; scroll stays put for a mid-screen y
    reset(s);
    setPad(netLocalPad + 2 * 88, 2.0f, -3.0f);
    setPad(netLocalPad + 0 * 88, 100.0f, 100.0f);
    s.word[0] = 10; s.f(17) = 100.0f; s.f(18) = 200.0f; s.word[10] = 7;
    shopRunMouseLogic__FiiP12screenStructi(2, 1, &s, 50);
    CHECK(s.f(17) == 100.0f + 2.0f * k1 && s.f(18) == 200.0f + -3.0f * k1 && s.word[10] == 7);
    // x is limited to [word0, word0 + 320]
    reset(s);
    setPad(netLocalPad, -50.0f, 0.0f);
    s.word[0] = 10; s.f(17) = 20.0f; s.f(18) = 200.0f;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
    CHECK(s.f(17) == 10.0f);
    reset(s);
    setPad(netLocalPad, 50.0f, 0.0f);
    s.word[0] = 10; s.f(17) = 320.0f; s.f(18) = 200.0f;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
    CHECK(s.f(17) == 330.0f);
    // y is limited to [0, 428]
    reset(s);
    s.word[0] = 0; s.f(18) = -7.0f; s.f(17) = 5.0f; s.word[10] = 10;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
    CHECK(s.f(18) == 0.0f && s.word[10] == 6);        // y below 112 and below k4: -2 and -2
    reset(s);
    s.f(18) = 500.0f; s.word[10] = 10;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
    CHECK(s.f(18) == 428.0f && s.word[10] == 14);     // above k2 and k3: +2 and +2
    // scroll thresholds: 112, k4 = 22.4, k2 = 380.8, k3 = 425.6
    struct Case { float y; int delta; } cases[] = {
        {111.0f, -2}, {112.0f, 0}, {200.0f, 0}, {22.0f, -4}, {23.0f, -2}, {0.0f, -4}, {380.0f, 0}, {381.0f, 2}, {425.0f, 2}, {426.0f, 4}, {428.0f, 4}, {112.5f, 0}};
    for (const Case& c : cases) {
        reset(s);
        s.f(18) = c.y; s.word[10] = 20;
        shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
        if (s.word[10] != 20 + c.delta) { std::printf("y=%g got %d\n", c.y, s.word[10]); return 2; }
    }
    // scroll is limited to [0, limit]
    reset(s);
    s.f(18) = 300.0f; s.word[10] = -9;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
    CHECK(s.word[10] == 0);
    reset(s);
    s.f(18) = 300.0f; s.word[10] = 90;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
    CHECK(s.word[10] == 50);
    reset(s);
    s.f(18) = 300.0f; s.word[10] = 3;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, -4);
    CHECK(s.word[10] == -4);   // the limit wins even when negative (signed compare after the floor at 0)
    // movement is skipped by either per-side flag (only that side's), but the limits still apply
    for (int which = 0; which < 2; ++which) {
        reset(s);
        setPad(netLocalPad, 4.0f, 4.0f);
        (which == 0 ? shopDisableMouse : hideAbilityAdjustMouse)[1] = 1;
        s.word[0] = 0; s.f(17) = 100.0f; s.f(18) = 200.0f;
        shopRunMouseLogic__FiiP12screenStructi(0, 1, &s, 50);
        CHECK(s.f(17) == 100.0f && s.f(18) == 200.0f);
        shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
        CHECK(s.f(17) == 100.0f + 4.0f * k1);
    }
    reset(s);
    shopDisableMouse[0] = 1; s.f(17) = -50.0f; s.word[0] = 0; s.f(18) = 200.0f;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
    CHECK(s.f(17) == 0.0f);
    // multiHelpMessageOn[pn] set (any nonzero byte, including a negative one): nothing changes
    for (int v : {1, -128}) {
        reset(s);
        multiHelpMessageOn[3] = static_cast<signed char>(v);
        setPad(netLocalPad + 3 * 88, 4.0f, 4.0f);
        s.word[0] = 0; s.f(17) = 100.0f; s.f(18) = 600.0f; s.word[10] = 9;
        Screen before = s;
        shopRunMouseLogic__FiiP12screenStructi(3, 0, &s, 50);
        CHECK(std::memcmp(&before, &s, sizeof s) == 0);
        shopRunMouseLogic__FiiP12screenStructi(2, 0, &s, 50);  // another player is not blocked
        CHECK(s.f(18) == 428.0f);
    }
    // gameControlPaused: the pad is zeroPad, not netLocalPad
    reset(s);
    gameControlPaused = 1;
    setPad(netLocalPad, 9.0f, 9.0f);
    setPad(zeroPad, 1.0f, 2.0f);
    s.word[0] = 0; s.f(17) = 100.0f; s.f(18) = 200.0f;
    shopRunMouseLogic__FiiP12screenStructi(0, 0, &s, 50);
    CHECK(s.f(17) == 100.0f + 1.0f * k1 && s.f(18) == 200.0f + 2.0f * k1);
    std::puts("shop run mouse logic passed");
    return 0;
}
