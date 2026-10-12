#include <cstdint>
#include <cstring>
extern "C" char* norrath_text_hash_slots[8192];
extern "C" unsigned short norrath_text_missing_entries[16][20];
extern "C" unsigned char norrath_text_missing_cursor;
extern "C" unsigned short* textGetString__FPc(char*);
char* norrath_text_hash_slots[8192];
unsigned short norrath_text_missing_entries[16][20];
unsigned char norrath_text_missing_cursor;

static unsigned model_hash(const char* key) {
    uint32_t h = 0;
    for (const char* p = key; *p; p++) {
        h += (uint32_t)(int32_t)(signed char)*p;
        h += h << 10;
        h ^= (uint32_t)((int32_t)h >> 6);
    }
    h += h << 3;
    h ^= (uint32_t)((int32_t)h >> 11);
    return h & 0x1fff;
}

alignas(2) static unsigned char rec_a[16];
alignas(2) static unsigned char rec_b[16];
alignas(2) static unsigned char rec_c[16];

static void put(unsigned char* rec, const char* key, unsigned short unit) {
    size_t n = strlen(key);
    memcpy(rec, key, n + 1);
    size_t at = (n + 2) & ~(size_t)1;
    memcpy(rec + at, &unit, 2);
}

int main() {
    put(rec_a, "hello", 0x61);
    put(rec_b, "hello", 0x62);   // same key, stored in the next slot
    put(rec_c, "tab", 0x63);
    unsigned h = model_hash("hello");
    norrath_text_hash_slots[h] = (char*)rec_c;   // a different key occupying the home slot
    norrath_text_hash_slots[(h + 1) & 0x1fff] = (char*)rec_a;
    norrath_text_hash_slots[(h + 2) & 0x1fff] = (char*)rec_b;
    norrath_text_hash_slots[model_hash("tab")] = (char*)rec_c;
    // Overwritten if hashes collide; make the lookup deterministic.
    if (model_hash("tab") == h) return 99;

    char key[] = "hello";
    unsigned short* r = textGetString__FPc(key);
    // Probing skips the colliding key and returns the first match: record a.
    if ((unsigned char*)r != rec_a + 6 || r[0] != 0x61) return 1;
    // Quotes are stripped before hashing and comparing.
    char quoted[] = "\"hel\"lo\"";
    if (textGetString__FPc(quoted) != r) return 2;
    char tab[] = "tab";
    unsigned short* t = textGetString__FPc(tab);
    if ((unsigned char*)t != rec_c + 4 || t[0] != 0x63) return 3;

    // Miss: rotating "NF:" entries.
    norrath_text_missing_cursor = 14;
    char miss[] = "nope";
    unsigned short* m1 = textGetString__FPc(miss);
    if (m1 != norrath_text_missing_entries[15] || norrath_text_missing_cursor != 15) return 4;
    if (m1[0] != 'N' || m1[1] != 'F' || m1[2] != ':') return 5;
    for (int i = 0; i < 4; i++) if (m1[3 + i] != (unsigned char)"nope"[i]) return 6;
    if (m1[7] != 0) return 7;
    unsigned short* m2 = textGetString__FPc(miss);
    if (m2 != norrath_text_missing_entries[0] || norrath_text_missing_cursor != 0) return 8;
    // Long key: only 16 units are copied, entry stays terminated.
    char longkey[] = "abcdefghijklmnopqrstuvwxyz";
    unsigned short* m3 = textGetString__FPc(longkey);
    for (int i = 0; i < 16; i++) if (m3[3 + i] != (unsigned short)('a' + i)) return 9;
    if (m3[19] != 0) return 10;
    // Non-ASCII key bytes are sign-extended into the units.
    char hi[] = "\xe9";
    unsigned short* m4 = textGetString__FPc(hi);
    if (m4[3] != 0xffe9) return 11;
    return 0;
}
