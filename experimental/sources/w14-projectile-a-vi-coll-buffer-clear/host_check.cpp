// Host contract for Clear__12VICollBuffer; not an EE input. __builtin_vec_delete logs its argument and the object bytes at the time of the
// call. The authored function was also compared with the original in a private R5900 run (random objects with and without an array).
#include <cstdio>
#include <cstring>
#include <cstdint>
static unsigned char arena[0x200];
static int deletes;
static void* deleted;
static unsigned char at_delete[0x80];
extern "C" void __builtin_vec_delete(void* pointer) {
    ++deletes; deleted = pointer; std::memcpy(at_delete, arena + 64, sizeof at_delete);
}
#include "clear.cpp"
static int run(bool with_array) {
    for (int i = 0; i < 0x200; ++i) arena[i] = static_cast<unsigned char>(i * 11 + 5);
    unsigned char before[0x80];
    unsigned char* o = arena + 64;
    deletes = 0; deleted = nullptr;
    // On the host the word at +88 cannot hold a real pointer, so only the low 32 bits are compared.
    unsigned w = with_array ? 0x12345678u : 0u;
    std::memcpy(o + 88, &w, 4);
    std::memcpy(before, o, sizeof before);
    int r = Clear__12VICollBuffer(o);
    if (r != 0) return 1;
    if (deletes != (with_array ? 1 : 0)) return 2;
    if (with_array) {
        if (static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(deleted)) != 0x12345678u) return 3;
        if (std::memcmp(at_delete, before, sizeof before) != 0) return 4;  // nothing cleared before the delete
    }
    static const unsigned zero[] = {8, 12, 16, 44, 48, 52, 56, 60, 64, 68, 72, 76, 84, 88};
    for (unsigned off : zero) { unsigned v; std::memcpy(&v, o + off, 4); if (v != 0) return 5; }
    if (o[80] != 0 || o[81] != 0xff) return 6;
    for (unsigned i = 0; i < sizeof before; ++i) {
        bool written = (i >= 8 && i < 20) || (i >= 44 && i < 80) || (i >= 80 && i < 82) || (i >= 84 && i < 92);
        if (!written && o[i] != before[i]) return 7;
    }
    for (int i = 0; i < 64; ++i) if (arena[i] != static_cast<unsigned char>(i * 11 + 5)) return 8;
    return 0;
}
int main() { int r = run(true); if (r) return r; return run(false); }
