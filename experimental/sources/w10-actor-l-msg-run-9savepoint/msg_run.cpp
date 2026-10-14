#include "../w10-actor-l-common/access.hpp"
// Observed msg_run__9Savepoint at 0x00214bf8, 2424 bytes. Per-frame step. Words/bytes below are
// object offsets; "state" is the animation member at +76, "light" the member at +136, "group"
// the table at the word at +36 (model = its word +12). Steps in order:
//  1. byte[196] = objectActiveProcessing(object, signed byte[196]).
//  A. When word[132] is nonzero: if the global MC_LastModeSuccessful is nonzero, msgHudSavePoint(
//     object) runs and the global is cleared; otherwise word[132] is cleared when
//     MC_GetAsyncMode returns zero. Then when word[204] is nonzero: if word[128] is zero, add the
//     animation "rise.anm" of lump "savepnt.lmp" to the state (mode 4), store
//     modelGetActiveChunks(model, 1) in the doubleword at +120 and set word[128] to 1; if word[204]
//     is 1, play the sequence "game_lvl_save.anm" of lump "fx.lmp" on the object at rate 1.0; then,
//     if word[204] is still nonzero, spawn the burst described below, add one to word[204]
//     and, when it reaches 4 or more, clear word[204] and word[132].
//     Burst: a random vector inside the unit ball (rejection loop of three eRand draws per try)
//     is drawn and not used further; n = modelGetNmShadowPoints(model);
//     modelGetShadowPos(model, state, copy of the Point3 at +16, halfword at +56, n - 1, &out,
//     &scalar, null, 1.0) fills `out`; a Matrix33 is set to identity and rotated about axis 2 by
//     ((125 * word[204]) << 20) >> 16 (the already incremented counter); then 66 passes (s from
//     0 by 1000 while s <= 65535) each rotate by 1000 about axis 2, transform the vector (1,0,0)
//     with matTransform, draw three random values (eRand & 7) - 3 as the velocity, and run 80
//     inner steps. Each inner step draws a, b (as ints times 2^-16) and builds scale =
//     a * (b * 650.0 + 50.0) + 24.0, position = out + transformed vector * scale, then draws c
//     and calls P_AddParticle(saveParticle, position, velocity, c * 2^-16).
//  B. When word[128] is nonzero: it is incremented (wrapping zero to one); when bit 7 of the
//     halfword at +82 is set, byte[80] = 0 and "float.anm" of "savepnt.lmp" is added to the state
//     (mode 3). Light floats: f = isin(word[128] << 9) * 4.0 + 120.0 goes to light+28 and
//     light+36, 1.0 / (f * f) to light+40 and light+32; animAdvanceAnimation(state, object+16, 0,
//     0) runs. When word[128] is zero and byte[197] is nonzero: two particle spawns, from random
//     ball vectors, with saveParticle2 (first position object+16 raised by 10 * vector, then by 50
//     along z); see the code.
//  C. When frameNm & 7 is zero: updatePlayersReady(object); then if signed byte[200] is nonzero,
//     nmPlayersAlive differs from nmPlayers and nmPlayers >= 2, find the last connected player
//     whose float at player[i] + 172 is <= 0 (entries are 23984 bytes); if found and its short in
//     gameDeathTimerSingle is zero, set that short to 240, raise the player at the object's
//     position, set netForceLocalUpdate, enumerateAlivePlayers, reset the camera when the player is
//     local, update the camera, and in a network game start scene 4 when the player is local.
//     Then, unless gameCheckNoSaveZone is nonzero and when signed byte[198] is nonzero: if
//     signed byte[199] > 0, clear byte[198], set byte[197] and call MC_SaveCheckPoint(0, 0, 0); if
//     it is -1 and signed byte[201] is nonzero, show the text of "CheckPointNeedPlayers" with the
//     local player number as an info message (120).
// Unknown: field meanings. Nothing here ran on the EE.
extern "C" {
int objectActiveProcessing__FP10GameObjectii(void* object, int value);
void msgHudSavePoint__FP9Savepoint(void* self);
int MC_GetAsyncMode__Fv();
char* lumpFindResource__FPcT0(char* lump, char* name);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int mode);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
unsigned eRand__10GameObject(void* self);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, const void* position, short yaw, int index, void* out, float* scalar, void* matrix, float scale);
void matIdent__FP8Matrix33(void* matrix);
void matRot__FP8Matrix33is(void* matrix, int axis, short angle);
void matTransform__FP6Point3P8Matrix33T0(Point3* out, void* matrix, Point3* in);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float value);
float isin__Fi(int angle);
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, Point3* position, void* second, short* third);
void updatePlayersReady__9Savepoint(void* self);
int netPlayerIsConnected__Fi(int player);
void playerRaisePlayer__FiG6Point3(int player, const void* position);
void enumerateAlivePlayers__Fv();
int netPlayerIsLocal__Fi(int player);
void Reset__6Cameraii(void* camera, int a, int b);
void Update__6Camera(void* camera);
void coreStartSwitchScene__Fi(int scene);
int gameCheckNoSaveZone__Fv();
void MC_SaveCheckPoint__Fiii(int a, int b, int c);
unsigned short* textGetString__FPc(char* key);
int netGetLocalPlayerNm__Fi(int index);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int a, int b, char c);
extern unsigned char saveParticle[];
extern unsigned char saveParticle2[];
extern signed char MC_LastModeSuccessful;
extern unsigned char netForceLocalUpdate;
extern signed char netNetworkMode;
extern int nmPlayers;
extern int nmPlayersAlive;
extern int frameNm;
extern unsigned char player[];
extern short gameDeathTimerSingle[];
extern unsigned char* gameCam;

