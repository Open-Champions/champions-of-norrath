// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "reorder_palette.cpp"
#include <cstdio>
#include <cstring>
#include <utility>
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    bool ok = true;
    for (int format : {2, 5, 3}) {
        for (unsigned entries : {0u, 1u, 32u, 64u, 96u}) {
            std::memset(arena, 0, sizeof arena);
            unsigned* self = reinterpret_cast<unsigned*>(arena);
            self[4] = 1;
            self[13] = format;
            self[10] = entries;
            self[11] = 0x100;
            self[78] = 0x1000;
            unsigned char* pal = arena + 0x1100;
            for (int i = 0; i < 0x800; ++i) pal[i] = static_cast<unsigned char>(i * 7 + 1);
            unsigned char want[0x800];
            std::memcpy(want, pal, sizeof want);
            int group = format == 2 ? 128 : 64;
            int blocks = format == 3 ? 0 : (entries + 31) / 32;
            for (int b = 0; b < blocks; ++b) {
                int half = format == 2 ? 32 : 16;
                for (int j = 0; j < half; ++j)
                    std::swap(want[b * group + half + j], want[b * group + 2 * half + j]);
            }
            int r = ReorderPalette__9VISurface(arena);
            if (r != (format == 3 ? -1 : 0)) ok = false;
            if (std::memcmp(pal, want, sizeof want) != 0) ok = false;
            self[4] = 0;
            std::memcpy(want, pal, sizeof want);
            if (ReorderPalette__9VISurface(arena) != 0 || std::memcmp(pal, want, sizeof want) != 0) ok = false;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
