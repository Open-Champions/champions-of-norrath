// Host check only; not a build input.
#include "reset.cpp"
#include <cstdio>
int main() {
    int object[7] = {1, 2, 3, 4, 5, 6, 7};
    Reset__14VICameraEffect(object);
    bool ok = object[0] == 0 && object[1] == 0 && object[2] == 0 && object[3] == 0 && object[4] == 0 && object[5] == 6 && object[6] == 7;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
