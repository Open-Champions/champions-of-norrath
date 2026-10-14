#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "queue_key_event.cpp"

int main() {
    alignas(16) unsigned char keyboard[32] = {};
    signed char* queue = reinterpret_cast<signed char*>(arena + 256);
    const unsigned buffer = arena_base + 256;
    std::memcpy(keyboard + 8, &buffer, 4);
    signed char key[2] = {-3, 9};
    VIKeyboard* kb = reinterpret_cast<VIKeyboard*>(keyboard);
    VIConsoleKey* k = reinterpret_cast<VIConsoleKey*>(key);
    QueueKeyEvent__10VIKeyboardP12VIConsoleKey(kb, k);
    int count;
    std::memcpy(&count, keyboard + 12, 4);
    if (count != 1 || queue[0] != -3 || queue[1] != 9 || queue[2] != 0) return 1;
    count = 127;
    std::memcpy(keyboard + 12, &count, 4);
    key[0] = 5; key[1] = -6;
    QueueKeyEvent__10VIKeyboardP12VIConsoleKey(kb, k);
    std::memcpy(&count, keyboard + 12, 4);
    if (count != 128 || queue[254] != 5 || queue[255] != -6) return 2;
    QueueKeyEvent__10VIKeyboardP12VIConsoleKey(kb, k);
    std::memcpy(&count, keyboard + 12, 4);
    if (count != 128 || queue[256] != 0) return 3;
    count = -1;
    std::memcpy(keyboard + 12, &count, 4);
    QueueKeyEvent__10VIKeyboardP12VIConsoleKey(kb, k);  // negative count is below 128: stored at index -1 words
    std::memcpy(&count, keyboard + 12, 4);
    if (count != 0 || queue[-2] != 5) return 4;
    return 0;
}