namespace {
inline unsigned draw(unsigned char* object) { return eRand__10GameObject(object); }
inline float unit(unsigned char* object) {
    const float value = static_cast<float>(static_cast<int>(draw(object))) * w10l::bits(0x37800000u);
    return value + value - 1.0f;
}
inline void ball(unsigned char* object, Point3& p) {
    do {
        p.x = unit(object);
        p.y = unit(object);
        p.z = unit(object);
    } while (1.0f < p.x * p.x + p.y * p.y + p.z * p.z);
}
inline float scaled(unsigned char* object) { return static_cast<float>(static_cast<int>(draw(object))) * w10l::bits(0x37800000u); }
}  // namespace

void msg_run__9Savepoint(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    o[196] = static_cast<unsigned char>(objectActiveProcessing__FP10GameObjectii(self, static_cast<signed char>(o[196])));
    void* const state = o + 76;
    if (w10l::sword(o, 132) != 0) {
        if (MC_LastModeSuccessful != 0) {
            msgHudSavePoint__FP9Savepoint(self);
            MC_LastModeSuccessful = 0;
        } else if (MC_GetAsyncMode__Fv() == 0) {
            w10l::sword(o, 132) = 0;
        }
        if (w10l::sword(o, 204) != 0) {
            if (w10l::sword(o, 128) == 0) {
                animAdd__FP14AnimationStateP15AnimationHeaderi(
                    state, lumpFindResource__FPcT0(const_cast<char*>("savepnt.lmp"), const_cast<char*>("rise.anm")), 4);
                const unsigned long long chunks = modelGetActiveChunks__FP12_modelHeaderi(
                    w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), 12)), 1);
                w10l::sword(o, 128) = 1;
                w10l::dword(o, 120) = chunks;
            }
            if (w10l::sword(o, 204) == 1) {
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
                    lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("game_lvl_save.anm")), self, 1.0f);
            }
            if (w10l::sword(o, 204) != 0) {
                Point3 unused;
                ball(o, unused);
                void* const model = w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), 12));
                const int count = modelGetNmShadowPoints__FP12_modelHeader(model);
                Point3 here = *reinterpret_cast<Point3*>(o + 16);
                Point3 out;
                float scalar;
                modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                    w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), 12)), state, &here, w10l::half(o, 56),
                    count - 1, &out, &scalar, nullptr, 1.0f);
                w10l::sword(o, 204) = w10l::sword(o, 204) + 1;
                float matrix[9];
                matIdent__FP8Matrix33(matrix);
                const int counter = w10l::sword(o, 204);
                matRot__FP8Matrix33is(matrix, 2, static_cast<short>(static_cast<int>(static_cast<unsigned>(counter * 125) << 20) >> 16));
                Point3 axis = {1.0f, 0.0f, 0.0f};
                for (int pass = 0; pass <= 0xffff; pass += 1000) {
                    matRot__FP8Matrix33is(matrix, 2, 1000);
                    Point3 direction;
                    matTransform__FP6Point3P8Matrix33T0(&direction, matrix, &axis);
                    Point3 velocity;
                    velocity.x = static_cast<float>(static_cast<int>(draw(o) & 7u) - 3);
                    velocity.y = static_cast<float>(static_cast<int>(draw(o) & 7u) - 3);
                    velocity.z = static_cast<float>(static_cast<int>(draw(o) & 7u) - 3);
                    for (int step = 79; step >= 0; --step) {
                        const float first = scaled(o);
                        const float second = scaled(o);
                        const float scale = first * (second * w10l::bits(0x44228000u) + w10l::bits(0x42480000u)) + w10l::bits(0x41c00000u);
                        const Point3 offset = {direction.x * scale, direction.y * scale, direction.z * scale};
                        Point3 position = {out.x + offset.x, out.y + offset.y, out.z + offset.z};
                        const float value = scaled(o);
                        P_AddParticle__FP11ParticleDefP6Point3T1f(saveParticle, &position, &velocity, value);
                    }
                }
                if (w10l::sword(o, 204) >= 4) {
                    w10l::sword(o, 204) = 0;
                    w10l::sword(o, 132) = 0;
                }
            }
        }
    }
    if (w10l::sword(o, 128) != 0) {
        const int next = w10l::sword(o, 128) + 1;
        w10l::sword(o, 128) = next;
        if (next == 0) w10l::sword(o, 128) = 1;
        if ((static_cast<unsigned short>(w10l::half(o, 82)) & 0x80u) != 0) {
            o[80] = 0;
            animAdd__FP14AnimationStateP15AnimationHeaderi(
                state, lumpFindResource__FPcT0(const_cast<char*>("savepnt.lmp"), const_cast<char*>("float.anm")), 3);
        }
        unsigned char* const light = o + 136;
        const float f = isin__Fi(w10l::sword(o, 128) << 9) * 4.0f + 120.0f;
        const float squared = f * f;
        w10l::real(light, 28) = f;
        w10l::real(light, 36) = f;
        w10l::real(light, 40) = 1.0f / squared;
        w10l::real(light, 32) = 1.0f / squared;
        animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(state, reinterpret_cast<Point3*>(o + 16), nullptr, nullptr);
    } else if (static_cast<signed char>(o[197]) != 0) {
        Point3 direction;
        ball(o, direction);
        const Point3 offset = {direction.x * 10.0f, direction.y * 10.0f, direction.z * 10.0f};
        const Point3 base = *reinterpret_cast<Point3*>(o + 16);
        const Point3 lifted = {base.x + offset.x, base.y + offset.y, base.z + offset.z};
        Point3 position = {lifted.x + 0.0f, lifted.y, lifted.z + 50.0f};
        Point3 other;
        ball(o, other);
        Point3 velocity = {other.x * 10.0f, other.y * 10.0f, other.z * 10.0f};
        P_AddParticle__FP11ParticleDefP6Point3T1f(saveParticle2, &position, &velocity, 0.0f);
    }
    if ((frameNm & 7) != 0) return;
    updatePlayersReady__9Savepoint(self);
    int chosen = -1;
    if (static_cast<signed char>(o[200]) != 0 && nmPlayersAlive != nmPlayers && nmPlayers >= 2) {
        for (int index = 0; index < nmPlayers; ++index) {
            if (netPlayerIsConnected__Fi(index) != 0) {
                if (*reinterpret_cast<const float*>(player + 172 + index * 23984) <= 0.0f) chosen = index;
            }
        }
        if (chosen != -1 && gameDeathTimerSingle[chosen] == 0) {
            gameDeathTimerSingle[chosen] = 240;
            Point3 position = *reinterpret_cast<Point3*>(o + 16);
            playerRaisePlayer__FiG6Point3(chosen, &position);
            netForceLocalUpdate = 1;
            enumerateAlivePlayers__Fv();
            if (netPlayerIsLocal__Fi(chosen) != 0) Reset__6Cameraii(gameCam, 0, 0);
            Update__6Camera(gameCam);
            if (netNetworkMode != 0 && netPlayerIsLocal__Fi(chosen) != 0) coreStartSwitchScene__Fi(4);
        }
    }
    if (gameCheckNoSaveZone__Fv() != 0) return;
    if (static_cast<signed char>(o[198]) == 0) return;
    const signed char mode = static_cast<signed char>(o[199]);
    if (mode > 0) {
        o[198] = 0;
        o[197] = 1;
        MC_SaveCheckPoint__Fiii(0, 0, 0);
    } else if (mode == -1 && static_cast<signed char>(o[201]) != 0) {
        unsigned short* const text = textGetString__FPc(const_cast<char*>("CheckPointNeedPlayers"));
        hudSetInfoMessage__FPUsiic(text, 120, netGetLocalPlayerNm__Fi(0), 0);
    }
}
}
