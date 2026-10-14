// Observed msg_run__8JumpGate at 0x00282520, 2720 bytes.
// Per-frame behaviour of a jump gate. o is the object, "pos" is the point at o+16 and "gate" is the
// global point jumpGatePos. The byte at o+208 is a flag that msg_draw sets to 1; this routine clears
// it on every path that ends normally (the exits through the first test below leave it as it is
// only when it is already cleared by the same store). "unit draw" is a vector drawn by rejection from
// three eRand__10GameObject values, each (float)value * 2^-16, doubled, minus 1.0, in x, y, z order,
// redrawn while 1.0 < (x*x + y*y) + z*z.
//  1. o+76 = objectActiveProcessing__FP10GameObjectii(o, signed byte o+76, 480).
//  2. When the signed byte o+79 is nonzero: when findTagString__10GameObjectPci(o, "script", 0) is
//     nonzero, ok = scriptStart__FPc(findTagString(o, "script", 0)) != 0 (ok starts as 1); when
//     (frameNm & 31) == 0, lightActivate__FP5Light(o+256) runs if ok and the byte o+208 is nonzero,
//     otherwise lightDeactivate__FP5Light(o+256).
//  3. A zero signed byte o+76 clears o+208 and returns.
//  4a. With o+79 nonzero (and o+208 nonzero): modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3
//     (word at o+348, null, 0, gate, null) and gate += pos; animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps
//     (o+216, pos, null, null); the attachment call again with the state o+216 and gate += pos again.
//     When ok: if the word jumping is zero, fxRunCreatureShock is called (word o+348, o+216, a copy of
//     pos, the 16-bit sum of the halfword o+56 plus 16384, o+316, floats 1.0, 1.0, 0.0, 0x5e808080).
//     Then the float o+212 = (float o+284) - 0.1666667, raised to 60.0 when lower; with one chance in
//     20 (eRand % 20 == 0) it grows by (eRand * 2^-16) * 120.0; it is clipped to 300.0 from above; the
//     light members o+284 and o+292 get that value and o+296 and o+288 get 1.0 / (value * value).
//  4b. With o+79 zero: the attachment call (state o+216 when the signed byte o+77 is nonzero, else
//     null) and gate += pos. Then by the signed byte o+78:
//       0: when jumping is nonzero and the signed byte o+220 is zero: lightActivate(o+256),
//          sprintf(buf, "%s_activate.anm", o+80), animAdd__FP14AnimationStateP15AnimationHeaderi
//          (o+216, lumpFindResource__FPcT0(o+144, buf), 4) and o+78 = 1;
//       1: when bit 7 of the halfword o+222 is set: o+220 = 0, the same with "%s_idle.anm" and mode
//          3, and o+78 = 2.
//     Then, when the signed byte o+220 is nonzero: if jumping is nonzero, one particle at
//     gate + 16 * (unit draw) with velocity 9.693954 * (unit draw), through P_AddParticle with
//     jumpParticle2 and 0.0; and the light position at o+304 becomes gate + (0, 0, 36.0)
//     (x gets 0.0 added, y is copied as is), followed by lightUpdatePosition__FP5Light(o+256).
//  5. When the signed byte o+77 is nonzero: a zero o+208 clears it and returns; otherwise a particle
//     at gate with velocity 10.0 * (unit draw) (jumpParticleOn), animAdvanceAnimation(o+216, pos,
//     null, null); then, if o+79 is nonzero and ok and engineTaskActive__FPc("jumpgate") is zero: the
//     first player slot i (0 .. nmPlayersAlive-1) whose record (player + 23984 * playersAlive[i]) has an
//     object whose squared distance to pos exceeds 10000.0 stops the scan; if the scan ran through all
//     slots, jumpGateInit__FiPcc(playersAlive[nmPlayersAlive], findTagString(o, "destination", 0),
//     o+208 == 0) runs (the index is one past the last slot). o+208 is then cleared.
//  6. With o+77 zero: when o+79 is zero o+208 is cleared and the routine returns; when ok, the same
//     scan with limit 10000.0 runs and, if it ran through all slots, jumpGateInit as above with
//     o+77 = 1 and the routine clears o+208 and returns. Otherwise, when o+208 is nonzero, the first
//     slot i whose player is local (netPlayerIsLocal__Fi) and whose object is closer than 46225.0
//     (squared distance below) ends a scan; if it stopped before the end of the slots,
//     hudSetInfoMessage__FPUsiic(textGetString__FPc("JumpGateHurtWarn"), 180, playersAlive[i], 0)
//     runs. o+208 is cleared.
// Inferred: names only from symbols. Unknown: the meaning of o+76/77/78/79, o+208, o+212, o+220.
// Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
int objectActiveProcessing__FP10GameObjectii(void* object, int a, int b);
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int scriptStart__FPc(char* script);
void lightActivate__FP5Light(void* light);
void lightDeactivate__FP5Light(void* light);
void lightUpdatePosition__FP5Light(void* light);
void modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(void* model, void* animation, int index, void* out, void* second);
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* animation, void* position, void* second, void* third);
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
    void* model, void* animation, const void* position, short yaw, void* state, float a, float b, float c, unsigned color);
