// Observed bounce__11FireBomblet at 0x0028a9b8, 752 bytes. Argument: the object o (FireBomblet).
// Called by msg_run__11FireBomblet when the bomb has hit the ground. Nothing is returned.
// Names: vel = floats at o+76, o+80, o+84; pos = floats at o+16, o+20; C = signed byte at +154 (a
// count); the signed bytes at +155 and +157 are called P and Q; the float at +160 is H; the word at
// +172 is a pointer to a byte counter, called K. "rand" is syncRandState = syncRandState * 0x19660d
// + 0x3c6ef35f followed by its upper halfword (an unsigned shift for the float draws, a signed
// shift for the floorAdd argument). Float constants without a symbol: about 0.33 (0x3ea8f5c2, read through
// the global pointer) and the others below as bit patterns.
//  1. When C is zero, soundPlaySequence(lumpFindResource("firebomb.lmp",
//     "catapult_fireball_hit.anm"), o, 1.0) runs. Then C is read again and stored plus one.
//  2. vel is multiplied component-wise by the 0x3ea8f5c2 single (about 0.33) when P is nonzero, else by 0.5.
//  3. When vz is negative it is replaced by -vz. Then, when C (as just stored) is not 1, K is not
//     null and the signed byte K points to is below 3: one rand step is taken; when bits 16..17 of
//     it are zero a second step is taken and floorAdd({x, y, H + 1.0}, (short)(second >> 16 signed),
//     24.0, 9, 0) runs, followed by incrementing the byte at K. (The first step is stored in either
//     case.)
//  4. When Q is zero the function returns. Otherwise, when cutMode is zero and frameNm % 37 is not
//     zero it returns too.
//  5. Four rand steps (unsigned halfword draws u1..u4, each scaled by 2^-16) give
//     dx = (u1 - 0.5) * (u2 * 40.0 + 10.0) and dy = (u3 - 0.5) * (u4 * 40.0 + 10.0). A block of
//     1192 bytes is allocated; when that succeeds, one more rand step r is taken and the StaticFire
//     constructor runs on the block with ({x + dx, y + dy, H}, 240.0, (r % 10) * 60 + 240, 0.5).
// Inferred: Q requests the extra fire effect (the float draws do not run without it) and K counts
// how many floor marks this bomb has made. Unknown: the meaning of P, Q, C beyond a bounce count,
// H, and the StaticFire arguments. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern int frameNm;
extern signed char cutMode;
extern unsigned syncRandState;
void* lumpFindResource__FPcT0(const char* name, const char* resource);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void floorAdd__FG6Point3sfii(Point3 where, short a, float b, int c, int d);
void* blockAlloc__Fi(int size);
void StaticFire_construct(void* self, Point3 where, float a, int b, float c) __asm__("__10StaticFireG6Point3fif");
}

#if defined(W12_BOUNCE_HOST)
extern unsigned char* w12_bounce_host_base;
#endif

namespace {
// Pointer words in the object are 32-bit addresses on the target; a host contract keeps them as
// offsets from w12_bounce_host_base.
inline signed char* counter_from_word(unsigned value) {
    if (value == 0) return nullptr;
#if defined(W12_BOUNCE_HOST)
    return reinterpret_cast<signed char*>(w12_bounce_host_base + value);
#else
    return reinterpret_cast<signed char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}
inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }
inline unsigned rand_step() {
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    return syncRandState;
}
}  // namespace

extern "C" void bounce__11FireBomblet(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    float* const vel = reinterpret_cast<float*>(o + 76);
    float* const pos = reinterpret_cast<float*>(o + 16);
    signed char* const count = reinterpret_cast<signed char*>(o + 154);
    if (*count == 0) {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("firebomb.lmp", "catapult_fireball_hit.anm"), self, 1.0f);
    }
    *count = static_cast<signed char>(*count + 1);
    const float scale = *reinterpret_cast<signed char*>(o + 155) != 0 ? bits(0x3ea8f5c2u) : 0.5f;
    vel[0] = vel[0] * scale;
    vel[1] = vel[1] * scale;
    vel[2] = vel[2] * scale;
    const float height = *reinterpret_cast<float*>(o + 160);
    if (vel[2] < 0.0f) {
        vel[2] = -vel[2];
        if (*count != 1) {
            signed char* const marks = counter_from_word(*reinterpret_cast<unsigned*>(o + 172));
            if (marks != nullptr && *marks < 3) {
                if (((rand_step() >> 16) & 3u) == 0) {
                    const int second = static_cast<int>(rand_step()) >> 16;
                    floorAdd__FG6Point3sfii(Point3{pos[0], pos[1], height + 1.0f}, static_cast<short>(second), 24.0f, 9, 0);
                    *reinterpret_cast<unsigned char*>(counter_from_word(*reinterpret_cast<unsigned*>(o + 172))) += 1;
                }
            }
        }
    }
    if (*reinterpret_cast<signed char*>(o + 157) == 0) return;
    if (cutMode == 0 && frameNm % 37 != 0) return;
    const float scaleDown = bits(0x37800000u);
    const float u1 = static_cast<float>(rand_step() >> 16) * scaleDown;
    const float u2 = static_cast<float>(rand_step() >> 16) * scaleDown;
    const float u3 = static_cast<float>(rand_step() >> 16) * scaleDown;
    const float u4 = static_cast<float>(rand_step() >> 16) * scaleDown;
    const float dx = (u1 - 0.5f) * (u2 * 40.0f + 10.0f);
    const float dy = (u3 - 0.5f) * (u4 * 40.0f + 10.0f);
    void* const storage = blockAlloc__Fi(1192);
    if (storage == nullptr) return;
    const unsigned r = rand_step() >> 16;
    StaticFire_construct(storage, Point3{pos[0] + dx, pos[1] + dy, height}, 240.0f, static_cast<int>(r % 10u) * 60 + 240, 0.5f);
}
