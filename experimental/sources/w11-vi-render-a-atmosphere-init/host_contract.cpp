#include "init.cpp"
#include <string.h>
#include <math.h>

// the two image-local byte tables are supplied by the host check
const unsigned char sRainParticleDist[600] = {3,10,17,24,31,38,45,52,59,66,73,80,87,94,101,108,115,122,129,136,143,150,157,164,171,178,185,192,199,206,213,220,227,234,241,248,255,6,13,20,27,34,41,48,55,62,69,76,83,90,97,104,111,118,125,132,139,146,153,160,167,174,181,188,195,202,209,216,223,230,237,244,251,2,9,16,23,30,37,44,51,58,65,72,79,86,93,100,107,114,121,128,135,142,149,156,163,170,177,184,191,198,205,212,219,226,233,240,247,254,5,12,19,26,33,40,47,54,61,68,75,82,89,96,103,110,117,124,131,138,145,152,159,166,173,180,187,194,201,208,215,222,229,236,243,250,1,8,15,22,29,36,43,50,57,64,71,78,85,92,99,106,113,120,127,134,141,148,155,162,169,176,183,190,197,204,211,218,225,232,239,246,253,4,11,18,25,32,39,46,53,60,67,74,81,88,95,102,109,116,123,130,137,144,151,158,165,172,179,186,193,200,207,214,221,228,235,242,249,0,7,14,21,28,35,42,49,56,63,70,77,84,91,98,105,112,119,126,133,140,147,154,161,168,175,182,189,196,203,210,217,224,231,238,245,252,3,10,17,24,31,38,45,52,59,66,73,80,87,94,101,108,115,122,129,136,143,150,157,164,171,178,185,192,199,206,213,220,227,234,241,248,255,6,13,20,27,34,41,48,55,62,69,76,83,90,97,104,111,118,125,132,139,146,153,160,167,174,181,188,195,202,209,216,223,230,237,244,251,2,9,16,23,30,37,44,51,58,65,72,79,86,93,100,107,114,121,128,135,142,149,156,163,170,177,184,191,198,205,212,219,226,233,240,247,254,5,12,19,26,33,40,47,54,61,68,75,82,89,96,103,110,117,124,131,138,145,152,159,166,173,180,187,194,201,208,215,222,229,236,243,250,1,8,15,22,29,36,43,50,57,64,71,78,85,92,99,106,113,120,127,134,141,148,155,162,169,176,183,190,197,204,211,218,225,232,239,246,253,4,11,18,25,32,39,46,53,60,67,74,81,88,95,102,109,116,123,130,137,144,151,158,165,172,179,186,193,200,207,214,221,228,235,242,249,0,7,14,21,28,35,42,49,56,63,70,77,84,91,98,105,112,119,126,133,140,147,154,161,168,175,182,189,196,203,210,217,224,231,238,245,252,3,10,17,24,31,38,45,52,59,66,73,80,87,94,101,108,115,122,129,136,143,150,157,164,171,178,185,192,199,206,213,220,227,234,241,248,255,6,13,20,27,34,41,48,55,62,69,76,83,90,97,104,111,118,125,132,139,146,153,160,167,174,181,188,195,202,209,216,223,230,237,244,251,2,9,16,23,30,37,44,51,58,65,72,79,86,93,100};
const unsigned char mStarParticleDist[384] = {11,16,21,26,31,36,41,46,51,56,61,66,71,76,81,86,91,96,101,106,111,116,121,126,131,136,141,146,151,156,161,166,171,176,181,186,191,196,201,206,211,216,221,226,231,236,241,246,251,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245,250,255,4,9,14,19,24,29,34,39,44,49,54,59,64,69,74,79,84,89,94,99,104,109,114,119,124,129,134,139,144,149,154,159,164,169,174,179,184,189,194,199,204,209,214,219,224,229,234,239,244,249,254,3,8,13,18,23,28,33,38,43,48,53,58,63,68,73,78,83,88,93,98,103,108,113,118,123,128,133,138,143,148,153,158,163,168,173,178,183,188,193,198,203,208,213,218,223,228,233,238,243,248,253,2,7,12,17,22,27,32,37,42,47,52,57,62,67,72,77,82,87,92,97,102,107,112,117,122,127,132,137,142,147,152,157,162,167,172,177,182,187,192,197,202,207,212,217,222,227,232,237,242,247,252,1,6,11,16,21,26,31,36,41,46,51,56,61,66,71,76,81,86,91,96,101,106,111,116,121,126,131,136,141,146,151,156,161,166,171,176,181,186,191,196,201,206,211,216,221,226,231,236,241,246,251,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245,250,255,4,9,14,19,24,29,34,39,44,49,54,59,64,69,74,79,84,89,94,99,104,109,114,119,124,129,134};

