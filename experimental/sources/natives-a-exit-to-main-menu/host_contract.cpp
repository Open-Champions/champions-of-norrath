extern "C" int gameBailFromGameLoop = 0;
extern "C" int amxExitToMainMenu__FP5__amxPi(void*, int*);
int main() {
    int params[2] = {4, 44};
    gameBailFromGameLoop = 0;
    if (amxExitToMainMenu__FP5__amxPi(0, params) != 0) return 1;
    if (gameBailFromGameLoop != 1 || params[1] != 44) return 2;
    gameBailFromGameLoop = 6;
    if (amxExitToMainMenu__FP5__amxPi(0, params) != 0 || gameBailFromGameLoop != 1) return 3;
    return 0;
}
