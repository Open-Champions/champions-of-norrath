// Host check only; not a build input. The base constructor is a recorder; the object is
// filled with a pattern so any store other than the table word at offset 32 would show.
// The same call and changed bytes were compared with the original in a private
// interpreter run (4 random fills, identical logs and memory).
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

unsigned char _vt$10VIWndLegal[8];
static unsigned char storage[64];
static int calls;
extern "C" void __5VIWnd(void* self) { if (self == storage) ++calls; }

int main() {
    std::memset(storage, 0x5a, sizeof storage);
    __10VIWndLegal(storage);
    unsigned word;
    std::memcpy(&word, storage + 32, 4);
    bool ok = calls == 1 && word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$10VIWndLegal));
    for (int i = 0; i < 64; ++i)
        if ((i < 32 || i >= 36) && storage[i] != 0x5a) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
