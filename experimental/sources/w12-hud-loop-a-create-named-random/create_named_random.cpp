// Observed createNamedRandom__4itemPQ24item11descriptioniii at 0x00103f28, 1304 bytes, 432-byte frame
// (item::createNamedRandom). Arguments: out (72-byte description), limit (value cap), category (-1 for any),
// level. Returns 1 when a description was written, else 0 (the registers hold other values on the early
// paths; callers use the result as a flag).
// Table: _4item$namedItemTable, 64-byte rows ended by a zero first word. Row fields used: +0 name text,
// +4 signed halfword minimum level, +29 category byte, +40 64-bit flags.
// 1. wizards = the number of nmPlayers records (23984-byte stride, word at +80) equal to 4; fraction =
//    wizards / nmPlayers in single precision. flagA starts 0. For category 1 one draw r is taken (draw =
//    state >> 16 after state = state * 0x19660d + 0x3c6ef35f, stored in syncRandState) and flagA = ((int)(fraction
//    * 65536.0f) < r % 65535).
// 2. A row is eligible when its flags lack 0x10000 and bit 35, category is -1 or equals the row's, level is
//    not below the row's minimum, and, with flagA set, it is not (category byte 1 with flag 0x80000).
//    The eligible rows are counted; none gives result 0.
// 3. Ten rounds, best starting at -1. Each round clears a local description (attribute halfwords +36, +44,
//    +52, +60 = -1, bytes +32 and +33 = 0xff, halfwords +16..+22 and +24..+30 = 0), draws k = draw % count and
//    walks the rows: the first eligible row whose running value (k, decreased by one for each eligible row
//    seen) is not above zero after the decrease is chosen (so k = 0 and k = 1 choose the first eligible row).
//    The row name is copied, create(name, local, -1, 1) runs, value = getValue(local). Unless the name contains
//    "potion", an attribute id 14 with first value min(2 * (int)powf(max(value - 1200, 0) / 2000, 0.66f) + 1,
//    50) is stored in the first attribute slot that is free (-1) or already 14, with the next two halfwords 0.
//    The local description is kept (copied to out, best = value) when value >= best and value <= limit.
// 4. best < 0 gives 0. Otherwise the roll field is rewritten as in createRandomCombineableItem: n = (flags of
//    the row named by out's first word >> 11) & 31; bits 11..15 of the 64-bit word at out + 8 become
//    (n > 0 ? draw % n : 0) << 11, drawing only when n > 0. Result 1.
// Unknown: category and flag meanings, the factor 0.66 and the wizard fraction's role beyond selecting flagA.
extern "C" {
extern unsigned char _4item$namedItemTable[];
extern int nmPlayers;
extern unsigned syncRandState;
extern unsigned char player[];
char* strcpy(char* to, const char* from);
char* strstr(const char* text, const char* pattern);
float powf(float base, float exponent);
int getValue__4itemPQ24item11description(const void* item);
void create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b);
#if defined(NORRATH_HOST_CONTRACT)
unsigned char* norrath_cnr_pointer(unsigned word);
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

int half_at(const unsigned char* address) { return static_cast<short>(load_u32(address) & 0xffffu); }

const char* text_at(const unsigned char* address) {
    const unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    return reinterpret_cast<const char*>(norrath_cnr_pointer(word));
#else
    return reinterpret_cast<const char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

const unsigned char* row_pointer(const unsigned char* address) {
    const unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_cnr_pointer(word);
#else
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

unsigned draw() {
    syncRandState = syncRandState * 0x19660dU + 0x3c6ef35fU;
    return syncRandState >> 16;
}

float float_from_bits(unsigned bits) {
    union {
        unsigned word;
        float value;
    } u;
    u.word = bits;
    return u.value;
}

bool eligible(const unsigned char* row, int category, int level, bool flag_a) {
    const unsigned long long flags = load_u64(row + 40);
    if ((flags & 0x10000ull) != 0) return false;
    if (category != -1 && static_cast<signed char>(row[29]) != category) return false;
    if (level < half_at(row + 4)) return false;
    if ((flags & 0x800000000ull) != 0) return false;
    if (flag_a && static_cast<signed char>(row[29]) == 1 && (flags & 0x80000ull) != 0) return false;
    return true;
}
}

extern "C" int createNamedRandom__4itemPQ24item11descriptioniii(unsigned char* out, int limit, int category, int level) {
    int wizards = 0;
    for (int index = 0; index < nmPlayers; ++index)
        if (static_cast<int>(load_u32(player + 80 + static_cast<unsigned>(index) * 23984u)) == 4) ++wizards;
    const float fraction = static_cast<float>(wizards) / static_cast<float>(nmPlayers);
    bool flag_a = false;
    if (category == 1) {
        const unsigned r = draw();
        flag_a = static_cast<int>(fraction * float_from_bits(0x47800000u)) < static_cast<int>(r % 65535u);
    }
    int count = 0;
    for (const unsigned char* row = _4item$namedItemTable; load_u32(row) != 0; row += 64)
        if (eligible(row, category, level, flag_a)) ++count;
    if (count == 0) return 0;

    alignas(8) unsigned char item[72];
    char buffer[256];
    int best = -1;
    for (int round = 9; round >= 0; --round) {
        for (int i = 0; i < 4; ++i) store_u16(item + 36 + 8 * i, 0xffffu);
        item[32] = 0xff;
        item[33] = 0xff;
        for (int i = 0; i < 4; ++i) {
            store_u16(item + 16 + 2 * i, 0);
            store_u16(item + 24 + 2 * i, 0);
        }
        int remaining = static_cast<int>(draw() % static_cast<unsigned>(count));
        const unsigned char* row = _4item$namedItemTable;
        for (;; row += 64) {
            if (!eligible(row, category, level, flag_a)) continue;
            if (--remaining > 0) continue;
            break;
        }
        strcpy(buffer, text_at(row));
        create__4itemPcPQ24item11descriptionii(buffer, item, -1, 1);
        const int value = getValue__4itemPQ24item11description(item);
        if (strstr(text_at(row), "potion") == 0) {
            int base = value - 1200;
            if (base < 0) base = 0;
            base /= 2000;
            int amount = static_cast<int>(powf(static_cast<float>(base), float_from_bits(0x3f28f5c2u))) * 2 + 1;
            if (!(amount < 51)) amount = 50;
            for (int slot = 0; slot < 4; ++slot) {
                unsigned char* id = item + 36 + 8 * slot;
                const int current = half_at(id);
                if (current == -1 || current == 14) {
                    store_u16(id, 14);
                    store_u16(id + 2, static_cast<unsigned>(amount));
                    store_u16(id + 4, 0);
                    store_u16(id + 6, 0);
                    break;
                }
            }
        }
        if (value < best) continue;
        if (limit < value) continue;
        for (int i = 0; i < 72; ++i) out[i] = item[i];
        best = value;
    }
    if (best < 0) return 0;
    const unsigned char* chosen = row_pointer(out);
    int rolls = static_cast<int>((load_u64(chosen + 40) >> 11) & 0x1fu);
    int pick = rolls;
    if (rolls > 0) pick = static_cast<int>(draw() % static_cast<unsigned>(rolls));
    unsigned long long word = load_u64(out + 8);
    word = (word & 0xffffffffffff07ffull) | static_cast<unsigned long long>(static_cast<unsigned>(pick) << 11);
    store_u64(out + 8, word);
    return 1;
}
