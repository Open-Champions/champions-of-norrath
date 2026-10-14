// Host contract for msg_draw__19FireArrowProjectile; not an EE input. The callee logs its arguments. The authored
// function was also compared with the original in a private R5900 run (4 scenarios, every original word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static Point3 got_position;
static float got_size, got_size2;
static int got[4], calls;
static void* got_texture;
static short got_spin;
static int got_flag;
extern "C" {
void* haloTexture4 = reinterpret_cast<void*>(0x1234);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float size, float size2, int a, int b, int c, int d, void* texture, short spin, int flag) {
    ++calls; got_position = position; got_size = size; got_size2 = size2;
    got[0] = a; got[1] = b; got[2] = c; got[3] = d; got_texture = texture; got_spin = spin; got_flag = flag;
}
}
#include "msg_draw.cpp"

int main() {
    unsigned char object[64] = {};
    float v[3] = {1.5f, -2.0f, 3.25f};
    std::memcpy(object + 16, v, 12);
    float k = 0.5f;
    std::memcpy(object + 28, &k, 4);
    msg_draw__19FireArrowProjectile(object);
    if (calls != 1 || got_position.x != 1.5f || got_position.y != -2.0f || got_position.z != 3.25f) return 1;
    if (got_size != 10.0f || got_size2 != 704.0f) return 2;
    if (got[0] != 147 || got[1] != 98 || got[2] != 73 || got[3] != 73) return 3;
    if (got_texture != haloTexture4 || got_spin != 0 || got_flag != 6) return 4;
    return 0;
}
