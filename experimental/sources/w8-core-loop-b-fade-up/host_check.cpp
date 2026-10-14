// Host contract for coreInterfaceFadeUp__Fi. Not an EE plan input.
#include "fade_up.cpp"

float coreInterfaceFadeUpVal[4] = {1.0f, 1.0f, 1.0f, 1.0f};

int main() {
    coreInterfaceFadeUp__Fi(2);
    if (coreInterfaceFadeUpVal[2] != 0.0f) return 1;
    if (coreInterfaceFadeUpVal[0] != 1.0f || coreInterfaceFadeUpVal[1] != 1.0f ||
        coreInterfaceFadeUpVal[3] != 1.0f)
        return 2;
    coreInterfaceFadeUp__Fi(0);
    coreInterfaceFadeUp__Fi(3);
    coreInterfaceFadeUp__Fi(1);
    for (int i = 0; i < 4; ++i)
        if (coreInterfaceFadeUpVal[i] != 0.0f) return 3;
    return 0;
}
