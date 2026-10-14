// Host check for bombResMap. Not an EE plan input; it includes the definition file.
#include "bombResMap.cpp"
int main() {
    if (sizeof bombResMap != 32u) return 1;
    for (unsigned char byte : bombResMap) if (byte != 0) return 2;
    return 0;
}
