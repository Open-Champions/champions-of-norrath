// Host check for harm1. Not an EE plan input; it includes the definition file.
#include "harm1.cpp"
int main() {
    if (sizeof harm1 != 80u) return 1;
    for (unsigned char byte : harm1) if (byte != 0) return 2;
    return 0;
}
