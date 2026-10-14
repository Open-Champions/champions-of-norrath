// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 16];
static unsigned char scratch[0x80];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    if (address >= 0x70000000u) return scratch + (address - 0x70000000u);
    return arena + (address - arena_base);
}
static int n, order[8]; static const float* seen_in3; static const unsigned char* seen_m0; static const unsigned char* seen_m1; static float seen_in4[4];
static unsigned char* seen_font; static const char* seen_text; static int seen_flag, width_ret; static float w_scale = 1.0f;
extern "C" void __ml__FRC7VIVect3RC10VIMatrix44(float* r, const float* v, const unsigned char* m) {
    order[n++] = 1; seen_in3 = v; seen_m0 = m;
    r[0] = v[0] * 2.0f; r[1] = v[1] * 2.0f; r[2] = v[2] + 1.0f; r[3] = 5.0f;
}
extern "C" void __ml__FRC7VIVect4RC10VIMatrix44(float* r, const float* v, const unsigned char* m) {
    order[n++] = 2; seen_m1 = m; std::memcpy(seen_in4, v, 16);
    r[0] = v[0] * w_scale; r[1] = v[1] * w_scale; r[2] = v[2]; r[3] = v[3] * w_scale;
}
extern "C" int GetStringWidth__7VIRFontPCci(unsigned char* f, const char* t, int fl) { order[n++] = 3; seen_font = f; seen_text = t; seen_flag = fl; return width_ret; }
#include "calc_centered_string_screen_rect.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    bool ok = true;
    W(19516) = arena_base + 0x6000;
    for (unsigned i = 0; i < 3; ++i) { W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x100 * i; W(0x7000 + 0x100 * i + 20) = 16 + 2 * i; }
    W(17748) = 640; W(17752) = 480;
    float pos[3] = {0.125f, 0.25f, 2.0f};
    int rect[4] = {-1, -1, -1, -1};
    width_ret = 51; w_scale = 1.0f;
    n = 0;
    // view = (0.25, 0.5, 3, 5); clip = (0.25, 0.5, 3, 5); x = 0.05, y = 0.1 after dividing by w = 5
    int r = CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect(arena, 1, pos, "hello", rect);
    float inv = 1.0f / 5.0f, x = 0.25f * inv, y = 0.5f * inv;
    int sx = (int)(x * 320.0f + 320.0f), sy = (int)((-y) * 240.0f + 240.0f);
    ok = ok && r == 0 && n == 3 && order[0] == 1 && order[1] == 2 && order[2] == 3 && seen_in3 == pos
        && seen_m0 == scratch && seen_m1 == scratch + 0x40 && seen_in4[0] == 0.25f && seen_in4[1] == 0.5f && seen_in4[2] == 3.0f && seen_in4[3] == 5.0f
        && seen_font == arena + 0x7100 && std::strcmp(seen_text, "hello") == 0 && seen_flag == 1;
    ok = ok && rect[0] == sx - 25 && rect[1] == sy - 9 && rect[2] == rect[0] + 51 && rect[3] == rect[1] + 18;
    // odd width and extents; negative y: truncation toward zero
    W(17748) = 641; W(17752) = 479; width_ret = 10; n = 0;
    pos[1] = -3.0f;
    CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect(arena, 2, pos, "x", rect);
    inv = 1.0f / 5.0f; x = 0.25f * inv; y = -6.0f * inv;
    sx = (int)(x * 320.0f + 320.0f); sy = (int)((-y) * 239.0f + 239.0f);
    ok = ok && rect[0] == sx - 5 && rect[1] == sy - 10 && rect[2] == rect[0] + 10 && rect[3] == rect[1] + 20;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
