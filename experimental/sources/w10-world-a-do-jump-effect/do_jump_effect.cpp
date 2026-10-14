// Observed doJumpEffect__Fi at 0x00283b00, 2040 bytes in SLUS_205.65. One int argument `n`
// (read as a float multiplier, in the code below `amount`). The routine is the per-frame
// effect of the jump gate (the names of the globals are the image's own symbol names).
//  1. hudNoDraw = 1 and hudLetterBox() is called. A factor `zoomFactor` starts at 0.25. When
//     Camera::GetToggle(gameCam) is nonzero it becomes 1.2 (the 32-bit constant 0x3f999999), and
//     then 1.0 unless that toggle equals 2.
//  2. engineSetBlur((int)(jumpGateTime * 24 + 80)); then the camera yaw is raised by
//     (int)(jumpGateTime * 186 * amount) and the zoom distance lowered by
//     (int)(jumpGateTime * amount * (zoomFactor * 1.654)) (as a float), and
//     Camera::SetNoTargetUpdates(gameCam, 5).
//  3. With jumpGateHurtDest nonzero: for each of the nmPlayersAlive players (player number
//     playersAlive[i]), one step of a linear-congruential generator (state eRandState,
//     state * 1664525 + 1013904223, using the upper 16 bits modulo 10) decides whether the player
//     is chosen (when the remainder is 0). A chosen player whose jumpHidden byte is zero starts a
//     lightning bolt from 50 above its position to jumpGatePos (P_StartLightning with the image's
//     constants 492, 4, 7.7, 4.4 and colour 0x5e808080) and a creature shock effect (fxRunCreatureShock
//     with the object's virtual +128 result as model, 1.5, and the floats at +224 and +228).
//     Chosen or not, the short fields at +140..+154 of the player's object are set (10 at +140,
//     +142, +148, +150; 128 at +144, +152; 64 at +146, +154).
//  4. Otherwise: when `jumping` is 30, every living player's object gets frameNm at +252 and 120 at
//     +936. Then, with count = (int)(jumpGateTime * 35): for every living player 50 particles
//     (jumpParticleJump) are made at a random point on the line from 50 above the player's position
//     towards jumpGatePos (four generator steps per particle: fraction along the line, then x, y, z
//     of a velocity of (u - 0.5) * 5); then `count` more particles from random model shadow
//     points (modelGetNmShadowPoints / modelGetShadowPos with a generator step choosing the point,
//     velocity from three more steps).
// Inferred: this is the effect around entering a jump gate; no other behavior is claimed.
// Unknown: the types behind the object offsets used, the meaning of the constants, and the
// generator's owner (eRandState is shared).
//
// Host builds define NORRATH_HOST_ARENA: addresses stored in memory are offsets into an arena.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_base;
extern "C" unsigned char* norrath_host_slot_call_pointer(unsigned function, unsigned char* self);
#endif

struct Point3 {
    float x, y, z;
};

extern "C" {
extern signed char hudNoDraw;
extern signed char jumpHidden;
extern float jumpGateTime;
extern int jumpGateHurtDest;
extern int nmPlayersAlive;
extern int eRandState;
extern int jumping;
extern int frameNm;
extern unsigned gameCam;
extern int playersAlive[];
extern unsigned char player[];
extern Point3 jumpGatePos;
extern unsigned char jumpParticleJump[];

void hudLetterBox__Fv();
int GetToggle__6Camera(void* camera);
void engineSetBlur__Fi(int amount);
int GetYaw__6Camera(void* camera);
void SetYaw__6Camerai(void* camera, int yaw);
float GetZoomDistance__6Camera(void* camera);
void SetZoomDistance__6Cameraf(void* camera, float distance);
void SetNoTargetUpdates__6Camerai(void* camera, int frames);
void P_StartLightning__FG6Point3T0iiffUi(Point3* from, Point3* to, int a, int b, float c, float d, unsigned color);
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
    void* model, void* animation, Point3* position, short yaw, void* fx_state, float a, float b, float c, unsigned color);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float arg);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
int modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, Point3* position, short angle, int index, Point3* out, float* scratch, void* matrix, float scale);
}

namespace {
inline unsigned char* at(unsigned word) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
template <class T> inline T& field(unsigned char* base, unsigned offset) {
    return *reinterpret_cast<T*>(base + offset);
}
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
inline unsigned char* object_of(int playerNumber) {
    return at(field<unsigned>(player + playerNumber * 23984, 21872));
}
// One step of the generator used throughout: state = state * 1664525 + 1013904223.
inline unsigned step(unsigned state) { return state * 0x19660du + 0x3c6ef35fu; }
}