int eRand__10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float extra);
int sprintf(char* destination, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* animation, void* header, int mode);
int engineTaskActive__FPc(char* name);
int netPlayerIsLocal__Fi(int player);
void jumpGateInit__FiPcc(int player, char* name, char flag);
unsigned short* textGetString__FPc(char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int a, int b, char c);
extern int frameNm;
extern int jumping;
extern float jumpGatePos[3];
extern unsigned char jumpParticle2[];
extern unsigned char jumpParticleOn[];
extern int playersAlive[4];
extern int nmPlayersAlive;
extern unsigned char player[];
}

namespace {
void unit_draw(void* object, float* v) {
    float sum;
    do {
        float x = static_cast<float>(eRand__10GameObject(object)) * 0x1.0p-16f;
        x = x + x;
        v[0] = x - 1.0f;
        float y = static_cast<float>(eRand__10GameObject(object)) * 0x1.0p-16f;
        y = y + y;
        v[1] = y - 1.0f;
        float z = static_cast<float>(eRand__10GameObject(object)) * 0x1.0p-16f;
        z = z + z;
        v[2] = z - 1.0f;
        sum = v[0] * v[0] + v[1] * v[1];
        sum = sum + v[2] * v[2];
    } while (1.0f < sum);
}

void add_position(const float* pos) {
    jumpGatePos[0] = jumpGatePos[0] + pos[0];
    jumpGatePos[1] = jumpGatePos[1] + pos[1];
    jumpGatePos[2] = jumpGatePos[2] + pos[2];
}

// The squared distance, (dx*dx + dy*dy) + dz*dz, from pos to the object of player slot i's record.
float player_distance_squared(const float* pos, int slot) {
    const unsigned char* record = player + playersAlive[slot] * 23984;
    const float* other = reinterpret_cast<const float*>(w10h::from_word(*reinterpret_cast<const unsigned*>(record + 21872)) + 16);
    float dx = other[0] - pos[0];
    float dy = other[1] - pos[1];
    float dz = other[2] - pos[2];
    float sum = dx * dx + dy * dy;
    return sum + dz * dz;
}

void* model_of(unsigned char* o) { return w10h::from_word(w10h::word_at(o, 348)); }
}  // namespace

