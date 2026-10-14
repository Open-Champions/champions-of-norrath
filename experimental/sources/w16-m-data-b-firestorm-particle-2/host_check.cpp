// Host check for firestormParticle2. Not an EE plan input; it includes the definition file.
#include "firestormParticle2.cpp"
int main() {
    if (sizeof firestormParticle2 != 80u) return 1;
    for (unsigned char byte : firestormParticle2) if (byte != 0) return 2;
    return 0;
}
