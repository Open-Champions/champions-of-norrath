// Host check for resetPlayerAnimations__Fi (includes the source; build the file alone).
#define NORRATH_INVENTORY_HOST 1
#include "reset_player_animations.cpp"
#include <cstring>
#include <string>
#include <vector>
unsigned char player[2 * 23984];
const unsigned char norrath_inventory_reset_resource_names_view[96] = {};
static const void* table[16]; static unsigned used = 0;
static unsigned intern(const void* p) { table[used] = p; return 0x1000 + 0x10 * used++; }
const void* inventory_items::host_resolve(unsigned w) { return table[(w - 0x1000) / 0x10]; }
static std::vector<std::string> calls; static char marker[4];
static char* seen_lump; static char* seen_name; static void* reset_arg;
extern "C" char* lumpFindResource__FPcT0(char* l, char* n) { calls.push_back("lump"); seen_lump = l; seen_name = n; return marker; }
extern "C" void animResetFramestates__FP14AnimationState(void* s) { calls.push_back("reset"); reset_arg = s; }
alignas(8) static unsigned char object[1200], holder[104 * 16];
static unsigned obj_word, holder_word, lump_word; static char lump_name[8];
static unsigned& rw(unsigned char* b, unsigned o) { return *reinterpret_cast<unsigned*>(b + o); }
static void reset(int b, float f, int use_lump) {
    std::memset(object, 0xaa, sizeof object); std::memset(holder, 0, sizeof holder);
    for (int i = 0; i < 16; ++i) { rw(holder, 104 * i + 68) = 0x1000 + i; rw(holder, 104 * i + 76) = 0x2000 + i; rw(holder, 104 * i + 84) = 0x3000 + i; rw(holder, 104 * i + 92) = 0x4000 + i; }
    rw(holder, 0) = lump_word;
    rw(object, 36) = holder_word; object[1106] = static_cast<unsigned char>(use_lump);
    std::memset(player, 0, sizeof player);
    rw(player + 23984, 21872) = obj_word; *reinterpret_cast<float*>(player + 23984 + 172) = f; player[23984 + 23582] = static_cast<unsigned char>(b);
    calls.clear();
}
int main() {
    obj_word = intern(object); holder_word = intern(holder); lump_word = intern(lump_name);
    reset(3, 1.0f, 0);
    resetPlayerAnimations__Fi(1);
    if (calls != std::vector<std::string>{"reset"} || reset_arg != object + 288) return 1;
    if (rw(object, 296) != 0x1003 || rw(object, 328) != 0x2003 || rw(object, 360) != 0x3003 || rw(object, 392) != 0x4003) return 2;
    if (*reinterpret_cast<float*>(object + 304) != 1.0f || *reinterpret_cast<float*>(object + 312) != 1.0f) return 3;
    for (unsigned o : {324u, 356u, 388u, 420u}) if (*reinterpret_cast<short*>(object + o) != 4613) return 4;
    for (unsigned o : {336u, 344u, 368u, 376u, 400u, 408u, 432u, 440u}) if (rw(object, o) != 0) return 5;
    if (object[0] != 0xaa || object[1107] != 0xaa || object[452] != 0xaa) return 6;       // nothing else is written
    // resource path: slot by row
    const int rows[] = {1, 2, 3, 4, 7, 0, 5, 6, 9, -1 + 16};
    const int slots[] = {0, 1, 2, 3, 4, 5, 5, 5, 5, 5};
    for (int i = 0; i < 10; ++i) {
        reset(rows[i], 0.5f, 1);
        resetPlayerAnimations__Fi(1);
        if (calls != std::vector<std::string>{"lump", "reset"}) return 7;
        if (seen_lump != lump_name || seen_name != reinterpret_cast<const char*>(norrath_inventory_reset_resource_names_view) + 16 * slots[i]) return 8;
        if (rw(object, 296) != inventory_items::word_of(marker)) return 9;
    }
    reset(1, 0.0f, 0); resetPlayerAnimations__Fi(1); if (!calls.empty() || object[0] != 0xaa) return 10;      // not above zero
    reset(1, -2.0f, 0); resetPlayerAnimations__Fi(1); if (!calls.empty()) return 11;
    reset(1, 1.0f, 0); rw(player + 23984, 21872) = 0; resetPlayerAnimations__Fi(1); if (!calls.empty()) return 12;
    return 0;
}
