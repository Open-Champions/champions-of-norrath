// Host check only; not a build input.
#include "init.cpp"
#include <cstdio>
int main() {
    int object[2] = {7, 9};
    int r = Init__13VIActorFilter(object);
    bool ok = r == 0 && object[0] == -1 && object[1] == 9;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
