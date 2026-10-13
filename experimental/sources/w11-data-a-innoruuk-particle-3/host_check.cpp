// Host contract for innoruukParticle3. Not an EE plan input.
extern "C" unsigned char innoruukParticle3[80];
int main() {
    if (sizeof innoruukParticle3 != 80u) return 1;
    for (unsigned i = 0; i < sizeof innoruukParticle3; ++i)
        if (innoruukParticle3[i] != 0) return 2;
    return 0;
}
