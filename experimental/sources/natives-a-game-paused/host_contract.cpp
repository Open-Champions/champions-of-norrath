extern "C" int gamePause = 0;
extern "C" int amxGamePaused__FP5__amxPi(void*, int*);
int main() {
    gamePause = 0;
    if (amxGamePaused__FP5__amxPi(0, 0) != 0) return 1;
    gamePause = -5;
    if (amxGamePaused__FP5__amxPi(0, 0) != -5) return 2;
    gamePause = 2;
    if (amxGamePaused__FP5__amxPi(0, 0) != 2) return 3;
    return 0;
}
