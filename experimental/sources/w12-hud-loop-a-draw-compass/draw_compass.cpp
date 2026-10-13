// Observed drawCompass__Fv at 0x001c6a50, 852 bytes, 272-byte frame. Nothing happens when
// localNmPlayersAlive is zero, validLocalPlayer is -1, the word at byte 21872 of that player record
// (23984-byte stride) is zero, DLG_Active() is nonzero, or the byte drawCompassOn is zero.
// Otherwise: a 64-byte copy of worldViewMatrix is made and matTrans(copy, 0, 0, 0) cancels its
// translation; matUntransform(out, copy, in) is applied with in = the three float words at
// .lit4 0x00480604.. (bit patterns 0xbde977c8, 0x3e20c38f, 0xc0b8273d), and worldCameraPos is added to
// out. A Matrix34 is set to identity and translated by out. With e the word at record byte 21872, the
// angle is iatan2(e.f[5] - drawCompassPoint[1], e.f[4] - drawCompassPoint[0]) and matRot(matrix, 2,
// (short)(angle - 32768)) rotates it. Then modelSetNoZRead(); the model, then the texture, are
// looked up as lumpFindResource("compass.lmp", "compass.vif") and ("compass.lmp", "compass.tex");
// modelDraw(model, texture, 5, (0, 0, 1), matrix, 0, -1, 0, 0); modelResetOptionalParams();
// lightDisableVolumetricLighting(); lightDisableLocalLights(); the same two lookups and the same
// modelDraw call again; modelResetOptionalParams(); lightResetOptionalParams(). Finally
// fontStart(smallFont, 5, 0x632f6159, -1); fontPrintWrapCenteredu(65, 394, drawCompassTitle, 131, -100,
// 500, 18); fontEnd().
// Inferred: draws a compass model that points at drawCompassPoint for the local player, with a title.
// Unknown: the layout of the entity (only floats at +16 and +20 are read), the matrix typedefs'
// field names, why the model is drawn twice, and the meaning of the constant triple.
extern "C" {
extern int localNmPlayersAlive;
extern int validLocalPlayer;
extern unsigned char player[];
extern signed char drawCompassOn;
extern float worldViewMatrix[16];
extern float worldCameraPos[3];
extern float drawCompassPoint[];
extern unsigned short drawCompassTitle[];
extern void* smallFont;
int DLG_Active__Fv();
void matTrans__FP8Matrix44fff(float* matrix, float x, float y, float z);
void matUntransform__FP6Point3P8Matrix44T0(float* out, float* matrix, float* in);
void matIdent__FP8Matrix34(float* matrix);
void matTrans__FP8Matrix34fff(float* matrix, float x, float y, float z);
int iatan2__Fff(float y, float x);
void matRot__FP8Matrix34is(float* matrix, int axis, short angle);
void modelSetNoZRead__Fv();
void* lumpFindResource__FPcT0(const char* group, const char* name);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const float* offset, float* matrix, void* animation,
    unsigned long flags, void* record, int extra);
void modelResetOptionalParams__Fv();
void lightDisableVolumetricLighting__Fv();
void lightDisableLocalLights__Fv();
void lightResetOptionalParams__Fv();
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
void fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int width, int a, int b, int c);
void fontEnd__Fv();
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_draw_compass_host_base;
#endif

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

// The record word is a 32-bit target address; the host contract maps it into a test buffer.
const unsigned char* entity_at(const unsigned char* address) {
    unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_draw_compass_host_base + word;
#else
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

float float_from_bits(unsigned bits) {
    union {
        unsigned word;
        float value;
    } u;
    u.word = bits;
    return u.value;
}

float float_at(const unsigned char* address) { return float_from_bits(load_u32(address)); }
}

extern "C" void drawCompass__Fv() {
    float lookAt[3] = {float_from_bits(0xbde977c8u), float_from_bits(0x3e20c38fu), float_from_bits(0xc0b8273du)};
    if (localNmPlayersAlive == 0) return;
    if (validLocalPlayer == -1) return;
    if (load_u32(player + static_cast<unsigned>(validLocalPlayer) * 23984u + 21872u) == 0) return;
    if (DLG_Active__Fv() != 0) return;
    if (drawCompassOn == 0) return;

    float view[16];
    for (int i = 0; i < 16; ++i) view[i] = worldViewMatrix[i];
    matTrans__FP8Matrix44fff(view, 0.0f, 0.0f, 0.0f);
    float position[3];
    matUntransform__FP6Point3P8Matrix44T0(position, view, lookAt);
    position[0] = position[0] + worldCameraPos[0];
    position[1] = position[1] + worldCameraPos[1];
    position[2] = position[2] + worldCameraPos[2];

    float matrix[12];
    matIdent__FP8Matrix34(matrix);
    matTrans__FP8Matrix34fff(matrix, position[0], position[1], position[2]);
    const unsigned char* entity = entity_at(player + static_cast<unsigned>(validLocalPlayer) * 23984u + 21872u);
    int angle = iatan2__Fff(float_at(entity + 20) - drawCompassPoint[1], float_at(entity + 16) - drawCompassPoint[0]);
    matRot__FP8Matrix34is(matrix, 2, static_cast<short>(angle - 32768));
    modelSetNoZRead__Fv();

    const float up[3] = {0.0f, 0.0f, 1.0f};
    void* model = lumpFindResource__FPcT0("compass.lmp", "compass.vif");
    void* texture = lumpFindResource__FPcT0("compass.lmp", "compass.tex");
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 5, up, matrix, nullptr, 0xffffffffUL, nullptr, 0);
    modelResetOptionalParams__Fv();
    lightDisableVolumetricLighting__Fv();
    lightDisableLocalLights__Fv();
    model = lumpFindResource__FPcT0("compass.lmp", "compass.vif");
    texture = lumpFindResource__FPcT0("compass.lmp", "compass.tex");
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 5, up, matrix, nullptr, 0xffffffffUL, nullptr, 0);
    modelResetOptionalParams__Fv();
    lightResetOptionalParams__Fv();

    fontStart__FP4FontiUii(smallFont, 5, 0x632f6159u, -1);
    fontPrintWrapCenteredu__FiiPUsiiii(65, 394, drawCompassTitle, 131, -100, 500, 18);
    fontEnd__Fv();
}
