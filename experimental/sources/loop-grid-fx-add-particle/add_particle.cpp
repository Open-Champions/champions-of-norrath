// Observed P_AddParticle__FP11ParticleDefP6Point3T1f at 0x180da0, extent 728.
// Arguments: the definition, a position, a velocity, and a float age.
// Definition words used: +0 type index (a 16-byte entry of the current
// system), +4 descriptor slot (its low halfword is stored), +8 use count,
// +24 a float that acts as a fall term. currentPS (0x48430c) points at the
// system. Entry words: +0 first block, +4 last block, +12 next record index
// in the last block, where 257 means no space.
//  1. A zero use count asks checkCanAllocDescriptor__Fv and stops on zero.
//  2. An entry with index 257 takes a block from freeBlocks (0x4869f0): the
//     old last block's word 0 and the entry's last-block word take it, the
//     first-block word takes it when zero, freeBlocks becomes the block's word
//     0, and the index becomes 1. An empty free list stops here.
//  3. A zero use count then calls allocDescriptor__FP11ParticleDef.
//  4. The record is at block + ((last - blockList) << 10) + index * 16. The
//     index advances by one; if that value has low five bits equal to 31 it
//     advances by two more (the footer record and next header are skipped).
//  5. The use count rises by one. The record halfword at +14 is the
//     descriptor halfword << 2.
//  6. With age > 0, n = (int)(age * 60) frames are back-stepped: position
//     minus velocity * (n / 16), z also minus (vz - 32 * n * g) * (n / 16) and
//     g * n * n, with g the float at def+24. Halfwords: x at +0, y at +2, z
//     at +4, P_FrameNum - n at +6, velocity * 256 at +8 and +10, and
//     (vz - 32 * n * g) * 256 at +12.
//     Otherwise (age <= 0 or not a number) the position and velocity * 256
//     are stored directly and +6 is P_FrameNum. All conversions truncate.
// The original also spills the velocity and moved position to its stack;
// those dead stores are not reproduced.
extern "C" {
extern unsigned char currentPS[];
extern unsigned char freeBlocks[];
extern unsigned char blockList[];
extern unsigned char block[];
extern unsigned short P_FrameNum;
int checkCanAllocDescriptor__Fv();
int allocDescriptor__FP11ParticleDef(unsigned char* definition);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
struct Point3 { float x, y, z; };
Address address(const void* p) { return reinterpret_cast<Address>(p); }
unsigned char* bytes(Address a) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(static_cast<unsigned>(a)));
#else
    return reinterpret_cast<unsigned char*>(a);
#endif
}
unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
void write_half(Address a, int value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
float read_float(Address a) {
    float value;
    __builtin_memcpy(&value, bytes(a), sizeof value);
    return value;
}
}

extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(unsigned char* definition, Point3* position, Point3* velocity, float age) {
    const Address def = address(definition);
    const Address entry = read_word(address(currentPS)) + read_word(def) * 16u;
    if (read_word(def + 8u) == 0u) {
        if (checkCanAllocDescriptor__Fv() == 0) return;
    }
    if (read_word(entry + 12u) == 257u) {
        const unsigned fresh = read_word(address(freeBlocks));
        if (fresh == 0u) return;
        const unsigned last = read_word(entry + 4u);
        if (last != 0u) write_word(last, fresh);
        write_word(entry + 4u, fresh);
        const unsigned first = read_word(entry);
        write_word(address(freeBlocks), read_word(fresh));
        if (first == 0u) write_word(entry, fresh);
        write_word(entry + 12u, 1u);
    }
    if (read_word(def + 8u) == 0u) allocDescriptor__FP11ParticleDef(definition);
    const unsigned index = read_word(entry + 12u);
    const Address record = address(block) + ((read_word(entry + 4u) - static_cast<unsigned>(address(blockList))) << 10) + index * 16u;
    unsigned next = index + 1u;
    write_word(entry + 12u, next);
    if ((next & 31u) == 31u) write_word(entry + 12u, next + 2u);
    write_word(def + 8u, read_word(def + 8u) + 1u);
    const unsigned slot = bytes(def)[4] | (static_cast<unsigned>(bytes(def)[5]) << 8);
    write_half(record + 14u, static_cast<int>(slot << 2));
    const Point3 motion = *velocity;
    if (0.0f < age) {
        const float gravity = read_float(def + 24u);
        const int frames = static_cast<int>(age * 60.0f);
        const float count = static_cast<float>(frames);
        const float spread = count * 0.0625f;
        const float fall = motion.z - static_cast<float>(frames << 5) * gravity;
        const float x = position->x - motion.x * spread;
        const float y = position->y - motion.y * spread;
        const float z = position->z - fall * spread - gravity * count * count;
        write_half(record + 6u, static_cast<int>(P_FrameNum) - frames);
        write_half(record + 8u, static_cast<int>(motion.x * 256.0f));
        write_half(record + 10u, static_cast<int>(motion.y * 256.0f));
        write_half(record + 2u, static_cast<int>(y));
        write_half(record + 0u, static_cast<int>(x));
        write_half(record + 12u, static_cast<int>(fall * 256.0f));
        write_half(record + 4u, static_cast<int>(z));
    } else {
        write_half(record + 4u, static_cast<int>(position->z));
        write_half(record + 6u, static_cast<int>(P_FrameNum));
        write_half(record + 0u, static_cast<int>(position->x));
        write_half(record + 2u, static_cast<int>(position->y));
        write_half(record + 8u, static_cast<int>(motion.x * 256.0f));
        write_half(record + 10u, static_cast<int>(motion.y * 256.0f));
        write_half(record + 12u, static_cast<int>(motion.z * 256.0f));
    }
}
