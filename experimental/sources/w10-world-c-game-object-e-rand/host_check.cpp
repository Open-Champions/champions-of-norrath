// Host contract for norrath_game_object_e_rand. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct RandomObject { unsigned char before_seed[68]; unsigned seed; unsigned char after[8]; };
extern "C" int norrath_game_object_e_rand(RandomObject*);
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    RandomObject object;
    std::memset(&object, 0xaa, sizeof object);
    object.seed = 0;
    expect(norrath_game_object_e_rand(&object) == (0x3c6ef35f >> 16) && object.seed == 0x3c6ef35fu, "seed 0 gives the increment");
    unsigned long long wide = (0x3c6ef35full * 0x19660dull + 0x3c6ef35full) & 0xffffffffull;
    expect(norrath_game_object_e_rand(&object) == static_cast<int>(wide >> 16) && object.seed == static_cast<unsigned>(wide), "second draw follows the recurrence");
    object.seed = 0xffffffffu;
    unsigned long long w3 = (0xffffffffull * 0x19660dull + 0x3c6ef35full) & 0xffffffffull;
    int r = norrath_game_object_e_rand(&object);
    expect(r == static_cast<int>(w3 >> 16) && r >= 0 && r <= 65535, "wrap-around, result in range");
    expect(object.before_seed[0] == 0xaa && object.before_seed[67] == 0xaa && object.after[0] == 0xaa, "only the seed word is written");
    if (failures) return 1;
    std::printf("norrath_game_object_e_rand contract passed\n");
    return 0;
}
