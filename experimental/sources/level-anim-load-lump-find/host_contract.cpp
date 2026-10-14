#include "lump_find.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
#include <string>
#include <vector>
static unsigned compares;
extern "C" int norrath_string_compare(const char* l, const char* r) {
    ++compares;
    const int v = std::strcmp(l, r);
    return v < 0 ? -7 : v > 0 ? 9 : 0;
}
int main() {
    unsigned char* lump = arena_at(0x100);
    unsigned char* text = arena_at(0x8000);
    for (unsigned count = 0; count <= 40; ++count) {
        std::vector<std::string> names;
        // Names are spaced so that every gap has an absent key between entries.
        for (unsigned i = 0; i != count; ++i) { char b[16]; std::snprintf(b, sizeof b, "n%03u", i * 2 + 2); names.push_back(b); }
        sh_(lump, count);
        unsigned char* cursor = text;
        for (unsigned i = 0; i != count; ++i) {
            std::memcpy(cursor, names[i].c_str(), names[i].size() + 1);
            unsigned char* rec = lump + 4 + 12 * i;
            sw_(rec, norrath_host_address(cursor)); sw_(rec + 4, 0x1000 + i); sw_(rec + 8, 0x2000 + i);
            cursor += 8;
        }
        for (unsigned k = 0; k <= count * 2 + 3; ++k) {
            char key[16]; std::snprintf(key, sizeof key, "n%03u", k);
            unsigned char* got = lumpFind__FPvPc(lump, key);
            unsigned want = 0xffffffffu;
            for (unsigned i = 0; i != count; ++i) if (names[i] == key) want = i;
            if (want == 0xffffffffu) {
                if (got != nullptr) { std::printf("count %u key %s expected miss\n", count, key); return 1; }
            } else if (got != lump + 4 + 12 * want) {
                std::printf("count %u key %s wrong record\n", count, key); return 2;
            }
        }
    }
    // Search cost: 31 sorted entries are resolved within five comparisons.
    sh_(lump, 31); unsigned char* cursor = text;
    for (unsigned i = 0; i != 31; ++i) {
        std::snprintf(reinterpret_cast<char*>(cursor), 8, "n%03u", i + 1);
        sw_(lump + 4 + 12 * i, norrath_host_address(cursor)); cursor += 8;
    }
    compares = 0; char key[] = "n001";
    if (lumpFind__FPvPc(lump, key) != lump + 4) return 3;
    if (compares != 5) { std::printf("compares %u\n", compares); return 4; }
    // A miss on a single record compares exactly once.
    sh_(lump, 1); compares = 0; char absent[] = "zzzz";
    if (lumpFind__FPvPc(lump, absent) != nullptr || compares != 1) return 5;
    // An empty lump reads no record and compares nothing.
    sh_(lump, 0); compares = 0;
    if (lumpFind__FPvPc(lump, absent) != nullptr || compares != 0) return 6;
    return 0;
}
