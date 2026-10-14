// Host contract for setActiveChunks__10Teleporter; not an EE input. The function must leave all
// memory alone, whatever it is handed.
#include "set_active_chunks.cpp"

#include <cstring>

int main() {
    unsigned char before[64], arena[64];
    for (int i = 0; i < 64; ++i) before[i] = arena[i] = static_cast<unsigned char>(i * 7 + 1);
    setActiveChunks__10Teleporter();
    return std::memcmp(before, arena, 64) == 0 ? 0 : 1;
}
