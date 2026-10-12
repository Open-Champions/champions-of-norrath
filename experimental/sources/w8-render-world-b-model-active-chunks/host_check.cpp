extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(const void* model, unsigned selected);

alignas(8) static unsigned char buf[512];
static void put64(int off, unsigned long long v) { *reinterpret_cast<unsigned long long*>(buf + off) = v; }

int main() {
    for (auto& b : buf) b = 0;
    put64(8, 0x00ff00ff00ff00ffull);
    const unsigned long long base = ~0x00ff00ff00ff00ffull;
    // Flag bit 1 clear: only the complement is returned, whatever the mask.
    buf[16] = 0x01;
    buf[33] = 4;
    put64(64, 0xffull);
    if (modelGetActiveChunks__FP12_modelHeaderi(buf, 1) != base) return 1;
    // Flag set, zero mask: still just the complement.
    buf[16] = 0x02;
    if (modelGetActiveChunks__FP12_modelHeaderi(buf, 0) != base) return 2;
    // Records at header + 4*16. Record 0 adds 0x00ff (low bits), removes nothing.
    put64(64, 0x00000000000000ffull); put64(72, 0);
    // Record 1 adds high bit pattern and removes bit 63.
    put64(80, 0x8000000000000000ull | 0x00f0000000000000ull); put64(88, 0x8000000000000000ull);
    // Record 2 removes 0xff00 of the complement and adds nothing.
    put64(96, 0); put64(104, 0x000000000000ff00ull);
    // Record 3 would add a marker; it must be ignored by mask 0b0111.
    put64(112, 0x1234000000000000ull); put64(120, 0);
    if (modelGetActiveChunks__FP12_modelHeaderi(buf, 1) != (base | 0xffull)) return 3;
    const unsigned long long want01 = ((base | 0xffull) | (0x80f0000000000000ull)) & ~0x8000000000000000ull;
    if (modelGetActiveChunks__FP12_modelHeaderi(buf, 3) != want01) return 4;
    // Selecting only record 1 skips record 0.
    const unsigned long long want1 = (base | 0x80f0000000000000ull) & ~0x8000000000000000ull;
    if (modelGetActiveChunks__FP12_modelHeaderi(buf, 2) != want1) return 5;
    const unsigned long long want012 = want01 & ~0xff00ull;
    if (modelGetActiveChunks__FP12_modelHeaderi(buf, 7) != want012) return 6;
    // A removed bit is removed after the same record's add: add then remove same bit.
    put64(96, 0x0000000000000100ull); put64(104, 0x0000000000000100ull);
    if (modelGetActiveChunks__FP12_modelHeaderi(buf, 4) != (base & ~0x100ull)) return 7;
    // A higher bit beyond the record walked by the highest set bit is not read.
    if (modelGetActiveChunks__FP12_modelHeaderi(buf, 8) != (base | 0x1234000000000000ull)) return 8;
    return 0;
}
