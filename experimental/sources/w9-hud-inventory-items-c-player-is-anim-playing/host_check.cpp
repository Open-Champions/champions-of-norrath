// Host check for playerIsAnimPlaying (includes the source; build the file alone): lump lookup arguments, scan range and stride.
#define NORRATH_INVENTORY_HOST 1
#include "../inventory-items-record/record.hpp"
#include <cstring>
#include <map>
#include <string>
unsigned char player[1];
namespace { std::map<unsigned, const void*> words; unsigned next = 0x1000; }
static unsigned intern(const void* p) { words[next] = p; next += 0x10; return next - 0x10; }
const void* inventory_items::host_resolve(unsigned w) { return words.at(w); }
static std::string seen_lump, seen_name; static int lookups; static char resource[8];
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) { ++lookups; seen_lump = lump; seen_name = name; return resource; }
#include "player_is_anim_playing.cpp"
// Word equal to what word_of(resource) yields on this host.
static void put(unsigned char* o, int i, unsigned w) { std::memcpy(o + 296 + 32 * i, &w, 4); }
int main() {
    unsigned char object[512] = {};
    char lump[] = "eq.lmp"; char name[] = "dh_idle.anm";
    unsigned lump_word = intern(lump);
    unsigned holder[1] = {lump_word};
    unsigned holder_word = intern(holder);
    std::memcpy(object + 36, &holder_word, 4);
    const unsigned rw = inventory_items::word_of(resource);
    object[292] = 3;
    put(object, 0, rw + 4); put(object, 1, 0); put(object, 2, rw);
    if (playerIsAnimPlaying__6PlayerPc(object, name) != 1) return 1;
    if (seen_lump != "eq.lmp" || seen_name != "dh_idle.anm" || lookups != 1) return 2;
    object[292] = 2;                       // match sits at index 2, outside a count of 2
    if (playerIsAnimPlaying__6PlayerPc(object, name) != 0) return 3;
    object[292] = 0; if (playerIsAnimPlaying__6PlayerPc(object, name) != 0 || lookups != 3) return 4;
    object[292] = static_cast<unsigned char>(-4); put(object, 0, rw);
    if (playerIsAnimPlaying__6PlayerPc(object, name) != 0) return 5;  // negative count scans nothing
    object[292] = 1; if (playerIsAnimPlaying__6PlayerPc(object, name) != 1) return 6;
    return 0;
}
