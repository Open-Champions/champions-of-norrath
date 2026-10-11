extern "C" int nmPlayers = 0;
extern "C" int amxGetNumPlayers__FP5__amxPi(void*, int*);
int main() {
    nmPlayers = 0;
    if (amxGetNumPlayers__FP5__amxPi(0, 0) != 0) return 1;
    nmPlayers = 4;
    if (amxGetNumPlayers__FP5__amxPi(0, 0) != 4) return 2;
    nmPlayers = -1;
    if (amxGetNumPlayers__FP5__amxPi(0, 0) != -1) return 3;
    return 0;
}
