// frameNumWrapped at 0x00181550, 116 bytes, in SLUS_205.65 (a local, non-exported symbol).
// Observed: calls frameNumWrappedPS on globalSystem (0x0037f730, 256 bytes), then walks the
// 16 records of the array PS (0x006f5ad0) in order, each 312 bytes. A record whose word at
// byte 304 is nonzero is passed, by address, to frameNumWrappedPS. Nothing is returned.
// Inferred: the 16 by 312 shape comes from the loop count and stride; the array may be longer.
// Unknown: what the word at byte 304 means (the loop only tests it against zero) and the
// declared element count of PS.
struct ParticleSystem;
extern "C" void frameNumWrappedPS__FP14ParticleSystem(ParticleSystem* system);
extern "C" unsigned char globalSystem[];
extern "C" unsigned char PS[];

extern "C" void frameNumWrapped__Fv() {
    frameNumWrappedPS__FP14ParticleSystem(reinterpret_cast<ParticleSystem*>(globalSystem));
    for (int index = 0; index < 16; ++index) {
        unsigned char* record = PS + index * 312;
        if (*reinterpret_cast<unsigned*>(record + 304) != 0u)
            frameNumWrappedPS__FP14ParticleSystem(reinterpret_cast<ParticleSystem*>(record));
    }
}
