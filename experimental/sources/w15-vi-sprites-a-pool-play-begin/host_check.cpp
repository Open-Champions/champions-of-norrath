// Host check only; not a build input.
#include "begin.cpp"
static const void* seen;
static int calls;
extern "C" int End__Ct6VIPool1Z13VIHSpritePlay(const void* pool) { seen = pool; ++calls; return 777; }
int main() {
    int pool[8] = {};
    pool[4] = 5;
    if (Begin__Ct6VIPool1Z13VIHSpritePlay(pool) != 5 || calls) return 1;
    pool[4] = 0;
    if (Begin__Ct6VIPool1Z13VIHSpritePlay(pool) != 0 || calls) return 2;
    pool[4] = -1;
    if (Begin__Ct6VIPool1Z13VIHSpritePlay(pool) != 777 || calls != 1 || seen != pool) return 3;
    return 0;
}
