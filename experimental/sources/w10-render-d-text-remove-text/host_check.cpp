// Host check for textRemoveText: removes each record's own table slot, probing past other entries and wrapping.
#include <cstdint>
#include <cstdio>
#include <cstring>
extern "C" char* norrath_text_hash_slots[8192];
extern "C" void textRemoveText__FPv(void*);
char* norrath_text_hash_slots[8192];
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
// Independent model of the hash using unsigned 32-bit words.
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
int main() {
    alignas(2) static unsigned char block[256];
    unsigned char* p = block;
    const char* keys[4] = {"ab", "wxyz", "\xe9q", "k"};
    const unsigned short wides[4][3] = {{0x41, 0, 0}, {0x42, 0x43, 0}, {0x44, 0, 0}, {0x45, 0, 0}};
    const int wide_units[4] = {2, 3, 2, 2};
    char* addr[4];
    for (int k = 0; k < 4; k++) {
        addr[k] = (char*)p;
        size_t n = strlen(keys[k]);
        memcpy(p, keys[k], n + 1);
        p += n + 1;
        if ((uintptr_t)p & 1) *p++ = 0;
        memcpy(p, wides[k], wide_units[k] * 2);
        p += wide_units[k] * 2;
    }
    *p = 0;
    static char other;
    unsigned h0 = model_hash(keys[0]);
    norrath_text_hash_slots[h0] = &other;               // foreign entry at the home slot
    norrath_text_hash_slots[(h0 + 1) & 0x1fff] = addr[0];
    norrath_text_hash_slots[model_hash(keys[1])] = addr[1];
    norrath_text_hash_slots[model_hash(keys[2])] = addr[2];
    unsigned h3 = model_hash(keys[3]);
    norrath_text_hash_slots[h3] = &other;               // force wrap probe if the home is the last slot
    norrath_text_hash_slots[(h3 + 1) & 0x1fff] = &other;
    norrath_text_hash_slots[(h3 + 2) & 0x1fff] = addr[3];
    textRemoveText__FPv(block);
    CHECK(norrath_text_hash_slots[(h0 + 1) & 0x1fff] == 0 && norrath_text_hash_slots[h0] == &other);
    CHECK(norrath_text_hash_slots[model_hash(keys[1])] == 0 || model_hash(keys[1]) == h0 || model_hash(keys[1]) == h3);
    CHECK(norrath_text_hash_slots[model_hash(keys[2])] == 0 || model_hash(keys[2]) == h0 || model_hash(keys[2]) == h3);
    CHECK(norrath_text_hash_slots[(h3 + 2) & 0x1fff] == 0);
    CHECK(norrath_text_hash_slots[h3] == &other && norrath_text_hash_slots[(h3 + 1) & 0x1fff] == &other);
    int left = 0;
    for (int i = 0; i < 8192; i++) for (int k = 0; k < 4; k++) if (norrath_text_hash_slots[i] == addr[k]) left++;
    CHECK(left == 0);
    if (fails) return 1;
    puts("textRemoveText ok");
    return 0;
}
