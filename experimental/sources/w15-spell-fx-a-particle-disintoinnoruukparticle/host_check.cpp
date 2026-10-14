// Host contract for disintoInnoruukParticle. Not an EE plan input.
extern "C" unsigned char disintoInnoruukParticle[80];
int main() {
    if (sizeof disintoInnoruukParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof disintoInnoruukParticle; ++i)
        if (disintoInnoruukParticle[i] != 0) return 2;
    return 0;
}
