// Observed: GlobalInit__14VIEffectVolumeP7VIScene at 0x011466f8 is 2108 bytes and
// returns 0. It stores its argument in the word _14VIEffectVolume$mScene
// (0x011b1a98), then fills three image-named tables of 48-byte records, each
// record being four 3-float vertices. Every record is built the same way: a
// zeroed local VIMatrix44 (constructed inline), a rotation set on it, each
// local vertex multiplied through it with Mul__C10VIMatrix44RC7VIVect3R7VIVect3
// (matrix, in, out), and the matrix destroyed with _$_10VIMatrix44(matrix, 2).
//  1. sFogCard (0x011b0dd8), 8 records, index i = 0..7: vertices (-.5, .5, 0),
//     (.5, .5, 0), (.5, -.5, 0), (-.5, -.5, 0); RotateZ__10VIMatrix44f by
//     ((float)i * 0.125f doubled) * 0x40490fda (pi); each vertex goes to the record.
//  2. sDustCard (0x011b0f58), 60 records, i = 0..59: the same vertices; angle
//     ((float)-i * 0x3c888888 (1/60) doubled) * pi; RotateZ.
//  3. sFallEffect (0x011b0298), 60 records, for j = 0..29 two records per pass,
//     with theta = (((float)j * 0x3d088888 (1/30)) doubled) * pi and one matrix
//     (constructed before the pass, destroyed after the second record):
//     record j: vertices (-.1, -.5, -.1), (.1, -.5, -.1), (.1, -.5, .1),
//     (-.1, -.5, .1) (0x3dcccccc / 0xbdcccccc); RotateY(theta); all four
//     vertices multiplied in place; RotateX(sinf(theta) * 0x3ecccccc); in place
//     again; RotateZ(-cosf(theta) * 0x3ecccccc); the four vertices are
//     multiplied into the record, and 0.5f is added to the y of each.
//     record 30 + j (same matrix, now carrying those rotations): vertices
//     (-.1, -.4, -.1), (.1, -.4, -.1), (.1, -.4, .1), (-.1, -.4, .1)
//     (0xbecccccc for -.4); RotateX(sinf(theta) * 0x3ecccccc); in place;
//     RotateY(sinf(theta) * 0x3e6b851e); multiplied into the record, and
//     0x3ecccccc is added to the y of each.
// Both rotations are applied on the matrix as the image does; Rotate* are
// unresolved imports and nothing is known of whether they replace or compose.
// Inferred: sFogCard/sDustCard are billboard card quads and sFallEffect is a set
// of falling-leaf quads, from the image's own table names. Unknown: what Rotate*
// do to the existing matrix contents.
extern "C" {
extern void* _14VIEffectVolume$mScene;
extern unsigned char sFogCard[];
extern unsigned char sDustCard[];
extern unsigned char sFallEffect[];

void RotateX__10VIMatrix44f(void* matrix, float angle);
void RotateY__10VIMatrix44f(void* matrix, float angle);
void RotateZ__10VIMatrix44f(void* matrix, float angle);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* in, void* out);
void _$_10VIMatrix44(void* matrix, int flags);
float sinf(float x);
float cosf(float x);

static float float_from_bits(unsigned int bits) {
    return __builtin_bit_cast(float, bits);
}

struct Matrix {
    alignas(16) float m[16];
};

static void clear_matrix(Matrix& matrix) {
    for (int i = 0; i < 16; ++i) matrix.m[i] = 0.0f;
}

static void set_card(float (*v)[3]) {
    v[0][0] = -0.5f; v[0][1] = 0.5f; v[0][2] = 0.0f;
    v[1][0] = 0.5f; v[1][1] = 0.5f; v[1][2] = 0.0f;
    v[2][0] = 0.5f; v[2][1] = -0.5f; v[2][2] = 0.0f;
    v[3][0] = -0.5f; v[3][1] = -0.5f; v[3][2] = 0.0f;
}

static void set_box(float (*v)[3], float y, float low, float high) {
    v[0][0] = low; v[0][1] = y; v[0][2] = low;
    v[1][0] = high; v[1][1] = y; v[1][2] = low;
    v[2][0] = high; v[2][1] = y; v[2][2] = high;
    v[3][0] = low; v[3][1] = y; v[3][2] = high;
}

static void transform_in_place(const Matrix& matrix, float (*v)[3]) {
    float result[3];
    for (int k = 0; k < 4; ++k) {
        Mul__C10VIMatrix44RC7VIVect3R7VIVect3(&matrix, v[k], result);
        v[k][0] = result[0]; v[k][1] = result[1]; v[k][2] = result[2];
    }
}

static void transform_to(const Matrix& matrix, float (*v)[3], unsigned char* record) {
    for (int k = 0; k < 4; ++k) Mul__C10VIMatrix44RC7VIVect3R7VIVect3(&matrix, v[k], record + 12 * k);
}

static void add_to_y(unsigned char* record, float amount) {
    float* y = reinterpret_cast<float*>(record);
    y[1] = y[1] + amount;
    y[4] = y[4] + amount;
    y[7] = y[7] + amount;
    y[10] = y[10] + amount;
}

int GlobalInit__14VIEffectVolumeP7VIScene(void* scene) {
    const float pi = float_from_bits(0x40490fdau);
    _14VIEffectVolume$mScene = scene;
    float v[4][3];

    for (int i = 0; i < 8; ++i) {
        set_card(v);
        Matrix matrix;
        clear_matrix(matrix);
        float angle = static_cast<float>(i) * float_from_bits(0x3e000000u);
        angle = angle + angle;
        RotateZ__10VIMatrix44f(&matrix, angle * pi);
        transform_to(matrix, v, sFogCard + 48 * i);
        _$_10VIMatrix44(&matrix, 2);
    }
    for (int i = 0; i < 60; ++i) {
        set_card(v);
        Matrix matrix;
        clear_matrix(matrix);
        float angle = static_cast<float>(-i) * float_from_bits(0x3c888888u);
        angle = angle + angle;
        RotateZ__10VIMatrix44f(&matrix, angle * pi);
        transform_to(matrix, v, sDustCard + 48 * i);
        _$_10VIMatrix44(&matrix, 2);
    }
    const float low = float_from_bits(0xbdccccccu);
    const float high = float_from_bits(0x3dccccccu);
    const float scale = float_from_bits(0x3ecccccc);
    for (int j = 0; j < 30; ++j) {
        Matrix matrix;
        clear_matrix(matrix);
        float theta = static_cast<float>(j) * float_from_bits(0x3d088888u);
        theta = theta + theta;
        theta = theta * pi;
        unsigned char* upper = sFallEffect + 48 * j;
        unsigned char* lower = sFallEffect + 1440 + 48 * j;

        set_box(v, -0.5f, low, high);
        RotateY__10VIMatrix44f(&matrix, theta);
        transform_in_place(matrix, v);
        RotateX__10VIMatrix44f(&matrix, sinf(theta) * scale);
        transform_in_place(matrix, v);
        RotateZ__10VIMatrix44f(&matrix, -cosf(theta) * scale);
        transform_to(matrix, v, upper);
        add_to_y(upper, 0.5f);

        set_box(v, float_from_bits(0xbecccccc), low, high);
        RotateX__10VIMatrix44f(&matrix, sinf(theta) * scale);
        transform_in_place(matrix, v);
        RotateY__10VIMatrix44f(&matrix, sinf(theta) * float_from_bits(0x3e6b851eu));
        transform_to(matrix, v, lower);
        add_to_y(lower, scale);
        _$_10VIMatrix44(&matrix, 2);
    }
    return 0;
}
}