enum { MIX = 1, INTENSITY, LAVA, NEW, SIN, COS, RAND, CREATE, MATERIAL, MINIT, FILL, MODULATE, BLEND, LOD, ZWRITE, ZTEST, DITHER,
       HPR, MUL, DTOR, NODE, ADDTAIL, LISTINIT, EMIT };
static int events[8192]; static int event_n;
static unsigned int rand_n;
static unsigned int new_sizes[4]; static int new_n; static int fail_new = -1;
static unsigned char arena[3][65600];
static float sin_args[256]; static int sin_n; static float cos_args[256]; static int cos_n;
static int create_n; static int fail_create = -1;
static int blend_modes[8]; static int blend_n; static int ztest_values[8]; static int ztest_n; static int lod_n; static float lod_values[4];
static int added_to[512]; static unsigned char* add_args[512]; static unsigned char* add_lists[512]; static int add_n;
static unsigned char* node_args[512]; static int node_n; static int emit_n; static unsigned char* emit_args[80];
static float hpr_angles[128][3]; static int hpr_zero = 1; static int hpr_n; static int mul_n; static float mul_in[128][4]; static unsigned char* mul_out[128];
static int dtor_n; static int dtor_ok = 1; static int minit_values[8]; static int minit_n;
static float arg_values[3]; static int mat_ids[8]; static int mat_n; static int fill_ok = 1;
static unsigned char* list_inits[4]; static int list_n;

static unsigned int next_rand() { return (++rand_n * 2654435761u) & 0x7fffffffu; }
static void ev(int e) { events[event_n++] = e; }

extern "C" void SetEffectMix__12VIAtmospheref(void*, float v) { arg_values[0] = v; ev(MIX); }
extern "C" void SetIntensity__12VIAtmospheref(void*, float v) { arg_values[1] = v; ev(INTENSITY); }
extern "C" void SetLavastormIntensity__12VIAtmospheref(void*, float v) { arg_values[2] = v; ev(LAVA); }
extern "C" void* __builtin_vec_new(unsigned int size) {
    new_sizes[new_n] = size; int k = new_n++; ev(NEW);
    return k == fail_new ? 0 : arena[k];
}
extern "C" void __7VILNode(void* n) { node_args[node_n++] = static_cast<unsigned char*>(n); ev(NODE); }
extern "C" void Init__7VILList(void* l) { list_inits[list_n++] = static_cast<unsigned char*>(l); ev(LISTINIT); }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { add_lists[add_n] = static_cast<unsigned char*>(l); add_args[add_n++] = static_cast<unsigned char*>(n); ev(ADDTAIL); }
extern "C" void __17VIParticleEmitter(void* e) { emit_args[emit_n++] = static_cast<unsigned char*>(e); ev(EMIT); }
extern "C" int rand(void) { ev(RAND); return static_cast<int>(next_rand()); }
extern "C" float sinf(float x) { sin_args[sin_n++] = x; ev(SIN); return 0.5f; }
extern "C" float cosf(float x) { cos_args[cos_n++] = x; ev(COS); return 0.25f; }
extern "C" int CreateMaterial__8VIRasteri(void*, int flags) { ev(CREATE); if (flags != 0) return -99; int k = create_n++; return k == fail_create ? -1 : 5 + k; }
extern "C" void* Material__8VIRasteri(void*, int id) { ev(MATERIAL); mat_ids[mat_n++] = id; return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(0x1000 + id)); }
extern "C" void Init__10VIMateriali(void*, int v) { ev(MINIT); minit_values[minit_n++] = v; }
extern "C" void SetLayerFillType__10VIMateriali18VIMaterialFillType(void*, int layer, int fill) { ev(FILL); if (layer != 0 || fill != 0) fill_ok = 0; }
extern "C" void SetLayerModulate__10VIMaterialii(void*, int layer, int v) { ev(MODULATE); if (layer != 0 || v != 1) fill_ok = 0; }
extern "C" void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void*, int layer, int mode) { ev(BLEND); if (layer != 0) fill_ok = 0; blend_modes[blend_n++] = mode; }
extern "C" void SetLayerTextureLODBias__10VIMaterialif(void*, int layer, float bias) { ev(LOD); if (layer != 0) fill_ok = 0; lod_values[lod_n++] = bias; }
extern "C" void SetZWrite__10VIMateriali(void*, int v) { ev(ZWRITE); if (v != 0) fill_ok = 0; }
extern "C" void SetZTest__10VIMateriali(void*, int v) { ev(ZTEST); ztest_values[ztest_n++] = v; }
extern "C" void SetDither__10VIMateriali(void*, int v) { ev(DITHER); if (v != 0) fill_ok = 0; }
extern "C" void RotateHPR__10VIMatrix44RC7VIVect3(void* m, const void* a) {
    const unsigned int* w = static_cast<const unsigned int*>(m);
    for (int i = 0; i < 16; ++i) if (w[i] != 0) hpr_zero = 0;
    if ((reinterpret_cast<__UINTPTR_TYPE__>(m) & 15) != 0) hpr_zero = 0;
    memcpy(hpr_angles[hpr_n++], a, 12); ev(HPR);
}
extern "C" void Mul__C10VIMatrix44RC7VIVect4R7VIVect4(const void*, const void* in, void* out) {
    memcpy(mul_in[mul_n], in, 16); mul_out[mul_n++] = static_cast<unsigned char*>(out);
    float v[4] = {1.5f, 2.5f, 3.5f, static_cast<float>(mul_n)}; memcpy(out, v, 16); ev(MUL);
}
extern "C" void _$_10VIMatrix44(void*, int flags) { if (flags != 2) dtor_ok = 0; ++dtor_n; ev(DTOR); }

