// Host check for firestormParticle1. Not an EE plan input; it includes the definition file.
#include "firestormParticle1.cpp"
int main() {
    if (sizeof firestormParticle1 != 80u) return 1;
    for (unsigned char byte : firestormParticle1) if (byte != 0) return 2;
    return 0;
}
