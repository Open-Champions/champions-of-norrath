// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static unsigned char obj[400]; static unsigned char* new_result; static unsigned new_size;
static int ctor_calls, add_calls, add_result; static unsigned char* seen_pool; static unsigned char* seen_item;
extern "C" unsigned char* __builtin_new(unsigned n) { new_size = n; return new_result; }
extern "C" void __7VIRFont(unsigned char* s) { ++ctor_calls; if (s != obj) ctor_calls += 100; }
extern "C" int Add__t6VIPool1ZP7VIRFontRCP7VIRFont(unsigned char* pool, unsigned char** item) { ++add_calls; seen_pool = pool; seen_item = *item; return add_result; }
#include "create_font.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>
int main() {
    bool ok = true;
    for (int null_new : {0, 1}) for (int r : {0, 3, -1}) for (int share : {0, 1}) {
        std::memset(obj, 0, sizeof obj); obj[4] = 9;
        new_result = null_new ? nullptr : obj; ctor_calls = add_calls = 0; add_result = r;
        int v = CreateFont__8VIRasteri(arena, share);
        if (null_new) { ok = ok && v == -1 && ctor_calls == 0 && add_calls == 0; continue; }
        ok = ok && new_size == 88 && ctor_calls == 1 && add_calls == 1 && seen_pool == arena + 19508 && seen_item == obj;
        if (r < 0) ok = ok && v == -1 && obj[4] == 9;
        else ok = ok && v == r && obj[4] == 9 + share;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
