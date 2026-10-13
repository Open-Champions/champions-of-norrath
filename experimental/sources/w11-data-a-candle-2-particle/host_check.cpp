// Host contract for candle2Particle. Not an EE plan input.
extern "C" unsigned char candle2Particle[80];
int main() {
    if (sizeof candle2Particle != 80u) return 1;
    for (unsigned i = 0; i < sizeof candle2Particle; ++i)
        if (candle2Particle[i] != 0) return 2;
    return 0;
}
