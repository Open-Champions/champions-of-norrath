// Host contract for msg_draw__16PoisonProjectile; not an EE input. 32-bit words are offsets from a host base and the callees log.
// The authored function was also compared with the original in a private R5900 run (11 scenarios over the model and texture
// indices, including negative ones, the yaw halfword and the scale; every original word covered).
#define W12_SPELL_C_HOST 1
#include "msg_draw.cpp"
#include <cstring>
unsigned char* w12_spell_c_host_base;
unsigned w12_spell_c_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static int matrix_calls, draw_calls;
static Point3 matrix_point, draw_point;
static short matrix_yaw;
static float matrix_scale;
static void *draw_model, *draw_texture, *draw_animation, *draw_record, *draw_matrix, *matrix_made;
static int draw_mode, draw_last;
static unsigned long draw_flags;
extern "C" {
void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* m, Point3 p, short yaw, float scale) {
    ++matrix_calls; matrix_made = m; matrix_point = p; matrix_yaw = yaw; matrix_scale = scale; m->f[0] = 42.0f;
}
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(void* model, void* texture, int mode, Point3 p, Matrix34* m,
                                                                                               void* animation, unsigned long flags, void* record, int last) {
    ++draw_calls; draw_model = model; draw_texture = texture; draw_mode = mode; draw_point = p; draw_matrix = m; draw_animation = animation;
    draw_flags = flags; draw_record = record; draw_last = last;
    return m->f[0] == 42.0f ? 0 : 1;
}
}
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, T = 0x400 };
int main() {
    w12_spell_c_host_base = arena;
    putf(O + 16, 1.0f); putf(O + 20, 2.0f); putf(O + 24, 3.0f); putf(O + 160, 0.75f);
    const short yaw = 0x7fff;
    std::memcpy(arena + O + 56, &yaw, 2);
    put32(O + 36, T); put32(O + 84, 2); put32(O + 88, static_cast<unsigned>(-1));
    put32(T + 2 * 8 + 4, 0x600);
    put32(T - 8 + 4, 0x700);
    msg_draw__16PoisonProjectile(arena + O);
    if (matrix_calls != 1 || draw_calls != 1) return 1;
    if (matrix_point.x != 1.0f || matrix_point.y != 2.0f || matrix_point.z != 3.0f || matrix_scale != 0.75f) return 2;
    if (matrix_yaw != static_cast<short>(0x7fff + 16384)) return 3;
    if (draw_model != arena + 0x600 || draw_texture != arena + 0x700) return 4;
    if (draw_mode != 1 || draw_point.x != 1.0f || draw_point.z != 3.0f || draw_matrix != matrix_made || draw_animation != arena + O + 100) return 5;
    if (draw_flags != static_cast<unsigned long>(-1) || draw_record != nullptr || draw_last != 0) return 6;
    return 0;
}
