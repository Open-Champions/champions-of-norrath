// Host check for target_objs.cpp; not an EE plan input.
#include "target_objs.cpp"

int main() {
    if (targetObjs[0] != 0 || targetObjs[9] != 0) return 1;
    unsigned char byte = 0;
    targetObjs[4] = &byte;
    if (targetObjs[4] != &byte) return 2;
    return 0;
}
