// Observed playerIsAnimPlaying__6PlayerPc at 0x001ac160, 112 bytes.
// Arguments: a Player object pointer (record layout is the 23984-byte player
// record) and a resource name. The word at object+36 points to a structure whose
// first word is the lump name; lumpFindResource(lump, name) gives a resource
// address. The signed byte at object+292 is a count; entries are 32 bytes apart
// starting at object+296 and the first word of each entry is compared with the
// resource address. Returns 1 at the first match, otherwise 0 (also when the
// count is zero or negative; the lookup happens either way).
// Unknown: what the entries are beyond their first word.
#include "../inventory-items-record/record.hpp"
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name);

extern "C" int playerIsAnimPlaying__6PlayerPc(unsigned char* object, char* name) {
    using namespace inventory_items;
    const unsigned* holder = resolve<unsigned>(*reinterpret_cast<unsigned*>(object + 36));
    char* resource = lumpFindResource__FPcT0(const_cast<char*>(resolve<char>(*holder)), name);
    const signed char count = static_cast<signed char>(object[292]);
    for (int index = 0; index < count; ++index) {
        if (*reinterpret_cast<unsigned*>(object + 296 + 32 * index) == word_of(resource)) return 1;
    }
    return 0;
}
