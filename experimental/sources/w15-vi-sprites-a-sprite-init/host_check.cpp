// Host check only; not a build input.
#include "init.cpp"
static void* seen; static int ret, calls;
extern "C" int Clear__9VIHSprite(void* o) { seen = o; ++calls; return ret; }
int main() {
    char o[4];
    int cases[5] = {0, 5, -1, -9, 0x7fffffff}; int want[5] = {0, 0, -1, -1, 0};
    for (int i = 0; i < 5; ++i) { ret = cases[i]; if (Init__9VIHSprite(o) != want[i] || seen != o || calls != i + 1) return 1 + i; }
    return 0;
}
