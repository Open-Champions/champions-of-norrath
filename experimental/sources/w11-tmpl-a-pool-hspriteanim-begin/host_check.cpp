#include "begin.cpp"
static const void* seen;
static int calls;
extern "C" int End__Ct6VIPool1ZP13VIHSpriteAnim(const void* pool) { seen = pool; ++calls; return 777; }
int main() {
    int pool[8] = {};
    pool[4] = 5;
    if (Begin__Ct6VIPool1ZP13VIHSpriteAnim(pool) != 5 || calls != 0) return 1;
    pool[4] = 0;
    if (Begin__Ct6VIPool1ZP13VIHSpriteAnim(pool) != 0 || calls != 0) return 2;
    pool[4] = -2;
    if (Begin__Ct6VIPool1ZP13VIHSpriteAnim(pool) != -2 || calls != 0) return 3;
    pool[4] = -1;
    if (Begin__Ct6VIPool1ZP13VIHSpriteAnim(pool) != 777 || calls != 1 || seen != pool) return 4;
    return 0;
}
