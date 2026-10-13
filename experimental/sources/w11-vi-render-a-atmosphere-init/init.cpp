// Observed: Init__12VIAtmospherelR8VIRasteri at 0x01066780 is 2844 bytes. Arguments:
// object, a 64-bit value, a raster. It returns 0, or -1 as soon as one of the five
// CreateMaterial__8VIRasteri calls (second argument 0) returns a negative id.
// Offsets below are into the object; floats are 4 bytes, "bits" means the exact
// float bit pattern used in the image.
//  1. word 8 = 200, float 4 = 10.0f, the 64-bit value is stored at 1488 and at
//     1480 (1480 is cleared again below), eight floats at 1196..1224 get bits
//     0x3d088888, floats at 60, 64, 68 are 0, then SetEffectMix__12VIAtmospheref(0),
//     SetIntensity__12VIAtmospheref(0), float 76 = 0 and
//     SetLavastormIntensity__12VIAtmospheref(0) are called in that order.
//  2. Plain stores: words 1556, 1476, 1548, 1552 = 1; floats 140 and 136 = bits
//     0x3dcccccc; floats 1472, 16, 1564 = 1.0f; floats 132, 128, 1544, 3876, 3880,
//     3884, 4504, 4508, 4512 = 0; 64 bits at 1480 = 0; words 1248, 1504, 1508,
//     1512, 1524, 1528, 1260, 1264, 1268, 3872, 1244, 3920, 4488, 4496, 4500 = -1;
//     words 1516, 1520, 1532, 1536, 1540, 12, 48, 4476, 1560 = 0.
//  3. 36 * count bytes come from __builtin_vec_new (count = word 8); when it
//     succeeds each 36-byte record has floats 0, 4, 8, 12, 16, 20, 24 set to 0 and
//     the pointer goes to word 12 (zero when the allocation fails).
//  4. 64 entries i = 0..63 of 12 bytes from 168 hold sinf, cosf and cosf of
//     theta = (((float)i * bits 0x40c90fda) * 0.015625f) doubled; the 64 floats
//     from 936 are ((131 * rand()) & 255) * bits 0x3b808080 + 1.0f.
//  5. Materials: words 160 and 164 get CreateMaterial ids; each is set up with
//     Material__8VIRasteri(raster, id), Init__10VIMateriali(m, 1),
//     SetLayerFillType(m, 0, 0), SetLayerModulate__10VIMaterialii(m, 0, 1),
//     SetLayerBlendMode(m, 0, 3), SetZWrite(m, 0), SetZTest(m, 1), SetDither(m, 0).
//     Word 56 and word 1244 follow with blend mode 1 (1244 also
//     SetLayerTextureLODBias__10VIMaterialif(m, 0, 1.0f) and ZTest 0; 56 has
//     ZTest 1); word 3920 is set up as 1244 is. A negative id is stored in its
//     word before the early return.
//  6. For each of count particles (when count is positive): a 36-byte record gets
//     (byte * bits 0x3b808080) * size - size * 0.5f for x, y, z from three bytes
//     of sRainParticleDist (size = float 4), floats 12, 16, 20 = 1.0f, float 24 =
//     ((float)r1 - (float)r2) * 2^-31 * bits 0x3da3d70a + bits 0x3dcccccc from two
//     rand() calls (r1 first), and word 32 = 0.
//  7. For 128 entries: a zeroed VIMatrix44, RotateHPR__10VIMatrix44RC7VIVect3 of
//     ((128 - b0) * 2.3f-ish bits 0x40133333 * bits 0x3cc90fda, (128 - b1) * pi *
//     bits 0x3b7ae147, (128 - b2) * pi * bits 0x3b7ae147) from three bytes of
//     mStarParticleDist, the Vect4 (0, 100, 0, 1) multiplied through
//     Mul__C10VIMatrix44RC7VIVect4R7VIVect4 into the 16 bytes at 1568 + 16 * i,
//     float 1580 + 16 * i = ((rand() / 131) & 255) * bits 0x3b808080 * 0.5f +
//     bits 0x3e999999, byte 3616 + i = (rand() / 131) & 31, and the matrix is
//     destroyed with _$_10VIMatrix44(m, 2).
//  8. 32 floats from 3744: ((sinf(theta) * 0.5f + 0.5f) * bits 0x3ecccccc) + 0.5f
//     with theta = ((float)i * 0.0625f) * pi.
//  9. Lists: 36 * count bytes of VILNode records (floats 16, 20, 24 zero) go to
//     word 48; word 32 = 0; Init__7VILList on this+20 and this+36; each record is
//     added to this+36 with AddTail__7VILListP7VILNode. A 16 + 32 * 2048 byte
//     block holds a count word of 32 and 32 elements of 2048 bytes (VILNode,
//     floats at 16..24 and 28..36 zero, __17VIParticleEmitter on +64 and +1056),
//     its pointer is word 4476; word 4460 = 0; Init__7VILList on this+4448 and
//     this+4464; the elements are added to this+4464.
// 10. A final block of stores sets floats 136, 140, 144, 148, 152, 156 and the
//     triples at 80, 92, 104 and 116 (see the constants in the code).
// Inferred: the object is the atmosphere (weather/sky) state; the tables look like
// precomputed ring, rain and star offsets. Unknown: field names, what the star and
// rain byte tables hold, and what Rotate* assume of the matrix. The two byte
// tables are file-local data of the image (sRainParticleDist, 600 bytes;
// mStarParticleDist, 384 bytes); their contents are not reproduced here.
extern "C" {
extern const unsigned char sRainParticleDist[600];
extern const unsigned char mStarParticleDist[384];

void SetEffectMix__12VIAtmospheref(void* object, float value);
void SetIntensity__12VIAtmospheref(void* object, float value);
void SetLavastormIntensity__12VIAtmospheref(void* object, float value);
void* __builtin_vec_new(unsigned int size);
void __7VILNode(void* node);
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);
void __17VIParticleEmitter(void* emitter);
int rand(void);
float sinf(float x);
float cosf(float x);
int CreateMaterial__8VIRasteri(void* raster, int flags);
void* Material__8VIRasteri(void* raster, int id);
void Init__10VIMateriali(void* material, int value);
void SetLayerFillType__10VIMateriali18VIMaterialFillType(void* material, int layer, int fill);
void SetLayerModulate__10VIMaterialii(void* material, int layer, int value);
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* material, int layer, int mode);
void SetLayerTextureLODBias__10VIMaterialif(void* material, int layer, float bias);
void SetZWrite__10VIMateriali(void* material, int value);
void SetZTest__10VIMateriali(void* material, int value);
void SetDither__10VIMateriali(void* material, int value);
void RotateHPR__10VIMatrix44RC7VIVect3(void* matrix, const void* angles);
void Mul__C10VIMatrix44RC7VIVect4R7VIVect4(const void* matrix, const void* in, void* out);
void _$_10VIMatrix44(void* matrix, int flags);

