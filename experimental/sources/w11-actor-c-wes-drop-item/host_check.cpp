// Host contract for dropItem__14WoodElfSoldieri; not an EE input. Pointer words are offsets from a host base and
// both callees record their arguments.
#define W11_ACTOR_C_HOST 1
#include "drop_item.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
static std::string calls;
static bool failFirst, failSecond;
static int allocCount;
static unsigned long long chunks = 0x0123456789abcdefull;
extern "C" {
void* blockAlloc__Fi(int size) {
    calls += "alloc" + std::to_string(size) + ";";
    const bool fail = (allocCount++ == 0) ? failFirst : failSecond;
    return fail ? nullptr : w11_actor_c_host_base + 6000 + allocCount * 700;
}
void __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(
    void* self, const float* position, short heading, void* model, void* animation, char* first, char* second, char* third, long long value, int flags,
    void* description, float scale) {
    char b[200];
    std::snprintf(b, sizeof b, "prop(%ld,%g,%g,%g,%d,%ld,%ld,%ld,%ld,%lld,%d,%d,%g);", static_cast<long>(static_cast<unsigned char*>(self) - w11_actor_c_host_base), position[0], position[1], position[2],
                  heading, static_cast<long>(static_cast<unsigned char*>(model) - w11_actor_c_host_base), static_cast<long>(reinterpret_cast<unsigned char*>(animation) - w11_actor_c_host_base),
                  static_cast<long>(reinterpret_cast<unsigned char*>(first) - w11_actor_c_host_base), static_cast<long>(reinterpret_cast<unsigned char*>(second) - w11_actor_c_host_base) * 1000 + (reinterpret_cast<unsigned char*>(third) - w11_actor_c_host_base),
                  value, flags, description != nullptr, scale);
    calls += b;
}
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int requested) {
    calls += "chunks(" + std::to_string(static_cast<unsigned char*>(model) - w11_actor_c_host_base) + "," + std::to_string(requested) + ");"; return chunks;
}
}
int main() {
    static unsigned char base[16384];
    w11_actor_c_host_base = base;
    int failures = 0;
    unsigned char* self = base + 64;
    struct Piece { long long a, b; unsigned bit; };
    const Piece variants[] = {{0x34000000ll, 0x40000000ll, 0x800}, {0x01800000ll, 0x02000000ll, 0x1000}, {0x34000000ll, 0x00180000ll, 0x2000}, {0x01800000ll, 0x02000000ll, 0x1000}};
    for (int which : {0, 1, 2}) for (unsigned variant = 0; variant < 5; ++variant) for (int fail = 0; fail < 4; ++fail) {
        std::memset(base, 0, sizeof base);
        const unsigned res = 1024; std::memcpy(self + 36, &res, 4);
        const unsigned modelWord = 2000, w0 = 3000, w8 = 3100, w16 = 3200;
        std::memcpy(base + 1024 + 12, &modelWord, 4); std::memcpy(base + 1024 + 0, &w0, 4); std::memcpy(base + 1024 + 8, &w8, 4); std::memcpy(base + 1024 + 16, &w16, 4);
        const float pos[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(self + 16, pos, 12);
        const short heading = -1234; std::memcpy(self + 56, &heading, 2);
        std::memcpy(self + 964, &variant, 4);
        const unsigned flags = 0x100; std::memcpy(self + 244, &flags, 4);
        failFirst = fail & 1; failSecond = fail & 2; allocCount = 0; calls.clear();
        dropItem__14WoodElfSoldieri(self, which);
        unsigned flagsAfter; std::memcpy(&flagsAfter, self + 244, 4);
        unsigned long long stored; std::memcpy(&stored, self + 552, 8);
        if (which != 1) {
            if (!calls.empty() || flagsAfter != flags || stored != 0) { std::fprintf(stderr, "which %d acted\n", which); ++failures; }
            continue;
        }
        std::string want;
        unsigned wantFlags = flags;
        if (variant < 4) {
            wantFlags |= variants[variant].bit;
            for (int piece = 0; piece < 2; ++piece) {
                want += "alloc584;";
                if (piece == 0 ? failFirst : failSecond) continue;
                char b[200];
                std::snprintf(b, sizeof b, "prop(%d,1,2,3,-1234,2000,%d,3000,%d,%lld,16385,0,1);", 6000 + (piece + 1) * 700, 64 + 288, 3100 * 1000 + 3200, piece == 0 ? variants[variant].a : variants[variant].b);
                want += b;
            }
        }
        want += "chunks(2000," + std::to_string(wantFlags) + ");";
        if (calls != want || flagsAfter != wantFlags || stored != chunks) { std::fprintf(stderr, "variant %u fail %d:\n got  %s\n want %s\n", variant, fail, calls.c_str(), want.c_str()); ++failures; }
    }
    if (!failures) std::puts("WoodElfSoldier dropItem ok");
    return failures != 0;
}
