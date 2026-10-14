// Host check only; not a build input.
#include <initializer_list>
#include "as_wave.cpp"
int main() {
    int o[4] = {1, 2, 0, 4};
    if (AsWave__7VISound(o) != o) return 1;
    for (int k : {1, 2, -1, 0x10000}) {
        o[2] = k;
        if (AsWave__7VISound(o) != nullptr) return 2;
    }
    if (o[0] != 1 || o[1] != 2 || o[3] != 4) return 3;
    return 0;
}