static float float_from_bits(unsigned int bits) {
    return __builtin_bit_cast(float, bits);
}
static float& float_at(unsigned char* base, int offset) {
    return *reinterpret_cast<float*>(base + offset);
}
static int& int_at(unsigned char* base, int offset) {
    return *reinterpret_cast<int*>(base + offset);
}
static unsigned int& word_at(unsigned char* base, int offset) {
    return *reinterpret_cast<unsigned int*>(base + offset);
}
static unsigned int to_word(const void* pointer) {
    return static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static unsigned char* pointer_at(unsigned char* base, int offset) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word_at(base, offset)));
}

struct Matrix {
    alignas(16) float m[16];
};
static void clear_matrix(Matrix& matrix) {
    for (int i = 0; i < 16; ++i) matrix.m[i] = 0.0f;
}

static void setup_material(void* raster, void* material, int blend_mode, bool lod_bias, int z_test) {
    Init__10VIMateriali(material, 1);
    SetLayerFillType__10VIMateriali18VIMaterialFillType(material, 0, 0);
    SetLayerModulate__10VIMaterialii(material, 0, 1);
    SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, blend_mode);
    if (lod_bias) SetLayerTextureLODBias__10VIMaterialif(material, 0, 1.0f);
    SetZWrite__10VIMateriali(material, 0);
    SetZTest__10VIMateriali(material, z_test);
    SetDither__10VIMateriali(material, 0);
    (void)raster;
}