extern "C" void doJumpEffect__Fi(int n) {
    hudNoDraw = 1;
    hudLetterBox__Fv();
    float zoomFactor = 0.25f;
    void* camera = at(gameCam);
    if (GetToggle__6Camera(camera) != 0) {
        zoomFactor = constant(0x3f999999u);
        if (GetToggle__6Camera(camera) != 2) zoomFactor = 1.0f;
    }
    const float amount = static_cast<float>(n);
    engineSetBlur__Fi(static_cast<int>(jumpGateTime * 24.0f + 80.0f));
    int yaw = GetYaw__6Camera(camera);
    SetYaw__6Camerai(camera, yaw + static_cast<int>(jumpGateTime * 186.0f * amount));
    float zoom = GetZoomDistance__6Camera(camera);
    float scaled = jumpGateTime * amount;
    float factor = zoomFactor * constant(0x3fd3b56bu);
    scaled = scaled * factor;
    SetZoomDistance__6Cameraf(camera, zoom - static_cast<float>(static_cast<int>(scaled)));
    SetNoTargetUpdates__6Camerai(camera, 5);

    if (jumpGateHurtDest != 0) {
        for (int i = 0; i < nmPlayersAlive; i++) {
            unsigned state = step(static_cast<unsigned>(eRandState));
            eRandState = static_cast<int>(state);
            if ((state >> 16) % 10u != 0u) continue;
            unsigned char* object = object_of(playersAlive[i]);
            if (jumpHidden == 0) {
                Point3 position = field<Point3>(object, 16);
                Point3 spot;
                spot.x = position.x + 0.0f;
                spot.y = position.y;
                spot.z = position.z + 50.0f;
                P_StartLightning__FG6Point3T0iiffUi(&spot, &jumpGatePos, 492, 4, constant(0x40f66666u), constant(0x408cccccu), 0x5e808080u);
                unsigned char* entry = at(field<unsigned>(object, 72)) + 128;
                short adjust = field<short>(entry, 0);
                unsigned function = field<unsigned>(entry, 4);
#ifdef NORRATH_HOST_ARENA
                unsigned char* model = norrath_host_slot_call_pointer(function, object + adjust);
#else
                unsigned char* model = reinterpret_cast<unsigned char* (*)(unsigned char*)>(static_cast<__UINTPTR_TYPE__>(function))(object + adjust);
#endif
                Point3 here = field<Point3>(object, 16);
                fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
                    model, object + 288, &here, field<short>(object, 56), object + 824, 1.5f,
                    field<float>(object, 224), field<float>(object, 228), 0x5e808080u);
            }
            field<short>(object, 154) = 64;
            field<short>(object, 148) = 10;
            field<short>(object, 140) = 10;
            field<short>(object, 150) = 10;
            field<short>(object, 142) = 10;
            field<short>(object, 152) = 128;
            field<short>(object, 144) = 128;
            field<short>(object, 146) = 64;
        }
        return;
    }

    if (jumping == 30) {
        for (int i = 0; i < nmPlayersAlive; i++) {
            unsigned char* object = object_of(playersAlive[i]);
            field<int>(object, 252) = frameNm;
            field<int>(object, 936) = 120;
        }
    }
    const int count = static_cast<int>(jumpGateTime * 35.0f);
    const float unit = constant(0x37800000u);
    for (int j = 0; j < nmPlayersAlive; j++) {
        for (int k = 49; k >= 0; k--) {
            Point3 velocity;
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            velocity.z = 50.0f;
            unsigned char* object = object_of(playersAlive[j]);
            Point3 position = field<Point3>(object, 16);
            Point3 start;
            start.x = position.x + velocity.x;
            start.y = position.y;
            start.z = position.z + 50.0f;
            unsigned s1 = step(static_cast<unsigned>(eRandState));
            unsigned s2 = step(s1);
            unsigned s3 = step(s2);
            unsigned s4 = step(s3);
            float fraction = static_cast<float>(s1 >> 16) * unit;
            float ux = static_cast<float>(s2 >> 16) * unit;
            float uy = static_cast<float>(s3 >> 16) * unit;
            float uz = static_cast<float>(s4 >> 16) * unit;
            Point3 target = jumpGatePos;
            Point3 delta;
            delta.x = target.x - start.x;
            delta.y = target.y - start.y;
            delta.z = target.z - start.z;
            Point3 offset;
            offset.x = delta.x * fraction;
            offset.y = delta.y * fraction;
            offset.z = delta.z * fraction;
            Point3 spot;
            spot.x = start.x + offset.x;
            spot.y = start.y + offset.y;
            spot.z = start.z + offset.z;
            eRandState = static_cast<int>(s4);
            ux = ux - 0.5f;
            uy = uy - 0.5f;
            uz = uz - 0.5f;
            velocity.x = ux * 5.0f;
            velocity.y = uy * 5.0f;
            velocity.z = uz * 5.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(jumpParticleJump, &spot, &velocity, 0.0f);
        }
        unsigned char* object = object_of(playersAlive[j]);
        void* model = at(field<unsigned>(object, 1332));
        int points = modelGetNmShadowPoints__FP12_modelHeader(model);
        for (int c = count; c > 0; c--) {
            unsigned draw = step(static_cast<unsigned>(eRandState));
            eRandState = static_cast<int>(draw);
            object = object_of(playersAlive[j]);
            Point3 shadow;
            float scratch[4];
            Point3 position = field<Point3>(object, 16);
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                model, object + 288, &position, field<short>(object, 56), static_cast<int>(draw >> 16) % points, &shadow, scratch, 0, 1.0f);
            unsigned s1 = step(static_cast<unsigned>(eRandState));
            unsigned s2 = step(s1);
            unsigned s3 = step(s2);
            eRandState = static_cast<int>(s3);
            Point3 velocity;
            velocity.x = static_cast<float>(s1 >> 16) * unit - 0.5f;
            velocity.y = static_cast<float>(s2 >> 16) * unit - 0.5f;
            velocity.z = static_cast<float>(s3 >> 16) * unit - 0.5f;
            velocity.x = velocity.x * 5.0f;
            velocity.y = velocity.y * 5.0f;
            velocity.z = velocity.z * 5.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(jumpParticleJump, &shadow, &velocity, 0.0f);
        }
    }
}
