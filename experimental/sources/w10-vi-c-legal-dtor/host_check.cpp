// Host check only; not a build input. Bit 0 of the flag decides the delete; the table word is
// always written. Compared with the original in a private interpreter run (flags 0 to 3).
#include "dtor.cpp"
#include <cstdio>
#include <cstring>

unsigned char _vt$10VIWndLegal[8];
static unsigned char storage[64];
static int base_calls, base_flag = -1, deleted;
extern "C" void _$_5VIWnd(void* self, int flags) { if (self == storage) { ++base_calls; base_flag = flags; } }
extern "C" void __builtin_delete(void* pointer) { if (pointer == storage) ++deleted; }

int main() {
    bool ok = true;
    for (int flags = 0; flags < 4; ++flags) {
        base_calls = 0; deleted = 0; base_flag = -1;
        std::memset(storage, 0x5a, sizeof storage);
        dtor_10VIWndLegal(storage, flags);
        unsigned word;
        std::memcpy(&word, storage + 32, 4);
        ok = ok && base_calls == 1 && base_flag == 0 && deleted == (flags & 1) &&
             word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$10VIWndLegal));
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
