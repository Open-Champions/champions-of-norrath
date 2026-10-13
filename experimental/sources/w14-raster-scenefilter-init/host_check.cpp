// Host check only; not a build input.
#include "init.cpp"
#include <cstdio>
int main() {
    int object[6] = {1, 2, 3, 4, 5, 6};
    int r = Init__13VISceneFilter(object);
    bool ok = r == 0 && object[0] == -1 && object[1] == -1 && object[2] == -1 && object[3] == -1 && object[4] == 5 && object[5] == 6;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
