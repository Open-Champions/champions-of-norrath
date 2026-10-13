// Observed createRandomCombineableItem__4itemPQ24item11descriptionsPc at 0x00105aa8, 1204 bytes, 704-byte
// frame (item::createRandomCombineableItem). Arguments: the output description (72 bytes), a signed
// halfword level, and an optional name (null for "any"). Returns 1 when a description was produced, else 0.
// Item table rows are 64 bytes, from _4item$itemTable (published here under the view name
// norrath_item_table_view, as the neighbouring item sources do), ended by a row whose first word is zero:
// +0 name text pointer, +4 signed halfword minimum level, +29 category byte, +40 64-bit flag word,
// +48 a second text pointer. The floor is minimum = 17 for gameDifficulty 2, 27 for 3, otherwise -1.
// Eligible rows (counted first, then the k-th is chosen, k = (draw % count) with draw = state >> 16 after
// state = state * 0x19660d + 0x3c6ef35f stored back into syncRandState): category byte 2, flag bit 35
// set, level not below the row's minimum, and the row's minimum not below the floor. Without a name
// the flag bit 0x10000 must also be clear; with a name strcmp(row text at +48, name) must be zero (and
// bit 0x10000 is not examined). If no row is eligible the result is 0 at once.
// The chosen row's name text (word at +0) is copied to a local buffer and create(buffer, local,
// -1, 0) fills a local description whose attribute halfwords (+36, +44, +52, +60) are -1, bytes +32 and +33
// are 0xff and halfwords +16..+22 and +24..+30 are zero before the call; its 72 bytes are copied to the
// output. Then n = (flag word of the row named by the output's first word >> 11) & 31, and bits 11 to 15 of
// the 64-bit word at output + 8 are replaced by (n > 0 ? draw % n : 0) << 11, drawing once more from
// syncRandState only when n > 0. Result 1.
// Inferred: this picks a random combinable item (category 2, such as potions) usable at the given level.
// Unknown: the meaning of category 2, flag bit 35 and bit 0x10000, and the fields at +48 and the output's
// first word beyond their use here. The selection loop has no end test, as in the original; the counted
// rows guarantee it stops.
extern "C" {
extern unsigned char norrath_item_table_view[];
extern int gameDifficulty;
extern unsigned syncRandState;
int strcmp(const char* a, const char* b);
char* strcpy(char* to, const char* from);
void create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b);
#if defined(NORRATH_HOST_CONTRACT)
unsigned char* norrath_ccri_pointer(unsigned word);
#endif
}

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

unsigned long long load_u64(const unsigned char* address) {
    return static_cast<unsigned long long>(load_u32(address)) | (static_cast<unsigned long long>(load_u32(address + 4)) << 32);
}

void store_u16(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
}

void store_u64(unsigned char* address, unsigned long long value) {
    for (int i = 0; i < 8; ++i) address[i] = static_cast<unsigned char>(value >> (8 * i));
}

unsigned char* pointer_from_word(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_ccri_pointer(word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

int half_at(const unsigned char* address) { return static_cast<short>(load_u32(address) & 0xffffu); }

unsigned draw() {
    syncRandState = syncRandState * 0x19660dU + 0x3c6ef35fU;
    return syncRandState >> 16;
}

const unsigned long long kBit35 = 0x800000000ull;

// Row eligibility without the draw-dependent part: shared by the counting and the picking passes.
bool eligible(const unsigned char* row, const char* name, int level, int floor_level) {
    const unsigned long long flags = load_u64(row + 40);
    if (name == nullptr) {
        if ((flags & 0x10000ull) != 0) return false;
        if (static_cast<signed char>(row[29]) != 2) return false;
        if ((flags & kBit35) == 0) return false;
    } else {
        if (static_cast<signed char>(row[29]) != 2) return false;
        if ((flags & kBit35) == 0) return false;
        if (strcmp(reinterpret_cast<const char*>(pointer_from_word(load_u32(row + 48))), name) != 0) return false;
    }
    const int minimum = half_at(row + 4);
    if (level < minimum) return false;
    if (minimum < floor_level) return false;
    return true;
}
}

extern "C" int createRandomCombineableItem__4itemPQ24item11descriptionsPc(unsigned char* out, short level, char* name) {
    int floor_level = -1;
    if (gameDifficulty == 3) floor_level = 27;
    if (gameDifficulty == 2) floor_level = 17;
    const int wanted_level = level;
    int count = 0;
    for (const unsigned char* row = norrath_item_table_view; load_u32(row) != 0; row += 64)
        if (eligible(row, name, wanted_level, floor_level)) ++count;
    if (count == 0) return 0;

    alignas(8) unsigned char item[72];
    for (int i = 0; i < 4; ++i) store_u16(item + 36 + 8 * i, 0xffffu);
    item[32] = 0xff;
    item[33] = 0xff;
    for (int i = 0; i < 4; ++i) {
        store_u16(item + 16 + 2 * i, 0);
        store_u16(item + 24 + 2 * i, 0);
    }
    int remaining = static_cast<int>(draw() % static_cast<unsigned>(count));
    const unsigned char* row = norrath_item_table_view;
    for (;; row += 64) {
        if (!eligible(row, name, wanted_level, floor_level)) continue;
        if (--remaining >= 0) continue;
        break;
    }
    char buffer[256];
    strcpy(buffer, reinterpret_cast<const char*>(pointer_from_word(load_u32(row))));
    create__4itemPcPQ24item11descriptionii(buffer, item, -1, 0);
    for (int i = 0; i < 72; ++i) out[i] = item[i];
    const unsigned char* chosen = pointer_from_word(load_u32(out));
    int rolls = static_cast<int>((load_u64(chosen + 40) >> 11) & 0x1fu);
    int pick = rolls;
    if (rolls > 0) pick = static_cast<int>(draw() % static_cast<unsigned>(rolls));
    unsigned long long word = load_u64(out + 8);
    word = (word & 0xffffffffffff07ffull) | static_cast<unsigned long long>(static_cast<unsigned>(pick) << 11);
    store_u64(out + 8, word);
    return 1;
}
