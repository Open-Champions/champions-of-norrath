// Observed lgCodecOpen at 0x002eb0f8, 308 bytes. g_codecs (0x00447f90, 40
// bytes) is searched as two 20-byte records for one whose first word equals
// the identifier; no match returns 0x80000004. A match with an identifier other
// than 300 returns 0 and writes nothing. For 300, a 392-byte block from
// __wrap_malloc is cleared; two gsm_create results go to offsets 0x14 and
// 0x18; gsm_option runs on the first with option 3 and a pointer to the word 1;
// the matched 20-byte record is copied to the start of the block; the block
// address is stored through the second argument and 0 is returned. A null
// allocation is not checked. What the record fields mean is unknown.
extern "C" {
extern unsigned char g_codecs[];
void* __wrap_malloc(unsigned size);
void* memset(void* destination, int value, __SIZE_TYPE__ size);
void* gsm_create();
int gsm_option(void* state, int option, int* value);
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
unsigned lgCodecOpen(int identifier, unsigned char* result) {
    int found = -1;
    for (int index = 0; index < 2; ++index) {
        if (load_word(g_codecs + index * 20) == static_cast<unsigned>(identifier)) {
            found = index;
            break;
        }
    }
    if (found == -1) return 0x80000004u;
    if (identifier != 300) return 0u;
    unsigned char* block = static_cast<unsigned char*>(__wrap_malloc(392u));
    int option_value = 1;
    memset(block, 0, 392u);
    store_word(block + 0x14, pointer_bits(gsm_create()));
    store_word(block + 0x18, pointer_bits(gsm_create()));
    gsm_option(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(load_word(block + 0x14))), 3, &option_value);
    for (int index = 0; index < 20; ++index) block[index] = g_codecs[found * 20 + index];
    store_word(result, pointer_bits(block));
    return 0u;
}
}
