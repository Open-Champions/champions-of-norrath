// Host contract for msg_draw__7FireFly. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_draw.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

int frameNm;
void* haloTexture4;
static std::vector<int> cos_args, sin_args;
static std::vector<char> order;
static int face_calls = 0, beetle_calls = 0, failures = 0;
static float face_point[3], face_first, face_second, beetle_pos[3];
static int face_rgb[3], face_alpha, face_mode;
static short face_angle;
static void* face_texture;
static void* beetle_self;
extern "C" float icos__Fi(int a) { cos_args.push_back(a); order.push_back('c'); return 0.25f + static_cast<float>(a % 97) * 0.01f; }
extern "C" float isin__Fi(int a) { sin_args.push_back(a); order.push_back('s'); return -0.5f + static_cast<float>(a % 89) * 0.02f; }
extern "C" void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* point, float first, float second, int red, int green,
                                                           int blue, int alpha, void* texture, short angle, int mode) {
    ++face_calls; std::memcpy(face_point, point, 12); face_first = first; face_second = second;
    face_rgb[0] = red; face_rgb[1] = green; face_rgb[2] = blue; face_alpha = alpha; face_texture = texture;
    face_angle = angle; face_mode = mode; order.push_back('F');
}
extern "C" void msg_draw__10FireBeetle(void* self) {
    ++beetle_calls; beetle_self = self; std::memcpy(beetle_pos, static_cast<unsigned char*>(self) + 16, 12); order.push_back('B');
}
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    static unsigned char object[1200];
    for (int t : {0, 1, 7, -3, 12345}) for (int h : {0, 100, -100, 3000}) for (int n : {0, 5, 0x1ffff, -7}) {
        std::memset(object, 0x41, sizeof object);
        float pos[3] = {10.5f, -20.25f, 300.0f};
        std::memcpy(object + 16, pos, 12);
        write_word(address(object) + 808u, static_cast<unsigned>(t));
        write_u16(address(object) + 56u, static_cast<unsigned>(h) & 0xffffu);
        frameNm = n; static int tex; haloTexture4 = &tex;
        cos_args.clear(); sin_args.clear(); order.clear(); face_calls = beetle_calls = 0;
        msg_draw__7FireFly(object);
        expect(std::string(order.begin(), order.end()) == "csccssFcsB", "call order");
        expect(face_calls == 1 && beetle_calls == 1 && beetle_self == object, "one sprite and one FireBeetle draw");
        expect(cos_args.size() == 4 && sin_args.size() == 4, "trig call counts");
        expect(cos_args[0] == t * 1000 && sin_args[0] == t * 1000 && cos_args[1] == t * 1000, "clock angle");
        expect(cos_args[2] == h && sin_args[1] == h && cos_args[3] == h && sin_args[3] == h, "heading angle");
        expect(sin_args[2] == (n << 14), "pulse angle");
        // the position the sprite sees
        float x = pos[0] + (0.25f + float(cos_args[0] % 97) * 0.01f) * 10.0f;
        float y = pos[1] + (-0.5f + float(sin_args[0] % 89) * 0.02f) * 10.0f;
        float z = pos[2] + (0.25f + float(cos_args[1] % 97) * 0.01f) * 10.0f + 20.0f;
        float ox = (0.25f + float(h % 97) * 0.01f) * -5.0f;
        float oy = (-0.5f + float(h % 89) * 0.02f) * -5.0f;
        x += ox; y += oy; z += 30.0f;
        expect(face_point[0] == x && face_point[1] == y && face_point[2] == z, "sprite point");
        expect(face_first == 5.0f && face_rgb[0] == 250 && face_rgb[1] == 150 && face_rgb[2] == 0 && face_alpha == 40, "sprite constants");
        expect(face_texture == &tex && face_angle == 0 && face_mode == 2, "sprite texture, angle and mode");
        const float pulse = (-0.5f + float(sin_args[2] % 89) * 0.02f) * 1000.0f + __builtin_bit_cast(float, 0x467b0c00u);
        expect(face_second == pulse, "sprite second float");
        // after the draw the position is exactly the original
        float after[3]; std::memcpy(after, object + 16, 12);
        expect(std::memcmp(after, pos, 12) == 0, "position restored");
        for (unsigned i = 0; i < sizeof object; ++i) if ((i < 16 || i >= 28) && (i < 808 || i >= 812) && (i < 56 || i >= 58) && object[i] != 0x41) { expect(false, "stray write"); break; }
    }
    return failures ? 1 : 0;
}
