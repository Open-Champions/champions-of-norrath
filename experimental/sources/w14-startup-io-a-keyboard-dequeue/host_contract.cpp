#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[256];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "dequeue.cpp"

int main() {
    for (int count = -2; count <= 9; ++count) {
        signed char* queue = reinterpret_cast<signed char*>(arena + 64);
        for (int i = 0; i < 32; ++i) queue[i] = static_cast<signed char>(i * 7 - 90);
        signed char expect[32];
        std::memcpy(expect, queue, 32);
        alignas(8) int words[8] = {0x11, 0x22, 0, count, 0x33, 0x44, 0x55, 0x66};
        words[2] = arena_base + 64;
        signed char out[4] = {0x5a, 0x5a, 0x5a, 0x5a};
        int r = DequeueKeyEvent__10VIKeyboardP12VIConsoleKey(words, out);
        if (count == 0) {
            if (r != 0 || out[0] != 0x5a || out[1] != 0x5a || words[3] != 0) return 1;
            if (std::memcmp(queue, expect, 32) != 0) return 2;
            continue;
        }
        if (r != 1 || out[0] != expect[0] || out[1] != expect[1] || out[2] != 0x5a || out[3] != 0x5a) return 3;
        if (words[3] != count - 1) return 4;
        if (words[0] != 0x11 || words[1] != 0x22 || words[4] != 0x33 || words[7] != 0x66) return 5;
        for (int i = 0; i < 32; ++i) {
            signed char want = expect[i];
            if (i < 2 * (count - 1) && count > 0) want = expect[i + 2];
            if (queue[i] != want) return 6;
        }
    }
    return 0;
}
