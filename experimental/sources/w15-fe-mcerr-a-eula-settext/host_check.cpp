// Host check only; not a build input. The allocator returns a host buffer; the stored pointer word is
// compared by its low 32 bits.
#include "settext.cpp"
#include <cstdio>
#include <cstring>

static unsigned char heap[256];
static unsigned long asked, calls;
extern "C" void* __builtin_vec_new(unsigned long size) {
    asked = size; ++calls;
    std::memset(heap, 0xee, sizeof heap);
    return heap;
}

int main() {
    bool ok = true;
    int object[500];
    std::memset(object, 0x11, sizeof object);
    const unsigned short text[6] = {0x41, 9, 0x42, 9, 9, 0x43};
    SetText__9VIWndEulaPCUsi(object, text, 6);
    const unsigned short want[7] = {0x41, 32, 0x42, 32, 32, 0x43, 0};
    ok = ok && calls == 1 && asked == 14 && std::memcmp(heap, want, 14) == 0 && heap[14] == 0xee;
    ok = ok && object[387] == 1 && static_cast<unsigned>(object[388]) == static_cast<unsigned>(reinterpret_cast<unsigned long>(heap));
    ok = ok && object[386] == 0x11111111 && object[389] == 0x11111111;
    SetText__9VIWndEulaPCUsi(object, text, 0);
    ok = ok && asked == 2 && heap[0] == 0 && heap[1] == 0 && heap[2] == 0xee;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