int Init__12VIAtmospherelR8VIRasteri(void* object, long long value, void* raster) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    const float one_thirtieth = float_from_bits(0x3d088888u);
    const float tenth = float_from_bits(0x3dccccccu);
    const float byte_scale = float_from_bits(0x3b808080u);

    word_at(bytes, 8) = 200;
    float_at(bytes, 4) = 10.0f;
    *reinterpret_cast<long long*>(bytes + 1488) = value;
    *reinterpret_cast<long long*>(bytes + 1480) = value;
    for (int offset = 1224; offset >= 1196; offset -= 4) float_at(bytes, offset) = one_thirtieth;
    float_at(bytes, 60) = 0.0f; float_at(bytes, 64) = 0.0f; float_at(bytes, 68) = 0.0f;
    SetEffectMix__12VIAtmospheref(object, 0.0f);
    SetIntensity__12VIAtmospheref(object, 0.0f);
    float_at(bytes, 76) = 0.0f;
    SetLavastormIntensity__12VIAtmospheref(object, 0.0f);

    word_at(bytes, 1556) = 1;
    float_at(bytes, 140) = tenth;
    float_at(bytes, 1472) = 1.0f;
    word_at(bytes, 1476) = 1;
    float_at(bytes, 136) = tenth;
    float_at(bytes, 16) = 1.0f;
    word_at(bytes, 1548) = 1;
    word_at(bytes, 1552) = 1;
    float_at(bytes, 1564) = 1.0f;
    float_at(bytes, 132) = 0.0f;
    *reinterpret_cast<long long*>(bytes + 1480) = 0;
    float_at(bytes, 128) = 0.0f;
    int_at(bytes, 1248) = -1; int_at(bytes, 1504) = -1; int_at(bytes, 1508) = -1; int_at(bytes, 1512) = -1;
    word_at(bytes, 1516) = 0; word_at(bytes, 1520) = 0;
    int_at(bytes, 1524) = -1; int_at(bytes, 1528) = -1;
    float_at(bytes, 1544) = 0.0f;
    word_at(bytes, 1532) = 0; word_at(bytes, 1536) = 0; word_at(bytes, 1540) = 0;
    int_at(bytes, 1260) = -1; int_at(bytes, 1264) = -1; int_at(bytes, 1268) = -1;
    int_at(bytes, 3872) = -1; int_at(bytes, 1244) = -1; int_at(bytes, 3920) = -1;
    float_at(bytes, 3876) = 0.0f; float_at(bytes, 3880) = 0.0f; float_at(bytes, 3884) = 0.0f;
    word_at(bytes, 12) = 0;
    int count = static_cast<int>(word_at(bytes, 8));
    word_at(bytes, 48) = 0;
    word_at(bytes, 4476) = 0;
    int_at(bytes, 4488) = -1; int_at(bytes, 4496) = -1; int_at(bytes, 4500) = -1;
    float_at(bytes, 4504) = 0.0f; float_at(bytes, 4508) = 0.0f; float_at(bytes, 4512) = 0.0f;
    word_at(bytes, 1560) = 0;
    unsigned char* records = static_cast<unsigned char*>(__builtin_vec_new(count * 36));
    if (records != 0 && count != 0) {
        unsigned char* record = records;
        for (int remaining = count; remaining != 0; --remaining) {
            float_at(record, 0) = 0.0f; float_at(record, 4) = 0.0f; float_at(record, 8) = 0.0f;
            float_at(record, 12) = 0.0f; float_at(record, 16) = 0.0f; float_at(record, 20) = 0.0f;
            float_at(record, 24) = 0.0f;
            record += 36;
        }
    }
    word_at(bytes, 12) = to_word(records);

    const float full_turn = float_from_bits(0x40c90fdau);
    for (int i = 0; i < 64; ++i) {
        float theta = static_cast<float>(i) * full_turn;
        theta = theta * 0.015625f;
        theta = theta + theta;
        float_at(bytes, 168 + 12 * i) = sinf(theta);
        float_at(bytes, 172 + 12 * i) = cosf(theta);
        float_at(bytes, 176 + 12 * i) = cosf(theta);
        int mixed = rand() * 131;
        float_at(bytes, 936 + 4 * i) = static_cast<float>(mixed & 255) * byte_scale + 1.0f;
    }

    for (int i = 0; i < 2; ++i) {
        int id = CreateMaterial__8VIRasteri(raster, 0);
        word_at(bytes, 160 + 4 * i) = id;
        if (id < 0) return -1;
        setup_material(raster, Material__8VIRasteri(raster, id), 3, false, 1);
    }
    int id = CreateMaterial__8VIRasteri(raster, 0);
    word_at(bytes, 56) = id;
    if (id < 0) return -1;
    setup_material(raster, Material__8VIRasteri(raster, id), 1, false, 1);
    id = CreateMaterial__8VIRasteri(raster, 0);
    word_at(bytes, 1244) = id;
    if (id < 0) return -1;
    setup_material(raster, Material__8VIRasteri(raster, id), 1, true, 0);
    id = CreateMaterial__8VIRasteri(raster, 0);
    word_at(bytes, 3920) = id;
    if (id < 0) return -1;
    setup_material(raster, Material__8VIRasteri(raster, id), 1, true, 0);

    if (static_cast<int>(word_at(bytes, 8)) > 0) {
        const unsigned char* table = sRainParticleDist;
        int index = 0;
        int offset = 0;
        do {
            ++index;
            int first = rand();
            int second = rand();
            float size = float_at(bytes, 4);
            unsigned char* record = pointer_at(bytes, 12) + offset;
            offset += 36;
            float half = size * 0.5f;
            word_at(record, 32) = 0;
            float_at(record, 0) = (static_cast<float>(table[0]) * byte_scale) * size - half;
            float depth = ((static_cast<float>(first) - static_cast<float>(second)) * 4.656612873077393e-10f)
                          * float_from_bits(0x3da3d70au) + tenth;
            float_at(record, 4) = (static_cast<float>(table[1]) * byte_scale) * float_at(bytes, 4) - float_at(bytes, 4) * 0.5f;
            float_at(record, 8) = (static_cast<float>(table[2]) * byte_scale) * float_at(bytes, 4) - float_at(bytes, 4) * 0.5f;
            table += 3;
            float_at(record, 24) = depth;
            float_at(record, 12) = 1.0f;
            float_at(record, 16) = 1.0f;
            float_at(record, 20) = 1.0f;
        } while (index < static_cast<int>(word_at(bytes, 8)));
    }

    const float pi = float_from_bits(0x40490fdau);
    const unsigned char* star = mStarParticleDist;
    for (int i = 0; i < 128; ++i) {
        Matrix matrix;
        clear_matrix(matrix);
        float angles[3];
        int b0 = star[0], b1 = star[1], b2 = star[2];
        star += 3;
        angles[0] = (static_cast<float>(128 - b0) * float_from_bits(0x40133333u)) * float_from_bits(0x3cc90fdau);
        angles[1] = (static_cast<float>(128 - b1) * pi) * float_from_bits(0x3b7ae147u);
        angles[2] = (static_cast<float>(128 - b2) * pi) * float_from_bits(0x3b7ae147u);
        RotateHPR__10VIMatrix44RC7VIVect3(&matrix, angles);
        float direction[4] = {0.0f, 100.0f, 0.0f, 1.0f};
        Mul__C10VIMatrix44RC7VIVect4R7VIVect4(&matrix, direction, bytes + 1568 + 16 * i);
        int roll = rand() / 131;
        float_at(bytes, 1580 + 16 * i) = (static_cast<float>(roll & 255) * byte_scale) * 0.5f + float_from_bits(0x3e999999u);
        roll = rand() / 131;
        bytes[3616 + i] = static_cast<unsigned char>(roll & 31);
        _$_10VIMatrix44(&matrix, 2);
    }

    for (int i = 0; i < 32; ++i) {
        float theta = (static_cast<float>(i) * 0.0625f) * pi;
        float wave = sinf(theta);
        wave = wave * 0.5f;
        wave = wave + 0.5f;
        wave = wave * float_from_bits(0x3ecccccc);
        float_at(bytes, 3744 + 4 * i) = wave + 0.5f;
    }

    count = static_cast<int>(word_at(bytes, 8));
    unsigned char* nodes = static_cast<unsigned char*>(__builtin_vec_new(count * 36));
    if (nodes != 0) {
        unsigned char* node = nodes;
        for (int remaining = count; remaining != 0; --remaining) {
            __7VILNode(node);
            float_at(node, 16) = 0.0f; float_at(node, 20) = 0.0f; float_at(node, 24) = 0.0f;
            node += 36;
        }
    }
    word_at(bytes, 48) = to_word(nodes);
    word_at(bytes, 32) = 0;
    Init__7VILList(bytes + 20);
    Init__7VILList(bytes + 36);
    if (static_cast<int>(word_at(bytes, 8)) > 0) {
        int index = 0;
        int offset = 0;
        do {
            ++index;
            AddTail__7VILListP7VILNode(bytes + 36, pointer_at(bytes, 48) + offset);
            offset += 36;
        } while (index < static_cast<int>(word_at(bytes, 8)));
    }

    unsigned char* block = static_cast<unsigned char*>(__builtin_vec_new(65552));
    unsigned char* elements = 0;
    if (block != 0) {
        elements = block + 16;
        word_at(block, 0) = 32;
        unsigned char* element = elements;
        for (int remaining = 32; remaining != 0; --remaining) {
            __7VILNode(element);
            float_at(element, 16) = 0.0f; float_at(element, 20) = 0.0f; float_at(element, 24) = 0.0f;
            float_at(element, 28) = 0.0f; float_at(element, 32) = 0.0f; float_at(element, 36) = 0.0f;
            __17VIParticleEmitter(element + 64);
            __17VIParticleEmitter(element + 1056);
            element += 2048;
        }
    }
    word_at(bytes, 4476) = to_word(elements);
    word_at(bytes, 4460) = 0;
    Init__7VILList(bytes + 4448);
    Init__7VILList(bytes + 4464);
    int slot = 0;
    do {
        AddTail__7VILListP7VILNode(bytes + 4464, pointer_at(bytes, 4476) + (slot << 11));
        ++slot;
    } while (slot < 32);

    float_at(bytes, 136) = float_from_bits(0x3dc28f5cu);
    float_at(bytes, 152) = float_from_bits(0x3f4ccccc);
    float_at(bytes, 144) = 0.0f;
    float_at(bytes, 88) = float_from_bits(0x3b23d70au);
    float_at(bytes, 84) = float_from_bits(0x3c03126eu);
    float_at(bytes, 80) = float_from_bits(0x3b23d70au);
    float_at(bytes, 108) = float_from_bits(0xbe577857u);
    float_at(bytes, 112) = 0.0f;
    float_at(bytes, 104) = 0.0f;
    float_at(bytes, 140) = float_from_bits(0x3d4ccccc);
    float_at(bytes, 148) = 0.0f;
    float_at(bytes, 156) = 0.0f;
    float_at(bytes, 100) = float_from_bits(0x3c2dab9fu);
    float_at(bytes, 96) = float_from_bits(0x3c8d25edu);
    float_at(bytes, 92) = float_from_bits(0x3c2dab9fu);
    float_at(bytes, 124) = 0.0f;
    float_at(bytes, 120) = float_from_bits(0xbcb4245fu);
    float_at(bytes, 116) = 0.0f;
    return 0;
}
}
