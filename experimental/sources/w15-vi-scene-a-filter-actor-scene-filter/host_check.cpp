// Host check only; not a build input.
#include "filter.cpp"
int main() {
    alignas(8) unsigned char f[16] = {0};
    int* w = reinterpret_cast<int*>(f);
    const int words[] = {0, 1, -1, 7, 0x7fffffff, -0x7fffffff - 1};
    for (int v : words) {
        *w = v;
        for (int a : words) {
            int expect = v == a ? 1 : 0;
            if (FilterActor__13VISceneFilterR7VIScenei(f, f + 8, a) != expect) return 1;
        }
    }
    for (int i = 4; i < 16; ++i) if (f[i] != 0) return 2;
    return 0;
}
