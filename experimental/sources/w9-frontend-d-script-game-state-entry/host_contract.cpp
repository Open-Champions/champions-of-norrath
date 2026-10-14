#include "script_game_state_entry.cpp"

#include <cstdio>
#include <cstring>

char gameStateData[256];
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); return 1; } } while (0)

int main() {
    std::memset(gameStateData, 0x7e, sizeof gameStateData);
    const char raw[] = {'a','b',0,5, 'c','d','e',0,(char)0xfb, 'f',0,0, 0};
    std::memcpy(gameStateData, raw, sizeof raw);
    int value = 1234;
    CHECK(scriptGameStateEntry__FiPi(0, &value) == gameStateData && value == 5);
    CHECK(scriptGameStateEntry__FiPi(1, &value) == gameStateData + 4 && value == -5);
    value = 99;
    CHECK(scriptGameStateEntry__FiPi(2, &value) == gameStateData + 9 && value == 0);
    value = 99;
    CHECK(scriptGameStateEntry__FiPi(3, &value) == 0 && value == 99);
    CHECK(scriptGameStateEntry__FiPi(-1, &value) == 0 && value == 99);
    gameStateData[0] = 0;
    CHECK(scriptGameStateEntry__FiPi(0, &value) == 0 && value == 99);
    std::puts("ok");
    return 0;
}
