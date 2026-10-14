#include "ui_close.cpp"

#include <cstring>

static char* seen[4];
static int calls;
static unsigned char* object_bytes;

extern "C" void Unload__13VIStringTable(VIStringTable* table) {
    if (calls < 4) seen[calls] = reinterpret_cast<char*>(table);
    // The words are still intact while the tables are unloaded.
    unsigned word;
    std::memcpy(&word, object_bytes + 72, 4);
    if (word != 0x11111111u) seen[3] = reinterpret_cast<char*>(1);
    ++calls;
}

int main() {
    alignas(4) unsigned char object[96];
    std::memset(object, 0x5a, sizeof object);
    std::memset(object + 72, 0x11, 12);
    object_bytes = object;
    Close__4VIUI(reinterpret_cast<VIUI*>(object));
    if (calls != 3) return 1;
    if (seen[0] != reinterpret_cast<char*>(object) || seen[1] != reinterpret_cast<char*>(object) + 16 ||
        seen[2] != reinterpret_cast<char*>(object) + 32)
        return 2;
    if (seen[3] != 0) return 3;
    for (int i = 0; i < 96; ++i) {
        bool cleared = i >= 72 && i < 84;
        if (cleared ? object[i] != 0 : object[i] != 0x5a) return 4;
    }
    return 0;
}
