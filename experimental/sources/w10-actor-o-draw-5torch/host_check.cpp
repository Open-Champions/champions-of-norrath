// Host contract for msg_draw__5Torch; not an EE input. Includes the source.
#include "msg_draw.cpp"
#include <cstring>
#include <initializer_list>

int frameNm;
void* haloTexture2 = &frameNm;
static int seed_seen, runlist_calls, sprite_calls;
static float noise_value;
static Point3 sprite_pos;
static float sprite_a, sprite_b;
static int sprite_i[4], sprite_h;
static short sprite_g;
static void* sprite_tex;
static void* runlist_arg;
float ef1Noise__Fi(int seed) { seed_seen = seed; return noise_value; }
void objectAddToRunList__FP10GameObject(void* o) { ++runlist_calls; runlist_arg = o; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3* p, float a, float b, int c, int d, int e, int f, void* t, short g, int h) {
    ++sprite_calls; sprite_pos = *p; sprite_a = a; sprite_b = b;
    sprite_i[0] = c; sprite_i[1] = d; sprite_i[2] = e; sprite_i[3] = f; sprite_tex = t; sprite_g = g; sprite_h = h;
}

int main() {
    alignas(8) unsigned char object[160];
    for (int mode : {0, 1, 2, 3, 4, 5, 255, 130}) {
        for (int run : {0, 1, -1}) {
            for (float noise : {0.0f, 0.5f, -0.75f, 1.0f, 0.99f}) {
                for (int frame : {0, 7, -3}) {
                    std::memset(object, 0x5a, sizeof object);
                    const float pos[3] = {1.5f, -2.25f, 100.0f};
                    std::memcpy(object + 16, pos, 12);
                    object[136] = static_cast<unsigned char>(run);
                    object[138] = static_cast<unsigned char>(mode);
                    frameNm = frame; noise_value = noise;
                    seed_seen = 0; runlist_calls = 0; sprite_calls = 0;
                    msg_draw__5Torch(object);
                    const int want_seed = 0x7efe41 + static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(object)) * 0x19660du) + frame * 16;
                    if (seed_seen != want_seed) return 1;
                    if (runlist_calls != (run == 0 ? 1 : 0)) return 2;
                    if (run == 0 && runlist_arg != object) return 3;
                    if (object[136] != (run == 0 ? 1 : static_cast<unsigned char>(run)) || object[137] != 0) return 4;
                    if (sprite_calls != 1 || sprite_tex != haloTexture2 || sprite_g != 0) return 5;
                    const int flick = static_cast<int>(noise * 22.0f);
                    if (mode == 2 || mode == 3) {
                        if (sprite_pos.x != 1.5f || sprite_pos.y != -2.25f || sprite_pos.z != 100.0f) return 6;
                        if (sprite_a != 26.0f || sprite_b != static_cast<float>(flick * 64 + 20000)) return 7;
                        if (sprite_i[0] != 124 || sprite_i[1] != 81 || sprite_i[2] != 62 || sprite_i[3] != 125 || sprite_h != 0) return 8;
                    } else {
                        const float z = 100.0f - 6.0f + ((mode == 3 || mode == 4) ? 12.0f : 0.0f);
                        if (sprite_pos.x != 1.5f || sprite_pos.y != -2.25f || sprite_pos.z != z) return 9;
                        if (sprite_a != 25.0f || sprite_b != static_cast<float>(flick * 64 + 13996)) return 10;
                        if (sprite_i[0] != 250 || sprite_i[1] != 150 || sprite_i[2] != 0 || sprite_i[3] != 40 || sprite_h != 2) return 11;
                    }
                    for (int i = 0; i < 160; ++i) {
                        const bool known = i == 136 || i == 137 || i == 138 || (i >= 16 && i < 28);
                        if (!known && object[i] != 0x5a) return 12;
                    }
                }
            }
        }
    }
    return 0;
}
