// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "init.cpp"
#include <cstdio>
#include <cstring>
static unsigned char storage[0x1000];
static unsigned char block[4];
static void* next_block;
static int news, deletes;
static unsigned last_size;
extern "C" void* __builtin_vec_new(unsigned size) { ++news; last_size = size; return next_block; }
extern "C" void __builtin_vec_delete(void* pointer) { if (pointer == block) ++deletes; }
static bool run(void* result_block, int want_result, int want_deletes) {
    next_block = result_block; news = deletes = 0;
    std::memset(storage, 0x5a, sizeof storage);
    const int result = Init__9VIWndEula(storage);
    int zero, one;
    std::memcpy(&zero, storage + 1552, 4);
    std::memcpy(&one, storage + 1548, 4);
    bool ok = result == want_result && news == 1 && last_size == 0x10000 && deletes == want_deletes && zero == 0 && one == 1;
    for (int i = 0; i < 0x1000; ++i)
        if (!(i >= 1548 && i < 1556) && storage[i] != 0x5a) ok = false;
    return ok;
}
int main() {
    const bool ok = run(block, 0, 1) && run(nullptr, -1, 0);
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
