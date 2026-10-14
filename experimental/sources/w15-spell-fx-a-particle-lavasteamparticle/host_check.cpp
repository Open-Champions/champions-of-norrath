// Host contract for lavaSteamParticle. Not an EE plan input.
extern "C" unsigned char lavaSteamParticle[80];
int main() {
    if (sizeof lavaSteamParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof lavaSteamParticle; ++i)
        if (lavaSteamParticle[i] != 0) return 2;
    return 0;
}
