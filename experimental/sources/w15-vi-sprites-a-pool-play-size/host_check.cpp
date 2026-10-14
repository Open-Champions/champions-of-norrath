// Host check only; not a build input.
#include "size.cpp"
int main() {
    int pool[8] = {41, 1, 2, 3, 4, 5, 6, 7};
    if (Size__Ct6VIPool1Z13VIHSpritePlay(pool) != 41) return 1;
    pool[0] = -3;
    return Size__Ct6VIPool1Z13VIHSpritePlay(pool) == -3 ? 0 : 2;
}
