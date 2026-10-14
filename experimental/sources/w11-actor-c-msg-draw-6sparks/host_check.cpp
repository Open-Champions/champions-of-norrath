// Host contract for msg_draw__6Sparks; not an EE input. Callees are stubs that log.
#define W11_ACTOR_C_HOST 1
#include "msg_draw.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
int frameNm;
static int haloMarker;
void* haloTexture1 = &haloMarker;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%.9g", v); return b; }
extern "C" float isin__Fi(int angle) { calls += "isin(" + std::to_string(angle) + ");"; return static_cast<float>(angle % 977) / 977.0f; }
extern "C" void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float size, float size2, int a, int b, int c, int d, void* texture, short spin, int flag) {
    calls += "sprite(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(size) + "," + num(size2) + "," + std::to_string(a) + "," +
             std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + (texture == &haloMarker ? "H" : "?") + "," +
             std::to_string(spin) + "," + std::to_string(flag) + ");";
}
int main() {
    int failures = 0;
    for (int n : {0, 1, 63, 64, 127, 128, 129, 1000, 65535, 65536, 70000, -1, -77}) {
        frameNm = n;
        unsigned char object[64] = {};
        float pos[3] = {1.25f, -3.5f, 99.0f};
        std::memcpy(object + 16, pos, 12);
        calls.clear();
        msg_draw__6Sparks(object);
        const int angle = n << 10;
        const float pulse = static_cast<float>(angle % 977) / 977.0f * 250.0f;
        const short spin = static_cast<short>((n & 0x7f) << 9);
        const std::string want = "isin(" + std::to_string(angle) + ");sprite(1.25,-3.5,99,5," + num(pulse + 12583.0f) + ",255,200,50,128,H," +
                                 std::to_string(spin) + ",3);";
        if (calls != want) { std::fprintf(stderr, "n=%d\n got  %s\n want %s\n", n, calls.c_str(), want.c_str()); ++failures; }
    }
    if (!failures) std::puts("msg_draw 6Sparks ok");
    return failures != 0;
}
