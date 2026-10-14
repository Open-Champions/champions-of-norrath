// Host check for firestormParticle3. Not an EE plan input; it includes the definition file.
#include "firestormParticle3.cpp"
int main() {
    if (sizeof firestormParticle3 != 80u) return 1;
    for (unsigned char byte : firestormParticle3) if (byte != 0) return 2;
    return 0;
}