extern "C" void msg_run__8JumpGate(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    const float* pos = reinterpret_cast<const float*>(o + 16);
    bool ok = true;
    o[76] = static_cast<unsigned char>(objectActiveProcessing__FP10GameObjectii(self, static_cast<signed char>(o[76]), 480));
    if (static_cast<signed char>(o[79]) != 0) {
        if (findTagString__10GameObjectPci(self, "script", 0) != nullptr) {
            ok = scriptStart__FPc(findTagString__10GameObjectPci(self, "script", 0)) != 0;
        }
        if ((frameNm & 0x1f) == 0) {
            if (ok && static_cast<signed char>(o[208]) != 0) lightActivate__FP5Light(o + 256);
            else lightDeactivate__FP5Light(o + 256);
        }
    }
    if (static_cast<signed char>(o[76]) == 0) {
        o[208] = 0;
        return;
    }
    if (static_cast<signed char>(o[79]) != 0) {
        if (static_cast<signed char>(o[208]) != 0) {
            modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(model_of(o), nullptr, 0, jumpGatePos, nullptr);
            add_position(pos);
            animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(o + 216, o + 16, nullptr, nullptr);
            modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(model_of(o), o + 216, 0, jumpGatePos, nullptr);
            add_position(pos);
            if (ok) {
                if (jumping == 0) {
                    unsigned char point[12];
                    w10h::copy_bytes(point, o + 16, 12);
                    fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
                        model_of(o), o + 216, point, static_cast<short>(*reinterpret_cast<unsigned short*>(o + 56) + 16384), o + 316,
                        1.0f, 1.0f, 0.0f, 0x5e808080u);
                }
                float size = *reinterpret_cast<float*>(o + 284) - 0x1.555554p-3f;
                *reinterpret_cast<float*>(o + 212) = size;
                if (!(size < 60.0f)) {
                } else {
                    *reinterpret_cast<float*>(o + 212) = 60.0f;
                }
                if (static_cast<unsigned>(eRand__10GameObject(self)) % 20u == 0) {
                    float more = static_cast<float>(eRand__10GameObject(self)) * 0x1.0p-16f;
                    more = more * 120.0f;
                    *reinterpret_cast<float*>(o + 212) = *reinterpret_cast<float*>(o + 212) + more;
                }
                float value = *reinterpret_cast<float*>(o + 212);
                if (300.0f < value) {
                    *reinterpret_cast<float*>(o + 212) = 300.0f;
                    value = 300.0f;
                }
                // The compiler would fold 1.0 / (300.0 * 300.0) at build time with its own rounding; the
                // image divides at run time, so keep the value opaque to the optimizer.
#if defined(__mips__)
                __asm__("" : "+f"(value));
#endif
                const float square = value * value;
                *reinterpret_cast<float*>(o + 284) = value;
                *reinterpret_cast<float*>(o + 292) = value;
                const float inverse = 1.0f / square;
                *reinterpret_cast<float*>(o + 296) = inverse;
                *reinterpret_cast<float*>(o + 288) = inverse;
            }
        }
    } else {
        modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(
            model_of(o), static_cast<signed char>(o[77]) != 0 ? o + 216 : nullptr, 0, jumpGatePos, nullptr);
        add_position(pos);
        const signed char mode = static_cast<signed char>(o[78]);
        if (mode == 0) {
            if (jumping != 0 && static_cast<signed char>(o[220]) == 0) {
                lightActivate__FP5Light(o + 256);
                char buffer[96];
                sprintf(buffer, "%s_activate.anm", o + 80);
                animAdd__FP14AnimationStateP15AnimationHeaderi(o + 216, lumpFindResource__FPcT0(reinterpret_cast<char*>(o + 144), buffer), 4);
                o[78] = 1;
            }
        } else if (mode == 1) {
            if ((*reinterpret_cast<unsigned short*>(o + 222) & 0x80) != 0) {
                o[220] = 0;
                char buffer[96];
                sprintf(buffer, "%s_idle.anm", o + 80);
                animAdd__FP14AnimationStateP15AnimationHeaderi(o + 216, lumpFindResource__FPcT0(reinterpret_cast<char*>(o + 144), buffer), 3);
                o[78] = 2;
            }
        }
        if (static_cast<signed char>(o[220]) != 0) {
            if (jumping != 0) {
                float first[3];
                unit_draw(self, first);
                float velocity[3] = {first[0] * 0x1.3634dep+3f, first[1] * 0x1.3634dep+3f, first[2] * 0x1.3634dep+3f};
                float second[3];
                unit_draw(self, second);
                float offset[3] = {second[0] * 16.0f, second[1] * 16.0f, second[2] * 16.0f};
                float where[3] = {jumpGatePos[0] + offset[0], jumpGatePos[1] + offset[1], jumpGatePos[2] + offset[2]};
                P_AddParticle__FP11ParticleDefP6Point3T1f(jumpParticle2, where, velocity, 0.0f);
            }
            float lift[3] = {0.0f, 0.0f, 36.0f};
            float light_position[3] = {jumpGatePos[0] + lift[0], jumpGatePos[1], jumpGatePos[2] + lift[2]};
            w10h::copy_bytes(o + 304, light_position, 12);
            lightUpdatePosition__FP5Light(o + 256);
        }
    }
    if (static_cast<signed char>(o[77]) != 0) {
        if (static_cast<signed char>(o[208]) == 0) {
            o[208] = 0;
            return;
        }
        float direction[3];
        unit_draw(self, direction);
        float velocity[3] = {direction[0] * 10.0f, direction[1] * 10.0f, direction[2] * 10.0f};
        P_AddParticle__FP11ParticleDefP6Point3T1f(jumpParticleOn, jumpGatePos, velocity, 0.0f);
        animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(o + 216, o + 16, nullptr, nullptr);
        if (static_cast<signed char>(o[79]) != 0 && ok && engineTaskActive__FPc(const_cast<char*>("jumpgate")) == 0) {
            int slot = 0;
            const int count = nmPlayersAlive;
            if (count > 0) {
                do {
                    if (10000.0f < player_distance_squared(pos, slot)) break;
                    ++slot;
                } while (slot < count);
            }
            if (slot == count) {
                char* tag = findTagString__10GameObjectPci(self, "destination", 0);
                jumpGateInit__FiPcc(playersAlive[slot], tag, static_cast<signed char>(o[208]) == 0);
            }
        }
        o[208] = 0;
        return;
    }
    if (static_cast<signed char>(o[79]) == 0) {
        o[208] = 0;
        return;
    }
    bool initialised = false;
    if (ok) {
        int slot = 0;
        const int count = nmPlayersAlive;
        if (count > 0) {
            do {
                if (10000.0f < player_distance_squared(pos, slot)) break;
                ++slot;
            } while (slot < count);
        }
        if (slot == count) {
            char* tag = findTagString__10GameObjectPci(self, "destination", 0);
            jumpGateInit__FiPcc(playersAlive[slot], tag, static_cast<signed char>(o[208]) == 0);
            o[77] = 1;
            initialised = true;
        }
    }
    if (initialised) {
        o[208] = 0;
        return;
    }
    if (static_cast<signed char>(o[208]) != 0) {
        int slot = 0;
        if (nmPlayersAlive > 0) {
            do {
                if (netPlayerIsLocal__Fi(playersAlive[slot]) != 0) {
                    if (player_distance_squared(pos, slot) < 46225.0f) break;
                }
                ++slot;
            } while (slot < nmPlayersAlive);
        }
        if (slot != nmPlayersAlive) {
            hudSetInfoMessage__FPUsiic(textGetString__FPc(const_cast<char*>("JumpGateHurtWarn")), 180, playersAlive[slot], 0);
        }
    }
    o[208] = 0;
}
