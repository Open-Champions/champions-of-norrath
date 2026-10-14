// Host check for coreInterfaceBrightness. Not an EE plan input.
#include "core_interface_brightness.cpp"

int main() {
    if (sizeof(coreInterfaceBrightness) != 16 || coreInterfaceBrightness[0] != 1.0f || coreInterfaceBrightness[1] != 1.0f || coreInterfaceBrightness[2] != 1.0f || coreInterfaceBrightness[3] != 1.0f) return 1;
    return 0;
}
