// Host contract for textureDrawFloat__FP8_textureffiUiff. Not an EE plan input.
// Build with -ffp-contract=off. The host compiler need not fold 1/640 to the same
// bits as the EE compiler, so only the pass-through and the scaling shape are
// checked here; the paired run against the image checks the folded constants.
#include <cmath>
#include <cstdio>
struct Texture { int unused; };
static Texture* got_tex; static float got_x, got_y, got_u, got_v; static int got_a; static unsigned got_c; static int calls;
extern "C" void _textureDrawFloat__FP8_textureffiUiff(Texture* t, float x, float y, int a, unsigned c, float u, float v) {
    ++calls; got_tex = t; got_x = x; got_y = y; got_a = a; got_c = c; got_u = u; got_v = v;
}
extern "C" void textureDrawFloat__FP8_textureffiUiff(Texture*, float, float, int, unsigned, float, float);
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Texture t;
    textureDrawFloat__FP8_textureffiUiff(&t, 320.0f, 112.0f, -5, 0xdeadbeefu, 0.25f, 0.75f);
    expect(calls == 1, "one call");
    expect(got_tex == &t && got_a == -5 && got_c == 0xdeadbeefu && got_u == 0.25f && got_v == 0.75f, "pass-through arguments");
    expect(std::fabs(got_x - 320.0f) < 0.01f && std::fabs(got_y - 112.0f) < 0.01f, "scaling is near identity");
    if (failures) return 1;
    std::printf("textureDrawFloat contract passed\n");
    return 0;
}
