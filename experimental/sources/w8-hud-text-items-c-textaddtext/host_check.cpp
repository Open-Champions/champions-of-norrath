#include <cstdint>
#include <cstring>
extern "C" char* norrath_text_hash_slots[8192];
extern "C" void textAddText__FPv(void*);
char* norrath_text_hash_slots[8192];

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
    const char* keys[3] = {"ab", "wxyz", "\xe9q"};
    const unsigned short wides[3][3] = {{0x41, 0, 0}, {0x42, 0x43, 0}, {0x44, 0, 0}};
    const int wide_units[3] = {2, 3, 2};
    char* addr[3];
    for (int k = 0; k < 3; k++) {
        addr[k] = (char*)p;
        size_t n = strlen(keys[k]);
        memcpy(p, keys[k], n + 1);
        p += n + 1;
        if ((uintptr_t)p & 1) p++;
        memcpy(p, wides[k], wide_units[k] * 2);
        p += wide_units[k] * 2;
    }
    *p = 0;
    textAddText__FPv(block);
    int found = 0;
    for (int s = 0; s < 8192; s++) if (norrath_text_hash_slots[s]) found++;
    if (found != 3) return 1;
    for (int k = 0; k < 3; k++) {
        if (norrath_text_hash_slots[model_hash(keys[k])] != addr[k]) return 2 + k;
    }
    // Collision: a fourth record whose key is the same as the first uses the next slot.
    static unsigned char again[16];
    memcpy(again, "ab", 3);
    again[4] = 0; again[5] = 0; again[6] = 0;
    textAddText__FPv(again);
    unsigned s = model_hash("ab");
    if (norrath_text_hash_slots[s] != addr[0]) return 6;
    if (norrath_text_hash_slots[(s + 1) & 0x1fff] != (char*)again) return 7;
    // Wrap at the end of the table.
    for (int i = 0; i < 8192; i++) norrath_text_hash_slots[i] = 0;
    s = model_hash("ab");
    // Occupy from s through the last slot so the probe wraps to slot 0.
    for (unsigned i = s; i < 8192; i++) norrath_text_hash_slots[i] = (char*)1;
    textAddText__FPv(again);
    if (norrath_text_hash_slots[0] != (char*)again) return 8;
    // Empty block adds nothing.
    for (int i = 0; i < 8192; i++) norrath_text_hash_slots[i] = 0;
    static unsigned char empty[4] = {0, 0, 0, 0};
    textAddText__FPv(empty);
    for (int i = 0; i < 8192; i++) if (norrath_text_hash_slots[i]) return 9;
    return 0;
}
