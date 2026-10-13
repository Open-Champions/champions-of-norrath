// Host contract for cast__16SkillGroundPoundiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the lookups and the animation and sound calls log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
int w13_skill_cast_b_host_call(unsigned, void*, int, int) { return 0; }
static std::string calls;
static unsigned char header[64];
static char sound_marker;
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
#include "cast.cpp"
extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name) {
    calls += std::string("lump(") + lump + "," + name + ");";
    return std::strcmp(name, "dha_quake.anm") == 0 ? static_cast<void*>(header) : static_cast<void*>(&sound_marker);
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* hdr, float first, float second, int mode) {
    calls += "anim(" + std::to_string(static_cast<unsigned char*>(state) - w13_skill_cast_b_host_base) + "," + std::to_string(hdr == header) + "," +
             std::to_string(bits(first)) + "," + std::to_string(bits(second)) + "," + std::to_string(mode) + ");";
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += "sound(" + std::to_string(sound == &sound_marker) + "," + std::to_string(static_cast<unsigned char*>(object) - w13_skill_cast_b_host_base) + "," + std::to_string(volume) + ");";
}
}
static unsigned char arena[0x900] __attribute__((aligned(16)));
static void put32(unsigned char* where, unsigned value) { std::memcpy(where, &value, 4); }
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char* const creature = arena + 0x200;
    const unsigned record_word = 0x100, name_word = 0x120;
    std::memcpy(arena + name_word, "lumpname", 9);
    const unsigned flag_values[] = {0x00, 0x40, 0x41, 0xffffffbf, 0xffffffff};
    for (unsigned flags : flag_values) {
        std::memset(arena + 0x200, 0xa5, 0x600);
        std::memset(header, 0x77, sizeof header);
        put32(arena + 0x100, name_word);
        put32(creature + 12, flags);
        put32(creature + 36, record_word);
        calls.clear();
        void* r = cast_ground_pound(nullptr, 3, creature, Point3{1.0f, 2.0f, 3.0f}, 9);
        if (r != nullptr) return 1;
        if (!(flags & 0x40)) {
            if (!calls.empty()) return 2;
            unsigned int w; std::memcpy(&w, creature + 1400, 4);
            if (w != 0xa5a5a5a5u) return 3;
            for (unsigned char b : header) if (b != 0x77) return 4;
            continue;
        }
        const unsigned both = 0x3dcccccc;
        const std::string expected = "lump(lumpname,dha_quake.anm);anim(" + std::to_string(creature - arena + 288) + "," + std::to_string(1) + "," + std::to_string(both) + "," + std::to_string(both) + ",10);" +
                                     "lump(fx.lmp,spel_bw_pound.anm);sound(1," + std::to_string(creature - arena) + "," + std::to_string(1.0f) + ");";
        if (calls != expected) { std::puts(calls.c_str()); return 5; }
        unsigned int w; std::memcpy(&w, creature + 1400, 4);
        if (w != 10) return 6;
        unsigned char want[64]; std::memset(want, 0x77, sizeof want);
        want[24] = 1; want[32] = 2; want[40] = 4; want[48] = 8;
        put32(want + 20, 0x3e800000); put32(want + 28, 0x3f000000); put32(want + 36, 0x3f400000); put32(want + 44, 0x3fbd2e2f);
        if (std::memcmp(header, want, sizeof want) != 0) return 7;
    }
    return 0;
}
