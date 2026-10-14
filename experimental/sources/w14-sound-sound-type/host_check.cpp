// Host check only; not a build input.
#include "type.cpp"
int main() {
    int o[5] = {11, 22, 33, 44, 55};
    if (Type__C7VISound(o) != 33) return 1;
    o[2] = -7;
    if (Type__C7VISound(o) != -7) return 2;
    return 0;
}
