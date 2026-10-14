#include "world_difficulty.cpp"
#include <cstring>
int main() {
    static_assert(sizeof(worldDifficulty) == 4, "one word");
    unsigned bits;
    std::memcpy(&bits, &worldDifficulty, 4);
    if (bits != 0x3f800000u) return 1;
    worldDifficulty = 2.0f;
    return worldDifficulty == 2.0f ? 0 : 2;
}
