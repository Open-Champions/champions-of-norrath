// Host check only; not a build input.
#include <initializer_list>
#include "as_xm.cpp"
int main() {
    int o[4] = {1, 2, 1, 4};
    if (AsXm__7VISound(o) != o) return 1;
    for (int k : {0, 2, 3, -1, 0x10001}) {
        o[2] = k;
        if (AsXm__7VISound(o) != nullptr) return 2;
    }
    if (o[0] != 1 || o[1] != 2 || o[3] != 4) return 3;
    return 0;
}