static float getf(const unsigned char* b, int o) { float v; memcpy(&v, b + o, 4); return v; }
static unsigned int getw(const unsigned char* b, int o) { unsigned int v; memcpy(&v, b + o, 4); return v; }
static unsigned int bits_of(float f) { unsigned int v; memcpy(&v, &f, 4); return v; }
static void reset_all() {
    event_n = 0; rand_n = 0; new_n = 0; sin_n = cos_n = create_n = blend_n = ztest_n = lod_n = add_n = node_n = emit_n = 0;
    hpr_zero = 1; hpr_n = mul_n = dtor_n = minit_n = mat_n = list_n = 0; dtor_ok = fill_ok = 1; fail_new = fail_create = -1;
    for (int k = 0; k < 3; ++k) memset(arena[k], 0x5a, sizeof arena[k]);
}

int main() {
    static unsigned char object[4640];
    static int raster;
    memset(object, 0xee, sizeof object);
    reset_all();
    int r = Init__12VIAtmospherelR8VIRasteri(object, 0x1122334455667788LL, &raster);
    if (r != 0) return 1;
    if (events[0] != MIX || events[1] != INTENSITY || events[2] != LAVA || events[3] != NEW) return 2;
    if (arg_values[0] != 0.0f || arg_values[1] != 0.0f || arg_values[2] != 0.0f) return 3;
    long long v; memcpy(&v, object + 1488, 8);
    if (v != 0x1122334455667788LL) return 4;
    memcpy(&v, object + 1480, 8);
    if (v != 0) return 5;
    if (getw(object, 8) != 200 || getf(object, 4) != 10.0f) return 6;
    for (int o = 1196; o <= 1224; o += 4) if (getw(object, o) != 0x3d088888u) return 7;
    if (new_sizes[0] != 7200 || new_sizes[1] != 7200 || new_sizes[2] != 65552) return 8;
    // sin/cos table
    for (int i = 0; i < 64; ++i) {
        float theta = static_cast<float>(i) * float_from_bits(0x40c90fdau);
        theta = theta * 0.015625f; theta = theta + theta;
        if (sin_args[i] != theta || cos_args[2 * i] != theta || cos_args[2 * i + 1] != theta) return 9;
        if (getf(object, 168 + 12 * i) != 0.5f || getf(object, 172 + 12 * i) != 0.25f || getf(object, 176 + 12 * i) != 0.25f) return 10;
    }
    // randoms: one per table entry is consumed in order (rand number 1 is for entry 0)
    for (int i = 0; i < 64; ++i) {
        unsigned int rv = (static_cast<unsigned int>(i + 1) * 2654435761u) & 0x7fffffffu;
        float expected = static_cast<float>((rv * 131) & 255) * float_from_bits(0x3b808080u) + 1.0f;
        if (getf(object, 936 + 4 * i) != expected) return 11;
    }
    // materials
    static const int expect_ids[5] = {5, 6, 7, 8, 9};
    if (mat_n != 5) return 12;
    for (int i = 0; i < 5; ++i) if (mat_ids[i] != expect_ids[i]) return 12;
    if (getw(object, 160) != 5 || getw(object, 164) != 6 || getw(object, 56) != 7 || getw(object, 1244) != 8 || getw(object, 3920) != 9) return 13;
    static const int expect_blend[5] = {3, 3, 1, 1, 1}, expect_ztest[5] = {1, 1, 1, 0, 0};
    for (int i = 0; i < 5; ++i) if (blend_modes[i] != expect_blend[i] || ztest_values[i] != expect_ztest[i]) return 14;
    if (lod_n != 2 || lod_values[0] != 1.0f || lod_values[1] != 1.0f || minit_n != 5 || !fill_ok) return 15;
    for (int i = 0; i < 5; ++i) if (minit_values[i] != 1) return 15;
    // rain records
    unsigned char* rec = arena[0];
    for (int i = 0; i < 200; ++i) {
        unsigned int r1 = (static_cast<unsigned int>(65 + 2 * i) * 2654435761u) & 0x7fffffffu;
        unsigned int r2 = (static_cast<unsigned int>(66 + 2 * i) * 2654435761u) & 0x7fffffffu;
        const unsigned char* p = rec + 36 * i;
        for (int k = 0; k < 3; ++k) {
            float expected = (static_cast<float>(sRainParticleDist[3 * i + k]) * float_from_bits(0x3b808080u)) * 10.0f - 10.0f * 0.5f;
            if (getf(p, 4 * k) != expected) return 16;
        }
        float depth = ((static_cast<float>(static_cast<int>(r1)) - static_cast<float>(static_cast<int>(r2))) * 4.656612873077393e-10f)
                      * float_from_bits(0x3da3d70au) + float_from_bits(0x3dccccccu);
        if (getf(p, 24) != depth || getf(p, 12) != 1.0f || getf(p, 16) != 1.0f || getf(p, 20) != 1.0f || getw(p, 32) != 0) return 17;
        if (getw(p, 28) != 0x5a5a5a5au) return 18;   // untouched bytes stay
    }
    if (getw(object, 12) != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(arena[0]))) return 19;
    // stars
    if (hpr_n != 128 || mul_n != 128 || dtor_n != 128 || !hpr_zero || !dtor_ok) return 20;
    const float pi = float_from_bits(0x40490fdau);
    for (int i = 0; i < 128; ++i) {
        const unsigned char* t = mStarParticleDist + 3 * i;
        float a0 = (static_cast<float>(128 - t[0]) * float_from_bits(0x40133333u)) * float_from_bits(0x3cc90fdau);
        float a1 = (static_cast<float>(128 - t[1]) * pi) * float_from_bits(0x3b7ae147u);
        float a2 = (static_cast<float>(128 - t[2]) * pi) * float_from_bits(0x3b7ae147u);
        if (hpr_angles[i][0] != a0 || hpr_angles[i][1] != a1 || hpr_angles[i][2] != a2) return 21;
        if (mul_in[i][0] != 0.0f || mul_in[i][1] != 100.0f || mul_in[i][2] != 0.0f || mul_in[i][3] != 1.0f) return 22;
        if (mul_out[i] != object + 1568 + 16 * i) return 23;
        unsigned int q1 = ((static_cast<unsigned int>(465 + 2 * i) * 2654435761u) & 0x7fffffffu) / 131;
        unsigned int q2 = ((static_cast<unsigned int>(466 + 2 * i) * 2654435761u) & 0x7fffffffu) / 131;
        float w = (static_cast<float>(q1 & 255) * float_from_bits(0x3b808080u)) * 0.5f + float_from_bits(0x3e999999u);
        if (getf(object, 1580 + 16 * i) != w) return 24;
        if (getf(object, 1568 + 16 * i) != 1.5f || getf(object, 1572 + 16 * i) != 2.5f || getf(object, 1576 + 16 * i) != 3.5f) return 25;
        if (object[3616 + i] != (q2 & 31)) return 26;
    }
    // wave table
    for (int i = 0; i < 32; ++i) {
        float theta = (static_cast<float>(i) * 0.0625f) * pi;
        if (sin_args[64 + i] != theta) return 27;
        float expected = (((0.5f * 0.5f) + 0.5f) * float_from_bits(0x3ecccccc)) + 0.5f;
        if (getf(object, 3744 + 4 * i) != expected) return 28;
    }
    // lists and nodes
    if (node_n != 232 || add_n != 232 || emit_n != 64 || list_n != 4) return 29;
    if (list_inits[0] != object + 20 || list_inits[1] != object + 36 || list_inits[2] != object + 4448 || list_inits[3] != object + 4464) return 30;
    for (int i = 0; i < 200; ++i) {
        if (node_args[i] != arena[1] + 36 * i || add_args[i] != arena[1] + 36 * i || add_lists[i] != object + 36) return 31;
        if (getf(arena[1] + 36 * i, 16) != 0.0f || getf(arena[1] + 36 * i, 20) != 0.0f || getf(arena[1] + 36 * i, 24) != 0.0f) return 32;
    }
    if (getw(object, 48) != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(arena[1])) || getw(object, 32) != 0) return 33;
    if (getw(arena[2], 0) != 32) return 34;
    for (int i = 0; i < 32; ++i) {
        unsigned char* e = arena[2] + 16 + 2048 * i;
        if (node_args[200 + i] != e || add_args[200 + i] != e || add_lists[200 + i] != object + 4464) return 35;
        if (emit_args[2 * i] != e + 64 || emit_args[2 * i + 1] != e + 1056) return 36;
        for (int o = 16; o <= 36; o += 4) if (getf(e, o) != 0.0f) return 37;
    }
    if (getw(object, 4476) != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(arena[2] + 16)) || getw(object, 4460) != 0) return 38;
    // plain stores
    if (getw(object, 1556) != 1 || getw(object, 1476) != 1 || getw(object, 1548) != 1 || getw(object, 1552) != 1) return 39;
    if (getf(object, 1472) != 1.0f || getf(object, 16) != 1.0f || getf(object, 1564) != 1.0f) return 40;
    static const int minus_one[] = {1248, 1504, 1508, 1512, 1524, 1528, 1260, 1264, 1268, 3872, 4488, 4496, 4500};
    for (int o : minus_one) if (getw(object, o) != 0xffffffffu) return 41;
    static const int zero_words[] = {1516, 1520, 1532, 1536, 1540, 1560, 3876, 3880, 3884, 4504, 4508, 4512, 60, 64, 68, 76, 128, 132, 1544};
    for (int o : zero_words) if (getw(object, o) != 0) return 42;
    if (getw(object, 140) != 0x3d4cccccu || getw(object, 136) != 0x3dc28f5cu || getw(object, 152) != 0x3f4cccccu || getw(object, 144) != 0 ||
        getw(object, 148) != 0 || getw(object, 156) != 0) return 43;
    if (getw(object, 80) != 0x3b23d70au || getw(object, 84) != 0x3c03126eu || getw(object, 88) != 0x3b23d70au) return 44;
    if (getw(object, 92) != 0x3c2dab9fu || getw(object, 96) != 0x3c8d25edu || getw(object, 100) != 0x3c2dab9fu) return 45;
    if (getw(object, 104) != 0 || getw(object, 108) != 0xbe577857u || getw(object, 112) != 0) return 46;
    if (getw(object, 116) != 0 || getw(object, 120) != 0xbcb4245fu || getw(object, 124) != 0) return 47;

    // a negative material id is stored and returned at once
    memset(object, 0xee, sizeof object);
    reset_all(); fail_create = 2;
    r = Init__12VIAtmospherelR8VIRasteri(object, 1, &raster);
    if (r != -1 || getw(object, 56) != 0xffffffffu || create_n != 3 || mat_n != 2 || new_n != 1) return 48;
    if (getw(object, 160) != 5 || getw(object, 164) != 6 || getw(object, 1244) != 0xffffffffu) return 49;
    reset_all(); fail_create = 0;
    memset(object, 0xee, sizeof object);
    r = Init__12VIAtmospherelR8VIRasteri(object, 1, &raster);
    if (r != -1 || getw(object, 160) != 0xffffffffu || mat_n != 0) return 50;
    reset_all(); fail_create = 4;
    r = Init__12VIAtmospherelR8VIRasteri(object, 1, &raster);
    if (r != -1 || getw(object, 3920) != 0xffffffffu || mat_n != 4 || new_n != 1) return 51;

    // allocation failures: the node block and the emitter block simply leave their pointer words zero
    reset_all(); fail_new = 1;
    r = Init__12VIAtmospherelR8VIRasteri(object, 1, &raster);
    if (r != 0 || getw(object, 48) != 0 || node_n != 32 || add_n != 232) return 52;
    for (int i = 0; i < 200; ++i) if (add_args[i] != reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(36 * i))) return 53;
    reset_all(); fail_new = 2;
    r = Init__12VIAtmospherelR8VIRasteri(object, 1, &raster);
    if (r != 0 || getw(object, 4476) != 0 || emit_n != 0 || add_n != 232) return 54;
    for (int i = 0; i < 32; ++i) if (add_args[200 + i] != reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(i << 11))) return 55;
    return 0;
}
