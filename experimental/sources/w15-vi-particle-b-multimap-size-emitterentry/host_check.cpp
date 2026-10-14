// Host check only; not a build input.
#include "size.cpp"
#include <cstdio>
int main() {
    alignas(8) int map[8] = {0};
    const int in[] = {0, 1, 2, 10, -1, -50, 0x7fffffff};
    const int out[] = {0, 0, 1, 9, 0, 0, 0x7ffffffe};
    for (int i = 0; i < 7; ++i) {
        map[0] = in[i];
        map[1] = 7;
        if (Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map) != out[i] || map[1] != 7) { std::puts("FAIL"); return 1; }
    }
    std::puts("ok");
    return 0;
}
