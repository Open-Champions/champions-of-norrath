#include "check_for_monsters.cpp"
signed char noMonsters;
int main() {
    unsigned words[4] = {0, 0, 0, 0};
    unsigned char* object = reinterpret_cast<unsigned char*>(words);
    static const unsigned cases[][2] = {
        {0x20, 0}, {0x00, 1}, {0x08, 1}, {0x28, 1}, {0x60, 1}, {0x40, 1},
        {0x120, 0}, {0x30, 0}, {0xffffffd7u, 1}, {0xffffffa7u, 0},
    };
    for (unsigned i = 0; i < sizeof cases / sizeof cases[0]; ++i) {
        words[3] = cases[i][0];
        noMonsters = 1;
        checkForMonsters__FP10GameObject(object);
        if (noMonsters != static_cast<signed char>(cases[i][1])) return 1 + static_cast<int>(i);
    }
    return 0;
}
