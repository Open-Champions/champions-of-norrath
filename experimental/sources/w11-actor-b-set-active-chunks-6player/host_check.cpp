// Host contract for setActiveChunks__6Player; not an EE input. Includes the source.
#include "set_active_chunks.cpp"
#include <cstring>
int main() {
    unsigned char object[64];
    std::memset(object, 0x3c, sizeof object);
    setActiveChunks__6Player(object);
    for (unsigned char b : object) if (b != 0x3c) return 1;
    return 0;
}
