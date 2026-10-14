// Host check only; not a build input. Build with -fno-builtin-memcpy. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "reset_cache.cpp"
#include <cstdio>
#include <initializer_list>
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static int lock_result, unlock_result, calls_unlock, lock_count;
// Mock of the memcpy this unit declares (not the host libc one); plain byte loop.
extern "C" void* memcpy(void* dest, const void* source, unsigned size) {
    for (unsigned i = 0; i < size; ++i) static_cast<unsigned char*>(dest)[i] = static_cast<const unsigned char*>(source)[i];
    return dest;
}
extern "C" void* Surface__8VIRasteri(void*, int index) { return arena + 0x3f00 + index; }
extern "C" int LockPalette__9VISurfaceRiR12VIPixelStorePPUc(void*, int* count, void*, unsigned char** dest) {
    *count = lock_count;
    *dest = arena + 0x3000;
    return lock_result;
}
extern "C" int UnlockPalette__9VISurface(void*) { ++calls_unlock; return unlock_result; }

int main() {
    bool ok = true;
    for (int entries : {0, 1, 3}) {
        for (int slots : {0, 2}) {
            __builtin_memset(arena, 0x5a, sizeof arena);
            unsigned* font = reinterpret_cast<unsigned*>(arena);
            font[3] = 4; font[13] = 5; font[18] = 0x200;
            font[11] = entries; font[17] = 0x400; font[9] = slots; font[16] = 0x800;
            for (int i = 0; i < 16; ++i) arena[0x200 + i] = static_cast<unsigned char>(i + 1);
            lock_result = unlock_result = 0; calls_unlock = 0; lock_count = 3;
            if (ResetCache__7VIRFontR8VIRaster(arena, arena + 0x3800) != 0) ok = false;
            // min(count 3, +52 = 5) words copied
            for (int i = 0; i < 12; ++i) if (arena[0x3000 + i] != i + 1) ok = false;
            if (arena[0x3000 + 12] != 0x5a) ok = false;
            if (entries == 0 && (font[14] != 0xffffffffu || font[15] != 0xffffffffu)) ok = false;
            for (int i = 0; i < entries; ++i) {
                unsigned char* e = arena + 0x400 + 56 * i;
                unsigned short h; int prev, next;
                __builtin_memcpy(&h, e + 12, 2); __builtin_memcpy(&prev, e + 8, 4); __builtin_memcpy(&next, e + 4, 4);
                if (h != 0xffff || prev != i - 1 || next != (i == entries - 1 ? -1 : i + 1)) ok = false;
            }
            if (entries && (font[14] != 0 || font[15] != static_cast<unsigned>(entries - 1))) ok = false;
            for (int k = 0; k < slots; ++k) {
                unsigned short h;
                __builtin_memcpy(&h, arena + 0x800 + 8 + 12 * k, 2);
                if (h != 0xffff) ok = false;
            }
            unsigned short h; __builtin_memcpy(&h, arena + 0x800 + 8 + 12 * slots, 2);
            if (h == 0xffff) ok = false;
        }
    }
    __builtin_memset(arena, 0, 0x100);
    lock_result = -1; calls_unlock = 0;
    if (ResetCache__7VIRFontR8VIRaster(arena, arena + 0x3800) != -1 || calls_unlock != 0) ok = false;
    lock_result = 0; unlock_result = -1;
    if (ResetCache__7VIRFontR8VIRaster(arena, arena + 0x3800) != -1 || calls_unlock != 1) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
