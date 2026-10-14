// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "free.cpp"
#include <cstdio>
#include <cstring>
static unsigned char storage[0x1000];
static unsigned char block[4];
static int deletes;
extern "C" void __builtin_vec_delete(void* pointer) { if (pointer == block) ++deletes; }
int main() {
    bool ok = true;
    std::memset(storage, 0x5a, sizeof storage);
    void* pointer = block;
    std::memcpy(storage + 1552, &pointer, sizeof pointer);
    Free__9VIWndEula(storage);
    void* word;
    std::memcpy(&word, storage + 1552, sizeof word);
    ok = ok && deletes == 1 && word == nullptr;
    deletes = 0;
    std::memset(storage, 0x5a, sizeof storage);
    std::memset(storage + 1552, 0, sizeof(void*));
    Free__9VIWndEula(storage);
    ok = ok && deletes == 0;
    for (int i = 0; i < 0x1000; ++i)
        if (!(i >= 1552 && i < 1552 + static_cast<int>(sizeof(void*))) && storage[i] != 0x5a) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
