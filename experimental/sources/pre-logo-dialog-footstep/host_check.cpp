// Host contract for gameFootstepSound. Not an EE plan input.
#include "footstep.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" int eRandState = 0;

static int plays;
static char seen_lump[16];
static char seen_name[64];
static unsigned char seen_point[12];
static unsigned char* seen_point_ptr;
static unsigned seen_bits;
static int state_at_play;

extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    std::snprintf(seen_lump, sizeof seen_lump, "%s", lump);
    std::snprintf(seen_name, sizeof seen_name, "%s", name);
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x77u));
}

extern "C" void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, const unsigned char* point, float volume) {
    ++plays;
    state_at_play = eRandState;
    seen_point_ptr = const_cast<unsigned char*>(point);
    for (int i = 0; i < 12; ++i) seen_point[i] = point[i];
    std::memcpy(&seen_bits, &volume, 4);
    if (header != reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x77u))) plays = -100;
}

static int fail_name(const char* want) {
    return std::strcmp(seen_lump, "fx.lmp") != 0 || std::strcmp(seen_name, want) != 0;
}

int main() {
    unsigned char point[12];
    for (int i = 0; i < 12; ++i) point[i] = static_cast<unsigned char>(i + 1);
    eRandState = 0;
    plays = 0;
    gameFootstepSound__FG6Point3iii(point, 0, 99, 0);
    if (plays != 0 || eRandState != 0) return 1;

    eRandState = 0;
    gameFootstepSound__FG6Point3iii(point, 1, 5, 0x00400000);
    if (plays != 1 || eRandState != static_cast<int>(0x3c6ef35fu)) return 3;
    if (fail_name("fs_water1.anm") || seen_bits != 0x3f800000u) return 4;
    for (int i = 0; i < 12; ++i) if (seen_point[i] != point[i] || seen_point_ptr == point) return 5;

    eRandState = 1;
    plays = 0;
    gameFootstepSound__FG6Point3iii(point, 0, 0, 0x00400000);
    if (plays != 1 || fail_name("foly_plate2.anm")) return 6;
    if (eRandState != static_cast<int>(0x3c88596cu)) return 7;

    eRandState = 1;
    gameFootstepSound__FG6Point3iii(point, 0, 0, 0x00600000);
    if (fail_name("foly_plate2.anm")) return 8;

    eRandState = 1;
    gameFootstepSound__FG6Point3iii(point, 0, 0, 0x00200000);
    if (fail_name("foly_chainmail2.anm")) return 9;

    eRandState = 1;
    gameFootstepSound__FG6Point3iii(point, 0, 0, 0x00100000);
    if (fail_name("foly_leather2.anm")) return 10;

    eRandState = 1;
    gameFootstepSound__FG6Point3iii(point, 0, 0, 0x00080000);
    if (fail_name("foly_cloth1.anm")) return 11;

    eRandState = 0x12345678;
    gameFootstepSound__FG6Point3iii(point, 0, 0, 0x00080000);
    if (eRandState != static_cast<int>(0x75432777u) || fail_name("foly_cloth4.anm")) return 12;
    return 0;
}
