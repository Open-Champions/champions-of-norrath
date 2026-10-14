// Host check only; not a build input.
#include "ctor.cpp"
int main() {
    int obj[16];
    for (int& w : obj) w = -1;
    __13VISpellEffect(obj);
    for (int i = 0; i < 12; i++) if (obj[i] != 0) return 1;
    for (int i = 12; i < 16; i++) if (obj[i] != -1) return 2;
    return 0;
}
